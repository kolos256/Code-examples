/******************************************************************************
*
*
* PA15 COMMDISABLE ODO GPIO
*  0 - disconnect control board from comm bus
*  1 - connect control board from comm bus
*
* PA9  USART1_TX AF1 PPO
* PA10 USART1_RX AF7 PPO
* USART1_INCLK 48MHZ
* CB_RXDATA[5] Byte0 - CMDCODE Byte1-4 data 
*
* USART1_INCLK 48000000
*
*
******************************************************************************/

#include "COMMDRVV1.h"

#define ACBSLAVECMDS 15

extern uint32_t HAL_GetRealTick(void);
//UART_HandleTypeDef COMM_huart;
void (*COMM_rxproc)   (void);
void (*COMM_txproc)   (void);
void (*COMM_tcproc)   (void);
void (*COMM_idleproc) (void);

uint32_t COMM_cmdcounter;
uint32_t COMM_cmdchecker;
uint8_t COMM_SELFADDR;
uint8_t COMM_INBYTE;
uint32_t COMM_rxcounter;
COMM_SLAVECMD COMM_COMANDS[ACBSLAVECMDS];
uint8_t COMM_CURCMD;
uint8_t COMM_RXBUFFER[10];
uint8_t COMM_TXBUFFER[10]={0,0,0,0,0,0,0,0,0,0};
uint32_t COMM_TXCOUNTER;
uint8_t *COMM_TXPTR;
uint32_t COMM_STATE;
USART_TypeDef *COMM_instance;

#define COMM_IS_PARITY_ERROR() ((COMM_STATE)&0x00000001)
#define USART_TXEIEN(huart) (huart)->CR1|=USART_CR1_TXEIE
#define USART_TXEIDIS(huart) (huart)->CR1&=(~USART_CR1_TXEIE)
#define USART_TCIEN(huart) (huart)->CR1|=USART_CR1_TCIE
#define USART_TCIDIS(huart) (huart)->CR1&=(~USART_CR1_TCIE)
#define USART_MUTE(huart) (huart)->CR1|=USART_CR1_MME
#define USART_READ(huart) ((huart)->RDR)
#define USART_SEND(huart,DATA) ((huart)->TDR)=(DATA)

#define COMM_INCLK 48000000
uint8_t COMMSLAVEDRVV1_INIT(uint8_t ADDRESS,USART_TypeDef *COMMFORUSART,uint32_t Baudrate)
{
  uint8_t i;
  COMM_instance=COMMFORUSART;
  
  COMM_rxproc=COMM_HANDLER_addrcheck;
  COMM_txproc=COMM_HANDLER_datatx;
  COMM_idleproc=COMM_HANDLER_idle;
  COMM_tcproc=COMM_HANDLER_transfererror;
  
  COMM_cmdchecker=COMM_cmdcounter=0;
  COMM_SELFADDR=ADDRESS;
  COMM_INBYTE=0;
  COMM_rxcounter=0;
  COMM_CURCMD=0;
  COMM_TXCOUNTER=0;
  for(i=0;i<ACBSLAVECMDS;i++) {
    COMM_COMANDS[i].rxbytes=COMM_COMANDS[i].txbytes=0;
    COMM_COMANDS[i].Handler=NULL;
  }

  //config 9databit podd 2stop
  COMMFORUSART->CR1&=(~USART_CR1_UE);
  COMMFORUSART->CR1=(USART_CR1_PEIE|USART_CR1_IDLEIE|USART_CR1_RXNEIE|USART_CR1_TE|USART_CR1_RE|USART_CR1_PCE|USART_CR1_PS|(1<<USART_CR1_M_Pos));
  COMMFORUSART->CR2=((0x02<<USART_CR2_STOP_Pos));
  COMMFORUSART->CR3=0;
  COMMFORUSART->BRR=((COMM_INCLK+(Baudrate/2))/Baudrate);
  COMMFORUSART->GTPR=0;
  COMMFORUSART->CR1|=USART_CR1_UE;

#if defined(DBG1)
  COMMFORUSART->CR1&=(~(USART_CR1_IDLEIE|USART_CR1_RXNEIE));
  while(1)
  {
    while((COMMFORUSART->ISR&USART_ISR_TXE)==0);
    COMMFORUSART->TDR=0x55;
  }
#endif
  return 0;
}

uint8_t COMM_CMDREG(uint8_t CODE,void *txbuf,uint32_t txbytes,uint32_t rxbytes,void (*Handler) (void)){
  if(CODE>ACBSLAVECMDS){
   	return 1;
  }
  COMM_COMANDS[CODE].Handler=Handler;
  COMM_COMANDS[CODE].txaddr=txbuf;
  COMM_COMANDS[CODE].txbytes=txbytes;
  COMM_COMANDS[CODE].rxbytes=(rxbytes+2);
  return 0;
}

uint8_t COMM_CMDCHANGETX(uint8_t CODE,void *txbuf,uint32_t txbytes){
  if(CODE>ACBSLAVECMDS){
   	return 1;
  }
  COMM_COMANDS[CODE].txaddr=txbuf;
  COMM_COMANDS[CODE].txbytes=txbytes;
  return 0;
}

void COMM_rststate(void (*proc)(void)){
  COMM_rxcounter=0;
  COMM_rxproc=proc;
}

void COMM_HANDLER_idle(void){
  //Handle idle state
  if(COMM_rxproc==COMM_HANDLER_rxwaittx){
    COMM_rststate(COMM_HANDLER_rxwaittx);
  }else{
    COMM_rststate(COMM_HANDLER_addrcheck);
  }
}

void COMM_HANDLER_datatx(void){
	//transmit data bytes
	if(COMM_TXCOUNTER>0){
    	USART_SEND(COMM_instance,*COMM_TXPTR++);
		COMM_TXCOUNTER--;
	}
	if(COMM_TXCOUNTER==0){
		USART_TXEIDIS(COMM_instance);
		COMM_txproc=COMM_HANDLER_conftx;
		COMM_tcproc=COMM_HANDLER_transfercomplite;
	}
}

void COMM_HANDLER_conftx(void){	
	//transmit confirmation
	USART_SEND(COMM_instance,COMM_SELFADDR);
	if(COMM_TXCOUNTER>0){
		COMM_txproc=COMM_HANDLER_datatx;
	}else{
		USART_TXEIDIS(COMM_instance);
		COMM_tcproc=COMM_HANDLER_transfercomplite;
	}
}

void COMM_HANDLER_addrcheck(void){
  if(COMM_IS_PARITY_ERROR()||(COMM_INBYTE!=COMM_SELFADDR)){
		USART_MUTE(COMM_instance);
		COMM_rststate(COMM_HANDLER_addrcheck);
  }else{
		COMM_rxproc=COMM_HANDLER_cmdcheck;
  }
}

void COMM_HANDLER_rxwaittx(void){
	USART_MUTE(COMM_instance);
	COMM_rststate(COMM_HANDLER_rxwaittx);
}

//tcproc
void COMM_HANDLER_transfererror(void){
	COMM_rststate(COMM_HANDLER_addrcheck);
	COMM_txproc=COMM_HANDLER_conftx;
	USART_TCIDIS(COMM_instance);
	USART_TXEIDIS(COMM_instance);
}

void COMM_HANDLER_transfercomplite(void){
	COMM_rststate(COMM_HANDLER_addrcheck);
	COMM_tcproc=COMM_HANDLER_transfererror;
	COMM_txproc=COMM_HANDLER_conftx;
	USART_TCIDIS(COMM_instance);
}

void COMM_HANDLER_datarx(void)
{
	if(COMM_IS_PARITY_ERROR()){
		USART_MUTE(COMM_instance);
		COMM_rststate(COMM_HANDLER_addrcheck);
  }else{
		COMM_RXBUFFER[(COMM_rxcounter-2)]=COMM_INBYTE;
		if(COMM_rxcounter>=COMM_COMANDS[COMM_CURCMD].rxbytes){
			COMM_txproc=COMM_HANDLER_conftx;
			COMM_tcproc=COMM_HANDLER_transfercomplite;
			COMM_rststate(COMM_HANDLER_rxwaittx);
			USART_TXEIEN(COMM_instance);
			USART_TCIEN(COMM_instance);
			COMM_cmdcounter++;
		}
  }
}

void COMM_HANDLER_cmdcheck(void){
	if(COMM_IS_PARITY_ERROR()||
			(COMM_INBYTE>=ACBSLAVECMDS)||
			(COMM_COMANDS[COMM_INBYTE].rxbytes==0)){
		//command code error
		USART_MUTE(COMM_instance);
		COMM_rststate(COMM_HANDLER_addrcheck);
	}else{
		COMM_RXBUFFER[0]=COMM_CURCMD=COMM_INBYTE;
		//recieved required amount of bytes
		if(COMM_rxcounter==COMM_COMANDS[COMM_CURCMD].rxbytes){
			COMM_txproc=COMM_HANDLER_conftx;
			COMM_rxproc=COMM_HANDLER_rxwaittx;
			if(COMM_COMANDS[COMM_CURCMD].txbytes!=0){
				COMM_TXCOUNTER=(COMM_COMANDS[COMM_CURCMD].txbytes);
				COMM_TXPTR=(COMM_COMANDS[COMM_CURCMD].txaddr);
				COMM_tcproc=COMM_HANDLER_transfererror;
			}else{
				//command recieved
				COMM_tcproc=COMM_HANDLER_transfercomplite;
			}
			USART_TXEIEN(COMM_instance);
			USART_TCIEN(COMM_instance);
			COMM_cmdcounter++;
		}else{
			COMM_rxproc=COMM_HANDLER_datarx;
		}
	}
}

void COMM_IRQ_PROC(void)
{
	__disable_irq();
	COMM_instance->ICR=(COMM_STATE=COMM_instance->ISR);
	//idle
	if((COMM_STATE&(USART_ISR_IDLE&COMM_instance->CR1))){
		COMM_idleproc();
		COMM_instance->CR1&=(~USART_CR1_IDLEIE);
	}
	//RXNE or ORE
	if((COMM_STATE&(USART_ISR_RXNE|USART_ISR_ORE))){
		COMM_INBYTE=USART_READ(COMM_instance);
		COMM_rxcounter++;
		COMM_rxproc();
		COMM_instance->CR1|=USART_CR1_IDLEIE;
	}
	//TXE or TXEIE
	if((COMM_STATE&(USART_ISR_TXE&COMM_instance->CR1))){
		COMM_txproc();
	}
	//TC or TCIE
  if((COMM_STATE&(USART_ISR_TC&COMM_instance->CR1))){
    COMM_tcproc();
	}
	__enable_irq();
}

void COMM_CMDPROC(void){
	uint8_t CURCMDCODE;
	uint32_t TMPVAL1;
	TMPVAL1=COMM_cmdcounter;
	if(COMM_cmdchecker!=TMPVAL1){
		COMM_cmdchecker=TMPVAL1;
		CURCMDCODE=COMM_RXBUFFER[0];
		if(CURCMDCODE<=ACBSLAVECMDS){
			if(COMM_COMANDS[CURCMDCODE].Handler){
				COMM_COMANDS[CURCMDCODE].Handler();
	    }
    }
  }
}
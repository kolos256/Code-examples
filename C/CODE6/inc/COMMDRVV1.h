#ifndef _ACBSLAVEDRVV1_H_
#define _ACBSLAVEDRVV1_H_

#include <stdint.h>
#include "stm32f0xx_hal.h"

typedef struct 
{
  void (*Handler) (void);//cmdhandler for high level code
  void *txaddr; //adress of tx data
  uint32_t txbytes;//
  uint32_t rxbytes;//
}COMM_SLAVECMD;
extern COMM_SLAVECMD COMM_COMANDS[];
extern void (*COMM_rxproc)   (void);
extern void (*COMM_txproc)   (void);
extern void (*COMM_idleproc) (void);
extern uint32_t COMM_cmdcounter;

extern uint8_t COMM_RXBUFFER[10];
//idleproc
extern void COMM_HANDLER_idle(void);
//rxproc
extern void COMM_HANDLER_addrcheck(void);
extern void COMM_HANDLER_cmdcheck(void);
extern void COMM_HANDLER_rxwaittx(void);
//txproc
extern void COMM_HANDLER_datatx(void);
extern void COMM_HANDLER_conftx(void);
//TCPROC
extern void COMM_HANDLER_transfercomplite(void);
extern void COMM_HANDLER_transfererror(void);


extern void COMM_CMDPROC(void);

extern uint8_t COMM_CMDREG(uint8_t CODE,void *txbuf,uint32_t txbytes,uint32_t rxbytes,void (*Handler) (void));
extern uint8_t COMM_CMDCHANGETX(uint8_t CODE,void *txbuf,uint32_t txbytes);

extern uint8_t COMMSLAVEDRVV1_INIT(uint8_t ADDRESS,USART_TypeDef *COMMFORUSART,uint32_t Baudrate);
extern void COMM_IRQ_PROC(void);

#endif//_ACBSLAVEDRVV1_H_
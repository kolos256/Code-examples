/******************************************************************************
*DESIGNED FOR STM32F303CB
*hse bapass - для внешнего генератора для керамического рщонатора должен быть сброшен
*******************************************************************************/
//#include "RCCV2.h"
#include "STM32/PERIPH/RCC/RCCF303X.h"
#if defined(STM32F303X)
//Основные основные источники частоты
U32 HSIFRQ=8000000;
U32 HSEFRQ=0;
U32 LSIFRQ=0;
U32 LSEFRQ=0;
//Промежуточные частоты
U32 APB1FRQ=0;
U32 APB2FRQ=0;
U32 AHBFRQ=0;

void RCC_PERIPHRESETCMD(State CMD,RESETREG REG,RESETPERIPH PERIPH)
{
   switch(REG)
   {
      case APB2RSTR:
      switch(CMD)
      {
         case ENABLE:
         RCC.APB2RSTR.VALUE|=(PERIPH);
         break;
         case DISABLE:
         RCC.APB2RSTR.VALUE&=(~(PERIPH));
         break;
      }
         
      break;
      case APB1RSTR:
      switch(CMD)
      {
         case ENABLE:
         RCC.APB1RSTR.VALUE|=(PERIPH);
         break;
         case DISABLE:
         RCC.APB1RSTR.VALUE&=(~(PERIPH));
         break;
      }
      break;
      case AHBRSTR:
      switch(CMD)
      {
         case ENABLE:
         RCC.AHBRSTR.VALUE|=(PERIPH);
         break;
         case DISABLE:
         RCC.AHBRSTR.VALUE&=(~(PERIPH));
         break;
      }
      break;
   }
}


void RCC_PERIPHFULRST(RESETPERIPH PERIPH,RESETREG REG,U32 TIME)
{
   RCC_PERIPHRESETCMD(ENABLE,REG,PERIPH);
   while(TIME--);
   RCC_PERIPHRESETCMD(DISABLE,REG,PERIPH);
}

//AHB1LP,AHB2LP,AHB3LP,APB1LP,APB2LP

void RCC_PERIPHCLKENCMD(State CMD,ENABLEREG REG,ENABLEPERIPH PERIPH)
{
   switch(REG)
   {
      case AHBEN:
      switch(CMD)
      {
         case ENABLE:
         RCC.AHBENR.VALUE|=(PERIPH);
         break;
         case DISABLE:
         RCC.AHBENR.VALUE&=(~(PERIPH));
         break;
      }
         
      break;
      
      case APB1EN:
      switch(CMD)
      {
         case ENABLE:
         RCC.APB1ENR.VALUE|=(PERIPH);
         break;
         case DISABLE:
         RCC.APB1ENR.VALUE&=(~(PERIPH));
         break;
      }
      break;
      case APB2EN:
      switch(CMD)
      {
         case ENABLE:
         RCC.APB2ENR.VALUE|=(PERIPH);
         break;
         case DISABLE:
         RCC.APB2ENR.VALUE&=(~(PERIPH));
         break;
      }
      break;  
   }
}

/*void PLLINIT(PLL_SRC SRC, U8 PLLQ,U8 PLLM,U16 PLLN,PLLP_DIV PLLP)
{
  RCC.PLLCFGR.BIT.PLLQ=PLLQ;
  RCC.PLLCFGR.BIT.PLLM=PLLM;
  RCC.PLLCFGR.BIT.PLLN=PLLN;
  RCC.PLLCFGR.BIT.PLLP=PLLP;
  RCC.PLLCFGR.BIT.PLLSRC=SRC;
}*/
/*******************************************************************************
*MUL 1-16
*******************************************************************************/
void PLLINIT(PLL_SRC SRC,U8 MUL)
{
  RCC.CFGR.BIT.PLLSRC=SRC;
  RCC.CFGR.BIT.PLLMUL=(MUL-2);
}
//RCC.CFGR.VALUE=(CFGR_PLLX9|CFGR_PLLHSESRC|CFGR_AHBDIV1|CFGR_APB2DIV1|CFGR_APBDIV2|CFGR_PLLUSBDIV15);
/*******************************************************************************
*typedef enum {APB1_DIV1=0,APB1_DIV2=4,APB1_DIV4=5,APB1_DIV8=6,APB1_DIV16=7}APB1_DIV;
*typedef enum {APB2_DIV1=0,APB2_DIV2=4,APB2_DIV4=5,APB2_DIV8=6,APB2_DIV16=7}APB2_DIV;
*typedef enum {AHB_DIV1=0,AHB_DIV2=8,AHB_DIV4=9,AHB_DIV8=10,AHB_DIV16=11,AHB_DIV64=12,AHB_DIV128=13,AHB_DIV256=14,AHB_DIV512=15}AHB_DIV;
*******************************************************************************/
void APBAHBDIV(APB1_DIV DIV1,APB2_DIV DIV2,AHB_DIV DIV3)
{
  RCC.CFGR.BIT.PPRE1=DIV1;
  RCC.CFGR.BIT.PPRE2=DIV2;
  RCC.CFGR.BIT.HPRE=DIV3;
}

/*******************************************************************************
*HSEDIV 1-16
*FRQ - Частота HSE для удобства конфигурации
*******************************************************************************/
void HSECONFIG(U8 HSEDIV,U32 FRQ)
{
  HSEFRQ=FRQ;
  RCC.CFGR2.BIT.PREDIV=(HSEDIV-1);
}

void HSEBYPASS(State BYP)
{
   RCC.CR.BIT.HSEBYP=BYP;
}
//RCC.CFGR.VALUE=(CFGR_PLLX9|CFGR_PLLHSESRC|CFGR_AHBDIV1|CFGR_APB2DIV1|CFGR_APBDIV2|CFGR_PLLUSBDIV15);
//Конфигурация PLL
//Запускает и ждет стабилизации HSI
CMDRES HSECMD(U16 TIMEOUT,State CMD)
{
   RCC.CR.BIT.HSEON=CMD;
   while((TIMEOUT--)&&(RCC.CR.BIT.HSERDY!=CMD));
   if(TIMEOUT){return NORMAL;}else{return TIMEOUTERR;}
}

CMDRES HSICMD(U16 TIMEOUT,State CMD)
{
   RCC.CR.BIT.HSION=CMD;
   while((TIMEOUT--)&&(RCC.CR.BIT.HSIRDY!=CMD));
   if(TIMEOUT){return NORMAL;}else{return TIMEOUTERR;}
}
CMDRES PLLCMD(State CMD,U16 TIMEOUT)
{
  RCC.CR.BIT.PLLON=CMD;
   while((TIMEOUT--)&&(RCC.CR.BIT.PLLRDY!=CMD));
   if(TIMEOUT){return NORMAL;}else{return TIMEOUTERR;}
}

void USARTSETCLKSRC(U8 USEL,U8 CLKSRC)
{
  RCC.CFGR3.VALUE=(RCC.CFGR3.VALUE&(~(3<<USEL))|(CLKSRC<<USEL));
}

//Переключает системный источник тактовой частоты если все правильно возвращает NORMAL иначе ERROR
CMDRES SYSCLKSW(SYSSRC SOURSE,U16 TIMEOUT)
{
  RCC.CFGR.BIT.SW=SOURSE;
  while(TIMEOUT--)
  {
    if((RCC.CFGR.BIT.SWS==RCC.CFGR.BIT.SW))
    {
      return NORMAL;
    }
  }
  return TIMEOUTERR;
}

void SETADC12PRE(U8 PRE1)
{
  RCC.CFGR2.BIT.ADC12PRESC=PRE1;
}


void SETADC34PRE(U8 PRE1)
{
  RCC.CFGR2.BIT.ADC34PRESC=PRE1;
}
#endif //defined(STM32F303X)

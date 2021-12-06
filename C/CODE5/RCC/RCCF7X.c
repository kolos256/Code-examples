/******************************************************************************
*DESIGNED FOR STM32F7X
*hse bapass - для внешнего генератора для керамического рщонатора должен быть сброшен
*******************************************************************************/
#include "STM32/PERIPH/RCC/RCCF7X.h"

#if defined(STM32F7X)

void RCC_PERIPHRESETCMD(State CMD,RESETREG REG,RESETPERIPH PERIPH)
{
   U32 ANDMASK=0xFFFFFFFF,ORMASK=0x00000000;
   if(CMD==ENABLE)
   {
     ORMASK=PERIPH;
   }
   else
   {
  	 ANDMASK=(~PERIPH);
   }
   switch(REG)
   {
      case AHB1RST:
      RCC.AHB1RSTR.VALUE=((RCC.AHB1RSTR.VALUE&ANDMASK)|ORMASK);
      break;
      case AHB2RST:
      RCC.AHB2RSTR.VALUE=((RCC.AHB2RSTR.VALUE&ANDMASK)|ORMASK);
      break;
      case AHB3RST:
      RCC.AHB3RSTR.VALUE=((RCC.AHB3RSTR.VALUE&ANDMASK)|ORMASK);
      break;
      case APB1RST:
      RCC.APB1RSTR.VALUE=((RCC.APB1RSTR.VALUE&ANDMASK)|ORMASK);
      break;
      case APB2RST:
      RCC.APB2RSTR.VALUE=((RCC.APB2RSTR.VALUE&ANDMASK)|ORMASK);
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
   U32 ANDMASK=0xFFFFFFFF,ORMASK=0x00000000;
   if(CMD==ENABLE)
   {
	 ORMASK=PERIPH;
   }
   else
   {
	 ANDMASK=(~PERIPH);
   }
   switch(REG)
   {
      case AHB1EN:
      RCC.AHB1ENR.VALUE=((RCC.AHB1ENR.VALUE&ANDMASK)|ORMASK);
      break;
      case AHB2EN:
      RCC.AHB2ENR.VALUE=((RCC.AHB2ENR.VALUE&ANDMASK)|ORMASK);
      break;
      case AHB3EN:
      RCC.AHB3ENR.VALUE=((RCC.AHB3ENR.VALUE&ANDMASK)|ORMASK);
      break;
      case APB1EN:
      RCC.APB1ENR.VALUE=((RCC.AHB1ENR.VALUE&ANDMASK)|ORMASK);
      break;
      case APB2EN:
      RCC.APB2ENR.VALUE=((RCC.AHB2ENR.VALUE&ANDMASK)|ORMASK);
      break;
   }
}

/*******************************************************************************
*
*******************************************************************************/
void MAINPLLINIT(PLL_SRC SRC,U16 PLLN,PLLP_DIV PLLP,U8 PLLQ,U8 PLLM)
{
  RCC.PLLCFGR.BIT.PLLSRC=SRC;
  RCC.PLLCFGR.BIT.PLLM=PLLM;
  RCC.PLLCFGR.BIT.PLLP=PLLP;
  RCC.PLLCFGR.BIT.PLLQ=PLLQ;
  RCC.PLLCFGR.BIT.PLLN=PLLN;
}


void APBAHBDIV(APB_DIV APB1,APB_DIV APB2,AHB_DIV AHB)
{
  RCC.CFGR.BIT.PPRE1=APB1;
  RCC.CFGR.BIT.PPRE2=APB2;
  RCC.CFGR.BIT.HPRE=AHB;
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

CMDRES HSECMD(U16 TIMEOUT,State CMD)
{
   RCC.CR.BIT.HSEON=CMD;
   while((TIMEOUT--)&&(RCC.CR.BIT.HSERDY!=CMD));
   if(TIMEOUT){return NORMAL;}else{return TIMEOUTERR;}
}


void HSEBYPASS(State BYP)
{
   RCC.CR.BIT.HSEBYP=BYP;
}

//Переключает системный источник тактовой частоты если все правильно возвращает NORMAL иначе ERROR
CMDRES SYSCLKSW(SYSSRC SOURSE,U16 TIMEOUT)
{
  RCC.CFGR.BIT.SW=SOURSE;
  while(TIMEOUT--)
  {
    if(RCC.CFGR.BIT.SW==RCC.CFGR.BIT.SWS)
    {
      return NORMAL;
    }
  }
  return TIMEOUTERR;
}
#endif//defined(STM32F7X)


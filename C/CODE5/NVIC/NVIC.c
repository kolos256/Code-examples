#include "NVIC.h"

void NVICIRQCONFIG(NVICLINECONFIG CONFIG)
{
   U8 IRQNUMBER=CONFIG.IRQNUMBER;
   NVIC.IPR.IPR[IRQNUMBER]=(CONFIG.PRIORITY<<4);
   if(CONFIG.ISE)
   {
     NVIC.ISER.REG[(IRQNUMBER>>5)]|=(1<<(IRQNUMBER&0x1F));
   }
   else
   {
     NVIC.ICER.REG[(IRQNUMBER>>5)]|=(1<<(IRQNUMBER&0x1F));
   }
}

void NVICIRQENCMD(U8 IRQNUMBER,State CMD)
{
  if(CMD)
  {
    NVIC.ISER.REG[(IRQNUMBER>>5)]=(1<<(IRQNUMBER&0x1F));
  }
  else
  {
    NVIC.ICER.REG[(IRQNUMBER>>5)]=(1<<(IRQNUMBER&0x1F));
  }
}



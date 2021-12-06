
#include "ADC.h"


void ADCONOFFCMD(ADC_BYTES *ADCSEL,State CMD)
{
   if(CMD)
   {
      ADCSEL->CR2.BIT.ADON=1;
   }
   else
   {
      ADCSEL->CR2.BIT.ADON=0;
   }
}
//Задает количество каналов в последовательности 
void ADCREGCHSEQLENGTH(ADC_BYTES *ADCSEL,U8 LENGTH)
{
   if(LENGTH){ADCSEL->SQR1.BIT.L=(LENGTH-1);}
}

void ADCREGCHSET(ADC_BYTES *ADCSEL,U8 SEQNUM,ADCREGCH REGCH)
{
   //U8 SEQNUM=REGCH->SEQNUM;
   //SEQNUM&=0x0F;
   SEQNUM-=1;
   U32 *TMPREG;
   U8 ADCIN;
   if((REGCH.ADCIN<19)&&(SEQNUM<16))
   {
    ADCIN=REGCH.ADCIN;
    //Настраиваю частоту работы АЦП
    TMPREG=(U32*)((&(ADCSEL->SMPR2.VALUE))-(ADCIN/9));
    *TMPREG&=(~(0x7<<((ADCIN%9)*3)));
    *TMPREG|=(REGCH.SMPTIME<<((ADCIN%9)*3));
    //*TMPREG|=(REGCH->SMPR<<(SEQNUM%9));
    TMPREG=(U32*)((&(ADCSEL->SQR3.VALUE))-(SEQNUM/5));
    *TMPREG&=(~(0x1F<<((SEQNUM%5)*5)));
    *TMPREG|=(ADCIN<<((SEQNUM%5)*5));
   }
}

/*void ADCCMD(ADC_BYTES *ADCSEL,ADCCMDTYPE CMD)
{
   switch(CMD)
   {
      case :
   }
}*/
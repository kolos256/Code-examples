/*******************************************************************************
 
 The sequence below is required to start ADC operations:
  1. Enable the ADC internal voltage regulator (refer to the ADC voltage regulator enable 
  sequence).
  2.  The software must wait for the startup time of the ADC voltage regulator 
  (TADCVREG_STUP) before launching a calibration or enabling the ADC. This 
  temporization must be implemented by software. TADCVREG_STUP is equal to 10 ms in 
  the worst case process/temperature/power supply.

  ADVREGEN

 The software must wait for the startup time of the ADC voltage regulator 
 (TADCVREG_STUP) before launching a calibration or enabling the ADC
 TADCVREG_STUP is equal to 10 mks

 ADEN bit cannot be set during ADCAL=1 and 4 ADC clock cycle after the ADCAL bit is 
 cleared by hardware(end of the calibration).
   
*******************************************************************************/

#include "ADCF303X.h"

U8 ADC_ADVREGCMD(ADC_BYTES *ADC, State CMD,U32 TIMEOUT)
{
  if(CMD)
  {
    if(ADC->CR.BIT.ADVREGEN!=01)
    {
      ADC->CR.BIT.ADVREGEN=00;
      ADC->CR.BIT.ADVREGEN=01;
    }
  }
  else
  {
    if(ADC->CR.BIT.ADVREGEN!=10)
    {
      ADC->CR.BIT.ADVREGEN=00;
      ADC->CR.BIT.ADVREGEN=10;
    } 
    ADC->CR.BIT.ADVREGEN=00;
  }
  return false;
}

U8 ADC_CAL(ADC_BYTES *ADC,U32 TIMEOUT)
{
  U32 starttick=gettick();
  ADC->CR.BIT.ADCAL=1;
  while((!ADC->CR.BIT.ADCAL)&
         (tickstoperiod(starttick,gettick())<TIMEOUT));
  if(tickstoperiod(starttick,gettick())>TIMEOUT)
  {
    return 1;
  }
  else
  {
    return false;
  } 
}


U8 ADC_EN(ADC_BYTES *ADC,U32 TIMEOUT)
{
  U32 starttick=gettick();
  ADC->CR.BIT.ADEN=1;
  while((!ADC->ISR.BIT.ADRDY)&
         (tickstoperiod(starttick,gettick())<TIMEOUT));
  if(tickstoperiod(starttick,gettick())>TIMEOUT)
  {
    return 1;
  }
  else
  {
    return false;
  } 
}

void ADC_REGSTART(ADC_BYTES *ADC)
{
  ADC->CR.BIT.ADSTART=1;
}

U8 ADC_REGSTOP(ADC_BYTES *ADC,U32 TIMEOUT)
{
  U32 starttick=gettick();
  ADC->CR.BIT.ADSTP=1;
  while((ADC->CR.BIT.ADSTP)&
         (tickstoperiod(starttick,gettick())<TIMEOUT));
  if(tickstoperiod(starttick,gettick())>TIMEOUT)
  {
    return 1;
  }
  else
  {
    return false;
  } 
}

void ADC_INJSTART(ADC_BYTES *ADC)
{
  ADC->CR.BIT.JADSTART=1;
}

//Задает количество каналов в последовательности 
void ADC_REGCHENNALS(ADC_BYTES *ADCSEL,U8 LENGTH)
{
   if(LENGTH){ADCSEL->SQR1.BIT.L=(LENGTH-1);}
}

//void ADCREGCHSET(ADC_BYTES *ADCSEL,U8 SEQNUM,ADCREGCH REGCH)

void ADCREGCHSET(ADC_BYTES *ADCSEL,U8 SEQNUM,U8 ADCIN,U8 SMPTIME)
{
   //SEQNUM-=1;
   U32 *TMPREG;
   if((ADCIN<19)&&(SEQNUM<16))
   {
    //Настраиваю частоту работы АЦП
    TMPREG=(U32*)((&(ADCSEL->SMPR2.VALUE))-(ADCIN/9));
    *TMPREG&=(~(0x7<<((ADCIN%10)*3)));
    *TMPREG|=(SMPTIME<<((ADCIN%10)*3));
    TMPREG=(U32*)((&(ADCSEL->SQR1.VALUE))+(SEQNUM/5));
    *TMPREG&=(~(0x1F<<((SEQNUM%5)*6)));
    *TMPREG|=(ADCIN<<((SEQNUM%5)*6));
   }
}

U8 ADC_INJSTOP(ADC_BYTES *ADC,U32 TIMEOUT)
{
  U32 starttick=gettick();
  ADC->CR.BIT.JADSTART=1;
  while((ADC->CR.BIT.ADSTP)&
         (tickstoperiod(starttick,gettick())<TIMEOUT));
  if(tickstoperiod(starttick,gettick())>TIMEOUT)
  {
    return 1;
  }
  else
  {
    return false;
  } 
}

U8 ADC_CONFIG(ADC_BYTES *ADC,U32 CONFIG)
{
  ADC->CFGR.VALUE=CONFIG;
}

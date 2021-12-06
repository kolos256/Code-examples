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
#include "STM32\MULTITASKINGV1\MULTITASKINGV1.h"
#include "ADCF030X.h"


/*************************************************
Calibration software procedure
1. Ensure that ADEN=0 and DMAEN=0
2. Set A DCAL=1
3.  Wait until ADCAL=0 
*************************************************/

U8 ADC_CAL(ADC_BYTES *ADC,U32 TIMEOUT)
{
  U32 starttick=SYSTICKCOUNTER;
  TIMEOUT=mks_SysTick(TIMEOUT);
  ADC->CR.BIT.ADCAL=1;
  while((ADC->CR.BIT.ADCAL)&&
        (systickstoperiod(starttick,SYSTICKCOUNTER)<TIMEOUT)){tskret();}
  if(systickstoperiod(starttick,SYSTICKCOUNTER)>=TIMEOUT)
  {
    return 1;
  }
  else
  {
    return false;
  } 
}

/********************************************************************************************
1. Clear the ADRDY bit in ADC_ISR register by programming this bit to 1.
2.  Set ADEN=1 in the ADC_CR register.
3.  Wait until ADRDY=1 in the ADC_ISR register and continue to write ADEN=1 (ADRDY 
is set after the ADC startup time). This can be handled by interrupt if the interrupt is 
enabled by setting the ADRDYIE bit in the ADC_IER register.
********************************************************************************************/

U8 ADC_EN(ADC_BYTES *ADC,U32 TIMEOUT)
{
  U32 starttick=SYSTICKCOUNTER;
  TIMEOUT=mks_SysTick(TIMEOUT);
  ADC->ISR.BIT.ADRDY=0;
  ADC->CR.BIT.ADEN=1;
  while((!ADC->ISR.BIT.ADRDY)&&
         (systickstoperiod(starttick,SYSTICKCOUNTER)<TIMEOUT)){tskret();}
  if(systickstoperiod(starttick,SYSTICKCOUNTER)>TIMEOUT)
  {
    return 1;
  }
  else
  {
    return false;
  } 
}

void ADC_START(ADC_BYTES *ADC)
{
  ADC->CR.BIT.ADSTART=1;
}

U8 ADC_REGSTOP(ADC_BYTES *ADC,U32 TIMEOUT)
{
  U32 starttick=SYSTICKCOUNTER;
  TIMEOUT=mks_SysTick(TIMEOUT);
  ADC->CR.BIT.ADSTP=1;
  while((ADC->CR.BIT.ADSTP)&&
         (systickstoperiod(starttick,SYSTICKCOUNTER)<TIMEOUT)){tskret();}
  if(systickstoperiod(starttick,SYSTICKCOUNTER)>TIMEOUT)
  {
    return 1;
  }
  else
  {
    return false;
  } 
}
//adc chanels confiig
void ADC_CONFIGCHANELS(ADC_BYTES *ADCSEL,U32 CHANELS,U8 SMPLTIME)
{
  ADCSEL->CHSELR.VALUE=CHANELS;
  ADCSEL->SMPR.BIT.SMP=SMPLTIME;
}
void ADC_SETCLK(ADC_BYTES *ADCSEL,U8 CLKMODE)
{
  ADCSEL->CFGR2.BIT.CKMODE=CLKMODE;
}

U8 ADC_CONFIG(ADC_BYTES *ADC,U32 CONFIG)
{
  ADC->CFGR1.VALUE=CONFIG;
}

//Файл конфигурации текущего проекта
//#include "CONF.h"
#include "GPIOV2.h"

#if defined(GPIOV2)

void GPIO_PINCONFIG(GPIO_BYTES *PORT,U8 PINNUM, PIN PINSETTINGS)
{
   //U8 PINNUM=PINSETTINGS.BIT.PINNUMBER;
   U8 const1=PINNUM>>3;
   U32 MASK1=~(1<<(PINNUM)),MASK2=~(3<<(PINNUM<<1)),MASK3=~(0xF<<((PINNUM&7)<<2));

   PORT->ODR.VALUE=((PORT->ODR.VALUE&MASK1)|(PINSETTINGS.BIT.OUT<<PINNUM));
   PORT->OTYPER.VALUE=((PORT->OTYPER.VALUE&MASK1)|(PINSETTINGS.BIT.OT<<PINNUM));
   PORT->OSPEEDER.VALUE=((PORT->OSPEEDER.VALUE&MASK2)|(PINSETTINGS.BIT.OSPEED<<(PINNUM<<1)));
   PORT->PUPDR.VALUE=((PORT->PUPDR.VALUE&MASK2)|(PINSETTINGS.BIT.PUPD<<(PINNUM<<1)));
   if(const1)
   {
      PORT->AFRH.VALUE=((PORT->AFRH.VALUE&MASK3)|(PINSETTINGS.BIT.AF<<((PINNUM&7)<<2)));
   }
   else
   {
      PORT->AFRL.VALUE=((PORT->AFRL.VALUE&MASK3)|(PINSETTINGS.BIT.AF<<((PINNUM&7)<<2)));
   }
   PORT->MODER.VALUE=((PORT->MODER.VALUE&MASK2)|(PINSETTINGS.BIT.MODE<<(PINNUM<<1)));
}

/**********************************************
*Устанавливает бит номер PINNUMBER (0-15)
**********************************************/
void GPIO_BITSET(GPIO_BYTES *PORT,U8 PINNUMBER)
{
   PORT->BSRR.VALUE=(1<<(PINNUMBER));
}
/***************************************************
*Сбрасывает и устанавливает биты по маске
*BIT  0-15 -
*BIT 16-31 -
****************************************************/
void GPIO_BITSSETRESET(GPIO_BYTES *PORT,U32 BITMASK)
{
   PORT->BSRR.VALUE=(BITMASK);
}


void GPIO_BITSSETRESET1(GPIO_BYTES *PORT,BSRRVAL VALUE)
{
   PORT->BSRR.VALUE=VALUE.VALUE;
}

/**********************************************
*сбрасывает вывод порта номер PINNUMBER (0-15)
**********************************************/
void GPIO_BITRESET(GPIO_BYTES *PORT,U8 PINNUMBER)
{
   PORT->BSRR.VALUE=(0x10000<<(PINNUMBER));
}

void GPIOSETPINSTATE(GPIO_BYTES *PORT,U8 PINNUMBER,U8 STATE)
{
  if(STATE)
  {
    PORT->BSRR.VALUE=(0x00001<<(PINNUMBER));
  }
  else
  {
    PORT->BSRR.VALUE=(0x10000<<(PINNUMBER));
  }
}

void GPIOSEITCHTEST(GPIO_BYTES *PORT, U32 PERIOD,U8 PINNUMBER)
{
   U32 delay;
   while(1)
   {
      delay=PERIOD;
      while(delay--);
      PORT->ODR.VALUE^=(1<<(PINNUMBER));
   }
}
#endif

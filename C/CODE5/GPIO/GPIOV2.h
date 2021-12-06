#ifndef _GPIOV2_H_
#define _GPIOV2_H_

#include "CORE\CORE.h"
#ifdef USECONF
#include "CONF.h"
#include "devlist.h"
#endif//USECONF
//Основные частоты
#if defined(GPIOV2)

typedef struct
{
  union
  {
    volatile U32 VALUE;
    struct
    {
    volatile  U32 MODER0:2,MODER1:2,MODER2:2,MODER3:2,MODER4:2,MODER5:2,MODER6:2,MODER7:2,MODER8:2,MODER9:2,MODER10:2,MODER11:2,MODER12:2,MODER13:2,MODER14:2,MODER15:2;
    }BIT;
  }MODER;
  
  union
  {
    volatile U32 VALUE;
    struct
    {
    volatile U32 OT0:1,OT1:1,OT2:1,OT3:1,OT4:1,OT5:1,OT6:1,OT7:1,OT8:1,OT9:1,OT10:1,OT11:1,OT12:1,OT13:1,OT14:1,OT15:1,:16;
    }BIT;
  }OTYPER;
  
  union
  {
    volatile U32 VALUE;
    struct
    {
    volatile U32 OSPEEDR0:2,OSPEEDR1:2,OSPEEDR2:2,OSPEEDR3:2,OSPEEDR4:2,OSPEEDR5:2,OSPEEDR6:2,OSPEEDR7:2,OSPEEDR8:2,OSPEEDR9:2,OSPEEDR10:2,OSPEEDR11:2,OSPEEDR12:2,OSPEEDR13:2,OSPEEDR14:2,OSPEEDR15:2;
    }BIT;
  }OSPEEDER;
  
  union
  {
    volatile U32 VALUE;
    struct
    {
    volatile U32 PUPDR0:2,PUPDR1:2,PUPDR2:2,PUPDR3:2,PUPDR4:2,PUPDR5:2,PUPDR6:2,PUPDR7:2,PUPDR8:2,PUPDR9:2,PUPDR10:2,PUPDR11:2,PUPDR12:2,PUPDR13:2,PUPDR14:2,PUPDR15:2;
    }BIT;
  }PUPDR;
  
  union
  {
    volatile U32 VALUE;
    struct
    {
    volatile U32 IDR0:1,IDR1:1,IDR2:1,IDR3:1,IDR4:1,IDR5:1,IDR6:1,IDR7:1,IDR8:1,IDR9:1,IDR10:1,IDR11:1,IDR12:1,IDR13:1,IDR14:1,IDR15:1,:16;
    }BIT;
  }IDR;
  
  union
  {
    volatile U32 VALUE;
    volatile U8 VALUE8;
    struct
    {
    volatile U32 ODR0:1,ODR1:1,ODR2:1,ODR3:1,ODR4:1,ODR5:1,ODR6:1,ODR7:1,ODR8:1,ODR9:1,ODR10:1,ODR11:1,ODR12:1,ODR13:1,ODR14:1,ODR15:1,:16;
    }BIT;
  }ODR;
  
  union
  {
    volatile U32 VALUE;
    struct
    {
    volatile U32 BS0:1,BS1:1,BS2:1,BS3:1,BS4:1,BS5:1,BS6:1,BS7:1,BS8:1,BS9:1,BS10:1,BS11:1,BS12:1,BS13:1,BS14:1,BS15:1,BR0:1,BR1:1,BR2:1,BR3:1,BR4:1,BR5:1,BR6:1,BR7:1,BR8:1,BR9:1,BR10:1,BR11:1,BR12:1,BR13:1,BR14:1,BR15:1;
    }BIT;
  }BSRR;
  
  union
  {
    volatile U32 VALUE;
    struct
    {
    volatile U32 LCK0:1,LCK1:1,LCK2:1,LCK3:1,LCK4:1,LCK5:1,LCK6:1,LCK7:1,LCK8:1,LCK9:1,LCK10:1,LCK11:1,LCK12:1,LCK13:1,LCK14:1,LCK15:1,LCKK:1;
    }BIT;
  }LCKK;
  
  union
  {
    volatile U32 VALUE;
    struct
    {
    volatile U32 AFRL0:4,AFRL1:4,AFRL2:4,AFRL3:4,AFRL4:4,AFRL5:4,AFRL6:4,AFRL7:4;
    }BIT;
  }AFRL;
  
  union
  {
    volatile U32 VALUE;
    struct
    {
    volatile U32 AFRH0:4,AFRH1:4,AFRH2:4,AFRH3:4,AFRH4:4,AFRH5:4,AFRH6:4,AFRH7:4;
    }BIT;
  }AFRH;
  
}GPIO_BYTES;

//GPIOA
#ifdef GPIOA_BASE
#define GPIOA (*((GPIO_BYTES*)GPIOA_BASE))
#define GPA (U8)0x00
#endif//GPIOA_BASE
//GPIOB
#ifdef GPIOB_BASE
#define GPIOB (*((GPIO_BYTES*)GPIOB_BASE))
#define GPB (U8)0x01
#endif//GPIOB_BASE
//GPIOC
#ifdef GPIOC_BASE
#define GPIOC (*((GPIO_BYTES*)GPIOC_BASE))
#define GPC (U8)0x02
#endif//GPIOC_BASE
//GPIOD
#ifdef GPIOD_BASE
#define GPIOD (*((GPIO_BYTES*)GPIOD_BASE))
#define GPD (U8)0x03
#endif//GPIOD_BASE
//GPIOE
#ifdef GPIOE_BASE
#define GPIOE (*((GPIO_BYTES*)GPIOE_BASE))
#define GPE (U8)0x04
#endif//GPIOE_BASE
//GPIOF
#ifdef GPIOF_BASE
#define GPIOF (*((GPIO_BYTES*)GPIOF_BASE))
#define GPF (U8)0x05
#endif//GPIOF_BASE
//GPIOG
#ifdef GPIOG_BASE
#define GPIOG (*((GPIO_BYTES*)GPIOG_BASE))
#define GPG ((U8)0x06)
#endif//GPIOG_BASE
//GPIOH
#ifdef GPIOH_BASE
#define GPIOH (*((GPIO_BYTES*)GPIOH_BASE))
#define GPH (U8)0x07
#endif//GPIOH_BASE
//GPIOI
#ifdef GPIOI_BASE
#define GPIOI (*((GPIO_BYTES*)GPIOI_BASE))
#define GPI (U8)0x08
#endif//GPIOI_BASE
//GPIOJ
#ifdef GPIOJ_BASE
#define GPIOJ (*((GPIO_BYTES*)GPIOJ_BASE))
#define GPJ (U8)0x09
#endif//GPIOJ_BASE
//GPIOK
#ifdef GPIOK_BASE
#define GPIOK (*((GPIO_BYTES*)GPIOK_BASE))
#define GPK (U8)0x0A
#endif//GPIOK_BASE
//MODER
//Константы для настройкы вывода
#define INPIN  (U8)0x00
#define OUTPIN (U8)0x01
#define AFPIN  (U8)0x02
#define ANPIN  (U8)0x03
//OTYPER
#define PPOPIN (U8)0x00
#define ODOPIN (U8)0x01
//SPEEDR
#define PINVLSP (U8)0x00
#define PINLOSP (U8)0x01
#define PINMDSP (U8)0x02
#define PINHISP (U8)0x03
//PUPDR
#define PINNOPL (U8)0x00
#define PINPLUP (U8)0x01
#define PINPLDN (U8)0x02
//LCKR
#define PINNOLOCK (U8)0x00
#define PINLOCK   (U8)0x01

#if 0
typedef struct
{
  union
  {
    struct
    {
      U16 PORT:4,PINNUMBER:4,MODE:2,OT:1,OSPEED:2,PUPD:2,LCK:1,AF:4,OUT:1;
    }BIT;
    U16 VALUE;
  };
}PIN;
#endif
#if 1
typedef struct
{
  union
  {
    struct
    {
      U16 MODE:2,OT:1,OSPEED:2,PUPD:2,LCK:1,AF:4,OUT:1;
    }BIT;
    U16 VALUE;
  };
}PIN;
#endif

#if 0
typedef struct
{
   volatile U16 PORT:4,PINNUMBER:4,MODE:2,OT:1,OSPEED:2,PUPD:2,LCK:1,AF:4;
}PIN;
#endif 

typedef union
{
  volatile U32 VALUE;
  struct
  {
    volatile U32 BS0:1,BS1:1,BS2:1,BS3:1,BS4:1,BS5:1,BS6:1,BS7:1,BS8:1,BS9:1,BS10:1,BS11:1,BS12:1,BS13:1,BS14:1,BS15:1,BR0:1,BR1:1,BR2:1,BR3:1,BR4:1,BR5:1,BR6:1,BR7:1,BR8:1,BR9:1,BR10:1,BR11:1,BR12:1,BR13:1,BR14:1,BR15:1;
  }BIT;
}BSRRVAL;
extern void GPIO_PINCONFIG(GPIO_BYTES *PORT,U8 PINNUM, PIN PINSETTINGS);
extern void GPIO_PININIT(PIN PINSETTINGS);
extern void GPIO_BITSSETRESET1(GPIO_BYTES *PORT,BSRRVAL VALUE);
extern void GPIOSEITCHTEST(GPIO_BYTES *PORT, U32 PERIOD,U8 PINNUMBER);
extern void GPIOSETPINSTATE(GPIO_BYTES *PORT,U8 PINNUMBER,U8 STATE);
#endif//defined(GPIOV2)

#endif//_GPIOV2_H_

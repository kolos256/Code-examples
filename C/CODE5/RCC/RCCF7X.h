/******************************************************************************
*DESIGNED FOR STM32F303CB
*
*******************************************************************************/
#ifndef _RCCF7_H_
#define _RCCF7_H_

#include "CORE\CORE.h"
#ifdef USECONF
#include "CONF.h"
#include "devlist.h"
#endif//USECONF
//???????? ???????

#if defined(STM32F7X)

typedef struct
{
  union
  {
    volatile U32 VALUE;
	struct
	{
	  volatile U32 HSION:1,HSIRDY:1,:1,HSITRIM:5,HSICAL:8,HSEON:1,HSERDY:1,HSEBYP:1,CSSON:1,:4,PLLON:1,PLLRDY:1,PLLI2SON:1,PLLI2SRDY:1,PLLSAION:1,PLLSAIRDY:1;
    }BIT;
  }CR;
  union
  {
    volatile U32 VALUE;
    struct
    {
      volatile U32 PLLM:6,PLLN:9,:1,PLLP:2,:4,PLLSRC:1,:1,PLLQ:4;
    }BIT;
  }PLLCFGR;
  union
  {
    volatile U32 VALUE;
    struct
    {
      volatile U32 SW:2,SWS:2,HPRE:4,:2,PPRE1:3,PPRE2:3,RTCPRE:5,MCO1:2,I2SSRC:1,MCO1PRE:3,MCO2PRE:3,MCO2:2;
    }BIT;
  }CFGR;
  union
  {
    volatile U32 VALUE;
    struct
    {
      volatile U32 LSIRDYF:1,LSERDYF:1,HSIRDYF:1,HSERDYF:1,PLLRDYF:1,PLLI2SRDY:1,PLLSAIRDY:1,CSSF:1,LSIRDYIE:1,LSERDYIE:1,HSIRDYIE:1,HSERDYIE:1,PLLRDYIE:1,PLLI2SRDYIE:1,PLLSAIRDYIE:1,:1,LSIRDYC:1,LSERDYC:1,HSIRDYC:1,HSERDYC:1,PLLRDYC:1,PLLI2SRDYC:1,PLLSAIRDYC:1,CSSC:1;
    }BIT;
  }CIR;
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 GPIOARST:1,GPIOBRST:1,GPIOCRST:1,GPIODRST:1,GPIOERST:1,GPIOFRST:1,GPIOGRST:1,GPIOHRST:1,GPIOIRST:1,GPIOJRST:1,GPIOKRST:1,:1,CRCRST:1,:8,DMA1RST:1,DMA2RST:1,DMA2DRST:1,:1,ETHMACRST:1,:3,OTGHSRST:1;
    }BIT;
  }AHB1RSTR;
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 DCMIRST:1,:3,CRYPRST:1,HASHRST:1,RNGRST:1,OTGFSRST:1;
    }BIT;
  }AHB2RSTR;
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 FMCRST:1,QSPIRST:1;
    }BIT;
  }AHB3RSTR;
  const    U32 RESERVED0;     /*!< Reserved, 0x1C                                                                    */
  union
  {
    volatile U32 VALUE;
    struct
    {
      volatile U32 TIM2RST:1,TIM3RST:1,TIM4RST:1,TIM5RST:1,TIM6RST:1,TIM7RST:1,TIM12RST:1,TIM13RST:1,TIM14RST:1,LPTIM1RST:1,:1,WWDGRST:1,:2,SPI2RST:1,SPI3RST:1,SPDIFRXRST:1,USART2RST:1,USART3RST:1,USART4RST:1,USART5RST:1,I2C1RST:1,I2C2RST:1,I2C3RST:1,I2C4RST:1,CAN1RST:1,CAN2RST:1,CECRST:1,PWRRST:1,DACRST:1,USART7RST:1,USART8RST:1;
    }BIT;
  }APB1RSTR;
  union
  {
    volatile U32 VALUE;
    struct
    {
      volatile U32 TIM1RST:1,TIM8RST:1,:2,USART1RST:1,USART6RST:1,:2,ADCRST:1,:2,SDMMC1RST:1,SPI1RST:1,SPI4RST:1,SYSCFGRST:1,:1,TIM9RST:1,TIM10RST:1,TIM11RST:1,:1,SPI5RST:1,SPI6RST:1,SAI1RST:1,SAI2RST:1,:2,LTDCRST:1;
    }BIT;
  }APB2RSTR;
  const    U32 RESERVED1[2];  /*!< Reserved, 0x28-0x2C                                                               */
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 GPIOAEN:1,GPIOBEN:1,GPIOCEN:1,GPIODEN:1,GPIOEEN:1,GPIOFEN:1,GPIOGEN:1,GPIOHEN:1,GPIOIEN:1,GPIOJEN:1,GPIOKEN:1,:1,CRCEN:1,:5,BKPSRAMEN:1,:1,DTCMRAMEN:1,DMA1EN:1,DMA2EN:1,DMA2DEN:1,:1,ETHMACEN:1,ETHMACTXEN:1,ETHMACRXEN:1,ETHMACPTPEN:1,OTGHSEN:1,OTGHSULPIEN:1;
    }BIT;
  }AHB1ENR;
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 DCMIEN:1,:3,CRYPEN:1,HASHEN:1,RNGEN:1,OTGFSEN:1;
    }BIT;
  }AHB2ENR;
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 FMCEN:1,QSPIEN:1;
    }BIT;
  }AHB3ENR;
  const    U32 RESERVED2;/*!< Reserved, 0x3C                                                                    */
  union
  {
    volatile U32 VALUE;
    struct
    {
      volatile U32 TIM2EN:1,TIM3EN:1,TIM4EN:1,TIM5EN:1,TIM6EN:1,TIM7EN:1,TIM12EN:1,TIM13EN:1,TIM14EN:1,LPTIM1EN:1,:1,WWDGEN:1,:2,SPI2EN:1,SPI3EN:1,SPDIFRXEN:1,USART2EN:1,USART3EN:1,USART4EN:1,USART5EN:1,I2C1EN:1,I2C2EN:1,I2C3EN:1,I2C4EN:1,CAN1EN:1,CAN2EN:1,CECEN:1,PWREN:1,DACEN:1,USART7EN:1,USART8EN:1;
    }BIT;
  }APB1ENR;
  union
  {
    volatile U32 VALUE;
    struct
    {
      volatile U32 TIM1EN:1,TIM8EN:1,:2,USART1EN:1,USART6EN:1,:2,ADC1EN:1,ADC2EN:1,ADC3EN:1,SDMMC1EN:1,SPI1EN:1,SPI4EN:1,SYSCFGEN:1,:1,TIM9EN:1,TIM10EN:1,TIM11EN:1,:1,SPI5EN:1,SPI6EN:1,SAI1EN:1,SAI2EN:1,:2,LTDCEN:1;
    }BIT;
  }APB2ENR;
  const    U32 RESERVED3[2];  /*!< Reserved, 0x48-0x4C                                                               */
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 GPIOALPEN:1,GPIOBLPEN:1,GPIOCLPEN:1,GPIODLPEN:1,GPIOELPEN:1,GPIOFLPEN:1,GPIOGLPEN:1,GPIOHLPEN:1,GPIOILPEN:1,GPIOJLPEN:1,GPIOKLPEN:1,:1,CRCLPEN:1,AXILPEN:1,FLITFLPEN:1,SRAM1LPEN:1,SRAM2LPEN:1,BKPSRAMLPEN:1,:1,DTCMRAMLPEN:1,DMA1LPEN:1,DMA2LPEN:1,DMA2DLPEN:1,:1,ETHMACLPEN:1,ETHMACTXLPEN:1,ETHMACRXLPEN:1,ETHMACPTPLPEN:1,OTGHSLPEN:1,OTGHSULPLPEN:1;
    }BIT;
  }AHB1LPENR;
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 DCMILPEN:3,CRYPLPEN:1,HASHLPEN:1,RNGLPEN:1,OTGFSLPEN:1;
    }BIT;
  }AHB2LPENR;
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 FMCLPEN:1,QSPILPEN:1;
    }BIT;
  }AHB3LPENR;
  const    U32 RESERVED4;     /*!< Reserved, 0x5C                                                                    */
  union
  {
    volatile U32 VALUE;
    struct
    {
      volatile U32 TIM2LPEN:1,TIM3LPEN:1,TIM4LPEN:1,TIM5LPEN:1,TIM6LPEN:1,TIM7LPEN:1,TIM12LPEN:1,TIM13LPEN:1,TIM14LPEN:1,LPTIM1LPEN:1,:1,WWDGLPEN:1,:2,SPI2LPEN:1,SPI3LPEN:1,SPDIFRXLPEN:1,USART2LPEN:1,USART3LPEN:1,USART4LPEN:1,USART5LPEN:1,I2C1LPEN:1,I2C2LPEN:1,I2C3LPEN:1,I2C4LPEN:1,CAN1LPEN:1,CAN2LPEN:1,CECLPEN:1,PWRLPEN:1,DACLPEN:1,USART7LPEN:1,USART8LPEN:1;
    }BIT;
  }APB1LPENR;
  union
  {
    volatile U32 VALUE;
    struct
    {
      volatile U32 TIM1LPEN:1,TIM8LPEN:1,:2,USART1LPEN:1,USART6LPEN:1,:2,ADC1LPEN:1,ADC2LPEN:1,ADC3LPEN:1,SDMMC1LPEN:1,SPI1LPEN:1,SPI4LPEN:1,SYSCFGLPEN:1,:1,TIM9LPEN:1,TIM10LPEN:1,TIM11LPEN:1,:1,SPI5LPEN:1,SPI6LPEN:1,SAI1LPEN:1,SAI2LPEN:1,:2,LTDCLPEN:1;
    }BIT;
  }APB2LPENR;
  const    U32 RESERVED5[2];  /*!< Reserved, 0x68-0x6C                                                               */
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 LSEON:1,LSERDY:1,LSEBYP:1,LSEDRV:2,:3,RTCSEL:2,:5,RTCEN:1,BDRST:1;
    }BIT;
  }BDCR;
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 LSION:1,LSIRDY:1,:22,RMVF:1,BORRSTF:1,PINRSTF:1,PORRSTF:1,SFTRSTF:1,IWDGRSTF:1,WWDGRSTF:1,LPWRRSTF:1;
    }BIT;
  }CSR;
  const    U32 RESERVED6[2];  /*!< Reserved, 0x78-0x7C                                                               */
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 MODPER:13,INCSTEP:15,:2,SPREADSEL:1,SSCGEN:1;
    }BIT;
  }SSCGR;
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 :6,PLLI2SN:9,:1,PLLI2SP:2,:6,PLLI2SQ:4,PLLI2SR:3;
    }BIT;
  }PLLI2SCFGR;
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 :6,PLLSAIN:9,:1,PLLSAIP:2,:6,PLLSAIQ:4,PLLSAIR:3;
    }BIT;
  }PLLSAICFGR;
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 PLLI2SDIVQ:5,:3,PLLSAIDIVQ:5,:3,PLLSAIDIVR:2,SAI1SEL:2,SAI2SEL:2,TIMPRE:2;
    }BIT;
  }DCKCFGR1;
  union
  {
    volatile U32 VALUE;
    struct
    {
       volatile U32 UART1SEL:2,UART2SEL:2,UART3SEL:2,UART4SEL:2,UART5SEL:2,UART6SEL:2,UART7SEL:2,I2C1SEL:2,I2C2SEL:2,I2C3SEL:2,I2C4SEL:2,LPTIM1SEL:2,CECSEL:2,CK48MSEL:2,SDMMCSEL:2;
    }BIT;
  }DCKCFGR2;
}RCC_BYTES;

typedef enum 
{
   RESET,
   UNRESET,
   CLKEN,
   CLKDIS
}CMDTYPE;

typedef enum {APB2RST,APB1RST,AHB3RST,AHB2RST,AHB1RST}RESETREG;
typedef enum 
{
   //AHB1
   GPIOARST=(1<<0),
   GPIOBRST=(1<<1),
   GPIOCRST=(1<<2),
   GPIODRST=(1<<3),
   GPIOERST=(1<<4),
   GPIOFRST=(1<<5),
   GPIOGRST=(1<<6),
   GPIOHRST=(1<<7),
   GPIOIRST=(1<<8),
   GPIOJRST=(1<<9),
   GPIOKRST=(1<<10),
   CRCRST=(1<<12),
   DMA1RST=(1<<21),
   DMA2RST=(1<<22),
   DMA2DRST=(1<<23),
   ETHMACRST=(1<<25),
   OTGHSRST=(1<<29),
   //AHB2
   DCMIRST=(1<<0),
   CRYPRST=(1<<4),
   HASHRST=(1<<5),
   RNGRST=(1<<6),
   OTGFSRST=(1<<7),
   //AHB3
   FSMCRST=(1<<0),
   QSPIRST=(1<<1),
   //APB1
   TIM2RST=(1<<0),
   TIM3RST=(1<<1),
   TIM4RST=(1<<2),
   TIM5RST=(1<<3),
   TIM6RST=(1<<4),
   TIM7RST=(1<<5),
   TIM12RST=(1<<6),
   TIM13RST=(1<<7),
   TIM14RST=(1<<8),
   WWDGRST=(1<<11),
   SPI2RST=(1<<14),
   SPI3RST=(1<<15),
   SPDIFRXRST3RST=(1<<16),
   UART2RST=(1<<17),
   UART3RST=(1<<18),
   UART4RST=(1<<19),
   UART5RST=(1<<20),
   I2C1RST=(1<<21),
   I2C2RST=(1<<22),
   I2C3RST=(1<<23),
   I2C4RST=(1<<24),
   CAN1RST=(1<<25),
   CAN2RST=(1<<26),
   CECRST=(1<<27),
   PWRRST=(1<<28),
   DACRST=(1<<29),
   USART7RST=(1<<30),
   USART8RST=(1<<31),
   //APB2
   TIM1RST=(1<<0),
   TIM8RST=(1<<1),
   USART1RST=(1<<4),
   USART6RST=(1<<5),
   ADCRST=(1<<8),
   SDMMC1RST=(1<<11),
   SPI1RST=(1<<12),
   SPI4RST=(1<<13),
   SYSCFGRST=(1<<14),
   TIM9RST=(1<<16),
   TIM10RST=(1<<17),
   TIM11RST=(1<<18),
   SPI5RST=(1<<20),
   SPI6RST=(1<<21),
   SAI1RST=(1<<22),
   SAI2RST=(1<<23),
   LTDCRST=(1<<23)
}RESETPERIPH;

typedef enum {APB2EN,APB1EN,AHB3EN,AHB2EN,AHB1EN}ENABLEREG;
typedef enum 
{
   //AHB1
   GPIOAEN=(1<<0),
   GPIOBEN=(1<<1),
   GPIOCEN=(1<<2),
   GPIODEN=(1<<3),
   GPIOEEN=(1<<4),
   GPIOFEN=(1<<5),
   GPIOGEN=(1<<6),
   GPIOHEN=(1<<7),
   GPIOIEN=(1<<8),
   GPIOJEN=(1<<9),
   GPIOKEN=(1<<10),
   CRCEN=(1<<12),
   BKPSRAMEN=(1<<18),
   DTCMRAMEN=(1<<20),
   DMA1EN=(1<<21),
   DMA2EN=(1<<22),
   DMA2DEN=(1<<23),
   ETHMACEN=(1<<25),
   ETHMACTXEN=(1<<26),
   ETHMACRXEN=(1<<27),
   ETHMACPTPEN=(1<<28),
   OTGHSEN=(1<<29),
   OTGHSULPIEN=(1<<30),
   //AHB2
   DCMIEN=(1<<0),
   CRYPEN=(1<<4),
   HASHEN=(1<<5),
   RNGEN=(1<<6),
   OTGFSEN=(1<<7),
   //AHB3
   FSMCEN=(1<<0),
   QSPIEN=(1<<1),
   //APB1
   TIM2EN=(1<<0),
   TIM3EN=(1<<1),
   TIM4EN=(1<<2),
   TIM5EN=(1<<3),
   TIM6EN=(1<<4),
   TIM7EN=(1<<5),
   TIM12EN=(1<<6),
   TIM13EN=(1<<7),
   TIM14EN=(1<<8),
   WWDGEN=(1<<11),
   SPI2EN=(1<<14),
   SPI3EN=(1<<15),
   SPDIFRXEN3EN=(1<<16),
   UART2EN=(1<<17),
   UART3EN=(1<<18),
   UART4EN=(1<<19),
   UART5EN=(1<<20),
   I2C1EN=(1<<21),
   I2C2EN=(1<<22),
   I2C3EN=(1<<23),
   I2C4EN=(1<<24),
   CAN1EN=(1<<25),
   CAN2EN=(1<<26),
   CECEN=(1<<27),
   PWREN=(1<<28),
   DACEN=(1<<29),
   USART7EN=(1<<30),
   USART8EN=(1<<31),
   //APB2
   TIM1EN=(1<<0),
   TIM8EN=(1<<1),
   USART1EN=(1<<4),
   USART6EN=(1<<5),
   ADCEN=(1<<8),
   SDMMC1EN=(1<<11),
   SPI1EN=(1<<12),
   SPI4EN=(1<<13),
   SYSCFGEN=(1<<14),
   TIM9EN=(1<<16),
   TIM10EN=(1<<17),
   TIM11EN=(1<<18),
   SPI5EN=(1<<20),
   SPI6EN=(1<<21),
   SAI1EN=(1<<22),
   SAI2EN=(1<<23),
   LTDCEN=(1<<26)
}ENABLEPERIPH;


typedef enum {HSISRC=0,HSESRC=1,PLLSRC=2}SYSSRC;

typedef enum {PLLHSISRC=0,PLLHSESRC=1}PLL_SRC;
typedef enum {PLLP_2=0,PLLP_4=1,PLLP_6=1,PLLP_8=1}PLLP_DIV;


typedef enum {APB_DIV1=0,APB_DIV2=4,APB_DIV4=5,APB_DIV8=6,APB_DIV16=7}APB_DIV;
typedef enum {AHB_DIV1=0,AHB_DIV2=8,AHB_DIV4=9,AHB_DIV8=10,AHB_DIV16=11,AHB_DIV64=12,AHB_DIV128=13,AHB_DIV256=14,AHB_DIV512=15}AHB_DIV;

//#define RCC_BASE 0x40023800

#ifdef RCC_BASE
#define RCC (*((RCC_BYTES*)RCC_BASE))

extern void RCC_PERIPHFULRST(RESETPERIPH PERIPH,RESETREG REG,U32 TIME);
//extern void RCC_PERIPHRESETCMD(CMDTYPE CMD,RESETPERIPH PERIPH,RESETREG REG)
extern void RCC_PERIPHRESETCMD(State CMD,RESETREG REG,RESETPERIPH PERIPH);
extern void RCC_PERIPHCLKENCMD(State CMD,ENABLEREG REG,ENABLEPERIPH PERIPH);

extern CMDRES HSICMD(U16 TIMEOUT,State CMD);
extern CMDRES HSECMD(U16 TIMEOUT,State CMD);

extern void HSEBYPASS(State BYP);
extern void MAINPLLINIT(PLL_SRC SRC,U16 PLLN,PLLP_DIV PLLP,U8 PLLQ,U8 PLLM);
extern void APBAHBDIV(APB_DIV APB1,APB_DIV APB2,AHB_DIV AHB);

extern CMDRES PWR_OVDMODEN(U16 TIMEOUT);
extern CMDRES PLLCMD(State CMD,U16 TIMEOUT);

extern CMDRES SYSCLKSW(SYSSRC SOURSE,U16 TIMEOUT);

extern void USARTSETCLKSRC(U8 CLKSRC);

//pre1 divfactors
#define PRE1_DIV1   0x10
#define PRE1_DIV2   0x11
#define PRE1_DIV4   0x12
#define PRE1_DIV6   0x13
#define PRE1_DIV8   0x14
#define PRE1_DIV10  0x15
#define PRE1_DIV12  0x16
#define PRE1_DIV16  0x17
#define PRE1_DIV32  0x18
#define PRE1_DIV64  0x19
#define PRE1_DIV128 0x1A
#define PRE1_DIV256 0x1B

extern void SETADC12PRE(U8 PRE1);
extern void SETADC34PRE(U8 PRE1);

#endif//RCC_BASE

#endif//defined(STM32F7X)

#endif//_RCCF7_H_

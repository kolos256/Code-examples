#ifndef _ADCF030X_H_
#define _ADCF030X_H_

#include "CORE\CORE.h"
#ifdef USECONF
#include "CONF.h"
#endif//USECONF
#include "STM32\TIME\ticks.h"

//SMPR
#define ADC_1_5   0x00000000
#define ADC_7_5   0x00000001
#define ADC_13_5  0x00000002
#define ADC_28_5  0x00000003
#define ADC_41_5  0x00000004 
#define ADC_55_5  0x00000005
#define ADC_71_5  0x00000006
#define ADC_239_5 0x00000007
//CHEN
#define ADC_CH0SEL  0x00000001
#define ADC_CH1SEL  0x00000002
#define ADC_CH2SEL  0x00000004
#define ADC_CH3SEL  0x00000008
#define ADC_CH4SEL  0x00000010
#define ADC_CH5SEL  0x00000020
#define ADC_CH6SEL  0x00000040
#define ADC_CH7SEL  0x00000080
#define ADC_CH8SEL  0x00000100
#define ADC_CH9SEL  0x00000200
#define ADC_CH10SEL 0x00000400
#define ADC_CH11SEL 0x00000800
#define ADC_CH12SEL 0x00001000
#define ADC_CH13SEL 0x00002000
#define ADC_CH14SEL 0x00004000
#define ADC_CH15SEL 0x00008000
#define ADC_CH16SEL 0x00010000
#define ADC_CH17SEL 0x00020000 
//CKMODE
#define ADC_ASYNKCLK 0x00000000
#define ADC_PCLKDIV2 0x00000001
#define ADC_PCLKDIV4 0x00000002
typedef struct
{
  union
  {
    U32 VALUE;
    struct
    {
      U32 ADRDY:1,EOSMP:1,EOC:1,EOSEQ:1,OVR:1,:2,AWD:1;
    }BIT;
  }ISR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 ADRDYIE:1,EOSMPIE:1,EOCIE:1,EOSEQIE:1,OVRIE:1,:2,AWDIE:1;
    }BIT;
  }IER;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 ADEN:1,ADDIS:1,ADSTART:1,:1,ADSTP:1,:26,ADCAL:1;
    }BIT;
  }CR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 DMAEN:1,DMACFG:1,SCANDIR:1,RES:2,ALIGN:1,EXTSEL:3,:1,EXTEN:2,OVRMOD:1,CONT:1,WAIT:1,AUTOFF:1,DISCEN:1,:5,AWDSGL:1,AWDEN:1,:2,AWDCH:5;
    }BIT;
  }CFGR1;
  
  union
  {
    U32 VALUE;
    struct
    {
     U32 :30,CKMODE:2;
    }BIT;
  }CFGR2;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 SMP:3;
    }BIT;
  }SMPR;
  const U32 RESERVED0[2];
  union
  {
    U32 VALUE;
    struct
    {
      U32 LT:12,:4,HT:12;
    }BIT;
  }TR;
  const U32 RESERVED1[1];
  union
  {
    U32 VALUE;
    struct
    {
      U32 CHSEL0:1,CHSEL1:1,CHSEL2:1,CHSEL3:1,CHSEL4:1,CHSEL5:1,CHSEL6:1,CHSEL7:1,CHSEL8:1,CHSEL9:1,CHSEL10:1,CHSEL11:1,CHSEL12:1,CHSEL13:1,CHSEL14:1,CHSEL15:1,CHSEL16:1,CHSEL17:1;
    }BIT;
  }CHSELR;
  const U32 RESERVED2[5];
  union
  {
    U32 VALUE;
    U16 DATA16;
    U8 DATA8;
    struct
    {
      U32 DATA:16;
    }BIT;
  }DR;
  const U32 RESERVED3[177];
  union
  {
    U32 VALUE;
    U16 DATA16;
    U8 DATA8;
    struct
    {
    U32 :22,VREFEN:1,TSEN:1;
    }BIT;
  }CCR;
}ADC_BYTES;


#ifdef ADC1_BASE
#define ADC1 (*((ADC_BYTES*)ADC1_BASE))
#endif
#ifdef ADC2_BASE
#define ADC2 (*((ADC_BYTES*)ADC2_BASE))
#endif
#ifdef ADC3_BASE
#define ADC3 (*((ADC_BYTES*)ADC3_BASE))
#endif
#ifdef ADC4_BASE
#define ADC4 (*((ADC_BYTES*)ADC4_BASE))
#endif
#ifdef ADC5_BASE
#define ADC12 (*((ADC_COMON_BYTES*)ADC12_BASE))
#endif
#ifdef ADC6_BASE
#define ADC34 (*((ADC_COMON_BYTES*)ADC34_BASE))
#endif

extern U8 ADC_CAL(ADC_BYTES *ADC,U32 TIMEOUT);
extern U8 ADC_EN(ADC_BYTES *ADC,U32 TIMEOUT);
extern void ADC_START(ADC_BYTES *ADC);
extern U8 ADC_REGSTOP(ADC_BYTES *ADC,U32 TIMEOUT);
void ADC_CONFIGCHANELS(ADC_BYTES *ADCSEL,U32 CHANELS,U8 SMPLTIME);


#endif//_ADCF030X_H_
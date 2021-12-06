#ifndef _ADCV3_H_
#define _ADCV3_H_

#include "CORE\CORE.h"
#ifdef USECONF
#include "CONF.h"
#endif//USECONF
#include "STM32\TIME\ticks.h"

//SMPR
#define ADCV3_15   0x00000000
#define ADCV3_25   0x00000001
#define ADCV3_45   0x00000002
#define ADCV3_75   0x00000003
#define ADCV3_195  0x00000004 
#define ADCV3_615  0x00000005
#define ADCV3_1815 0x00000006
#define ADCV3_6015 0x00000007

typedef struct
{
  union
  {
    U32 VALUE;
    struct
    {
      U32 ADRDY:1,EOSMP:1,EOC:1,EOS:1,OVR:1,JEOC:1,JEOS:1,AWD1:1,AWD2:1,AWD3:1,JQOVF:1;
    }BIT;
  }ISR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 ADRDYIE:1,EOSMPIE:1,EOCIE:1,EOSIE:1,OVRIE:1,JEOCIE:1,JEOSIE:1,AWD1IE:1,AWD2IE:1,AWD3IE:1,JQOVFIE:1;
    }BIT;
  }IER;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 ADEN:1,ADDIS:1,ADSTART:1,JADSTART:1,ADSTP:1,JADSTP:1,:22,ADVREGEN:2,ADCALDIF:1,ADCAL:1;
    }BIT;
  }CR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 DMAEN:1,DMACFG:1,:1,RES:2,ALIGN:1,EXTSEL:4,EXTEN:2,OVRMOD:1,CONT:1,AUTDLY:1,:1,DISCEN:1,DISCNUM:3,JDISCEN:1,JQM:1,AWD1SGL:1,AWD1EN:1,JAWD1EN:1,JAUTO:1,AWD1CH:5;
    }BIT;
  }CFGR;
  const U32 RESERVED0[1];
  union
  {
    U32 VALUE;
    struct
    {
      U32 :3,SMP1:3,SMP2:3,SMP3:3,SMP4:3,SMP5:3,SMP6:3,SMP7:3,SMP8:3,SMP9:3;
    }BIT;
  }SMPR1;
  union
  {
    U32 VALUE;
    struct
    {
      U32 :3,SMP10:3,SMP11:3,SMP12:3,SMP13:3,SMP14:3,SMP15:3,SMP16:3,SMP17:3,SMP18:3;
    }BIT;
  }SMPR2;
  const U32 RESERVED1[1];
  union
  {
    U32 VALUE;
    struct
    {
      U32 LT1:12,:4,HT1:12;
    }BIT;
  }TR1;
  union
  {
    U32 VALUE;
    struct
    {
      U32 LT2:8,:8,HT2:12;
    }BIT;
  }TR2;
  union
  {
    U32 VALUE;
    struct
    {
      U32 LT3:8,:8,HT3:12;
    }BIT;
  }TR3;
  const U32 RESERVED2[1];
  union
  {
    U32 VALUE;
    struct
    {
      U32 L:4,:2,SQ1:5,:1,SQ2:5,:1,SQ3:5,:1,SQ4:5;
    }BIT;
  }SQR1;
  union
  {
    U32 VALUE;
    struct
    {
      U32 SQ5:5,:1,SQ6:5,:1,SQ7:5,:1,SQ8:5,:1,SQ9:5;
    }BIT;
  }SQR2;
  union
  {
    U32 VALUE;
    struct
    {
      U32 SQ10:5,:1,SQ11:5,:1,SQ12:5,:1,SQ13:5,:1,SQ14:5;
    }BIT;
  }SQR3;
  union
  {
    U32 VALUE;
    struct
    {
      U32 SQ15:5,:1,SQ16:5;
    }BIT;
  }SQR4;
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
  const U32 RESERVED3[2];
  union
  {
    U32 VALUE;
    struct
    {
      U32 JL:2,JEXTSEL:4,JEXTEN:2,JSQ1:5,:1,JSQ2:5,:1,JSQ3:5,:1,JSQ4:5;
    }BIT;
  }JSQR;
  const U32 RESERVED4[4];
  union
  {
    U32 VALUE;
    struct
    {
      U32 OFFSET1:12,:14,OFFSET1CH:5,OFFSET1EN:1;
    }BIT;
  }OFR1;
  union
  {
    U32 VALUE;
    struct
    {
      U32 OFFSET2:12,:14,OFFSET2CH:5,OFFSET2EN:1;
    }BIT;
  }OFR2;
  union
  {
    U32 VALUE;
    struct
    {
      U32 OFFSET3:12,:14,OFFSET3CH:5,OFFSET3EN:1;
    }BIT;
  }OFR3;
  union
  {
    U32 VALUE;
    struct
    {
      U32 OFFSET4:12,:14,OFFSET4CH:5,OFFSET4EN:1;
    }BIT;
  }OFR4;
  const U32 RESERVED5[4];
  union
  {
    U32 VALUE;
    struct
    {
      U32 JDATA1:15;
    }BIT;
  }JDR1;
  union
  {
    U32 VALUE;
    struct
    {
      U32 JDATA2:15;
    }BIT;
  }JDR2;
  union
  {
    U32 VALUE;
    struct
    {
      U32 JDATA3:15;
    }BIT;
  }JDR3;
  union
  {
    U32 VALUE;
    struct
    {
      U32 JDATA4:15;
    }BIT;
  }JDR4;
  const U32 RESERVED6[5];
  union
  {
    U32 VALUE;
    struct
    {
      U32 :1,AWD2CH:18;
    }BIT;
  }AWD2CR;
  union
  {
    U32 VALUE;
    struct
    {
      U32 :1,AWD3CH:18;
    }BIT;
  }AWD3CR;
  const U32 RESERVED7[2];
  union
  {
    U32 VALUE;
    struct
    {
      U32 :1,DIFSEL:18;
    }BIT;
  }DIFSEL;
  union
  {
    U32 VALUE;
    struct
    {
      U32 CALFACT_S:7,:9,CALFACT_D:7;
    }BIT;
  }CALFACT;
}ADC_BYTES;

typedef struct
{
  union
  {
    U32 VALUE;
    struct
    {
      U32 ADRDY_MST:1,EOSMP_MST:1,EOC_MST:1,EOS_MST:1,OVR_MST:1,JEOC_MST:1,JEOS_MST:1,AWD1_MST:1,AWD2_MST:1,AWD3_MST:1,JQOVF_MST:1,:5,ADRDY_SLV:1,EOSMP_SLV:1,EOC_SLV:1,EOS_SLV:1,OVR_SLV:1,JEOC_SLV:1,JEOS_SLV:1,AWD1_SLV:1,AWD2_SLV:1,AWD3_SLV:1,JQOVF_SLV:1;
    }BIT;
  }CSR;
  const U32 RESERVED1[1];
  union
  {
    U32 VALUE;
    struct
    {
      U32 DUAL:5,:3,DELAY:4,:1,DMACFG:1,MDMA:2,CKMODE:2,:4,VREFEN:1,TSEN:1,VBAT:1;
    }BIT;
  }CCR;

  union
  {
    U32 VALUE;
    struct
    {
      U32 RDATA_MST:16,RDATA_SLV:16;
    }BIT;
  }CDR;

}ADC_COMON_BYTES;

#ifdef ADC1V3
#define ADC1 (*((ADC_BYTES*)ADC1_BASE))
#endif
#ifdef ADC2V3
#define ADC2 (*((ADC_BYTES*)ADC2_BASE))
#endif
#ifdef ADC3V3
#define ADC3 (*((ADC_BYTES*)ADC3_BASE))
#endif
#ifdef ADC4V3
#define ADC4 (*((ADC_BYTES*)ADC4_BASE))
#endif
#ifdef ADC12V3
#define ADC12 (*((ADC_COMON_BYTES*)ADC12_BASE))
#endif
#ifdef ADC34V3
#define ADC34 (*((ADC_COMON_BYTES*)ADC34_BASE))
#endif

extern U8 ADC_ADVREGCMD(ADC_BYTES *ADC, State CMD,U32 TIMEOUT);
extern U8 ADC_CAL(ADC_BYTES *ADC,U32 TIMEOUT);
extern U8 ADC_EN(ADC_BYTES *ADC,U32 TIMEOUT);
extern void ADC_REGSTART(ADC_BYTES *ADC);
extern U8 ADC_REGSTOP(ADC_BYTES *ADC,U32 TIMEOUT);
extern void ADC_INJSTART(ADC_BYTES *ADC);
extern void ADC_REGCHENNALS(ADC_BYTES *ADCSEL,U8 LENGTH);
extern void ADCREGCHSET(ADC_BYTES *ADCSEL,U8 SEQNUM,U8 ADCIN,U8 SMPTIME);
extern U8 ADC_INJSTOP(ADC_BYTES *ADC,U32 TIMEOUT);

#endif//_ADCV3_H_
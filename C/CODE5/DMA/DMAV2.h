#ifndef _DMAV2_H_
#define _DMAV2_H_
/******************************************************************************
*!!!!!!!!
*Следует быть осторожным при использовании DMAV2_BYTES при попытке проверки 
*битов регистра CR происходит fault предположительно из за попытки чтения регистра
*не чрез команду чтения 32бит а через команду чтения 16 или 8 бит что запрещено по даташиту
*******************************************************************************/

#include "CORE\CORE.h"
#ifdef USECONF
#include "CONF.h"
#endif//USECONF

//CR
#define DMA_CHSEL(VALUE) ((VALUE)<<25)
#define DMA_MSNGTR 0x00000000
#define DMA_MINCR4 0x00800000
#define DMA_MINCR8 0x01000000
#define DMA_MINCR16 0x01800000
#define DMA_PSNGTR 0x00000000
#define DMA_PINCR4 0x00200000
#define DMA_PINCR8 0x00400000
#define DMA_PINCR16 0x00600000
#define DMA_CTM0 0x00000000
#define DMA_CTM1 0x00080000
#define DMA_DBM 0x00040000
#define DMA_PLOW 0x00000000
#define DMA_PMED 0x00010000
#define DMA_PHI 0x00020000
#define DMA_PVHI 0x00030000
#define DMA_PINCOS 0x00008000
#define DMA_M8BIT 0x00000000
#define DMA_M16BIT 0x00002000
#define DMA_M32BIT 0x00004000
#define DMA_P8BIT 0x00000000
#define DMA_P16BIT 0x00000800
#define DMA_P32BIT 0x00001000
#define DMA_MINC 0x00000400
#define DMA_PINC 0x00000200
#define DMA_CIRC 0x00000100
#define DMA_DIRPTOM 0x00000000
#define DMA_DIRMTOP 0x00000040
#define DMA_DIRMTOM 0x00000080
#define DMA_PFCTRL 0x00000020
#define DMA_TCIE 0x00000010
#define DMA_HTIE 0x00000008
#define DMA_TEIE 0x00000004
#define DMA_DMEIE 0x00000002
#define DMA_EN 0x00000001
//FCR
#define DMA_FEIE 0x00000080
#define DMA_DMDIS 0x00000004
#define DMA_FIFO_14 0x00000000
#define DMA_FIFO_12 0x00000001
#define DMA_FIFO_34 0x00000002
#define DMA_FIFO 0x00000003

//FLAGS
//errors flags
#define DMA_FEFLG  0x01
#define DMA_DMEFLG 0x04
#define DMA_TEFLG  0x08
//state flags
#define DMA_HTFLG  0x10
#define DMA_TCFLG  0x20

volatile typedef struct
{
  volatile union
  {
    volatile U32 VALUE;
    volatile struct
    {
      volatile U32 EN:1,DMEIE:1,TEIE:1,HTIE:1,TCIE:1,PFCTRL:1,DIR:2,CIRC:1,PINC:1,MINC:1,PSIZE:2,MSIZE:2,PINCOS:1,PL:2,DBM:1,CT:1,:1,PBURST:2,MBURST:2,CHSEL:3;
    }BIT;
  }CR;
  volatile union
  {
    volatile U32 VALUE;
    volatile struct
    {
      volatile U32 NDT:16;
    }BIT;
  }NDTR;
  volatile union
  {
    volatile U32 VALUE;
    struct
    {
      volatile U32 PAR:32;
    }BIT;
  }PAR;
  volatile union
  {
    volatile U32 VALUE;
    struct
    {
      volatile U32 M0A:32;
    }BIT;
  }M0AR;
  volatile union
  {
    volatile U32 VALUE;
    struct
    {
      volatile U32 M1A:32;
    }BIT;
  }M1AR;
  volatile union
  {
    volatile U32 VALUE;
    volatile struct
    {
      volatile U32 FTH:2,DMDIS:1,FS:3,:1,FEIE:1;
    }BIT;
  }FCR;
}DMAV2_STREAMREGS;


typedef struct
{
  union
  {
    U32 VALUE;
    struct
    {
      U32 FEIF0:1,:1,DMEIF0:1,TEIF0:1,HTIF0:1,TCIF0:1,FEIF1:1,:1,DMEIF1:1,TEIF1:1,HTIF1:1,TCIF1:1,:4,FEIF2:1,:1,DMEIF2:1,TEIF2:1,HTIF2:1,TCIF2:1,FEIF3:1,:1,DMEIF3:1,TEIF3:1,HTIF3:1,TCIF3:1;
    }BIT;
  }LISR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 FEIF4:1,:1,DMEIF4:1,TEIF4:1,HTIF4:1,TCIF4:1,FEIF5:1,:1,DMEIF5:1,TEIF5:1,HTIF5:1,TCIF5:1,:4,FEIF6:1,:1,DMEIF6:1,TEIF6:1,HTIF6:1,TCIF6:1,FEIF7:1,:1,DMEIF7:1,TEIF7:1,HTIF7:1,TCIF7:1;
    }BIT;
  }HISR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 CFEIF0:1,:1,CDMEIF0:1,CTEIF0:1,CHTIF0:1,CTCIF0:1,CFEIF1:1,:1,CDMEIF1:1,CTEIF1:1,CHTIF1:1,CTCIF1:1,:4,CFEIF2:1,:1,CDMEIF2:1,CTEIF2:1,CHTIF2:1,CTCIF2:1,CFEIF3:1,:1,CDMEIF3:1,CTEIF3:1,CHTIF3:1,CTCIF3:1;
    }BIT;
  }LIFCR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 CFEIF4:1,:1,CDMEIF4:1,CTEIF4:1,CHTIF4:1,CTCIF4:1,CFEIF5:1,:1,CDMEIF5:1,CTEIF5:1,CHTIF5:1,CTCIF5:1,:4,CFEIF6:1,:1,CDMEIF6:1,CTEIF6:1,CHTIF6:1,CTCIF6:1,CFEIF7:1,:1,CDMEIF7:1,CTEIF7:1,CHTIF7:1,CTCIF7:1;
    }BIT;
  }HIFCR;
  DMAV2_STREAMREGS STREAM[7];
}DMAV2_BYTES;

#ifdef DMA1V2
#define DMA1 (*((DMAV2_BYTES*)DMA1_BASE))

#endif//DMA1V2

#ifdef DMA2V2
#define DMA2 (*((DMAV2_BYTES*)DMA2_BASE))

#define DMA2_STREAMEN(STR) DMA2.STREAM[STR].CR.BIT.EN=1;
#define DMA2_STREAMDIS(STR) DMA2.STREAM[STR].CR.BIT.EN=0;\
        while(BB_ACCESS((DMA2_BASE+0x10+(STR*24)),0));
      //while(DMA2.STREAM[STR].CR.VALUE&BIT0);


#define DMA2_STREAMINIT(STR,CRVAL,FCRVAL,NDTRVAL,PARVAL,M0ARVAL,M1ARVAL)\
DMA2.STREAM[STR].CR.VALUE=(CRVAL);\
DMA2.STREAM[STR].FCR.VALUE=(FCRVAL);\
DMA2.STREAM[STR].NDTR.VALUE=(NDTRVAL);\
DMA2.STREAM[STR].PAR.VALUE=(PARVAL);\
DMA2.STREAM[STR].M0AR.VALUE=(M0ARVAL);\
DMA2.STREAM[STR].M1AR.VALUE=(M1ARVAL);
        
#endif//DMA2V2

extern void DMASTREAMINIT(DMAV2_BYTES* DMASEL,U8 STR,U32 CRVAL,U32 FCRVAL,U32 NDTRVAL,U32 PARVAL,U32 M0ARVAL,U32 M1ARVAL);
extern void DMASTREAMENCMD(DMAV2_BYTES* DMASEL,U8 Stream,State CMD);
extern void DMATRANSFERINIT(DMAV2_BYTES* DMASEL,U8 STR,void *MEMADDR,void *PERIPHADDR,U32 TRANSFERS);
extern U8 DMASTREAMGETFLAGS(DMAV2_BYTES* DMASEL,U8 STR);
extern void DMASTREAMCLEARFLAGS(DMAV2_BYTES* DMASEL,U8 STR,U8 FLAGS);

#endif//_DMAV2_H_
#ifndef _SPII2SV2_H_
#define _SPII2SV2_H_

#include "CORE\CORE.h"

#ifdef USECONF
#include "CONF.h"
#endif//USECONF

#define SPI_MULTITASKING1

#include "STM32\PERIPH\DMA\DMAV2.h"

#ifdef SPI_MULTITASKING1
  #include "STM32\MULTITASKINGV1\MULTITASKINGV1.h"
#endif

//cr1
#define SPII2SV2_DIV(DIV) ((DIV)<<3)

typedef struct
{
  union
  {
    volatile U32 VALUE;
    struct
    {
      volatile  U32 CPHA:1,CPOL:1,MSTRT:1,BR:3,SPE:1,LSBFIRST:1,SSI:1,SSM:1,RXONLY:1,CRCL:1,CRCNEXT:1,CRCEN:1,BIDIMODE:1;
    }BIT;
  }CR1;
  
  union
  {
    volatile U32 VALUE;
    struct
    {
      volatile U32 RXDMAEN:1,TXDMAEN:1,SSOE:1,NSSP:1,FRF:1,ERRIE:1,RXNEIE:1,TXEIE:1,DS:4,FRXTH:1,LDMA_RX:1,LDMA_TX:1;
    }BIT;
  }CR2;
  
  union
  {
    volatile U32 VALUE;
    struct
    {
      volatile U32 RXNE:1,TXE:1,CHSIDE:1,UDR:1,CRCERR:1,MODF:1,OVR:1,BSY:1,FRE:1,FRLVL:2,FTLVL:2;
    }BIT;
  }SR;
  
  union
  {
    volatile U32 VALUE;
    volatile U8 DATA8;
    volatile U16 DATA16;
    struct
    {
      volatile U32 DR:16;
    }BIT;
  }DR;
  
  union
  {
    volatile U32 VALUE;
    volatile U8 DATA8;
    volatile U16 DATA16;
    struct
    {
      volatile U32 CRCPOLY:16;
    }BIT;
  }CRCPR;
  
  union
  {
    volatile U32 VALUE;
    volatile U8 DATA8;
    volatile U16 DATA16;
    struct
    {
      volatile U32 RXCRC:16;
    }BIT;
  }RXCRCR;
  
  union
  {
    volatile U32 VALUE;
    volatile U8 DATA8;
    volatile U16 DATA16;
    struct
    {
      volatile U32 TXCRC:16;
    }BIT;
  }TXCRCR;
  
  union
  {
    volatile U8 VALUE8;
    volatile U32 VALUE;
    struct
    {
      volatile U32 CHLEN:1,DATLEN:2,CKPOL:1,I2SSTD:1,:1,PCMSYNC:1,I2CCFG:2,I2SE:1,I2SMOD:1;
    }BIT;
  }I2SCFGR;
  
  union
  {
    volatile U32 VALUE;
    struct
    {
      volatile U32 I2SDIV:8,ODD:1,MCKOE:1;
    }BIT;
  }I2SPR;
  
}SPII2SV2_BYTES;

#ifdef SPI1_BASE
#define SPI1 (*((SPII2SV2_BYTES*)SPI1_BASE))
#endif//SPI1_BASE

#ifdef SPI2_BASE
#define SPI2 (*((SPII2SV2_BYTES*)SPI2_BASE))
#endif//SPI1_BASE

#ifdef SPI3_BASE
#define SPI3 (*((SPII2SV2_BYTES*)SPI3_BASE))
#endif//SPI1_BASE

extern void SPI_ENCMD(SPII2SV2_BYTES *SPI,State CMD);
extern void SPI_CFG(SPII2SV2_BYTES *SPI,U32 CR1,U32 CR2);
extern void SPI_WRRD(SPII2SV2_BYTES *SPI,void *txbuf,void *rxbuf, U32 size);
extern void SPI_WRRD_DMA(SPII2SV2_BYTES *SPI,void *txbuf,void *rxbuf,U32 size,
                  DMAV2_BYTES *TXDMA,U8 TXSTREAM,DMAV2_BYTES *RXDMA,U8 RXSTREAM);

#endif//_SPII2SV2_H_
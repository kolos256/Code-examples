#include "SPII2SV2.h"

void SPI_ENCMD(SPII2SV2_BYTES *SPI,State CMD)
{
  SPI->CR1.BIT.SPE=CMD;
}

void SPI_CFG(SPII2SV2_BYTES *SPI,U32 CR1,U32 CR2)
{
  SPI->CR1.VALUE=CR1;
  SPI->CR2.VALUE=CR2;
}

void SPI_PREPARE(SPII2SV2_BYTES *SPI)
{
  U8 TMP1;
  while(SPI->SR.BIT.BSY);
  while((SPI->SR.BIT.RXNE)||(SPI->SR.BIT.OVR))
  {
    TMP1=SPI->DR.DATA8;
  }
}

void SPI_WRRD_DMA(SPII2SV2_BYTES *SPI,void *txbuf,void *rxbuf,U32 size,
                  DMAV2_BYTES *TXDMA,U8 TXSTREAM,DMAV2_BYTES *RXDMA,U8 RXSTREAM )
{
  U32 TMP1=0;
  if((txbuf!=0)&&(rxbuf!=0))
  {
    IRQ_DIS;
    DMASTREAMENCMD(TXDMA,TXSTREAM,DISABLE);
    DMASTREAMENCMD(RXDMA,RXSTREAM,DISABLE);
    DMASTREAMCLEARFLAGS(TXDMA,TXSTREAM,0x3F);
    DMASTREAMCLEARFLAGS(RXDMA,RXSTREAM,0x3F);
    DMATRANSFERINIT(TXDMA,TXSTREAM,txbuf,(void*)&SPI->DR.VALUE,size);
    DMATRANSFERINIT(RXDMA,RXSTREAM,rxbuf,(void*)&SPI->DR.VALUE,size);
    RXDMA->STREAM[RXSTREAM].CR.BIT.MINC=ENABLE;
    DMASTREAMENCMD(TXDMA,TXSTREAM,ENABLE);
    DMASTREAMENCMD(RXDMA,RXSTREAM,ENABLE);
    IRQ_EN;
#ifdef SPI_MULTITASKING1
    tskwaitstate((!(DMASTREAMGETFLAGS(TXDMA,TXSTREAM)&DMA_TCFLG))||
                 (!(DMASTREAMGETFLAGS(RXDMA,RXSTREAM)&DMA_TCFLG))||
                   (SPI->SR.BIT.BSY));
#else
    while((!(DMASTREAMGETFLAGS(TXDMA,TXSTREAM)&DMA_TCFLG))||
          (!(DMASTREAMGETFLAGS(RXDMA,RXSTREAM)&DMA_TCFLG)));
#endif
  }
  else if(txbuf!=0)
  {
    IRQ_DIS;
    DMASTREAMENCMD(TXDMA,TXSTREAM,DISABLE);
    DMASTREAMENCMD(RXDMA,RXSTREAM,DISABLE);
    DMASTREAMCLEARFLAGS(TXDMA,TXSTREAM,0x3F);
    DMASTREAMCLEARFLAGS(RXDMA,RXSTREAM,0x3F);
    DMATRANSFERINIT(TXDMA,TXSTREAM,txbuf,(void*)&SPI->DR.VALUE,size);
    DMATRANSFERINIT(RXDMA,RXSTREAM,&TMP1,(void*)&SPI->DR.VALUE,size);
    RXDMA->STREAM[RXSTREAM].CR.BIT.MINC=DISABLE;
    DMASTREAMENCMD(TXDMA,TXSTREAM,ENABLE);
    DMASTREAMENCMD(RXDMA,RXSTREAM,ENABLE);
    IRQ_EN;
#ifdef SPI_MULTITASKING1
    tskwaitstate((!(DMASTREAMGETFLAGS(TXDMA,TXSTREAM)&DMA_TCFLG))||
                 (!(DMASTREAMGETFLAGS(RXDMA,RXSTREAM)&DMA_TCFLG))||
                   (SPI->SR.BIT.BSY));
#else
    while((!(DMASTREAMGETFLAGS(TXDMA,TXSTREAM)&DMA_TCFLG))||
          (!(DMASTREAMGETFLAGS(RXDMA,RXSTREAM)&DMA_TCFLG)));
#endif
  }
}
//UNCOMPLITED UNTESTED
void SPI_WRRD_DMA_CRC(SPII2SV2_BYTES *SPI,void *txbuf,void *rxbuf,U32 size,
                  DMAV2_BYTES *TXDMA,U8 TXSTREAM,DMAV2_BYTES *RXDMA,U8 RXSTREAM )
{
  U32 TMP1=0;
  if((txbuf!=0)&&(rxbuf!=0))
  {
    SPI_ENCMD(SPI,DISABLE);
    SPI->CR1.BIT.CRCEN=1;
    SPI_ENCMD(SPI,ENABLE);
    DMASTREAMENCMD(TXDMA,TXSTREAM,DISABLE);
    DMASTREAMENCMD(RXDMA,RXSTREAM,DISABLE);
    DMASTREAMCLEARFLAGS(TXDMA,TXSTREAM,0x3F);
    DMASTREAMCLEARFLAGS(RXDMA,RXSTREAM,0x3F);
    DMATRANSFERINIT(TXDMA,TXSTREAM,txbuf,(void*)&SPI->DR.VALUE,size);
    DMATRANSFERINIT(RXDMA,RXSTREAM,rxbuf,(void*)&SPI->DR.VALUE,size);
    RXDMA->STREAM[RXSTREAM].CR.BIT.MINC=ENABLE;
    DMASTREAMENCMD(TXDMA,TXSTREAM,ENABLE);
    DMASTREAMENCMD(RXDMA,RXSTREAM,ENABLE);
    while((!(DMASTREAMGETFLAGS(TXDMA,TXSTREAM)&DMA_TCFLG))||
          (!(DMASTREAMGETFLAGS(RXDMA,RXSTREAM)&DMA_TCFLG))||
            (SPI->SR.BIT.BSY));
    
    SPI_ENCMD(SPI,DISABLE);
    SPI->CR1.BIT.CRCEN=0;
    SPI_ENCMD(SPI,ENABLE);
    
  }
  else if(txbuf!=0)
  {
    SPI_ENCMD(SPI,DISABLE);
    SPI->CR1.BIT.CRCEN=1;
    SPI_ENCMD(SPI,ENABLE);
    DMASTREAMENCMD(TXDMA,TXSTREAM,DISABLE);
    DMASTREAMENCMD(RXDMA,RXSTREAM,DISABLE);
    DMASTREAMCLEARFLAGS(TXDMA,TXSTREAM,0x3F);
    DMASTREAMCLEARFLAGS(RXDMA,RXSTREAM,0x3F);
    DMATRANSFERINIT(TXDMA,TXSTREAM,txbuf,(void*)&SPI->DR.VALUE,size);
    DMATRANSFERINIT(RXDMA,RXSTREAM,&TMP1,(void*)&SPI->DR.VALUE,size);
    RXDMA->STREAM[RXSTREAM].CR.BIT.MINC=DISABLE;
    DMASTREAMENCMD(TXDMA,TXSTREAM,ENABLE);
    DMASTREAMENCMD(RXDMA,RXSTREAM,ENABLE);
    while((!(DMASTREAMGETFLAGS(TXDMA,TXSTREAM)&DMA_TCFLG))||
          (!(DMASTREAMGETFLAGS(RXDMA,RXSTREAM)&DMA_TCFLG))||
            (SPI->SR.BIT.BSY));
    SPI_ENCMD(SPI,DISABLE);
    SPI->CR1.BIT.CRCEN=0;
    SPI_ENCMD(SPI,ENABLE);
  }
}

#if 1
void SPI_WRRD(SPII2SV2_BYTES *SPI,void *txbuf,void *rxbuf, U32 size)
{
  U32 TXCOUNTER,RXCOUNTER;
  U8 *p1,*p2,TMP1;
  if((txbuf!=0)&&(rxbuf!=0))
  {
    TXCOUNTER=RXCOUNTER=size;
    p1=txbuf;p2=rxbuf;
    while(!(SPI->SR.BIT.TXE));
    do
    { 
      while((SPI->SR.BIT.TXE)&&(TXCOUNTER))
      {
        SPI->DR.DATA8=*p1++;
        TXCOUNTER--;
      }
      while((SPI->SR.BIT.RXNE)&&(RXCOUNTER))
      {
        *p2++=SPI->DR.DATA8;
        RXCOUNTER--;
      }
    }
    while((TXCOUNTER)||(RXCOUNTER));
  }
  else if(txbuf!=0)
  {
    TXCOUNTER=RXCOUNTER=size;
    p1=txbuf;
    while(!(SPI->SR.BIT.TXE));
    do
    { 
      while((SPI->SR.BIT.TXE)&&(TXCOUNTER))
      {
        SPI->DR.DATA8=*p1++;
        TXCOUNTER--;
      }
      while((SPI->SR.BIT.RXNE)&&(RXCOUNTER))
      {
        TMP1=SPI->DR.DATA8;
        RXCOUNTER--;
      }
    }while((TXCOUNTER)||(RXCOUNTER));
  }
}
#endif

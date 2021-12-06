#ifndef _DMA2D_H_
#define _DMA2D_H_

typedef struct
{
  union
  {
    U32 VALUE;
    struct
    {
      U32 START:1,SUPS:1,ABORT:1,:5,TEIE:1,TCIE:1,TWIE:1,CAEIE:1,CTCIE:1,CEIE:1,:2,MODE:2;
    }BIT;
  }CR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 TEIF:1,TCIF:1,TWIF:1,CAEIF:1,CTCIF:1,CEIF:1;
    }BIT;
  }ISR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 CTEIF:1,CTCIF:1,CTWIF:1,CAECIF:1,CCTCIF:1,CCEIF:1;
    }BIT;
  }IFCR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 MA:32;
    }BIT;
  }FGMAR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 LO:14;
    }BIT;
  }FGOR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 MA:32;
    }BIT;
  }BGMAR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 LO:14;
    }BIT;
  }BGOR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 CM:4,CCM:1,START:1,:2,CS:8,AM:2,:6,ALPHA:8;
    }BIT;
  }FGPFCCR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U8 BLUE;
      U8 GREEN;
      U8 RED;
    }BIT;
  }FGCOLR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 CM:4,CCM:1,START:1,:2,CS:8,AM:2,:6,ALPHA:8;
    }BIT;
  }BGPFCCR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U8 BLUE;
      U8 GREEN;
      U8 RED;
    }BIT;
  }BGCOLR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 MA;
    }BIT;
  }FGCMAR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 MA;
    }BIT;
  }BGCMAR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 CM:3;
    }BIT;
  }OPFCCR;
  
  union
  {
    U32 COLOR32;
    U16 COLOR16;
    U8 COLORS[4];
    struct
    {
      U8 BLUE;
      U8 GREEN;
      U8 RED;
      U8 ALPHA;
    }ARGB8888;
    struct
    {
      U8 BLUE;
      U8 GREEN;
      U8 RED;
    }RGB888;
    struct
    {
      U16 BLUE:5,GREEN:6,RED:5;
    }RGB565;
    struct
    {
      U16 BLUE:5,GREEN:5,RED:5,ALPHA:1;
    }ARGB1555;
    struct
    {
      U16 BLUE:4,GREEN:4,RED:4,ALPHA:4;
    }ARGB4444;
  }OCOLR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 MA;
    }BIT;
  }OMAR;
  
  union
  {
    U32 VALUE;
    U16 VALUE16;
    struct
    {
      U32 LO:14;
    }BIT;
  }OOR;
  
  union
  {
    U32 VALUE;
    
    struct
    {
      U32 NL:16,PL:14;
    }BIT;
  }NLR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U16 LW;
    }BIT;
  }LWR;
  
  union
  {
    U32 VALUE;
    struct
    {
      U32 EN:1,:7,DT:8;
    }BIT;
  }AMTCR;
  
}DMA2D_BYTES;

#define DMA2D (*((DMA2D_BYTES*)DMA2D_BASE))

#endif//_DMA2D_H_
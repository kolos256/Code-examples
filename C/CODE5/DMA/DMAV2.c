#include "DMAV2.h"

void DMASTREAMINIT(DMAV2_BYTES* DMASEL,U8 STR,U32 CRVAL,U32 FCRVAL,U32 NDTRVAL,U32 PARVAL,U32 M0ARVAL,U32 M1ARVAL)
{
   DMASEL->STREAM[STR].CR.VALUE=(CRVAL);
   DMASEL->STREAM[STR].FCR.VALUE=(FCRVAL);
   DMASEL->STREAM[STR].NDTR.VALUE=(NDTRVAL);
   DMASEL->STREAM[STR].PAR.VALUE=(PARVAL);
   DMASEL->STREAM[STR].M0AR.VALUE=(M0ARVAL);
   DMASEL->STREAM[STR].M1AR.VALUE=(M1ARVAL);
}
const U8 MASKS1[4]={0,6,16,22};
U8 DMASTREAMGETFLAGS(DMAV2_BYTES* DMASEL,U8 STR)
{
   U8 FLAGS=0;
   if(STR&4)
   {
     FLAGS=((DMASEL->HISR.VALUE>>MASKS1[(STR&3)])&0x3F);
   }
   else
   {
     FLAGS=((DMASEL->LISR.VALUE>>MASKS1[(STR&3)])&0x3F);
   }
   return FLAGS;
}

void DMASTREAMCLEARFLAGS(DMAV2_BYTES* DMASEL,U8 STR,U8 FLAGS)
{
   U32 MASK1=((FLAGS&0x3F)<<MASKS1[(STR&3)]);
   if(STR&4)
   {
     DMASEL->HIFCR.VALUE=MASK1;
   }
   else
   {
     DMASEL->LIFCR.VALUE=MASK1;
   }
}


void DMATRANSFERINIT(DMAV2_BYTES* DMASEL,U8 STR,void *MEMADDR,void *PERIPHADDR,U32 TRANSFERS)
{
  DMAV2_STREAMREGS *CURSTR=&DMASEL->STREAM[STR];
  if(CURSTR->CR.BIT.CT)
  {
   CURSTR->M1AR.VALUE=((U32)MEMADDR);
  }
  else
  {
   CURSTR->M0AR.VALUE=((U32)MEMADDR);
  }
  CURSTR->PAR.VALUE=((U32)PERIPHADDR);
  CURSTR->NDTR.VALUE=TRANSFERS;
}


void DMASTREAMENCMD(DMAV2_BYTES* DMASEL,U8 STR,State CMD)
{
    if(CMD)
    {
       DMASEL->STREAM[STR].CR.BIT.EN=1;
    }
    else
    {
       DMASEL->STREAM[STR].CR.BIT.EN=0;
       while(DMASEL->STREAM[STR].CR.VALUE&1);
       //while(BB_ACCESS(((U32)&DMASEL+0x10+(STR*24)),0));
    }
}


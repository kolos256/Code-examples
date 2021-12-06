/***********************************************************************************************************************************************
 * @file  <MULTITASKINGV1.c>
 * @brief < This file contains basic code that implements simple multitasking. >
 *
 * 
 * @author <Dmitry Kolosyanko Dmitriy.kolosyanko@gmail.com>
 * @date   <13/08/2016>
 *
 ***********************************************************************************************************************************************/

#include "MULTITASKINGV1.h"

/*****************************************************************************************

                                           psp
                                            |
   -20  -1C  -18  -14  -10   -C    -8   -4  V  0    4     8    C    10    14   18    1C
  ----------------------------------------------------------------------------------------
  | r4 | r5 | r6 | r7 | r8 | r9 | r10 | r11 | r0 |  r1 | r2  | r3 | r12 | lr | pc | xPSR |
  ----------------------------------------------------------------------------------------


******************************************************************************************/


TCB *STARTTSK0=0;
TCB *LASTTSK0=0;
TCB *STARTTSK1=0;
TCB *LASTTSK1=0;
TCB *CURTSK=0;
TCB *NEXTTSK=0;
TSKSTACK IRQSTACK;
U32 SYSTICKCOUNTER=0;

U32 IRQBUFER[10];

#define PendSVPrio  255
#define SysTickPrio 0

#define STACKCHAR 0xFFFFFFFF
#define err1handle while(1);

/***********************************************
* 
* initialise new task
* task - structure with task parameters
* stackmem - memory for tasck stack
* stacksize - size of stackmem in bytes
* return pointer to initialized task
*
*************************************************/
TCB* inittask(TCB *task,void *stackmem,U16 stacksize,void (*handler) (void))
{
  U32 i;
  task->STACK.STACKSTART=(((U8*)stackmem)+(stacksize<<2));
  task->STACK.SP=(void*)((U8*)task->STACK.STACKSTART-sizeof(IRQFRAME)-4);
  task->STACK.STACKSIZE=stacksize;
  
  for(i=0;i<stacksize;i++)
  {
    ((U32*)stackmem)[i]=STACKCHAR;
  }
  task->STACK.regs->PSR=0x01000000;
  task->STACK.regs->PC=((U32)handler);
  task->STACK.regs->LR=(U32)task->STACK.SP;
  task->CRITICAl=0;
  return task; 
}

/**************************************************
* 
* add task newtask  to nonpreemptive tasklist
* if successfull return 0 otherwice error code
* at least one nonpreemptive task must be added
*
***************************************************/
U8 addtsk0(TCB *newtask)
{
  U8 STATE=0;
  if(newtask==0)
  {
    STATE=1;
    err1handle;
  }
  if(STARTTSK0)
  {
    LASTTSK0->NEXT=newtask;
    LASTTSK0=newtask;
    newtask->NEXT=STARTTSK1;
    if(CURTSK->NEXT==newtask)
    {
      NEXTTSK=newtask;
    }
  }
  else
  {
    CURTSK=LASTTSK0=STARTTSK0=newtask;
    newtask->NEXT=STARTTSK1;
    if(CURTSK->NEXT!=0)
    {
      NEXTTSK=CURTSK->NEXT;
    }
    
  }
  return STATE;
}

/**************************************************
* 
* add task newtask  to preemptive tasklist
* if successfull return 0 otherwice error code
* at least one preemptive task must be added
*
***************************************************/
U8 addtsk1(TCB *newtask)
{
  U8 STATE=0;
#if 0
  if((STARTTSK0==0)||(newtask==0))
  {
    STATE=1;
    err1handle;
  }
#endif
  if(STARTTSK1)
  {
    newtask->NEXT=STARTTSK1->NEXT;
    STARTTSK1->NEXT=newtask;
    STARTTSK1=newtask;
    if(CURTSK!=0)
    {
      if(CURTSK->NEXT==newtask)
      {
        NEXTTSK=newtask;
      }
    }
  }
  else
  {
    STARTTSK1=newtask;
    if(LASTTSK0) 
    {
      LASTTSK0->NEXT=newtask;
    }
    
    newtask->NEXT=STARTTSK1;
    if(CURTSK!=0)
    {
      if(CURTSK->NEXT==newtask)
      {
        NEXTTSK=newtask;
      }
    }
  }
  return STATE;
}

/**************************************************
* 
* return tick counter
*
***************************************************/

U32 getsystemtick(void)
{
  return SYSTICKCOUNTER;
}

U32 systickstoperiod(U32 starttick,U32 endtick)
{
  return (endtick-starttick);
}

/**************************************************
* 
* suspend the task for the specified number of ticks
*
***************************************************/
void tsktickswait(U32 TicksPeriod)
{
  U32 STARTTICK=SYSTICKCOUNTER;
  while((SYSTICKCOUNTER-STARTTICK)<TicksPeriod){tskret();}
}

/**************************************************
* 
* suspend the task for the specified time in milliseconds
*
***************************************************/
void tskmswait(U32 msPeriod)
{
  U32 STARTTICK=SYSTICKCOUNTER;
  msPeriod=ms_SysTick(msPeriod);
  while((SYSTICKCOUNTER-STARTTICK)<msPeriod){tskret();}
}

/**************************************************
* 
* suspend the task for the specified time in milliseconds
*
***************************************************/
void tskmkswait(U32 mksPeriod)
{
  U32 STARTTICK=SYSTICKCOUNTER;
  mksPeriod=mks_SysTick(mksPeriod);
  while((SYSTICKCOUNTER-STARTTICK)<mksPeriod){tskret();}
}


/**************************************************
* 
* start multitasking
*
***************************************************/
void startmultitasking(void *IRQSTACKMEM,U16 IRQSTACKSIZE)
{
  U8 STATE;
  #if 1
    if((STARTTSK0==0)||(LASTTSK0==0)||(STARTTSK1==0))
    {
      STATE=1;
      err1handle;
    }
  #endif
  
  SCB.SHP[(14-8)]=PendSVPrio;
  SCB.SHP[(15-8)]=SysTickPrio;
  IRQSTACK.STACKSTART=(((U32*)IRQSTACKMEM)+IRQSTACKSIZE);
  IRQSTACK.SP=((U32*)IRQSTACK.STACKSTART-1);
  
  IRQSTACK.STACKSIZE=IRQSTACKSIZE;
  
  setirqstack((void*)(((U8*)IRQSTACKMEM)+(IRQSTACKSIZE<<2)-sizeof(IRQFRAME)-4));
  //init system tick timer
  SysTick.VAL.VALUE=0;
  SysTick.LOAD.VALUE=450;//50mks ticks on 9mhz frq
  SysTick.CTRL.VALUE=(STK_AHBDIV8CLK|STK_IEN|STK_EN);

  IRQ_EN;
  startfirsttsk();
}

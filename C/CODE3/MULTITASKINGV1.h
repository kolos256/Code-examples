#ifndef _MULTITASKINGV1_H_
#define _MULTITASKINGV1_H_

#include "CORE\CORE.h"
#ifdef USECONF
#include "CONF.h"
#include "devlist.h"
#endif//USECONF

#ifdef CORTEXM0

#include "STM32\CORE\CM0_CORE.h"

#endif//CORTEXM0

#ifdef CORTEXM4

#include "STM32\CORE\CM4_CORE.h"

#endif//CORTEXM4

#include "STM32\PERIPH\TIMERS\SYSTICK.h"
#include "STM32\PERIPH\NVIC\NVIC.h"
#if 0
//100mksticks
#define SysTick_ms(ticks) ((ticks)/10)
#define ms_SysTick(ticks) ((ticks)*10)
#define SysTick_mks(ticks) ((ticks)*100)
#define mks_SysTick(ticks) ((ticks)/100)
#endif
#if 1
//50mksticks
#define SysTick_ms(ticks) ((ticks)/20)
#define ms_SysTick(ticks) ((ticks)*20)
#define SysTick_mks(ticks) ((ticks)*50)
#define mks_SysTick(ticks) ((ticks)/50)
#endif

#define USERAREA U8 CRITICAl;

typedef struct{
 U32 R0;
 U32 R1;
 U32 R2;
 U32 R3;
 U32 R12;
 U32 LR;
 U32 PC;
 U32 PSR;
}IRQFRAME;

typedef struct{
  union
  {
   void *SP;
   IRQFRAME *regs;
  };
  void *STACKSTART;//cells 
  U32 STACKSIZE; 
}TSKSTACK;

typedef struct TCB{
 TSKSTACK STACK;
 struct TCB *NEXT;
 USERAREA;
}TCB;

extern U32 SYSTICKCOUNTER;
extern TCB *STARTTSK0;
extern TCB *LASTTSK0;
extern TCB *STARTTSK1;
extern TCB *LASTTSK1;
extern TCB *CURTSK;
extern TCB *NEXTTSK;
extern TSKSTACK IRQSTACK;

extern U8 addtsk0(TCB *newtask);
extern U8 addtsk1(TCB *newtask);
extern TCB* inittask(TCB *task,void *stackmem,U16 stacksize,void (*handler) (void));

extern void tsktickswait(U32 TicksPeriod);
extern void tskmswait(U32 msPeriod);
extern void tskmkswait(U32 mksPeriod);

extern U32 systickstoperiod(U32 starttick,U32 endtick);

//ASM proc
extern void setirqstack (void *mem);
extern void startfirsttsk(void);
extern void startmultitasking(void *IRQSTACKMEM,U16 IRQSTACKSIZE);
extern void tskret(void);
extern void entercritical(void);
extern void exitcritical(void);
extern void tskwaitstate(bool STATE);


#define tskwaitstate(STATE) while((STATE)){tskret();}

#endif//_MULTITASKINGV1_H_

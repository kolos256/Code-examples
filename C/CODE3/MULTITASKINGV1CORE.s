/***********************************************************************************************************************************************
 * @file  <MULTITASKINGV1CORE.s>
 * @brief < This file contains low-level assembly code for multitasking. >
 *
 * 
 * @author <Dmitry Kolosyanko Dmitriy.kolosyanko@gmail.com>
 * @date   <13/08/2016>
 *
 ***********************************************************************************************************************************************/


       .syntax unified
       .cpu cortex-m7
       .fpu softvfp
       .thumb
        
        
        .global PendSV_Handler
        .global SysTick_Handler
        .global startfirsttsk
        .global setirqstack
        .global tskret
        .global tst1


.equ    SCB_ICSR     ,0xE000ED04
.equ    PENDSTSET_BIT,0x10000000
.equ    NEXTTSKSH    ,12
.equ    USERAREASH   ,16
.equ    CRITICALSH   ,0



//PENDSW IRQ TASKSWITCH procidure
tsksw_handler:  @tasksw
        cpsid i
        //push {lr}
        ldr r0,=CURTSK
        ldr r1,[r0]
        @save curtask context to task stack
        mrs r2,psp
        str r2,[r1]@save psp to tsksp
        stmdb r2!,{r4,r5,r6,r7,r8,r9,r10,r11}
        dsb
        isb
        @restore nexttask context from task stack
        ldr r1,=NEXTTSK
        ldr r2,[r1]
        ldr r3,[r2]
        msr psp,r3
        ldmdb r3!,{r4,r5,r6,r7,r8,r9,r10,r11}
        dsb
        isb
        str r2,[r0]@
        ldr r0,[r2,#NEXTTSKSH]
        //ldr r0,[r0]
        str r0,[r1]@save to NEXTTSK
        //ldr lr,=0xFFFFFFFD
        cpsie i
        //pop {pc}
        bx lr

.thumb_set PendSV_Handler,tsksw_handler

Tick_Handler: @dotick
        ldr r0,=SYSTICKCOUNTER
        dsb
        ldr r1,[r0]
        add r1,r1,#1
        str r1,[r0]
        @nexttask=starttask0
        ldr r0,=NEXTTSK
        ldr r1,=STARTTSK0
        ldr r1,[r1]
        str r1,[r0]
        @trigger PendSV irq
        ldr r0,=SCB_ICSR
        mov r1,#PENDSTSET_BIT
        str r1,[r0]
        //ldr r0,=ICSR
        bx lr

.thumb_set SysTick_Handler,Tick_Handler


        
startfirsttsk:
        @select psp as stack pointer
        mov r0,#2
        msr control,r0
        isb
        dsb

        ldr r0,=STARTTSK0
        ldr r0,[r0]
        ldr r0,[r0]
        msr psp,r0
        ldr r1,[r0,#28]@load xPSR
        msr PSR_nzcvqg,r1
        pop {r0,r1,r2,r3,r12,lr}
        pop {pc}
        //pop {r0,r1,r2,r3,r12,lr,pc}

//void setirqstack ( void *mem )
setirqstack:
        msr msp,r0
        bx lr

tskret:
        push {r0,r1,lr}
        ldr r0,=SCB_ICSR
        mov r1,#PENDSTSET_BIT
        str r1,[r0]
        wfi
        pop {r0,r1,pc}
        //bx lr

entercritical:
        push {r0,r1,lr}
        ldr r0,=255
        msr basepri,r1
        ldr r0,=CURTSK 
        ldr r0,[r0]  
        ldr r1,[r0,#(USERAREASH+CRITICALSH)]
        add r1,r1,#1
        str r1,[r0,#(USERAREASH+CRITICALSH)]
        pop {r0,r1,pc}
        //bx lr

exitcritical:
        push {r0,r1,lr}
        ldr r0,=CURTSK 
        ldr r0,[r0]  
        ldr r1,[r0,#(USERAREASH+CRITICALSH)]
        subs r1,r1,#1
        str r1,[r0,#(USERAREASH+CRITICALSH)]
        itt eq
        ldreq r0,=0
        msreq basepri,r1
        pop {r0,r1,pc}
        //bx lr

        

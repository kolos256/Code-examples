#ifndef _CORE_H_
#define _CORE_H_

#define USECONF
//константы для битовых операций
#define BIT0 0x00000001
#define BIT1 0x00000002
#define BIT2 0x00000004
#define BIT3 0x00000008
#define BIT4 0x00000010
#define BIT5 0x00000020
#define BIT6 0x00000040
#define BIT7 0x00000080
#define BIT8 0x00000100
#define BIT9 0x00000200
#define BIT10 0x00000400
#define BIT11 0x00000800
#define BIT12 0x00001000
#define BIT13 0x00002000
#define BIT14 0x00004000
#define BIT15 0x00008000
#define BIT16 0x00010000
#define BIT17 0x00020000
#define BIT18 0x00040000
#define BIT19 0x00080000
#define BIT20 0x00100000
#define BIT21 0x00200000
#define BIT22 0x00400000
#define BIT23 0x00800000
#define BIT24 0x01000000
#define BIT25 0x02000000
#define BIT26 0x04000000
#define BIT27 0x08000000
#define BIT28 0x10000000
#define BIT29 0x20000000
#define BIT30 0x40000000
#define BIT31 0x80000000

#define ABS(X)   ((X) > 0 ? (X) : (-(X)))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
//#define ABS(X) ((X) > 0 ? (X) : (-1*(X)))

//#define U32 unsigned int
//#define U16 unsigned short int
//#define U8 unsigned char
#define NONE 0x00000000

typedef unsigned char   U8;
typedef signed char     S8; 
typedef unsigned short  U16;
typedef signed short    S16;
typedef unsigned int    U32;
typedef signed int      S32;
typedef enum {false = 0, true = !false} bool;
//typedef enum {DISABLE = 0, ENABLE = !DISABLE} State;
typedef enum {DISABLE = 0, ENABLE = 1} State;
//typedef enum { false = 0, true = !false}bool;

//HSETYPE
#define CRYSTALRESONATOR 0x01;
//Макрос для доступа к 32битной ячейке памяти по её адрессу
#define IoReg32(ADDR) *((volatile unsigned long *)(ADDR))
//Преобразует адрес в число пригодное для записи в 32 битный регистр
#define A32(VALUE) (volatile unsigned long)(VALUE)
#define A16(VALUE) (volatile unsigned short int)(VALUE)
//Макрос для доступа к 16битной ячейке памяти по её адрессу
#define IoReg16(ADDR) *((volatile unsigned short int *)(ADDR))
#define IoReg16Init(ADDR,BITS) IoReg16(ADDR)=(BITS);
//Макрос для доступа к 8битной ячейке памяти по её адрессу
#define IoReg8(ADDR) *((volatile unsigned char *)(ADDR))
//Значения полученные экспериментально
#define BB_PERIPH 0x40000000
#define BB_BASE 0x42000000
//Чтение или запись бита через bit-banding
#define BB_ACCESS(ADDR,BIT) *((volatile unsigned long *)(BB_BASE + ((ADDR - BB_PERIPH)*32) + (BIT*4)))
//Устанавливает один бит 32 битного регистра
#define BITSET32(ADDR,BIT) (IoReg32(ADDR)|=(0x00000001<<(BIT)))
//Устанавливает один бит 8 битного регистра
#define BITSET8(ADDR,BIT) (IoReg8(ADDR)|=(01<<(BIT)))




//Старший и младший байты переменой типа U16
#define U16Low 0
#define U16High 1
//Старший и младший байты переменой типа S16
#define S16Low 0
#define S16High 1
//Старший, младший и средние байты переменой типа U32
#define U32Low 3
#define U32Middle1 2
#define U32Middle2 1
#define U32High 0
//Извлекает из многобайтовых переменных указанный байт(Работает быстрее high() и low())
#define GetByte(Byte,Var) ((U8*)&(Var))[(Byte)]
//Старшие и младшие двабайта переменной типа U32
#define U32HighHW 0
#define U32LowHW 1
#define GetHalfWord(HalfWord,Var) ((U16*)&(Var))[(HalfWord)]
//Разрешение и запрет прерываний
#define IRQ_EN asm("cpsie i");
#define IRQ_DIS asm("cpsid i");
#define _DSB asm("DSB");
#define _ISB asm("ISB");
#define _NOP asm("NOP");

typedef enum {NORMAL=0,TIMEOUTERR}CMDRES;

#endif//_CORE_H_

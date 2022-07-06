#ifndef __USART_H_
#define __USART_H_
//#include "stm32f4xx_hal.h"
#include "stm32f0xx_hal.h"
#include "ticks.h"
#include "COMMDRVV1.h"

extern UART_HandleTypeDef huart1;

extern int8_t USART1_UART_Init(void);
extern int8_t USART_Transmit(UART_HandleTypeDef *huart,uint8_t *pTxData,uint16_t TxSize,uint32_t ByteTimeout);

#endif//__USART_H_
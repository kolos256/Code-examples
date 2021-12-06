#ifndef _INTERFACES_H_
#define _INTERFACES_H_

#include "base.h"
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include "USARTSUBSYSV1.h"
#include "exceptionsv1.h"

extern uint8_t I2C1_RXFLAG1;
extern uint8_t I2C1_TXFLAG1;
extern I2C_HandleTypeDef hi2c1;
extern xSemaphoreHandle i2c1_evt1;
extern uint8_t i2c_readregs(I2C_HandleTypeDef *hi2c,uint8_t devaddr, void* databuf, uint8_t startregaddr,uint16_t regstoread);
extern uint8_t i2c_writeregs(I2C_HandleTypeDef *hi2c,uint8_t devaddr, void* databuf, uint8_t startregaddr,uint16_t regstowrite);

extern uint8_t i2c_writereg_a16(I2C_HandleTypeDef *hi2c,uint8_t devaddr, void* databuf, uint16_t startregaddr,uint16_t regstowrite);
extern uint8_t i2c_readregs_a16(I2C_HandleTypeDef *hi2c,uint8_t devaddr, void* databuf, uint16_t startregaddr,uint16_t regstoread);
#define USESPI6
//#define USEUSART6
#define USESI2C1
#define USESI2C2

extern uint8_t SPI_SELECT(uint8_t SELECT,uint8_t STATE);

//extern uint8_t SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi,xQueueHandle Queue, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size,SemaphoreHandle_t xMutex);
//extern uint8_t SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi,xQueueHandle Queue, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size,SemaphoreHandle_t xMutex,uint32_t BaudRate,uint8_t Select);
extern uint8_t SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi,xQueueHandle Queue, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size,uint32_t BaudRate);

//extern uint8_t SPI_Transmit_DMA(SPI_HandleTypeDef *hspi,xQueueHandle Queue, uint8_t *pTxData, uint16_t Size,SemaphoreHandle_t xMutex);
//extern uint8_t SPI_Transmit_DMA(SPI_HandleTypeDef *hspi,xQueueHandle Queue, uint8_t *pTxData, uint16_t Size,SemaphoreHandle_t xMutex,uint32_t BaudRate,uint8_t Select);
extern uint8_t SPI_Transmit_DMA(SPI_HandleTypeDef *hspi,xQueueHandle Queue, uint8_t *pTxData, uint16_t Size,uint32_t BaudRate);

//extern uint8_t SPI_Receive_DMA(SPI_HandleTypeDef *hspi,xQueueHandle Queue,uint8_t *pRxData, uint16_t Size,SemaphoreHandle_t xMutex);
//extern uint8_t SPI_Receive_DMA(SPI_HandleTypeDef *hspi,xQueueHandle Queue,uint8_t *pRxData, uint16_t Size,SemaphoreHandle_t xMutex,uint32_t BaudRate,uint8_t Select);
extern uint8_t SPI_Receive_DMA(SPI_HandleTypeDef *hspi,xQueueHandle Queue,uint8_t *pRxData, uint16_t Size,uint32_t BaudRate);

extern uint8_t SPI_SELECT_DEVICE(uint8_t Select,SemaphoreHandle_t xMutex);
extern uint8_t SPI_DESELECT_DEVICE(uint8_t Select,SemaphoreHandle_t xMutex);


extern uint8_t UART_TransmitRecieve(UART_HandleTypeDef *huart,xQueueHandle Queue,uint8_t *pTxData,uint16_t TxSize,uint8_t *pRxData, uint16_t RxSize);
extern uint8_t UART_Transmit(UART_HandleTypeDef *huart,xQueueHandle Queue,uint8_t *pTxData,uint16_t TxSize);
extern uint8_t UART_Receive(UART_HandleTypeDef *huart,xQueueHandle Queue,uint8_t *pRxData,uint16_t RxSize);

extern uint8_t MY_UART_Recieve(UART_HandleTypeDef *huart,uint8_t *pRxData,uint16_t RxSize);

#if defined(USESPI6)

//SPi2
extern SPI_HandleTypeDef hspi6;
extern xQueueHandle SPI6_Q1;
extern SemaphoreHandle_t SPI6_xMutex1;

//#define SPI6_TransmitReceive_DMA(pTxData,pRxData,Size) SPI_TransmitReceive_DMA(&hspi6,SPI6_Q1,pTxData,pRxData,Size,NULL)
#define SPI6_TransmitReceive_DMA(pTxData,pRxData,Size,BaudRate) SPI_TransmitReceive_DMA(&hspi6,SPI6_Q1,pTxData,pRxData,Size,BaudRate)
#define SPI6_Transmit_DMA(pTxData,Size,BaudRate)  SPI_Transmit_DMA(&hspi6,SPI6_Q1,pTxData,Size,BaudRate)
#define SPI6_Receive_DMA(pData,Size,BaudRate) SPI_Receive_DMA(&hspi6,SPI6_Q1,pData,Size,BaudRate)

#endif//defined(USESPI6)

//#define USESPI2

#if defined(USESPI2)
extern uint8_t interfaces_SPI2_INIT(void);

extern SPI_HandleTypeDef hspi2;
extern xQueueHandle SPI2_Q1;
//extern SemaphoreHandle_t SPI6_xMutex1;

#define SPI2_TransmitReceive_DMA(pTxData,pRxData,Size,BaudRate) SPI_TransmitReceive_DMA(&hspi2,SPI2_Q1,pTxData,pRxData,Size,BaudRate)
#define SPI2_Transmit_DMA(pTxData,Size,BaudRate)  SPI_Transmit_DMA(&hspi2,SPI2_Q1,pTxData,Size,BaudRate)
#define SPI2_Receive_DMA(pData,Size,BaudRate) SPI_Receive_DMA(&hspi2,SPI2_Q1,pData,Size,BaudRate)

#endif//defined(USESPI2)
//#define UART2_TransmitRecieve(pTxData,TxSize,pRxData,RxSize) UART_TransmitRecieve(&huart2,UART2_Q1,pTxData,TxSize,pRxData,RxSize)
//#define UART2_Transmit(pTxData,TxSize) UART_Transmit(&huart2,UART2_Q1,pTxData,TxSize)
//#define UART2_Receive(pRxData,RxSize)  UART_Receive(&huart2,UART2_Q1,pRxData,RxSize)




//extern I2CV1_STATEMACHINE I2C1PARAMS;
extern uint8_t INTERFACES_INIT(void);

#endif//_INTERFACES_H

#include "USART.h"
/******************************************************************************
*
* USART1_PINS
* PA9  - USART1_TX AF1
* PA10 - USART1_RX AF1
*
******************************************************************************/
UART_HandleTypeDef huart1;

void USART1_IRQHandler(void){
	COMM_IRQ_PROC();
}

#if 1
int8_t USART1_UART_Init(void){
  GPIO_InitTypeDef GPIO_InitStruct;
  __HAL_RCC_USART1_CLK_ENABLE();
  if(COMMSLAVEDRVV1_INIT(0x01,USART1,115200)!=0){
	return 1;
  }
  //Enable USART1 IRQ
  HAL_NVIC_SetPriority(USART1_IRQn, 10, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  return 0;
}
#else
int8_t USART1_UART_Init(void){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_USART1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  huart1.Instance = USART1;
  //huart1.Init.BaudRate = 115200;
  huart1.Init.BaudRate = 927600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_2;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK){
    return 0;
  }
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  return 0;
}
#endif

int8_t USART_Transmit(UART_HandleTypeDef *huart,uint8_t *pTxData,uint16_t TxSize,uint32_t ByteTimeout){
  uint32_t StartTick=HAL_GetTick();
  while(TxSize--){
    while(!((huart->Instance->ISR)&USART_ISR_TXE)&&((HAL_GetTick()-StartTick)<=ByteTimeout));
    if((HAL_GetTick()-StartTick)>ByteTimeout){
      return 1;
    }
    huart->Instance->TDR=*pTxData++;
  }
  return 0;
}

/** required for printf working over usart */
int _write(int file, char *ptr, int len) {
  (void) file;  /* Not used, avoid warning */
  USART_Transmit(&huart1,(uint8_t*)ptr,(uint16_t)len,1000);
  return len;
}
 
/** required for printf working over usart */
int _write_r(struct _reent *r, int file, const void *ptr, size_t len) {
  (void) file;  /* Not used, avoid warning */
  (void) r;     /* Not used, avoid warning */
  USART_Transmit(&huart1,(uint8_t*)ptr,(uint16_t)len,1000);
  return len;
}
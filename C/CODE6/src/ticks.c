#include "ticks.h"
#if 1
extern __IO uint32_t uwTick;
extern uint32_t uwTickPrio;
extern HAL_TickFreqTypeDef uwTickFreq;
#define stkinclk 48000000
#define stkreqclk 100000

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority){
  if(HAL_SYSTICK_Config((stkinclk/stkreqclk))!=0){
    return HAL_ERROR;
  }

  /* Configure the SysTick IRQ priority */
  if (TickPriority<(1UL<<__NVIC_PRIO_BITS)){
    HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0U);
    uwTickPrio=TickPriority;
  }else{
    return HAL_ERROR;
  }
  return HAL_OK;
}

// return ms ticks
uint32_t HAL_GetTick(void){
  return (uwTick/100);
}
// return real ticks in this case mks
uint32_t HAL_GetRealTick(void){
  return uwTick;
}
//delay in real ticks in tis case mks
void HAL_RealDelay(uint32_t Delay){
  uint32_t tickstart = HAL_GetRealTick();
  while((HAL_GetRealTick() - tickstart) < Delay);
}
//ms delay
void HAL_Delay(uint32_t Delay){
  uint32_t tickstart = HAL_GetTick();
  while((HAL_GetTick() - tickstart) < Delay);
}

void HAL_IncTick(void){
  uwTick++;
}

#endif
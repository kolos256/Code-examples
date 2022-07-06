#ifndef __TICKS_H_
#define __TICKS_H_

#include "stm32f0xx_hal.h"
/**
 * @brief get tick counter. In the current implementation, one tick is 10 microseconds
 * 
 * @return uint32_t tick counter
 */
extern uint32_t HAL_GetRealTick(void);

/**
 * @brief delay in ticks
 */
extern void HAL_RealDelay(uint32_t Delay);

/**
 * @brief convert microseconds to ticks  
 */
#define HAL_mks_to_ticks(mks) ((mks)/10U)
/**
 * @brief convert milliseconds to ticks  
 */
#define HAL_ms_to_ticks(ms) ((ms)*100)

#endif//__TICKS_H_
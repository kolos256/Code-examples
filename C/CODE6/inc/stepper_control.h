#ifndef _STEPPER_CONTROL_H_
#define _STEPPER_CONTROL_H_

#include "ticks.h"
//#include "gd32f1x0.h"
#include "stm32f0xx_hal.h"
#include "my_fsm.h"
#include "stdbool.h"

typedef enum {
	ok = 0,
	hw_error
} my_stepper_hw_state;

typedef enum {
	state_rotate = 1,
	state_stop,
	state_hold,
	state_error
} my_stepper_state;

typedef struct {
	my_fsm main_fsm;
	int32_t step_counter;
	uint32_t step_period;
	uint32_t steps_to_do;
	int8_t direction;
	int32_t hold_power;
	int32_t active_power;
	bool step_complite;
	my_stepper_state state;

	uint32_t hold_start_tick;
	uint32_t hold_period;
	
	my_stepper_hw_state hw_state;
	int32_t micro_step;
	int16_t *phase_a_data;
	int16_t *phase_b_data;
	int16_t phase_data_length;
}my_stepper_control;


/** hardware dependent code. to do separate this code into a separate file **/
extern my_stepper_hw_state my_stepper_hw_do_micro_step(uint8_t mode, int32_t phase_a, int32_t phase_b);
extern my_stepper_hw_state my_stepper_hw_do_step(my_stepper_control *stepper);
extern my_stepper_hw_state my_stepper_hw_init(my_stepper_control *stepper);
extern my_stepper_hw_state my_stepper_hw_hold(my_stepper_control *stepper);
extern my_stepper_hw_state my_stepper_hw_unhold(my_stepper_control *stepper);

/**
 * @brief init stepper hardware
 * 
 * @param stepper - pointer to stepper parameters
 * @param start_step_counter - initial value of step counter
 * @param active_power - limit motor current in rotate mode.0 - minimum current 1000 - maximum current (hardware dependant)
 * @param hold_power - limit motor current in rotate mode.0 - minimum current 1000 - maximum current (hardware dependant)
 * @param hold_period_mks - time to switch in hold mode after stopping
 * @return my_stepper_state - if everything is ok return hold_state
 **/
extern my_stepper_state my_stepper_control_init(my_stepper_control *stepper,int32_t start_step_counter,int32_t active_power,int32_t hold_power, uint32_t hold_period_mks);
/**
 * @brief rotates the stepper motor a given number of steps in a given direction at a given speed
 * 
 * @param stepper - pointer to stepper parameters 
 * @param frq - rotation frequency hertz. sign determines the direction of rotation
 * @param steps - how many steps to do
 */
extern void my_stepper_move(my_stepper_control *stepper,int32_t frq, uint32_t steps);
/**
 * @brief main processing function call it in cycle as frequently as possible.
 * 
 * @param stepper - pointer to stepper parameters
 */
extern void my_stepper_control_proc(my_stepper_control *stepper);
/**
 * @brief set active_power range 0 - 1000
 * 
 * @param stepper - pointer to stepper parameters
 * @param active_power - limit motor current in rotate mode.0 - minimum current 1000 - maximum current (hardware dependant)
 */
extern void my_stepper_control_set_active_power(my_stepper_control *stepper, int32_t active_power);
/**
 * @brief set hold_power range 0 - 1000
 * 
 * @param stepper - pointer to stepper parameters
 * @param hold_power limit motor current in rotate mode.0 - minimum current 1000 - maximum current (hardware dependant)
 */
extern void my_stepper_control_set_hold_power(my_stepper_control *stepper, int32_t hold_power);
/**
 * @brief return current state of motor controller
 * 
 * @param stepper - pointer to stepper parameters
 * @return my_stepper_state - 
 */
extern my_stepper_state my_stepper_control_get_state(my_stepper_control *stepper);

#endif//_STEPPER_CONTROL_H_
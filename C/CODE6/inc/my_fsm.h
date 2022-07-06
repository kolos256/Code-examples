#ifndef _MY_FSM_H_
#define _MY_FSM_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "ticks.h"

struct my_fsm_t;
typedef struct my_fsm_t my_fsm;
typedef uint32_t my_fsm_signal_t;

typedef void (*my_fsm_callback)(my_fsm *machine);

struct my_fsm_t{
	uint32_t state_wait_start_tick; //!< start tick for wait state see my_fsm_state_wait
	uint32_t state_wait_delay; //!< delay for wait state see my_fsm_state_wait
	my_fsm_callback prev_state;//!< previous state callback
	my_fsm_callback current_state;//!< current state callback
	my_fsm_callback next_state;//!< next state for state switching
	my_fsm_callback after_wait_state;//!< state to switch after time wait
	my_fsm_signal_t signals;//!< reserved for future use
	void* private_data;//!< user private data
};

//signal constants
#define my_fsm_sig_no ((my_fsm_signal_t)0) //!<no signal

//predefined states
/**
 * @brief specail wait state
 * @warning do not switch manualy !!!
 */
extern void my_fsm_state_wait(my_fsm *machine);

//sate constants
#define my_fsm_state_no (NULL) //!<no active state

/**
 * @brief return state signals
 * @param [in] my_fsm* - state machine params pointer
 * @return my_fsm_signal_t - signals 
 **/
extern my_fsm_signal_t my_fsm_get_signals(my_fsm *machine);

/**
 * @brief function to switch state
 * @param [in] my_fsm* machine - state machine params pointer
 * @param [in] delay_mks - microseconds delay before switching to a new state. if zero then switch immediately
 * @param [in] next_state - next state callback ( see \c my_fsm_callback )
 * @warning call from state callback only !!
 **/
extern void my_fsm_switch_state(my_fsm *machine, uint32_t delay_mks, my_fsm_callback next_state);

/**
 * @brief checks if there was a state switch
 * @param [in] my_fsm* machine - state machine params pointer.
 * @return bool true - state switch (first run of state callback)
 *              false - no state switch
 * @warning call from state callback only !!
 **/
extern bool my_fsm_is_st_switch(my_fsm *machine);

/**
 * @brief init state machine function. Call it once before my_fsm_proc function.
 * @param [in] my_fsm* machine - state machine params pointer.
 * @param [in] my_fsm_callback initial_state - initial state.
 * @param [in] privat_data pointer to user data.
 * @return int8_t - 0 if everything fine otherwise error code
 **/
extern int8_t my_fsm_init(my_fsm *machine, my_fsm_callback initial_state, void* private_data);

/**
 * @brief main processing function call it in cycle as frequently as possible.
 * @param [in] my_fsm* - state machine params pointer
 * @return int8_t - 0 if everything fine otherwise error code
 **/
extern int8_t my_fsm_proc(my_fsm *machine);


#endif//_MY_FSM_H_

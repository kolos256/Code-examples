#include "my_fsm.h"

my_fsm_signal_t my_fsm_get_signals(my_fsm *machine){
	return machine->signals;
}

bool my_fsm_is_st_switch(my_fsm *machine){
	return (machine->prev_state != machine->next_state)?true:false;
}

void my_fsm_switch_state(my_fsm *machine, uint32_t delay_mks, my_fsm_callback next_state){
	if(delay_mks != 0){
		machine->state_wait_delay = HAL_mks_to_ticks(delay_mks);
		machine->next_state = my_fsm_state_wait;
		machine->after_wait_state = next_state;
		machine->state_wait_start_tick = HAL_GetRealTick();
	} else {
		machine->next_state = next_state;
	}
}

void my_fsm_state_wait(my_fsm *machine){
	if((HAL_GetRealTick() - machine->state_wait_start_tick) >= machine->state_wait_delay){
		//switch state
		machine->next_state = machine->after_wait_state;
	}
}

int8_t my_fsm_init(my_fsm *machine, my_fsm_callback initial_state, void* private_data){
	machine->signals = my_fsm_sig_no;
	machine->current_state = initial_state;
	machine->prev_state = my_fsm_state_no;
	machine->next_state = initial_state;
	machine->after_wait_state = my_fsm_state_no;
	machine->private_data = private_data;
	return 0;
}

int8_t my_fsm_proc(my_fsm *machine){
	// switch to next state
	machine->current_state = machine->next_state;
	if(machine->current_state!=my_fsm_state_no){
		machine->current_state(machine);
	}
	
	if(machine->current_state != my_fsm_state_wait){
		// remember previous state except my_fsm_state_wait (remember state before switch)
		machine->prev_state = machine->current_state;
	}
	return 0;
}

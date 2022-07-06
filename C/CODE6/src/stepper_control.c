/*****************************************************************
AD0 - PF0
AD1 - PF1

AIN1 - PA0
AIN2 - PA1
PWMA - PA6 TIM3_CH1 AF1

STBY - PA2
BIN1 - PA3
BIN2 - PA4 
PWMB - PA7 TIM3_CH2 AF1

        +--------+
        | TB6612 |      
 --AIN1-+        |       --+
 --AIN2-+        +-A01     | stepper phase A
 --PWMA-+        +-A02     |
       	|        |       --+
 --BIN1-+        |       --+
 --BIN2-+        +-B01     | stepper phase B
 --PWMB-+        +-B02     |
        |        |       --+
 --STBY-+        |
        |        |
        +--------+


******************************************************************/
#include "stepper_control.h"

/*************************************************************************
 * low level hardware code
 ***********************************************************************/
#define my_stepper_hardware_phase_pwm_30khz 1600
void my_stepper_hw_standby(bool state){
	if(state){
		GPIOA->BSRR=(GPIO_PIN_2<<16);
	}else{
		GPIOA->BSRR=GPIO_PIN_2;
	}
}
uint8_t my_stepper_hw_init(my_stepper_control *stepper){
	//half step phase sequance
	static int16_t phase_a_seq[8]={1600, 0   ,-1600,-1600,-1600, 0   , 1600, 1600};
	static int16_t phase_b_seq[8]={1600, 1600, 1600, 0   ,-1600,-1600,-1600, 0   };
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	TIM3->CR1&=(~TIM_CR1_CEN); //disable TIM3
	TIM3->CR1=(TIM_CR1_ARPE|(0<<TIM_CR1_CMS_Pos)|(TIM_COUNTERMODE_UP<<TIM_CR1_DIR_Pos));
	TIM3->CR2=0x00000000;
	TIM3->SMCR=0x00000000;
	TIM3->DIER=0x00000000;
	TIM3->SR=0x00000000;
	TIM3->CCMR1=(
		(0x6<<TIM_CCMR1_OC1M_Pos)|TIM_CCMR1_OC1PE|TIM_CCMR1_OC1FE|(0<<TIM_CCMR1_CC1S_Pos)|
		(0x6<<TIM_CCMR1_OC2M_Pos)|TIM_CCMR1_OC2PE|TIM_CCMR1_OC2FE|(0<<TIM_CCMR1_CC2S_Pos)
	);
	TIM3->CCMR2=0x00000000;
	TIM3->CCER=(TIM_CCER_CC2E|TIM_CCER_CC1E);
	TIM3->CNT=0;
	TIM3->PSC=0;
	TIM3->ARR=my_stepper_hardware_phase_pwm_30khz-1;
	TIM3->CCR1=my_stepper_hardware_phase_pwm_30khz/2;
	TIM3->CCR2=my_stepper_hardware_phase_pwm_30khz/2;
	TIM3->CCR3=0x00000000;
	TIM3->CCR4=0x00000000;
	TIM3->DCR=0x00000000;
	TIM3->DMAR=0x00000000;
	TIM3->CR1|=TIM_CR1_CEN; //enable TIM3
	TIM3->EGR=1;
	GPIOA->ODR&=~(GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  //must be init before hold
	stepper->micro_step = 0;
	stepper->phase_a_data=phase_a_seq;
	stepper->phase_b_data=phase_b_seq;
	stepper->phase_data_length=(sizeof(phase_a_seq)/sizeof(phase_a_seq[0]));
	my_stepper_hw_standby(false);

	return 0;
}

my_stepper_hw_state my_stepper_hw_do_micro_step(uint8_t mode, int32_t phase_a, int32_t phase_b){
	(void)mode;
	__disable_irq();
	if(phase_a == 0){
		GPIOA->BSRR=((GPIO_PIN_0|GPIO_PIN_1)<<16);
	}else{
		if(phase_a > 0){
			GPIOA->BSRR=((GPIO_PIN_0<<16)|GPIO_PIN_1);
		}else{
			GPIOA->BSRR=(GPIO_PIN_0|(GPIO_PIN_1<<16));
		}
	}
	if(phase_b == 0){
		GPIOA->BSRR=((GPIO_PIN_3|GPIO_PIN_4)<<16);
	}else{
		if(phase_b > 0){
			GPIOA->BSRR=((GPIO_PIN_3<<16)|GPIO_PIN_4);
		}else{
			GPIOA->BSRR=(GPIO_PIN_3|(GPIO_PIN_4<<16));
		}
	}
	TIM3->CCR1=(phase_a<0)?-phase_a:phase_a;
	TIM3->CCR2=(phase_b<0)?-phase_b:phase_b;
	__enable_irq();
	return ok;
}

int32_t my_stepper_hw_power_calculate(int32_t pwm, int32_t power){
	return ((pwm*power)/1000L);
}

my_stepper_hw_state my_stepper_hw_do_step(my_stepper_control *stepper){
	stepper->micro_step+=stepper->direction;
	if(stepper->micro_step >= stepper->phase_data_length){
		stepper->micro_step=0;
	}
	if(stepper->micro_step < 0){
		stepper->micro_step=(stepper->phase_data_length-1);
	}
	my_stepper_hw_do_micro_step(
		0,
		my_stepper_hw_power_calculate((int32_t)stepper->phase_a_data[stepper->micro_step],(int32_t)stepper->active_power),
		my_stepper_hw_power_calculate((int32_t)stepper->phase_b_data[stepper->micro_step],(int32_t)stepper->active_power)
	);
	if((stepper->micro_step)&0x01){
		stepper->step_complite = false;
	}else{
		stepper->step_complite = true;
	}
	return ok;
}

my_stepper_hw_state my_stepper_hw_hold(my_stepper_control *stepper){
	my_stepper_hw_do_micro_step(
		0,
		my_stepper_hw_power_calculate((int32_t)stepper->phase_a_data[stepper->micro_step],(int32_t)stepper->hold_power),
		my_stepper_hw_power_calculate((int32_t)stepper->phase_b_data[stepper->micro_step],(int32_t)stepper->hold_power)
	);
	return ok;
}

my_stepper_hw_state my_stepper_hw_unhold(my_stepper_control *stepper){
	my_stepper_hw_do_micro_step(
		0,
		my_stepper_hw_power_calculate((int32_t)stepper->phase_a_data[stepper->micro_step],(int32_t)stepper->active_power),
		my_stepper_hw_power_calculate((int32_t)stepper->phase_b_data[stepper->micro_step],(int32_t)stepper->active_power)
	);
	return ok;
}

/** ---------------------------- my_stepper fsm states ----------------------------**/
void my_stepper_hold_proc(my_fsm *machine);
void my_stepper_step_start_proc(my_fsm *machine);
void my_stepper_step_end_proc(my_fsm *machine);
void my_stepper_hold_wait(my_fsm *machine);

void my_stepper_hold_proc(my_fsm *machine){
	my_stepper_control *stepper = (my_stepper_control*)(machine->private_data);
	if(stepper->steps_to_do>0){
		my_stepper_hw_unhold(stepper);
		my_fsm_switch_state(machine,stepper->step_period,my_stepper_step_start_proc);
	}
}

void my_stepper_hold_wait_proc(my_fsm *machine){
	my_stepper_control *stepper = (my_stepper_control*)(machine->private_data);
	if((HAL_GetRealTick() - stepper->hold_start_tick) >= stepper->hold_period){
		my_stepper_hw_hold(stepper);
		my_fsm_switch_state(machine,0,my_stepper_hold_proc);
		stepper->state=state_hold;
	} else {
		if(stepper->steps_to_do > 0){
			my_fsm_switch_state(machine,0,my_stepper_step_start_proc);
		}
	}
}

void my_stepper_step_start_proc(my_fsm *machine){
	my_stepper_control *stepper = (my_stepper_control*)(machine->private_data);
	if(my_fsm_is_st_switch(machine)){
		stepper->state=state_rotate;
	}
	stepper->hw_state=my_stepper_hw_do_step(stepper);
	my_fsm_switch_state(machine, stepper->step_period,my_stepper_step_end_proc);
}

void my_stepper_step_end_proc(my_fsm *machine){
	my_stepper_control *stepper = (my_stepper_control*)(machine->private_data);
	if(stepper->step_complite){ // wait step completion
		stepper->step_counter+=stepper->direction;
		if(stepper->steps_to_do > 0){
			stepper->steps_to_do--;
			my_fsm_switch_state(machine,0,my_stepper_step_start_proc);
		}
		if(stepper->steps_to_do <= 0){
			stepper->hold_start_tick=HAL_GetRealTick();
			my_fsm_switch_state(machine,0,my_stepper_hold_wait_proc);
			stepper->state=state_stop;
		}
	} else { //is step not compliate switch to step start
		my_fsm_switch_state(machine,0,my_stepper_step_start_proc);
	}
}

/** ---------------------------- my_stepper main code ----------------------------**/

my_stepper_state my_stepper_control_init(my_stepper_control *stepper,int32_t start_step_counter,int32_t active_power,int32_t hold_power, uint32_t hold_period_mks){
	stepper->step_counter=start_step_counter;
	stepper->step_period = 0;
	stepper->micro_step = 0;
	stepper->steps_to_do = 0;
	stepper->direction = 0;
	stepper->active_power = active_power;
	stepper->hold_power = hold_power;
	stepper->hold_period = HAL_mks_to_ticks(hold_period_mks);
	stepper->hw_state = my_stepper_hw_init(stepper);
	stepper->state=state_hold;
	my_stepper_hw_hold(stepper);
	stepper->step_complite = false;
	my_fsm_init(&(stepper->main_fsm),my_stepper_hold_proc,(void*)stepper);
	return state_hold;
}

void my_stepper_control_set_active_power(my_stepper_control *stepper, int32_t active_power){
	stepper->active_power = active_power;
	if(stepper->state == state_hold){
		// rehold if already in hold state
		my_stepper_hw_hold(stepper);
	}
}

void my_stepper_control_set_hold_power(my_stepper_control *stepper, int32_t hold_power){
	stepper->hold_power = hold_power;
}

void my_stepper_move(my_stepper_control *stepper,int32_t frq, uint32_t steps){
	if(frq < 0){
		frq = - frq;
		stepper->direction = -1;
	} else {
		stepper->direction =  1;
	}
	stepper->step_period = (500000/frq);	
	stepper->steps_to_do = steps;
}

void my_stepper_control_proc(my_stepper_control *stepper){
	my_fsm_proc(&(stepper->main_fsm));
}

my_stepper_state my_stepper_control_get_state(my_stepper_control *stepper){
	return stepper->state;
}

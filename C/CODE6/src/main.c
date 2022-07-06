/*---------------------------------includes---------------------------------*/
#include "stm32f0xx_hal.h"
#include "USART.h"
#include "stepper_control.h"
#include <stdio.h>
/*----------------------------interrupt handlers----------------------------*/
void NMI_Handler(void)
{
  while(1);
}

/*@brief This function handles Hard fault interrupt*/
void HardFault_Handler(void){
  while(1);
}

/*@brief This function handles Memory management fault*/
void MemManage_Handler(void){
  while(1);
}

/*@brief This function handles Pre-fetch fault, memory access fault*/
void BusFault_Handler(void){
  while(1);
}

/*@brief This function handles Undefined instruction or illegal state*/
void UsageFault_Handler(void){
  while(1);
}

/*@brief This function handles System service call via SWI instruction*/
void SVC_Handler(void){
  while(1);
}

/*@brief This function handles Debug monitor*/
void DebugMon_Handler(void)
{
  while(1);
}

/*@brief This function handles Pendable request for system service*/
void PendSV_Handler(void)
{
  while(1);
}

/*@brief This function handles System tick timer*/
void SysTick_Handler(void)
{
	//GPIOA->ODR^=GPIO_PIN_9;
  HAL_IncTick();
}

/*----------------------------main code----------------------------*/

/**@brief System Clock Configuration*/
int8_t SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){
    return 1;
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK){
    return 2;
  }
  return 0;
}

uint32_t testdata1=0x55665566;

void comm_cmd_test1(){
	testdata1 = *((uint32_t*)(COMM_RXBUFFER+1));
}


//
void test1_init(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIOA->ODR|=(GPIO_PIN_9|GPIO_PIN_10);
}

my_stepper_control stepper1;

void state1_proc(my_fsm *machine);
void state2_proc(my_fsm *machine);

void state1_proc(my_fsm *machine){
	GPIOA->ODR&=~(GPIO_PIN_9);
	my_fsm_switch_state(machine,1000,state2_proc);
}

void state2_proc(my_fsm *machine){
	GPIOA->ODR|=GPIO_PIN_9;
	my_fsm_switch_state(machine,2000,state1_proc);
}


/** ------------------------ motor commands -------------------------*/
int32_t stepper1_speed=0;
int32_t stepper1_steps=0;
void comm_cmd_set_speed(){
	stepper1_speed = *((int32_t*)(COMM_RXBUFFER+1));
}

void comm_cmd_move(){
	stepper1_steps = *((int32_t*)(COMM_RXBUFFER+1));
  my_stepper_move(&stepper1, stepper1_speed, (uint32_t)stepper1_steps);
}

void comm_cmd_set_active_power(){
  my_stepper_control_set_active_power(&stepper1, *((int32_t*)(COMM_RXBUFFER+1)));
}

void comm_cmd_set_hold_power(){
  my_stepper_control_set_hold_power(&stepper1, *((int32_t*)(COMM_RXBUFFER+1)));
}

int main(){
	uint8_t Test1[]="Test1 \r\n";
	HAL_Init();
	SystemClock_Config();
  USART1_UART_Init();
  my_stepper_control_init(&stepper1,1000,500,100,1000000);
  /**-------------------------regiser commands-------------------------**/
	COMM_CMDREG(0x00,NULL,0x00,0x04,comm_cmd_set_speed);
  COMM_CMDREG(0x01,NULL,0x00,0x04,comm_cmd_move);
  COMM_CMDREG(0x02,NULL,0x00,0x04,comm_cmd_set_active_power);
  COMM_CMDREG(0x03,NULL,0x00,0x04,comm_cmd_set_hold_power);
  COMM_CMDREG(0x04,(void*)&(stepper1.state),sizeof(stepper1.state),0x00,NULL);
	
  for(;;){
    my_stepper_control_proc(&stepper1);
    COMM_CMDPROC();
	}
}
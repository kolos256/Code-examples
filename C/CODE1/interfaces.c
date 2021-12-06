/***********************************************************************************************************************************************
 * @file  <MULTITASKINGV1CORE.s>
 * @brief < The file contains the implementation of basic functions for working with hardware interfaces UART SPI I2C >
 * code is implemented using the HAL library and freeRTOS multitasking 
 * 
 * @author <Dmitry Kolosyanko Dmitriy.kolosyanko@gmail.com>
 * @date   <10/03/2018>
 *
 ***********************************************************************************************************************************************/

#include "interfaces.h"

/*******************************************************
 *
 * I2C1 PCLK1 54000000 mhz
 * I2C1_SMBA - PE3
 * I2C1_SCL  - PB7 AF4
 * I2C1_SDA  - PB8 AF4
 * I2C1_EV - 31
 * I2C1_ER - 32
 *
 *
 * DMA1 STREAM5 CHANNEL1 - I2C1RX
 * DMA1 STREAM6 CHANNEL1 - I2C1TX
 *
 *
 *******************************************************/
BaseType_t xHigherPriorityTaskWoken1;

xSemaphoreHandle i2c1_evt1=NULL;
I2C_HandleTypeDef hi2c1;

xSemaphoreHandle i2c2_evt1=NULL;
I2C_HandleTypeDef hi2c2;



struct I2C_Config{
	I2C_HandleTypeDef *hi2c;
	xSemaphoreHandle i2c_evt;
}I2C_Gloabal_Configs[2];

struct I2C_Config *Get_I2C_CONFIG(I2C_HandleTypeDef *hi2c)
{
	uint32_t i;
	for(i=0;i<(sizeof(I2C_Gloabal_Configs)/sizeof(I2C_Gloabal_Configs[0]));i++)
	{
		if(I2C_Gloabal_Configs[i].hi2c==hi2c)
		{
			return &I2C_Gloabal_Configs[i];
		}
	}
	return NULL;
}

/*************************************
* i2c interrupts handlers
*
**************************************/

void I2C1_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&hi2c1);
}

void I2C1_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&hi2c1);
}

void I2C2_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&hi2c2);
}

void I2C2_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&hi2c2);
}


//require FreeRtos
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	BaseType_t xHigherPriorityTaskWoken1;
  switch(((uint32_t)hi2c->Instance))
  {
    case ((uint32_t)I2C1):
		xSemaphoreGiveFromISR(I2C_Gloabal_Configs[0].i2c_evt,xHigherPriorityTaskWoken1);
    break;
	case ((uint32_t)I2C2):
		xSemaphoreGiveFromISR(I2C_Gloabal_Configs[1].i2c_evt,xHigherPriorityTaskWoken1);
	break;
    default:
      while(1);
      break;
  }
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	BaseType_t xHigherPriorityTaskWoken1;
  switch(((uint32_t)hi2c->Instance))
  {
    case ((uint32_t)I2C1):
		xSemaphoreGiveFromISR(I2C_Gloabal_Configs[0].i2c_evt,xHigherPriorityTaskWoken1);
    break;
	case ((uint32_t)I2C2):
		xSemaphoreGiveFromISR(I2C_Gloabal_Configs[1].i2c_evt,xHigherPriorityTaskWoken1);
    break;
	default:
    	while(1);
    	break;
  }

}




uint8_t i2c_readregs(I2C_HandleTypeDef *hi2c,uint8_t devaddr, void* databuf, uint8_t startregaddr,uint16_t regstoread)
{
	uint8_t STATE1;
	uint32_t rxtimeoutmks=MAX(pdMKS_TO_TICKS((regstoread+1)*100),1);
	uint32_t txtimeoutmks=pdMKS_TO_TICKS(2*100);
	struct I2C_Config *I2CCFG;
	if((I2CCFG=Get_I2C_CONFIG(hi2c))==NULL){
		#if defined(DBG)
		DEBUGV1_PUTS("\r\nERROR1.1: ");
		#endif
		return 1;
	}
	if(I2CCFG->i2c_evt==NULL){
		#if defined(DBG)
		DEBUGV1_PUTS("\r\nERROR1.2: ");
		#endif
		return 2;
	}
	if(STATE1=HAL_I2C_Master_Transmit_IT(hi2c,(devaddr<<1),&startregaddr,1)){
		#if defined(DBG)
		DEBUGV1_PUTS("\r\nERROR1.3: ");
		#endif
		DEBUGV1_PUTS("\n\rSTATE1: ");
		DEBUGV1_U32PRINT(0,STATE1,10);
		HAL_I2C_Master_Abort_IT(hi2c,((devaddr<<1)|1));
		return 3;
	}
	if(!xSemaphoreTake(I2CCFG->i2c_evt,txtimeoutmks)){
		#if defined(DBG)
		DEBUGV1_PUTS("\r\nERROR1.4: ");
		#endif
		HAL_I2C_Master_Abort_IT(hi2c,((devaddr<<1)|1));
		return 4;
	}
	if((hi2c1.State)!=HAL_I2C_STATE_READY){
		#if defined(DBG)
		DEBUGV1_PUTS("\r\nERROR1.5: ");
		#endif
		HAL_I2C_Master_Abort_IT(hi2c,((devaddr<<1)|1));
		return 5;
	}
	if(HAL_I2C_Master_Receive_IT(hi2c,((devaddr<<1)|1), (uint8_t*)databuf,regstoread)){
		#if defined(DBG)
		DEBUGV1_PUTS("\r\nERROR1.6: ");
		#endif
		return 6;
	}
	if(!xSemaphoreTake(I2CCFG->i2c_evt,rxtimeoutmks)){
		#if defined(DBG)
		DEBUGV1_PUTS("\r\nERROR1.7: ");
		#endif
		HAL_I2C_Master_Abort_IT(hi2c,((devaddr<<1)|1));
		return 7;
	}
	if((hi2c->State)!=HAL_I2C_STATE_READY){
		#if defined(DBG)
		DEBUGV1_PUTS("\r\nERROR1.8: ");
		#endif
		HAL_I2C_Master_Abort_IT(hi2c,((devaddr<<1)|1));
		return 8;
	}
	return 0;
}

uint8_t i2c_writeregs(I2C_HandleTypeDef *hi2c,uint8_t devaddr, void* databuf, uint8_t startregaddr,uint16_t regstowrite)
{
	uint8_t TMPBUFFER[32];
	uint32_t txtimeoutmks=MAX(pdMKS_TO_TICKS(((regstowrite+1)*100)),1);
	struct I2C_Config *I2CCFG;
	if((I2CCFG=Get_I2C_CONFIG(hi2c))==NULL){
		return 1;
	}
	if(I2CCFG->i2c_evt==NULL){
		return 2;
	}
	if((regstowrite+1)>(sizeof(TMPBUFFER))){
		return 3;
	}
	TMPBUFFER[0]=startregaddr;
	for(uint32_t i=0;i<regstowrite;i++){
		TMPBUFFER[i+1]= ((uint8_t*)databuf)[i];
	}
	if(HAL_I2C_Master_Transmit_IT(hi2c,(devaddr<<1),TMPBUFFER,(regstowrite+1))){
		return 4;
	}
	if(!xSemaphoreTake(I2CCFG->i2c_evt,txtimeoutmks)){
		HAL_I2C_Master_Abort_IT(hi2c,(devaddr<<1));
		return 5;
	}
	if((hi2c->State)!=HAL_I2C_STATE_READY){
		HAL_I2C_Master_Abort_IT(hi2c,(devaddr<<1));
		return 6;
	}	

	return 0;
}

/**********************
 *
 * PB8  - I2C1_SCL  AF4
 * PB9  - I2C1_SDA  AF4
 * PB5  - I2C1_SMBA AF4
 * PA15 - TS_RST
 *
 **********************/

uint8_t I2C1_INIT(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();


	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x00501D57;//400khz
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		return 1;
	}

	/**Configure Analogue filter*/
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		return 2;
	}

	/**Configure Digital filter*/
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 4) != HAL_OK)
	{
		return 3;
	}

    
    	/*Configure GPIO pin : PB7,8 */
	GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(I2C1_EV_IRQn,10, 0);
	HAL_NVIC_SetPriority(I2C1_ER_IRQn,10, 0);
	HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
	HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

	return 0;
}






#if 1

/**********************
 *
 * PH4  - I2C1_SCL  AF4
 * PH5  - I2C1_SDA  AF4
 *
 **********************/
uint8_t I2C2_INIT(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_I2C2_CLK_ENABLE();


	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x00501D57;//400khz
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK)
	{
		return 1;
	}

	/**Configure Analogue filter*/
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		return 2;
	}

	/**Configure Digital filter*/
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 4) != HAL_OK)
	{
		return 3;
	}
	#if 0
   	GPIOH->ODR|=0x00000030;
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
    	#else
	
	GPIOH->ODR|=0x00000030;
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
	#endif
#if 1
	HAL_NVIC_SetPriority(I2C2_EV_IRQn,10, 0);
	HAL_NVIC_SetPriority(I2C2_ER_IRQn,10, 0);
	HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
	HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
#endif
	return 0;
}

uint8_t I2C2_REINIT(void)
{

	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_I2C2_CLK_ENABLE();

	__HAL_RCC_I2C2_FORCE_RESET();
	__HAL_RCC_I2C2_RELEASE_RESET();
	
	HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
	HAL_NVIC_DisableIRQ(I2C2_ER_IRQn);
	HAL_NVIC_ClearPendingIRQ(I2C1_EV_IRQn);
	HAL_NVIC_ClearPendingIRQ(I2C1_EV_IRQn);


	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x00501D57;//400khz
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK)
	{
		return 1;
	}

	/**Configure Analogue filter*/
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		return 2;
	}

	/**Configure Digital filter*/
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 4) != HAL_OK)
	{
		return 3;
	}

    

#if 1
	HAL_NVIC_SetPriority(I2C2_EV_IRQn,10, 0);
	HAL_NVIC_SetPriority(I2C2_ER_IRQn,10, 0);
	HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
	HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
#endif
	return 0;
}

/*************************************************************************************************************************/
const DMA_InitTypeDef DMARXMODE1={
		.Direction = DMA_PERIPH_TO_MEMORY,
		.PeriphInc = DMA_PINC_DISABLE,
		.MemInc = DMA_MINC_ENABLE,
		.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
		.MemDataAlignment = DMA_MDATAALIGN_BYTE,
		.Mode = DMA_NORMAL,
		.Priority = DMA_PRIORITY_LOW,
		.FIFOMode = DMA_FIFOMODE_ENABLE,
		.FIFOThreshold=DMA_FIFO_THRESHOLD_1QUARTERFULL,
		.MemBurst=DMA_MBURST_SINGLE,
		.PeriphBurst=DMA_MBURST_SINGLE
};

const DMA_InitTypeDef DMATXMODE1={
		.Direction = DMA_MEMORY_TO_PERIPH,
		.PeriphInc = DMA_PINC_DISABLE,
		.MemInc = DMA_MINC_ENABLE,
		.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
		.MemDataAlignment = DMA_MDATAALIGN_BYTE,
		.Mode = DMA_NORMAL,
		.Priority = DMA_PRIORITY_LOW,
		.FIFOMode = DMA_FIFOMODE_ENABLE,
		.FIFOThreshold=DMA_FIFO_THRESHOLD_1QUARTERFULL,
		.MemBurst=DMA_MBURST_SINGLE,
		.PeriphBurst=DMA_MBURST_SINGLE
};

const SPI_InitTypeDef SPIMODE1={
		.Mode = SPI_MODE_MASTER,
		.Direction = SPI_DIRECTION_2LINES,
		.DataSize = SPI_DATASIZE_8BIT,
		.CLKPolarity = SPI_POLARITY_LOW,
		.NSS = SPI_NSS_SOFT,
		.BaudRatePrescaler =SPI_BAUDRATEPRESCALER_64,
		.FirstBit = SPI_FIRSTBIT_MSB,
		.TIMode = SPI_TIMODE_DISABLE,
		.CRCCalculation = SPI_CRCCALCULATION_DISABLE,
		.CRCPolynomial = 10
};

#if defined(USESPI1)
//SPI1
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;
xQueueHandle SPI1_Q1=NULL;
#endif

//#define USESPI2

#if defined(USESPI2)
//SPI2
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi2_rx;
DMA_HandleTypeDef hdma_spi2_tx;
xQueueHandle SPI2_Q1=NULL;

/*************************************
 *
 *  SPI2_MOSI PB15 AF5
 *  SPI2_MISO PB14 AF5
 *  SPI2_SCK  PB13 AF5
 *  SPI2_NSS  PB12 AF5 ()
 *
 *
 * DMA1 Stream 4 CHANNEL0 SPI2_TX
 * DMA1 Stream 3 Channel0 SPI2_RX
 *
 *************************************/

uint8_t interfaces_SPI2_TEST(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	hspi2.Instance = SPI2;
	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_SPI2_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

#if 0
	while(1)
	{
		GPIOC->ODR^=0x02;
		HAL_Delay(10);
	}
#endif
#if 0
	while(1)
	{
		GPIOC->ODR^=0x04;
		HAL_Delay(10);
	}
#endif
#if 0
	while(1)
	{
		GPIOD->ODR^=0x08;
		HAL_Delay(10);
	}
#endif
#if 0
	while(1)
	{
		GPIOB->ODR^=0x10;
		HAL_Delay(10);
	}
#endif
	return 0;
}



uint8_t interfaces_SPI2_INIT(void)
{
#if 0
	interfaces_SPI2_TEST();
#endif
	GPIO_InitTypeDef GPIO_InitStruct;
	hspi2.Instance = SPI2;
	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_SPI2_CLK_ENABLE();

	/* SPI2 DMA Init */
	/* SPI2_RX Init */
	hdma_spi2_rx.Instance = DMA1_Stream3;

	base_memcpy((void*)&(hdma_spi2_rx.Init),(void*)&DMARXMODE1,sizeof(DMA_InitTypeDef));
	hdma_spi2_rx.Init.Channel=0;

	if (HAL_DMA_Init(&hdma_spi2_rx) != HAL_OK)
	{
		return 1;
	}

	__HAL_LINKDMA(&hspi2,hdmarx,hdma_spi2_rx);

	/* SPI2_TX Init */
	hdma_spi2_tx.Instance = DMA1_Stream4;
	base_memcpy((void*)&(hdma_spi2_tx.Init),(void*)&DMATXMODE1,sizeof(DMA_InitTypeDef));
	hdma_spi2_tx.Init.Channel=0;

	if (HAL_DMA_Init(&hdma_spi2_tx) != HAL_OK)
	{
		return 2;
	}
	__HAL_LINKDMA(&hspi2,hdmatx,hdma_spi2_tx);

	/* SPI1 parameter configuration*/
	hspi2.Instance = SPI2;

	base_memcpy((void*)&(hspi2.Init),(void*)&SPIMODE1,sizeof(SPI_InitTypeDef));
	if (HAL_SPI_Init(&hspi2) != HAL_OK)
	{
		return 3;
	}

	SPI_SELECT(1,DISABLE);


	/* DMA interrupt init */
	/* DMA1_Stream3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
	/* DMA1_Stream4_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

	HAL_NVIC_SetPriority(SPI2_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(SPI2_IRQn);

	if(SPI2_Q1==NULL)
	{
		SPI2_Q1=xQueueCreate(2,sizeof(uint8_t));
	}
	if(SPI2_Q1==NULL)
	{
		return 4;
	}


	GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	return 0;
}


void DMA1_Stream4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_spi2_tx);
}

void DMA1_Stream3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_spi2_rx);
}

void SPI2_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&hspi2);
}

#endif


#if defined(USESPI6)

/*************************************
 *
 * init spi6 interface
 * SPI6_MISO PG12 AF5
 * SPI6_SCK	 PG13 AF5
 * SPI6_MOSI PG14 AF5
 *
 * DMA2 Stream 5 Channel1 SPI6_TX
 * DMA2 Stream 6 Channel1 SPI6_RX
 *
 *
 *************************************/

SPI_HandleTypeDef hspi6;
DMA_HandleTypeDef hdma_spi6_rx;
DMA_HandleTypeDef hdma_spi6_tx;
xQueueHandle SPI6_Q1=NULL;


uint8_t interfaces_SPI6_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	hspi6.Instance = SPI6;
	__HAL_RCC_DMA2_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOJ_CLK_ENABLE();
	__HAL_RCC_SPI6_CLK_ENABLE();

    #if 0
    DEBUGV1_PUTS("\n\rinterfaces_SPI6_INIT");
    for(;;);
    #endif
	/* SPI6 DMA Init */
	/* SPI6_RX Init */
	hdma_spi6_rx.Instance = DMA2_Stream6;
	hdma_spi6_rx.Init.Channel = DMA_CHANNEL_1;
	hdma_spi6_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_spi6_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_spi6_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_spi6_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_spi6_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_spi6_rx.Init.Mode = DMA_NORMAL;
	hdma_spi6_rx.Init.Priority = DMA_PRIORITY_LOW;
	hdma_spi6_rx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	hdma_spi6_rx.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_1QUARTERFULL;
	hdma_spi6_rx.Init.MemBurst=DMA_MBURST_SINGLE;
	hdma_spi6_rx.Init.PeriphBurst=DMA_MBURST_SINGLE;
	if (HAL_DMA_Init(&hdma_spi6_rx) != HAL_OK)
	{
		return 1;
	}

	__HAL_LINKDMA(&hspi6,hdmarx,hdma_spi6_rx);

	/* SPI6_TX Init */
	hdma_spi6_tx.Instance = DMA2_Stream5;
	hdma_spi6_tx.Init.Channel = DMA_CHANNEL_1;
	hdma_spi6_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	hdma_spi6_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_spi6_tx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_spi6_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_spi6_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_spi6_tx.Init.Mode = DMA_NORMAL;
	hdma_spi6_tx.Init.Priority = DMA_PRIORITY_LOW;
	hdma_spi6_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
	hdma_spi6_tx.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_1QUARTERFULL;
	hdma_spi6_tx.Init.MemBurst=DMA_MBURST_SINGLE;
	hdma_spi6_tx.Init.PeriphBurst=DMA_MBURST_SINGLE;


	if (HAL_DMA_Init(&hdma_spi6_tx) != HAL_OK)
	{
		return 2;
	}
	__HAL_LINKDMA(&hspi6,hdmatx,hdma_spi6_tx);

	/* SPI6 parameter configuration*/
	hspi6.Instance = SPI6;
	hspi6.Init.Mode = SPI_MODE_MASTER;
	hspi6.Init.Direction = SPI_DIRECTION_2LINES;
	hspi6.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi6.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi6.Init.NSS = SPI_NSS_SOFT;
	hspi6.Init.BaudRatePrescaler =SPI_BAUDRATEPRESCALER_64;
	hspi6.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi6.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi6.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi6.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi6) != HAL_OK)
	{
		return 3;
	}

	GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI6;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	//init select 1

	SPI_SELECT(1,DISABLE);

	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	//init select 2
	SPI_SELECT(2,DISABLE);

	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

	/* DMA interrupt init */
	/* DMA2_Stream5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
	/* DMA2_Stream6_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

	HAL_NVIC_SetPriority(SPI6_IRQn, 10, 0);
	HAL_NVIC_EnableIRQ(SPI6_IRQn);

	if(SPI6_Q1==NULL)
	{
		SPI6_Q1=xQueueCreate(2,sizeof(uint8_t));
	}
	if(SPI6_Q1==NULL)
	{
		return 4;
	}

	return 0;
}


void DMA2_Stream5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_spi6_tx);
}

void DMA2_Stream6_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_spi6_rx);
}

void SPI6_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&hspi6);
}

#endif


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	uint8_t TMP1;
	BaseType_t xHigherPriorityTaskWoken=pdFALSE;
#if defined(USESPI1)
	if(hspi==&hspi1)
	{
		TMP1=0x01;
		if(SPI1_Q1){xQueueSendFromISR(SPI1_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif
#if defined(USESPI2)
	if(hspi==&hspi2)
	{
		TMP1=0x01;
		if(SPI2_Q1){xQueueSendFromISR(SPI2_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

#if defined(USESPI6)
	if(hspi==&hspi6)
	{
		TMP1=0x01;
		if(SPI6_Q1){xQueueSendFromISR(SPI6_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	uint8_t TMP1;
	BaseType_t xHigherPriorityTaskWoken=pdFALSE;
#if defined(USESPI1)
	if(hspi==&hspi1)
	{
		TMP1=0x02;
		if(SPI1_Q1){xQueueSendFromISR(SPI1_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif
#if defined(USESPI2)
	if(hspi==&hspi2)
	{
		TMP1=0x02;
		if(SPI2_Q1){xQueueSendFromISR(SPI2_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

#if defined(USESPI6)
	if(hspi==&hspi6)
	{
		TMP1=0x02;
		if(SPI6_Q1){xQueueSendFromISR(SPI6_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	uint8_t TMP1;
	BaseType_t xHigherPriorityTaskWoken=pdFALSE;
#if defined(USESPI1)
	if(hspi==&hspi1)
	{
		TMP1=0x03;
		if(SPI1_Q1){xQueueSendFromISR(SPI1_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

#if defined(USESPI2)
	if(hspi==&hspi2)
	{
		TMP1=0x03;
		if(SPI2_Q1){xQueueSendFromISR(SPI2_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

#if defined(USESPI6)
	if(hspi==&hspi6)
	{
		TMP1=0x03;
		if(SPI6_Q1){xQueueSendFromISR(SPI6_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

}

void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
	uint8_t TMP1;
	BaseType_t xHigherPriorityTaskWoken=pdFALSE;
#if defined(USESPI1)
	if(hspi==&hspi1)
	{
		TMP1=0x04;
		if(SPI1_Q1){xQueueSendFromISR(SPI1_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

#if defined(USESPI2)
	if(hspi==&hspi2)
	{
		TMP1=0x04;
		if(SPI2_Q1){xQueueSendFromISR(SPI2_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

#if defined(USESPI6)
	if(hspi==&hspi6)
	{
		TMP1=0x04;
		if(SPI6_Q1){xQueueSendFromISR(SPI6_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

}

void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
	uint8_t TMP1;
	BaseType_t xHigherPriorityTaskWoken=pdFALSE;
#if defined(USESPI1)
	if(hspi==&hspi1)
	{
		TMP1=0x05;
		if(SPI1_Q1){xQueueSendFromISR(SPI1_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif
#if defined(USESPI2)
	if(hspi==&hspi2)
	{
		TMP1=0x05;
		if(SPI2_Q1){xQueueSendFromISR(SPI2_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

#if defined(USESPI6)
	if(hspi==&hspi6)
	{
		TMP1=0x05;
		if(SPI6_Q1){xQueueSendFromISR(SPI6_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

}

void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
	uint8_t TMP1;
	BaseType_t xHigherPriorityTaskWoken=pdFALSE;
#if defined(USESPI1)
	if(hspi==&hspi1)
	{
		TMP1=0x06;
		if(SPI1_Q1){xQueueSendFromISR(SPI1_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif
#if defined(USESPI2)
	if(hspi==&hspi2)
	{
		TMP1=0x06;
		if(SPI2_Q1){xQueueSendFromISR(SPI2_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

#if defined(USESPI6)
	if(hspi==&hspi6)
	{
		TMP1=0x06;
		if(SPI6_Q1){xQueueSendFromISR(SPI6_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	uint8_t TMP1;
	BaseType_t xHigherPriorityTaskWoken=pdFALSE;
#if defined(USESPI1)
	if(hspi==&hspi1)
	{
		TMP1=0x07;
		if(SPI1_Q1){xQueueSendFromISR(SPI1_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif
#if defined(USESPI2)
	if(hspi==&hspi2)
	{
		TMP1=0x07;
		if(SPI2_Q1){xQueueSendFromISR(SPI2_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

#if defined(USESPI6)
	if(hspi==&hspi6)
	{
		TMP1=0x07;
		if(SPI6_Q1){xQueueSendFromISR(SPI6_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

}

void HAL_SPI_AbortCpltCallback(SPI_HandleTypeDef *hspi)
{
	uint8_t TMP1;
	BaseType_t xHigherPriorityTaskWoken=pdFALSE;
#if defined(USESPI1)
	if(hspi==&hspi1)
	{
		TMP1=0x08;
		if(SPI1_Q1){xQueueSendFromISR(SPI1_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif
#if defined(USESPI2)
	if(hspi==&hspi2)
	{
		TMP1=0x08;
		if(SPI2_Q1){xQueueSendFromISR(SPI2_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif

#if defined(USESPI6)
	if(hspi==&hspi6)
	{
		TMP1=0x08;
		if(SPI6_Q1){xQueueSendFromISR(SPI6_Q1,&TMP1,&xHigherPriorityTaskWoken);}
	}
#endif
}


uint32_t SPI_GETSPICLK(SPI_TypeDef * SPIINSTANCE)
{
	//SPIINCLK
	//uint32_t HAL_RCC_GetSysClockFreq();
	//uint32_t HAL_RCC_GetHCLKFreq(void);
	//uint32_t HAL_RCC_GetPCLK1Freq(void);
	if(SPIINSTANCE==NULL)
	{
		return 0;
	}
	else if((SPIINSTANCE==SPI6)||(SPIINSTANCE==SPI5)||(SPIINSTANCE==SPI1)||(SPIINSTANCE==SPI4))
	{//PCLK2

		return HAL_RCC_GetPCLK2Freq();
	}
	else if((SPIINSTANCE==SPI3)||(SPIINSTANCE==SPI2))
	{
		return HAL_RCC_GetPCLK1Freq();
	}
	else
	{
		return 0;
	}
}
#if 0
uint32_t SPI_GETDIVIDER(uint32_t SPIINCLK,uint32_t REQCLK)
{
	uint32_t i=1,DIVCLK=(REQCLK/SPIINCLK);
	uint32_t DELTA;
	static const uint16_t DIVTBL[]={2,4,8,16,32,64,128,256};
	DELTA=ABS((DIVTBL[i]-DIVCLK));
	while(i<(sizeof(DIVTBL)/sizeof(DIVTBL[0])))
	{
		if(DELTA<ABS((DIVTBL[i]-DIVCLK)))
		{
			return DIVTBL[i-1];
		}
		DELTA=ABS((DIVTBL[i]-DIVCLK));
		i++;
	}
	return DIVTBL[i-1];
}
#endif

struct SPI_SELECT
{
	uint8_t PIN;
	GPIO_TypeDef *RXPORT;
};
const struct SPI_SELECT SELECT_TBL[]={
	{12,GPIOB},// SPI2_NSS (FLASH1)
    	{10,GPIOG} //SPI6_CS#_2 FRAM
};
/***************************************
 *
 * State 	true - set active level
 * 		false - set inactive
 *
 * SELECT - id of chip slect 0 - nothenig select
 * 		1 to N SELECT one of select lines
 *		N - number of configured select lines ( see const struct SPI_SELECT SELECT_TBL[])
 ***************************************/

uint8_t SPI_SELECT(uint8_t SELECT,uint8_t STATE)
{
	if(SELECT==0)
	{
		return 0;
	}
	else if(SELECT>((sizeof(SELECT_TBL)/sizeof(SELECT_TBL[0]))))
	{
		return 1;
	}
	else
	{
		//                                                 CLEAR LINE                               SET LINE
		SELECT_TBL[SELECT-1].RXPORT->BSRR=(STATE)?(0x00010000<<(SELECT_TBL[SELECT-1].PIN)):(0x00000001<<SELECT_TBL[SELECT-1].PIN);
		return 0;
	}
}


#if 1

uint32_t SPI_GETDIVIDER(uint32_t SPIINCLK,uint32_t REQCLK)
{
	uint32_t i=0,DIVCLK=(SPIINCLK/REQCLK);
	static const uint16_t DIVTBL[]={2,4,8,16,32,64,128,256};
	static const uint8_t CONSTTBL[]={0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38};

	if((REQCLK%SPIINCLK)==0)
	{
		while((i<((sizeof(DIVTBL)/sizeof(DIVTBL[0]))-1))&&(DIVCLK>DIVTBL[i]))
		{
			i++;
		}
	}
	else
	{
		while((i<((sizeof(DIVTBL)/sizeof(DIVTBL[0]))-1))&&(DIVCLK>=DIVTBL[i]))
		{
			i++;
		}
	}
	return (uint32_t)CONSTTBL[i];

}

uint8_t SPI_SELECT_DEVICE(uint8_t Select,SemaphoreHandle_t xMutex)
{
	if(xMutex!=NULL)
	{
		if(!xSemaphoreTake(xMutex,pdMKS_TO_TICKS(1000000)))
		{
			return 1;
		}
	}
	if(SPI_SELECT(Select,ENABLE))
	{
		if(xMutex!=NULL)
		{
			xSemaphoreGive(xMutex);
		}
		return 2;
	}
	return 0;
}

uint8_t SPI_DESELECT_DEVICE(uint8_t Select,SemaphoreHandle_t xMutex)
{
	if(SPI_SELECT(Select,DISABLE))
	{
		if(xMutex!=NULL)
		{
			xSemaphoreGive(xMutex);
		}
		return 1;
	}
	if(xMutex!=NULL)
	{
		xSemaphoreGive(xMutex);
	}
	return 0;
}

uint8_t SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi,xQueueHandle Queue, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size,uint32_t BaudRate)
{
	uint8_t MSGCODE=0;
	uint32_t BaudRatePrescaler;
	HAL_StatusTypeDef State1=HAL_OK;
#if 0
	TickType_t txrxtimeout=MAX((pdMKS_TO_TICKS(Size*50)),1);
#else
	TickType_t txrxtimeout=pdMS_TO_TICKS(100);
#endif
	if((Queue==NULL)||(pRxData==NULL)||(pTxData==NULL)||(Size==0)||(hspi==0))
	{
		return 1;
	}
	BaudRatePrescaler=SPI_GETDIVIDER(SPI_GETSPICLK(hspi->Instance),BaudRate);
	xQueueReceive(Queue,&MSGCODE,0);
	xQueueReceive(Queue,&MSGCODE,0);
#if 1
	if(BaudRatePrescaler!=hspi->Init.BaudRatePrescaler)
	{
		hspi->Init.BaudRatePrescaler=BaudRatePrescaler;
		if(HAL_SPI_Init(hspi) != HAL_OK)
		{
			return 3;
		}
	}
#endif

	if(State1=HAL_SPI_TransmitReceive_DMA(hspi,pTxData,pRxData,Size))
	{
		HAL_SPI_Abort_IT(hspi);
		return 5;
	}
	if(xQueueReceive(Queue,&MSGCODE,txrxtimeout))
	{
		if(MSGCODE!=0x06)
		{
			HAL_SPI_Abort_IT(hspi);
			return 6;
		}
	}
	else
	{
		HAL_SPI_Abort_IT(hspi);
		return 7;
	}

    return 0;
}

uint8_t SPI_Transmit_DMA(SPI_HandleTypeDef *hspi,xQueueHandle Queue, uint8_t *pTxData, uint16_t Size,uint32_t BaudRate)
{
	uint8_t MSGCODE=0;
	uint32_t BaudRatePrescaler;
	HAL_StatusTypeDef State1=HAL_OK;
#if 0
	TickType_t txtimeout=MAX((pdMKS_TO_TICKS(Size)*5),1);
#else
	TickType_t txtimeout=pdMS_TO_TICKS(100);
#endif
	if((Queue==NULL)||(pTxData==NULL)||(Size==0)||(hspi==0))
	{
		return 1;
	}
	BaudRatePrescaler=SPI_GETDIVIDER(SPI_GETSPICLK(hspi->Instance),BaudRate);
#if 1
	if(BaudRatePrescaler!=hspi->Init.BaudRatePrescaler)
	{
		hspi->Init.BaudRatePrescaler=BaudRatePrescaler;
		if (HAL_SPI_Init(hspi) != HAL_OK)
		{
			return 3;
		}
	}
#endif
	xQueueReceive(Queue,&MSGCODE,0);
	xQueueReceive(Queue,&MSGCODE,0);
	if(State1=HAL_SPI_Transmit_DMA(hspi,pTxData,Size))
	{
		HAL_SPI_Abort_IT(hspi);
		return 5;
	}
	do{
		if(xQueueReceive(Queue,&MSGCODE,txtimeout))
		{
			if(MSGCODE==0x04)
			{
				txtimeout=MAX((txtimeout/2),1);
			}
			else if(MSGCODE!=0x01)
			{
				HAL_SPI_Abort_IT(hspi);
				return 6;
			}
		}
		else
		{
			HAL_SPI_Abort_IT(hspi);
			return 7;
		}
	}while(MSGCODE!=0x01);
    return 0;
}

uint8_t SPI_Receive_DMA(SPI_HandleTypeDef *hspi,xQueueHandle Queue,uint8_t *pRxData, uint16_t Size,uint32_t BaudRate)
{
	uint8_t MSGCODE=0;
	uint32_t BaudRatePrescaler;
	HAL_StatusTypeDef State1=HAL_OK;
#if 0
	TickType_t rxtimeout=MAX((pdMKS_TO_TICKS(Size)*10),1);
#else
	TickType_t rxtimeout=pdMS_TO_TICKS(100);
#endif

	if((Queue==NULL)||(pRxData==NULL)||(Size==0))
	{
		return 1;
	}
	BaudRatePrescaler=SPI_GETDIVIDER(SPI_GETSPICLK(hspi->Instance),BaudRate);
#if 1
	if(BaudRatePrescaler!=hspi->Init.BaudRatePrescaler)
	{
		hspi->Init.BaudRatePrescaler=BaudRatePrescaler;
		if (HAL_SPI_Init(hspi) != HAL_OK)
		{
			return 3;
		}
	}
#endif
	xQueueReceive(Queue,&MSGCODE,0);
	xQueueReceive(Queue,&MSGCODE,0);
	if(State1=HAL_SPI_Receive_DMA(hspi,pRxData,Size))
	{
		HAL_SPI_Abort_IT(hspi);
		return 5;
	}
	do{
		if(xQueueReceive(Queue,&MSGCODE,rxtimeout))
		{
			if(MSGCODE==0x05)
			{
				rxtimeout=MAX((rxtimeout/2),1);
			}
			else if(MSGCODE!=0x02)
			{
				HAL_SPI_Abort_IT(hspi);
				return 6;
			}
		}
		else
		{
			HAL_SPI_Abort_IT(hspi);
			return 7;
		}
	}while(MSGCODE!=0x02);
    return 0;
}
#endif

/*****************************************
*
* Initialization of the main hardware interfaces
* 
******************************************/
uint8_t INTERFACES_INIT(void)
{
	uint8_t STATE=0;
#if defined(USESI2C1)
	#if 0
		i2c1_evt1=xSemaphoreCreateBinary();
		if(i2c1_evt1 == NULL)
		{
			return 1;
		}
	#else
		I2C_Gloabal_Configs[0].i2c_evt=xSemaphoreCreateBinary();
		I2C_Gloabal_Configs[0].hi2c=&hi2c1;
		if(I2C_Gloabal_Configs[0].i2c_evt == NULL)
		{
			return 1;
		}
	#endif

	if(STATE=I2C1_INIT())
	{
		return 2;
	}
#endif//USESI2C1

#if defined(USESI2C2)
	I2C_Gloabal_Configs[1].i2c_evt=xSemaphoreCreateBinary();
	I2C_Gloabal_Configs[1].hi2c=&hi2c2;
	if(I2C_Gloabal_Configs[1].i2c_evt == NULL)
	{
		return 3;
	}

	if(STATE=I2C2_INIT())
	{
		return 4;
	}
#endif//USESI2C2

#if defined(USEUSART3)
	if(interfaces_UART3_INIT())
	{
		return 5;
	}
#endif//defined(USEUSART3)

#if defined(USESPI6)
	if(interfaces_SPI6_INIT())
	{
		return exc1_SPI6_INIT;
	}
#endif

#if defined(USEUSART2)
	if(interfaces_UART2_INIT())
	{
		return 6;
	}
#endif//defined(USEUSART2)

#if defined(USESPI2)
	if(interfaces_SPI2_INIT())
	{
		return 7;
	}
#endif//defined(USESPI2)

	return 0;
}

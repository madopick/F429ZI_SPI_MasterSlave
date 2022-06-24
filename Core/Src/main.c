
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi3;
SPI_HandleTypeDef hspi4;
DMA_HandleTypeDef hdma_spi3_rx;
DMA_HandleTypeDef hdma_spi3_tx;
DMA_HandleTypeDef hdma_spi4_rx;
DMA_HandleTypeDef hdma_spi4_tx;

UART_HandleTypeDef huart3;



#ifdef __GNUC__
	/* With GCC, small printf (option LD Linker->Libraries->Small printf set to 'Yes') calls __io_putchar() */
	#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
	#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/******************************************************************
  * @name   PUTCHAR_PROTOTYPE
  * @brief  Retargets the C library printf function to the USART.
  *****************************************************************/
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}



/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI3_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_SPI4_Init(void);

/* Private user code ---------------------------------------------------------*/
/* Buffer used for transmission */
uint8_t aTxSlaveBuffer[] = "SPI Slave MSG1";

/* Buffer used for reception */
#define SPI_TIMEOUT_MAX           	0x1000
#define DATA_LENGTH 				sizeof(aTxSlaveBuffer)
uint8_t aRxBuffer[DATA_LENGTH];

static volatile uint8_t slave_send = 0;
static volatile uint8_t master_rcv = 0;
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  //MX_DMA_Init();
  MX_SPI3_Init();
  MX_SPI4_Init();
  MX_USART3_UART_Init();


  printf("main loop\r\n");
  if(HAL_SPI_Receive_IT(&hspi3, (uint8_t *)&aRxBuffer[0], DATA_LENGTH) != HAL_OK)
  {
	  printf("SPI master error\r\n");
  }


  /* Infinite loop */
  while (1)
  {
	  while((HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY) && (HAL_SPI_GetState(&hspi4) != HAL_SPI_STATE_READY))
	  {
		  HAL_Delay(50);
	  }

	  if (HAL_SPI_Receive_IT(&hspi3, (uint8_t *)&aRxBuffer[0], DATA_LENGTH) != HAL_OK)
	  {
		  printf("SPI master error\r\n");
	  }


	  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
	  HAL_Delay(1000);


//	  if(HAL_SPI_Receive(&hspi3, (uint8_t *)&aRxBuffer[0], DATA_LENGTH, 1000) != HAL_OK)
//	  {
//		  printf("SPI master error\r\n");
//	  }
//	  else
//	  {
//		  printf("RXM: %s\r\n\n",(char*)aRxBuffer);
//		  //printf("SPI master OK\r\n");
//	  }

	  //memset(aRxBuffer, 0, DATA_LENGTH);
  }
}





void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance == SPI3)
	{
		master_rcv = 1;
		printf("RXM: %s\r\n\n",(char*)aRxBuffer);
	}
	else if(hspi->Instance == SPI4)
	{
		printf("RXS: %s\r\n\n",(char*)aRxBuffer);
		slave_send = 1;
	}
}


void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance == SPI3)
	{
		printf("SPI Master transmited\r\n");
		slave_send = 0;
	}
	else if(hspi->Instance == SPI4)
	{
		printf("SPI Slave transmited\r\n");
		slave_send = 0;
	}
}



void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	printf("SPI TX RX CB\r\n");
}


/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	 if ((GPIO_Pin == USER_Btn_Pin) && (slave_send == 0))
	 {
		 //printf("button EXTI\r\n");
		 //slave_send = 1;

#if 1
		 if(HAL_SPI_Transmit_IT(&hspi4, aTxSlaveBuffer, sizeof(aTxSlaveBuffer)) != HAL_OK)
		 {
			 //Error_Handler();
		 }
#endif

	 }
}




/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState 		= RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState 	= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM 		= 4;
  RCC_OscInitStruct.PLL.PLLN 		= 168;
  RCC_OscInitStruct.PLL.PLLP 		= RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ 		= 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType 		= RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              	  	  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 	= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{
  /* SPI3 parameter configuration*/
  hspi3.Instance 			= SPI3;
  hspi3.Init.Mode 			= SPI_MODE_MASTER;
  hspi3.Init.Direction 		= SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize 		= SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity 	= SPI_POLARITY_HIGH;
  hspi3.Init.CLKPhase 		= SPI_PHASE_1EDGE;
  hspi3.Init.NSS 			= SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi3.Init.FirstBit 		= SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode 		= SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial 	= 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI4_Init(void)
{
  /* SPI4 parameter configuration*/
  hspi4.Instance 			= SPI4;
  hspi4.Init.Mode 			= SPI_MODE_SLAVE;
  hspi4.Init.Direction 		= SPI_DIRECTION_2LINES;
  hspi4.Init.DataSize 		= SPI_DATASIZE_8BIT;
  hspi4.Init.CLKPolarity 	= SPI_POLARITY_HIGH;
  hspi4.Init.CLKPhase 		= SPI_PHASE_1EDGE;
  hspi4.Init.NSS 			= SPI_NSS_SOFT;
  hspi4.Init.FirstBit 		= SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode 		= SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial 	= 10;
  if (HAL_SPI_Init(&hspi4) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{
  huart3.Instance 				= USART3;
  huart3.Init.BaudRate 			= 115200;
  huart3.Init.WordLength 		= UART_WORDLENGTH_8B;
  huart3.Init.StopBits 			= UART_STOPBITS_1;
  huart3.Init.Parity		 	= UART_PARITY_NONE;
  huart3.Init.Mode 				= UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl 		= UART_HWCONTROL_NONE;
  huart3.Init.OverSampling 		= UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin 	= USER_Btn_Pin;
  GPIO_InitStruct.Mode 	= GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull 	= GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /* Enable and set EXTI lines 15 to 10 Interrupt */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 15, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin 	= LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull 	= GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  printf("Error Handler\r\n");

  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
	  HAL_Delay(100);
	  HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

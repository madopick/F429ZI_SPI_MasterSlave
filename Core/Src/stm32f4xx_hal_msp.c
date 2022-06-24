/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32f4xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

extern DMA_HandleTypeDef hdma_spi3_rx;
extern DMA_HandleTypeDef hdma_spi3_tx;
extern DMA_HandleTypeDef hdma_spi4_rx;
extern DMA_HandleTypeDef hdma_spi4_tx;

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* External functions --------------------------------------------------------*/

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/
}

/**
* @brief SPI MSP Initialization
* This function configures the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hspi->Instance==SPI3)
  {
    /* Peripheral clock enable */
    __HAL_RCC_SPI3_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**SPI3 GPIO Configuration
    PC10     ------> SPI3_SCK
    PC11     ------> SPI3_MISO
    PC12     ------> SPI3_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//    /* SPI3 DMA Init */
//    /* SPI3_RX Init */
//    hdma_spi3_rx.Instance = DMA1_Stream0;
//    hdma_spi3_rx.Init.Channel = DMA_CHANNEL_0;
//    hdma_spi3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
//    hdma_spi3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_spi3_rx.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_spi3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    hdma_spi3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    hdma_spi3_rx.Init.Mode = DMA_NORMAL;
//    hdma_spi3_rx.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_spi3_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_spi3_rx) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    __HAL_LINKDMA(hspi,hdmarx,hdma_spi3_rx);
//
//    /* SPI3_TX Init */
//    hdma_spi3_tx.Instance = DMA1_Stream5;
//    hdma_spi3_tx.Init.Channel = DMA_CHANNEL_0;
//    hdma_spi3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_spi3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_spi3_tx.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_spi3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    hdma_spi3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    hdma_spi3_tx.Init.Mode = DMA_NORMAL;
//    hdma_spi3_tx.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_spi3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_spi3_tx) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    __HAL_LINKDMA(hspi,hdmatx,hdma_spi3_tx);

    HAL_NVIC_SetPriority(SPI3_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(SPI3_IRQn);

  }
  else if(hspi->Instance==SPI4)
  {
    /* Peripheral clock enable */
    __HAL_RCC_SPI4_CLK_ENABLE();

    __HAL_RCC_GPIOE_CLK_ENABLE();
    /**SPI4 GPIO Configuration
    PE2     ------> SPI4_SCK
    PE5     ------> SPI4_MISO
    PE6     ------> SPI4_MOSI
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

//    /* SPI4 DMA Init */
//    /* SPI4_RX Init */
//    hdma_spi4_rx.Instance = DMA2_Stream0;
//    hdma_spi4_rx.Init.Channel = DMA_CHANNEL_4;
//    hdma_spi4_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
//    hdma_spi4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_spi4_rx.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_spi4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    hdma_spi4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    hdma_spi4_rx.Init.Mode = DMA_NORMAL;
//    hdma_spi4_rx.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_spi4_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_spi4_rx) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    __HAL_LINKDMA(hspi,hdmarx,hdma_spi4_rx);
//
//    /* SPI4_TX Init */
//    hdma_spi4_tx.Instance = DMA2_Stream1;
//    hdma_spi4_tx.Init.Channel = DMA_CHANNEL_4;
//    hdma_spi4_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_spi4_tx.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_spi4_tx.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_spi4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    hdma_spi4_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    hdma_spi4_tx.Init.Mode = DMA_NORMAL;
//    hdma_spi4_tx.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_spi4_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_spi4_tx) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    __HAL_LINKDMA(hspi,hdmatx,hdma_spi4_tx);

    HAL_NVIC_SetPriority(SPI4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI4_IRQn);

  }

}

/**
* @brief SPI MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hspi: SPI handle pointer
* @retval None
*/
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
  if(hspi->Instance==SPI3)
  {
    /* Peripheral clock disable */
    __HAL_RCC_SPI3_CLK_DISABLE();

    /**SPI3 GPIO Configuration
    PC10     ------> SPI3_SCK
    PC11     ------> SPI3_MISO
    PC12     ------> SPI3_MOSI
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12);

    /* SPI3 DMA DeInit */
    HAL_DMA_DeInit(hspi->hdmarx);
    HAL_DMA_DeInit(hspi->hdmatx);
  /* USER CODE BEGIN SPI3_MspDeInit 1 */

  /* USER CODE END SPI3_MspDeInit 1 */
  }
  else if(hspi->Instance==SPI4)
  {
    /* Peripheral clock disable */
    __HAL_RCC_SPI4_CLK_DISABLE();

    /**SPI4 GPIO Configuration
    PE2     ------> SPI4_SCK
    PE5     ------> SPI4_MISO
    PE6     ------> SPI4_MOSI
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2|GPIO_PIN_5|GPIO_PIN_6);

    /* SPI4 DMA DeInit */
    HAL_DMA_DeInit(hspi->hdmarx);
    HAL_DMA_DeInit(hspi->hdmatx);
  }

}


/**
* @brief UART MSP Initialization
* This function configures the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(huart->Instance==USART3)
  {
    /* Peripheral clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    GPIO_InitStruct.Pin 		= STLK_RX_Pin|STLK_TX_Pin;
    GPIO_InitStruct.Mode 		= GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull 		= GPIO_NOPULL;
    GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate 	= GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  }

}

/**
* @brief UART MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param huart: UART handle pointer
* @retval None
*/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
  if(huart->Instance==USART3)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOD, STLK_RX_Pin|STLK_TX_Pin);
  }

}


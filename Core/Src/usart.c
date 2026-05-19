/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "usart.h"


UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

void dma_usart1_init(void);
static void usart1_rx_start(void);
static void usart1_rx_push(uint16_t size);

uint8_t usart1_rx_dma_buffer[USART1_RX_DMA_SIZE];
char usart1_rx_line[USART1_RX_DMA_SIZE];
volatile uint8_t usart1_rx_line_ready = 0;
static uint16_t usart1_rx_line_pos = 0;


/* USART1 init function */

void MX_USART1_UART_Init(void)
{


  huart1.Instance = USART1;
  huart1.Init.BaudRate = 57600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  
  dma_usart1_init();
  
  

  

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{


  huart2.Instance = USART2;
  huart2.Init.BaudRate = 57600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }


}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  }
  else if(uartHandle->Instance==USART2)
  {

    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {

    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);


  }
  else if(uartHandle->Instance==USART2)
  {

    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);


  }
}


void dma_usart1_init(void){
    //configure usart1 rx dma
  __HAL_RCC_DMA1_CLK_ENABLE();
  hdma_usart1_rx.Instance                 = DMA1_Channel5;
  hdma_usart1_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_usart1_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_usart1_rx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart1_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_usart1_rx.Init.Mode                = DMA_NORMAL;
  hdma_usart1_rx.Init.Priority            = DMA_PRIORITY_MEDIUM; 
   if (HAL_DMA_Init(&hdma_usart1_rx ) != HAL_OK)
  {
    Error_Handler();
  }
  
  huart1.hdmarx = (&hdma_usart1_rx);
  
  //configure usart1 tx dma
    hdma_usart1_tx.Instance                 = DMA1_Channel4;
  hdma_usart1_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_usart1_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_usart1_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart1_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_usart1_tx.Init.Mode                = DMA_NORMAL;
  hdma_usart1_tx.Init.Priority            = DMA_PRIORITY_MEDIUM; 
   if (HAL_DMA_Init(&hdma_usart1_tx ) != HAL_OK)
  {
    Error_Handler();
  }
  
  huart1.hdmatx = (&hdma_usart1_tx);
  
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
  
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 6, 1);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

  HAL_NVIC_SetPriority(USART1_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  
  __HAL_LINKDMA(&huart1, hdmarx, hdma_usart1_rx);
  __HAL_LINKDMA(&huart1, hdmatx, hdma_usart1_tx);

  usart1_rx_line_pos = 0;
  usart1_rx_line_ready = 0;

  usart1_rx_start();
  
}

static void usart1_rx_start(void)
{
  if (HAL_UARTEx_ReceiveToIdle_DMA(&huart1, usart1_rx_dma_buffer, USART1_RX_DMA_SIZE) != HAL_OK)
  {
    Error_Handler();
  }

  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart->Instance != USART1)
  {
    return;
  }

  usart1_rx_push(Size);
  usart1_rx_start();
}

static void usart1_rx_push(uint16_t size)
{
  for (uint16_t index = 0; index < size; index++)
  {
    char rx_char = (char)usart1_rx_dma_buffer[index];

    if (rx_char == '\r')
    {
      continue;
    }

    if (usart1_rx_line_ready != 0)
    {
      continue;
    }

    if (rx_char == '\n')
    {
      usart1_rx_line[usart1_rx_line_pos] = 0;
      usart1_rx_line_ready = 1;
      usart1_rx_line_pos = 0;
      return;
    }

    if (usart1_rx_line_pos < (USART1_RX_DMA_SIZE - 1U))
    {
      usart1_rx_line[usart1_rx_line_pos] = rx_char;
      usart1_rx_line_pos++;
      usart1_rx_line[usart1_rx_line_pos] = 0;
    }
    else
    {
      usart1_rx_line_pos = 0;
      usart1_rx_line[0] = 0;
    }
  }
}

void USART1_RxClear(void)
{
  usart1_rx_line_ready = 0;
  usart1_rx_line_pos = 0;
  usart1_rx_line[0] = 0;
}



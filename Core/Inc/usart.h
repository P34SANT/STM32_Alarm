/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

extern DMA_HandleTypeDef hdma_usart1_rx;

extern DMA_HandleTypeDef hdma_usart1_tx;

extern uint8_t tmpstr ;

#define USART1_RX_DMA_SIZE 128U

extern uint8_t usart1_rx_dma_buffer[USART1_RX_DMA_SIZE];
extern char usart1_rx_line[USART1_RX_DMA_SIZE];
extern volatile uint8_t usart1_rx_line_ready;
void USART1_RxClear(void);

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);


#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */


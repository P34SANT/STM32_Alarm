
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "gpio.h"
#include "sensors.h"
#include "semphr.h"
#include "event_groups.h"
#include "commands.h"
#include "config.h"
#include "terminal.h"
void Error_Handler(void);


#define LED_Pin              GPIO_PIN_13
#define LED_GPIO_Port        GPIOC
#define sensor1_Pin          GPIO_PIN_8
#define sensor1_GPIO_Port    GPIOB
#define sensor2_Pin          GPIO_PIN_4
#define sensor2_GPIO_Port    GPIOB
#define sensor3_Pin          GPIO_PIN_5
#define sensor3_GPIO_Port    GPIOB
#define siren_Pin            GPIO_PIN_6
#define siren_GPIO_Port      GPIOB



extern EventBits_t waitBitsResult;//debugging in alarm


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

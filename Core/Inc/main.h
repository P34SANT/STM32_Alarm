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

void Error_Handler(void);


#define LED_Pin              GPIO_PIN_13
#define LED_GPIO_Port        GPIOC
#define sensor1_Pin          GPIO_PIN_3
#define sensor1_GPIO_Port    GPIOB
#define sensor2_Pin          GPIO_PIN_4
#define sensor2_GPIO_Port    GPIOB
#define sensor3_Pin          GPIO_PIN_5
#define sensor3_GPIO_Port    GPIOB
#define siren_Pin            GPIO_PIN_6
#define siren_GPIO_Port      GPIOB


#define CONFIG_MAGIC            0x43464731UL   // "CFG1"
#define CONFIG_VERSION          1U

#define PHONE_DIGITS            10U
#define PHONE_STR_LEN           (PHONE_DIGITS + 1U)   // + '\0'
#define MAX_OWNER_NUMBERS       5U

typedef struct
{
    uint32_t magic;
    uint16_t version;
    uint16_t length;

    uint8_t  arm_state;
    uint8_t  sensor1_enabled;
    uint8_t  sensor2_enabled;
    uint8_t  sensor3_enabled;
    uint8_t  siren_enabled;

    uint8_t  boot_grace_s;
    uint16_t sensor_grace_ms;
    uint16_t sensor_check_ms;
    uint16_t reserved0;

    char     call_number[PHONE_STR_LEN];
    char     owner_numbers[MAX_OWNER_NUMBERS][PHONE_STR_LEN];

    uint32_t crc32;
} Config_t;




#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SRV1_FB_Pin GPIO_PIN_2
#define SRV1_FB_GPIO_Port GPIOA
#define SRV2_FB_Pin GPIO_PIN_3
#define SRV2_FB_GPIO_Port GPIOA
#define SRV1_DIR_Pin GPIO_PIN_4
#define SRV1_DIR_GPIO_Port GPIOA
#define SRV2_DIR_Pin GPIO_PIN_5
#define SRV2_DIR_GPIO_Port GPIOA
#define SRV1_PWM_Pin GPIO_PIN_6
#define SRV1_PWM_GPIO_Port GPIOA
#define SRV2_PWM_Pin GPIO_PIN_7
#define SRV2_PWM_GPIO_Port GPIOA
#define RS485_DIR_Pin GPIO_PIN_8
#define RS485_DIR_GPIO_Port GPIOA
#define RS485_TX_Pin GPIO_PIN_9
#define RS485_TX_GPIO_Port GPIOA
#define RS485_RX_Pin GPIO_PIN_10
#define RS485_RX_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLOCK_Pin GPIO_PIN_14
#define SWCLOCK_GPIO_Port GPIOA
#define BUTTON_Pin GPIO_PIN_6
#define BUTTON_GPIO_Port GPIOB
#define LED_OK_Pin GPIO_PIN_7
#define LED_OK_GPIO_Port GPIOB
#define LED_ERROR_Pin GPIO_PIN_8
#define LED_ERROR_GPIO_Port GPIOB

#define SERVO1_PWM_TIM_CHANNEL TIM_CHANNEL_1
#define SERVO2_PWM_TIM_CHANNEL TIM_CHANNEL_2
#define SERVO1_FB_TIM_CHANNEL TIM_CHANNEL_3
#define SERVO2_FB_TIM_CHANNEL TIM_CHANNEL_4

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

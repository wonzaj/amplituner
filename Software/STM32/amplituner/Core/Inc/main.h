/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI5_RESET_Pin GPIO_PIN_14
#define SPI5_RESET_GPIO_Port GPIOC
#define SPI5_DC_Pin GPIO_PIN_15
#define SPI5_DC_GPIO_Port GPIOC
#define SPI5_CS_Pin GPIO_PIN_2
#define SPI5_CS_GPIO_Port GPIOC
#define USER_BUTTON_1_Pin GPIO_PIN_5
#define USER_BUTTON_1_GPIO_Port GPIOC
#define USER_BUTTON_1_EXTI_IRQn EXTI9_5_IRQn
#define USER_BUTTON_2_Pin GPIO_PIN_0
#define USER_BUTTON_2_GPIO_Port GPIOB
#define USER_BUTTON_2_EXTI_IRQn EXTI0_IRQn
#define USER_BUTTON_3_Pin GPIO_PIN_1
#define USER_BUTTON_3_GPIO_Port GPIOB
#define USER_BUTTON_3_EXTI_IRQn EXTI1_IRQn
#define USER_BUTTON_4_Pin GPIO_PIN_2
#define USER_BUTTON_4_GPIO_Port GPIOB
#define USER_BUTTON_4_EXTI_IRQn EXTI2_IRQn
#define POWER_BUTTON_Pin GPIO_PIN_10
#define POWER_BUTTON_GPIO_Port GPIOB
#define POWER_BUTTON_EXTI_IRQn EXTI15_10_IRQn
#define ENCODER_BUTTON_VOLUME_FRONT_Pin GPIO_PIN_12
#define ENCODER_BUTTON_VOLUME_FRONT_GPIO_Port GPIOB
#define ENCODER_BUTTON_VOLUME_FRONT_EXTI_IRQn EXTI15_10_IRQn
#define ENCODER_BUTTON_VOLUME_BACK_Pin GPIO_PIN_15
#define ENCODER_BUTTON_VOLUME_BACK_GPIO_Port GPIOB
#define ENCODER_BUTTON_VOLUME_BACK_EXTI_IRQn EXTI15_10_IRQn
#define ENCODER_BUTTON_LOUDNESS_Pin GPIO_PIN_8
#define ENCODER_BUTTON_LOUDNESS_GPIO_Port GPIOC
#define ENCODER_BUTTON_LOUDNESS_EXTI_IRQn EXTI9_5_IRQn
#define LD1_Pin GPIO_PIN_3
#define LD1_GPIO_Port GPIOH
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

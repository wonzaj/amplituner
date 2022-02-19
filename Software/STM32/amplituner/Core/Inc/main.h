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
#define Relay_POWER_Pin GPIO_PIN_13
#define Relay_POWER_GPIO_Port GPIOC
#define Relay_dac_mic_Pin GPIO_PIN_14
#define Relay_dac_mic_GPIO_Port GPIOC
#define SPI5_DC_Pin GPIO_PIN_15
#define SPI5_DC_GPIO_Port GPIOC
#define SPI5_CS_Pin GPIO_PIN_2
#define SPI5_CS_GPIO_Port GPIOC
#define ENC_TREBLE_1_Pin GPIO_PIN_0
#define ENC_TREBLE_1_GPIO_Port GPIOA
#define ENC_TREBLE_2_Pin GPIO_PIN_1
#define ENC_TREBLE_2_GPIO_Port GPIOA
#define ENCODER_RADIO_2_Pin GPIO_PIN_5
#define ENCODER_RADIO_2_GPIO_Port GPIOA
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
#define ENCODER_BUTTON_VOLUME_FRONT_Pin GPIO_PIN_12
#define ENCODER_BUTTON_VOLUME_FRONT_GPIO_Port GPIOB
#define ENCODER_BUTTON_VOLUME_FRONT_EXTI_IRQn EXTI15_10_IRQn
#define ENCODER_BUTTON_MIDDLE_Pin GPIO_PIN_14
#define ENCODER_BUTTON_MIDDLE_GPIO_Port GPIOB
#define ENCODER_BUTTON_MIDDLE_EXTI_IRQn EXTI15_10_IRQn
#define ENCODER_BUTTON_VOLUME_BACK_Pin GPIO_PIN_15
#define ENCODER_BUTTON_VOLUME_BACK_GPIO_Port GPIOB
#define ENCODER_BUTTON_VOLUME_BACK_EXTI_IRQn EXTI15_10_IRQn
#define ENC_VOL_BACK_2_Pin GPIO_PIN_6
#define ENC_VOL_BACK_2_GPIO_Port GPIOC
#define ENC_VOL_BACK_1_Pin GPIO_PIN_7
#define ENC_VOL_BACK_1_GPIO_Port GPIOC
#define ENCODER_BUTTON_LOUDNESS_Pin GPIO_PIN_8
#define ENCODER_BUTTON_LOUDNESS_GPIO_Port GPIOC
#define ENCODER_BUTTON_LOUDNESS_EXTI_IRQn EXTI9_5_IRQn
#define CS43L21_RESET_Pin GPIO_PIN_9
#define CS43L21_RESET_GPIO_Port GPIOC
#define ENC_LOUD_1_Pin GPIO_PIN_8
#define ENC_LOUD_1_GPIO_Port GPIOA
#define ENC_LOUD_2_Pin GPIO_PIN_9
#define ENC_LOUD_2_GPIO_Port GPIOA
#define POWER_BUTTON_Pin GPIO_PIN_10
#define POWER_BUTTON_GPIO_Port GPIOA
#define POWER_BUTTON_EXTI_IRQn EXTI15_10_IRQn
#define ENC_VOL_FRONT_1_Pin GPIO_PIN_15
#define ENC_VOL_FRONT_1_GPIO_Port GPIOA
#define ENCODER_BUTTON_TREBLE_Pin GPIO_PIN_11
#define ENCODER_BUTTON_TREBLE_GPIO_Port GPIOC
#define ENCODER_BUTTON_TREBLE_EXTI_IRQn EXTI15_10_IRQn
#define ENCODER_RADIO_1_Pin GPIO_PIN_2
#define ENCODER_RADIO_1_GPIO_Port GPIOD
#define ENC_VOL_FRONT_2_Pin GPIO_PIN_3
#define ENC_VOL_FRONT_2_GPIO_Port GPIOB
#define ENC_MIDDLE_1_Pin GPIO_PIN_4
#define ENC_MIDDLE_1_GPIO_Port GPIOB
#define ENC_MIDDLE_2_Pin GPIO_PIN_5
#define ENC_MIDDLE_2_GPIO_Port GPIOB
#define ENC_BAS_1_Pin GPIO_PIN_6
#define ENC_BAS_1_GPIO_Port GPIOB
#define ENC_BAS_2_Pin GPIO_PIN_7
#define ENC_BAS_2_GPIO_Port GPIOB
#define ENCODER_BUTTON_BASS_Pin GPIO_PIN_3
#define ENCODER_BUTTON_BASS_GPIO_Port GPIOH
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

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
#include "stm32f7xx_hal.h"

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
#define DISPLAY_SPI_CLK_Pin GPIO_PIN_2
#define DISPLAY_SPI_CLK_GPIO_Port GPIOE
#define DISPLAY_SPI_DC_Pin GPIO_PIN_3
#define DISPLAY_SPI_DC_GPIO_Port GPIOE
#define DISPLAY_SPI_CS_Pin GPIO_PIN_4
#define DISPLAY_SPI_CS_GPIO_Port GPIOE
#define DISPLAY_SPI_RESET_Pin GPIO_PIN_5
#define DISPLAY_SPI_RESET_GPIO_Port GPIOE
#define DISPLAY_SPI_MOSI_Pin GPIO_PIN_6
#define DISPLAY_SPI_MOSI_GPIO_Port GPIOE
#define POWER_BUTTON_PWM_LED_RED_Pin GPIO_PIN_7
#define POWER_BUTTON_PWM_LED_RED_GPIO_Port GPIOF
#define POWER_BUTTON_PWM_LED_GREEN_Pin GPIO_PIN_8
#define POWER_BUTTON_PWM_LED_GREEN_GPIO_Port GPIOF
#define USER_BUTTON_1_Pin GPIO_PIN_0
#define USER_BUTTON_1_GPIO_Port GPIOC
#define USER_BUTTON_1_EXTI_IRQn EXTI0_IRQn
#define USER_BUTTON_2_Pin GPIO_PIN_1
#define USER_BUTTON_2_GPIO_Port GPIOC
#define USER_BUTTON_2_EXTI_IRQn EXTI1_IRQn
#define USER_BUTTON_3_Pin GPIO_PIN_2
#define USER_BUTTON_3_GPIO_Port GPIOC
#define USER_BUTTON_3_EXTI_IRQn EXTI2_IRQn
#define USER_BUTTON_4_Pin GPIO_PIN_3
#define USER_BUTTON_4_GPIO_Port GPIOC
#define USER_BUTTON_4_EXTI_IRQn EXTI3_IRQn
#define ENCODER_VOL_FRONT_1_Pin GPIO_PIN_0
#define ENCODER_VOL_FRONT_1_GPIO_Port GPIOA
#define ENCODER_VOL_FRONT_1A1_Pin GPIO_PIN_1
#define ENCODER_VOL_FRONT_1A1_GPIO_Port GPIOA
#define ADC_IN_AUDIO_FRONT_LEFT_Pin GPIO_PIN_2
#define ADC_IN_AUDIO_FRONT_LEFT_GPIO_Port GPIOA
#define ADC_IN_AUDIO_FRONT_RIGHT_Pin GPIO_PIN_3
#define ADC_IN_AUDIO_FRONT_RIGHT_GPIO_Port GPIOA
#define DAC_OUT_LEFT_Pin GPIO_PIN_4
#define DAC_OUT_LEFT_GPIO_Port GPIOA
#define DAC_OUT_RIGHT_Pin GPIO_PIN_5
#define DAC_OUT_RIGHT_GPIO_Port GPIOA
#define ENCODER_VOL_BACK_1_Pin GPIO_PIN_6
#define ENCODER_VOL_BACK_1_GPIO_Port GPIOA
#define ENCODER_VOL_BACK_2_Pin GPIO_PIN_7
#define ENCODER_VOL_BACK_2_GPIO_Port GPIOA
#define ADC_IN_SUB_LEFT_Pin GPIO_PIN_4
#define ADC_IN_SUB_LEFT_GPIO_Port GPIOC
#define ADC_IN_AUDIO_SUB_RIGHT_Pin GPIO_PIN_5
#define ADC_IN_AUDIO_SUB_RIGHT_GPIO_Port GPIOC
#define ADC_IN_AUDIO_BACK_LEFT_Pin GPIO_PIN_0
#define ADC_IN_AUDIO_BACK_LEFT_GPIO_Port GPIOB
#define ADC_IN_AUDIO_BACK_RIGHT_Pin GPIO_PIN_1
#define ADC_IN_AUDIO_BACK_RIGHT_GPIO_Port GPIOB
#define POWER_BUTTON_Pin GPIO_PIN_11
#define POWER_BUTTON_GPIO_Port GPIOF
#define POWER_BUTTON_EXTI_IRQn EXTI15_10_IRQn
#define ENCODER_BUTTON_VOL_FRONT_Pin GPIO_PIN_13
#define ENCODER_BUTTON_VOL_FRONT_GPIO_Port GPIOF
#define ENCODER_BUTTON_VOL_FRONT_EXTI_IRQn EXTI15_10_IRQn
#define ENCODER_BUTTON_VOL_BACK_Pin GPIO_PIN_14
#define ENCODER_BUTTON_VOL_BACK_GPIO_Port GPIOF
#define ENCODER_BUTTON_VOL_BACK_EXTI_IRQn EXTI15_10_IRQn
#define BT_PREV_BUTTON_Pin GPIO_PIN_15
#define BT_PREV_BUTTON_GPIO_Port GPIOF
#define BT_NEXT_BUTTON_Pin GPIO_PIN_0
#define BT_NEXT_BUTTON_GPIO_Port GPIOG
#define BT_PAUSE_PLAY_BUTTON_Pin GPIO_PIN_1
#define BT_PAUSE_PLAY_BUTTON_GPIO_Port GPIOG
#define ENCODER_BUTTON_LOUD_Pin GPIO_PIN_7
#define ENCODER_BUTTON_LOUD_GPIO_Port GPIOE
#define ENCODER_BUTTON_LOUD_EXTI_IRQn EXTI9_5_IRQn
#define ENCODER_BUTTON_BASS_Pin GPIO_PIN_8
#define ENCODER_BUTTON_BASS_GPIO_Port GPIOE
#define ENCODER_BUTTON_BASS_EXTI_IRQn EXTI9_5_IRQn
#define ENCODER_TREBLE_1_Pin GPIO_PIN_9
#define ENCODER_TREBLE_1_GPIO_Port GPIOE
#define ENCODER_BUTTON_MIDDLE_Pin GPIO_PIN_10
#define ENCODER_BUTTON_MIDDLE_GPIO_Port GPIOE
#define ENCODER_BUTTON_MIDDLE_EXTI_IRQn EXTI15_10_IRQn
#define ENCODER_TREBLE_2_Pin GPIO_PIN_11
#define ENCODER_TREBLE_2_GPIO_Port GPIOE
#define ENCODER_BUTTON_TREBLE_Pin GPIO_PIN_12
#define ENCODER_BUTTON_TREBLE_GPIO_Port GPIOE
#define ENCODER_BUTTON_TREBLE_EXTI_IRQn EXTI15_10_IRQn
#define ENCODER_BUTTON_RADIO_Pin GPIO_PIN_15
#define ENCODER_BUTTON_RADIO_GPIO_Port GPIOB
#define ENCODER_BUTTON_RADIO_EXTI_IRQn EXTI15_10_IRQn
#define ENCODER_MIDDLE_1_Pin GPIO_PIN_9
#define ENCODER_MIDDLE_1_GPIO_Port GPIOD
#define ENCODER_MIDDLE_1_EXTI_IRQn EXTI9_5_IRQn
#define ENCODER_RADIO_1_Pin GPIO_PIN_12
#define ENCODER_RADIO_1_GPIO_Port GPIOD
#define ENCODER_RADIO_2_Pin GPIO_PIN_13
#define ENCODER_RADIO_2_GPIO_Port GPIOD
#define ENCODER_MIDDLE_2_Pin GPIO_PIN_6
#define ENCODER_MIDDLE_2_GPIO_Port GPIOG
#define ENCODER_MIDDLE_2_EXTI_IRQn EXTI9_5_IRQn
#define ENCODER_LOUD_1_Pin GPIO_PIN_6
#define ENCODER_LOUD_1_GPIO_Port GPIOC
#define ENCODER_LOUD_2_Pin GPIO_PIN_7
#define ENCODER_LOUD_2_GPIO_Port GPIOC
#define ENCODER_BASS_1_Pin GPIO_PIN_15
#define ENCODER_BASS_1_GPIO_Port GPIOA
#define UART_ESP_TX_Pin GPIO_PIN_10
#define UART_ESP_TX_GPIO_Port GPIOC
#define UART_ESP_RX_Pin GPIO_PIN_11
#define UART_ESP_RX_GPIO_Port GPIOC
#define RELAY_AUDIO_POWER_Pin GPIO_PIN_0
#define RELAY_AUDIO_POWER_GPIO_Port GPIOD
#define RELAY_DAC_MIC_Pin GPIO_PIN_1
#define RELAY_DAC_MIC_GPIO_Port GPIOD
#define ENCODER_BASS_2_Pin GPIO_PIN_3
#define ENCODER_BASS_2_GPIO_Port GPIOB
#define POWER_BUTTON_PWM_LED_YELLOW_Pin GPIO_PIN_8
#define POWER_BUTTON_PWM_LED_YELLOW_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

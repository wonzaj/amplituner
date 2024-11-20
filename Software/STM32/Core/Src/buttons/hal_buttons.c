/**
 ********************************************************************************
 * @file    hal_buttons.c
 * @author  macie
 * @date    Nov 15, 2024
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "hal_buttons.h"
#include "main.h"
#include "app_buttons.h"
#include "hal_encoders.h"

#include "cmsis_os2.h"
/************************************
 * EXTERN VARIABLESs
 ************************************/
extern osMessageQueueId_t buttonHandlerQueueHandle;
extern osThreadId_t buttonHandlerTaHandle;
/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	Queue_ButtonEvent_t Queue_ButtonEvent = {0};

	/* Check for available space */
	if(osMessageQueueGetSpace(buttonHandlerQueueHandle) == 0U)
	{
		return;
	}

	switch (GPIO_Pin)
	{
		case POWER_BUTTON_Pin:
			Queue_ButtonEvent.GPIO_Pin = POWER_BUTTON_Pin;

			if (HAL_GPIO_ReadPin(POWER_BUTTON_GPIO_Port, POWER_BUTTON_Pin) == GPIO_PIN_RESET)
			{
				Queue_ButtonEvent.State = Button_Pressed;
			}
			else if (HAL_GPIO_ReadPin(POWER_BUTTON_GPIO_Port, POWER_BUTTON_Pin) == GPIO_PIN_SET)
			{
				Queue_ButtonEvent.State = Button_Released;
			}
			break;

		case USER_BUTTON_1_Pin:
			Queue_ButtonEvent.GPIO_Pin = USER_BUTTON_1_Pin;

			if (HAL_GPIO_ReadPin(USER_BUTTON_1_GPIO_Port, USER_BUTTON_1_Pin) == GPIO_PIN_RESET)
			{
				Queue_ButtonEvent.State = Button_Pressed;
			}
			else if (HAL_GPIO_ReadPin(USER_BUTTON_1_GPIO_Port, USER_BUTTON_1_Pin) == GPIO_PIN_SET)
			{
				Queue_ButtonEvent.State = Button_Released;
			}
			break;

		case USER_BUTTON_2_Pin:
			Queue_ButtonEvent.GPIO_Pin = USER_BUTTON_2_Pin;

			if (HAL_GPIO_ReadPin(USER_BUTTON_2_GPIO_Port, USER_BUTTON_2_Pin) == GPIO_PIN_RESET)
			{
				Queue_ButtonEvent.State = Button_Pressed;
			}
			else if (HAL_GPIO_ReadPin(USER_BUTTON_2_GPIO_Port, USER_BUTTON_2_Pin) == GPIO_PIN_SET)
			{
				Queue_ButtonEvent.State = Button_Released;
			}
			break;

		case USER_BUTTON_3_Pin:
			Queue_ButtonEvent.GPIO_Pin = USER_BUTTON_3_Pin;

			if (HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin) == GPIO_PIN_RESET)
			{
				Queue_ButtonEvent.State = Button_Pressed;
			}
			else if (HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin) == GPIO_PIN_SET)
			{
				Queue_ButtonEvent.State = Button_Released;
			}
			break;

		case USER_BUTTON_4_Pin:
			Queue_ButtonEvent.GPIO_Pin = USER_BUTTON_4_Pin;

			if (HAL_GPIO_ReadPin(USER_BUTTON_4_GPIO_Port, USER_BUTTON_4_Pin) == GPIO_PIN_RESET)
			{
				Queue_ButtonEvent.State = Button_Pressed;
			}
			else if (HAL_GPIO_ReadPin(USER_BUTTON_4_GPIO_Port, USER_BUTTON_4_Pin) == GPIO_PIN_SET)
			{
				Queue_ButtonEvent.State = Button_Released;
			}
			break;

		case ENCODER_BUTTON_VOL_FRONT_Pin:
			Queue_ButtonEvent.GPIO_Pin = ENCODER_BUTTON_VOL_FRONT_Pin;

			if (HAL_GPIO_ReadPin(ENCODER_BUTTON_VOL_FRONT_GPIO_Port, ENCODER_BUTTON_VOL_FRONT_Pin) == GPIO_PIN_RESET)
			{
				Queue_ButtonEvent.State = Button_Pressed;
			}
			else if (HAL_GPIO_ReadPin(ENCODER_BUTTON_VOL_FRONT_GPIO_Port, ENCODER_BUTTON_VOL_FRONT_Pin) == GPIO_PIN_SET)
			{
				Queue_ButtonEvent.State = Button_Released;
			}
			break;

		case ENCODER_BUTTON_VOL_BACK_Pin:
			Queue_ButtonEvent.GPIO_Pin = ENCODER_BUTTON_VOL_BACK_Pin;

			if (HAL_GPIO_ReadPin(ENCODER_BUTTON_VOL_BACK_GPIO_Port, ENCODER_BUTTON_VOL_BACK_Pin) == GPIO_PIN_RESET)
			{
				Queue_ButtonEvent.State = Button_Pressed;
			}
			else if (HAL_GPIO_ReadPin(ENCODER_BUTTON_VOL_BACK_GPIO_Port, ENCODER_BUTTON_VOL_BACK_Pin) == GPIO_PIN_SET)
			{
				Queue_ButtonEvent.State = Button_Released;
			}
			break;

		case ENCODER_BUTTON_TREBLE_Pin:
			Queue_ButtonEvent.GPIO_Pin = ENCODER_BUTTON_TREBLE_Pin;

			if (HAL_GPIO_ReadPin(ENCODER_BUTTON_TREBLE_GPIO_Port, ENCODER_BUTTON_TREBLE_Pin) == GPIO_PIN_RESET)
			{
				Queue_ButtonEvent.State = Button_Pressed;
			}
			else if (HAL_GPIO_ReadPin(ENCODER_BUTTON_TREBLE_GPIO_Port, ENCODER_BUTTON_TREBLE_Pin) == GPIO_PIN_SET)
			{
				Queue_ButtonEvent.State = Button_Released;
			}
			break;

		case ENCODER_BUTTON_MIDDLE_Pin:
			Queue_ButtonEvent.GPIO_Pin = ENCODER_BUTTON_MIDDLE_Pin;

			if (HAL_GPIO_ReadPin(ENCODER_BUTTON_MIDDLE_GPIO_Port, ENCODER_BUTTON_MIDDLE_Pin) == GPIO_PIN_RESET)
			{
				Queue_ButtonEvent.State = Button_Pressed;
			}
			else if (HAL_GPIO_ReadPin(ENCODER_BUTTON_MIDDLE_GPIO_Port, ENCODER_BUTTON_MIDDLE_Pin) == GPIO_PIN_SET)
			{
				Queue_ButtonEvent.State = Button_Released;
			}
			break;

		case ENCODER_BUTTON_BASS_Pin:
			Queue_ButtonEvent.GPIO_Pin = ENCODER_BUTTON_BASS_Pin;

			if (HAL_GPIO_ReadPin(ENCODER_BUTTON_BASS_GPIO_Port, ENCODER_BUTTON_BASS_Pin) == GPIO_PIN_RESET)
			{
				Queue_ButtonEvent.State = Button_Pressed;
			}
			else if (HAL_GPIO_ReadPin(ENCODER_BUTTON_BASS_GPIO_Port, ENCODER_BUTTON_BASS_Pin) == GPIO_PIN_SET)
			{
				Queue_ButtonEvent.State = Button_Released;
			}
			break;

		case ENCODER_BUTTON_RADIO_Pin:
			Queue_ButtonEvent.GPIO_Pin = ENCODER_BUTTON_RADIO_Pin;

			if (HAL_GPIO_ReadPin(ENCODER_BUTTON_RADIO_GPIO_Port, ENCODER_BUTTON_RADIO_Pin) == GPIO_PIN_RESET)
			{
				Queue_ButtonEvent.State = Button_Pressed;
			}
			else if (HAL_GPIO_ReadPin(ENCODER_BUTTON_RADIO_GPIO_Port, ENCODER_BUTTON_RADIO_Pin) == GPIO_PIN_SET)
			{
				Queue_ButtonEvent.State = Button_Released;
			}
			break;

		case ENCODER_BUTTON_LOUD_Pin:
			Queue_ButtonEvent.GPIO_Pin = ENCODER_BUTTON_LOUD_Pin;

			if (HAL_GPIO_ReadPin(ENCODER_BUTTON_LOUD_GPIO_Port, ENCODER_BUTTON_LOUD_Pin) == GPIO_PIN_RESET)
			{
				Queue_ButtonEvent.State = Button_Pressed;
			}
			else if (HAL_GPIO_ReadPin(ENCODER_BUTTON_LOUD_GPIO_Port, ENCODER_BUTTON_LOUD_Pin) == GPIO_PIN_SET)
			{
				Queue_ButtonEvent.State = Button_Released;
			}
			break;

		case ENCODER_MIDDLE_1_Pin:
			Queue_ButtonEvent.GPIO_Pin = ENCODER_MIDDLE_1_Pin;

			if (HAL_GPIO_ReadPin(ENCODER_MIDDLE_1_GPIO_Port, ENCODER_MIDDLE_1_Pin) == GPIO_PIN_RESET)
			{
				Queue_ButtonEvent.State = Button_Pressed;
			}
			else if (HAL_GPIO_ReadPin(ENCODER_MIDDLE_1_GPIO_Port, ENCODER_MIDDLE_1_Pin) == GPIO_PIN_SET)
			{
				Queue_ButtonEvent.State = Button_Released;
			}
			break;

		case ENCODER_MIDDLE_2_Pin:
			Queue_ButtonEvent.GPIO_Pin = ENCODER_MIDDLE_2_Pin;

			if (HAL_GPIO_ReadPin(ENCODER_MIDDLE_2_GPIO_Port, ENCODER_MIDDLE_2_Pin) == GPIO_PIN_RESET)
			{
				Queue_ButtonEvent.State = Button_Pressed;
			}
			else if (HAL_GPIO_ReadPin(ENCODER_MIDDLE_2_GPIO_Port, ENCODER_MIDDLE_2_Pin) == GPIO_PIN_SET)
			{
				Queue_ButtonEvent.State = Button_Released;
			}
			break;
		default:
			break;
	}

	osMessageQueuePut(buttonHandlerQueueHandle, &Queue_ButtonEvent, 0U, 0U);
}

void HAL_Buttons_IRQ_TurnOff(void)
{
	__NVIC_DisableIRQ(EXTI0_IRQn);
	__NVIC_DisableIRQ(EXTI1_IRQn);
	__NVIC_DisableIRQ(EXTI2_IRQn);
	__NVIC_DisableIRQ(EXTI3_IRQn);
	__NVIC_DisableIRQ(EXTI9_5_IRQn);
	__NVIC_DisableIRQ(EXTI15_10_IRQn);
}

void HAL_Buttons_IRQ_TurnOn(void)
{
	__NVIC_EnableIRQ(EXTI0_IRQn);
	__NVIC_EnableIRQ(EXTI1_IRQn);
	__NVIC_EnableIRQ(EXTI2_IRQn);
	__NVIC_EnableIRQ(EXTI3_IRQn);
	__NVIC_EnableIRQ(EXTI9_5_IRQn);
	__NVIC_EnableIRQ(EXTI15_10_IRQn);
}

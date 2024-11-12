#include "hal_buttons.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == POWER_BUTTON_Pin)
	{
		if (HAL_GPIO_ReadPin(POWER_BUTTON_GPIO_Port, POWER_BUTTON_Pin) == GPIO_PIN_RESET)
		{
			Buttons_PowerButton_Pressed();
		}
		else if (HAL_GPIO_ReadPin(POWER_BUTTON_GPIO_Port, POWER_BUTTON_Pin) == GPIO_PIN_SET)
		{
			Buttons_PowerButton_Released();
		}
	}
	else if (GPIO_Pin == USER_BUTTON_1_Pin)
	{
		if (HAL_GPIO_ReadPin(USER_BUTTON_1_GPIO_Port, USER_BUTTON_1_Pin) == GPIO_PIN_RESET)
		{
			Buttons_UserButton1_Pressed();
		}
		else if (HAL_GPIO_ReadPin(USER_BUTTON_1_GPIO_Port, USER_BUTTON_1_Pin) == GPIO_PIN_SET)
		{
			Buttons_UserButton1_Released();
		}
	}
	else if (GPIO_Pin == USER_BUTTON_2_Pin)
	{
		if (HAL_GPIO_ReadPin(USER_BUTTON_2_GPIO_Port, USER_BUTTON_2_Pin) == GPIO_PIN_RESET)
		{
			Buttons_UserButton2_Pressed();
		}
		else if (HAL_GPIO_ReadPin(USER_BUTTON_2_GPIO_Port, USER_BUTTON_2_Pin) == GPIO_PIN_SET)
		{
			Buttons_UserButton2_Released();
		}
	}
	else if (GPIO_Pin == USER_BUTTON_3_Pin)
	{
		if (HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin) == GPIO_PIN_RESET)
		{
			Buttons_UserButton3_Pressed();
		}
		else if (HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin) == GPIO_PIN_SET)
		{
			Buttons_UserButton3_Released();
		}
	}
	else if (GPIO_Pin == USER_BUTTON_4_Pin)
	{
		if (HAL_GPIO_ReadPin(USER_BUTTON_4_GPIO_Port, USER_BUTTON_4_Pin) == GPIO_PIN_RESET)
		{
			Buttons_UserButton4_Pressed();
		}
		else if (HAL_GPIO_ReadPin(USER_BUTTON_4_GPIO_Port, USER_BUTTON_4_Pin) == GPIO_PIN_SET)
		{
			Buttons_UserButton4_Released();
		}
	}
	else if (GPIO_Pin == ENCODER_BUTTON_VOL_FRONT_Pin)
	{
		if (HAL_GPIO_ReadPin(ENCODER_BUTTON_VOL_FRONT_GPIO_Port, ENCODER_BUTTON_VOL_FRONT_Pin) == GPIO_PIN_RESET)
		{
			Buttons_EncoderVolFrontButton_Pressed();
		}
		else if (HAL_GPIO_ReadPin(ENCODER_BUTTON_VOL_FRONT_GPIO_Port, ENCODER_BUTTON_VOL_FRONT_Pin) == GPIO_PIN_SET)
		{
			Buttons_EncoderVolFrontButton_Released();
		}
	}
	else if (GPIO_Pin == ENCODER_BUTTON_VOL_BACK_Pin)
	{
		if (HAL_GPIO_ReadPin(ENCODER_BUTTON_VOL_BACK_GPIO_Port,ENCODER_BUTTON_VOL_BACK_Pin) == GPIO_PIN_RESET)
		{
			Buttons_EncoderVolBackButton_Pressed();
		}
		else if (HAL_GPIO_ReadPin(ENCODER_BUTTON_VOL_BACK_GPIO_Port, ENCODER_BUTTON_VOL_BACK_Pin) == GPIO_PIN_SET)
		{
			Buttons_EncoderVolBackButton_Released();
		}
	}
	else if (GPIO_Pin == ENCODER_BUTTON_TREBLE_Pin)
	{
		if (HAL_GPIO_ReadPin(ENCODER_BUTTON_TREBLE_GPIO_Port, ENCODER_BUTTON_TREBLE_Pin) == GPIO_PIN_RESET)
		{
			Buttons_EncoderTrebleButton_Pressed();
		}
		else if (HAL_GPIO_ReadPin(ENCODER_BUTTON_TREBLE_GPIO_Port, ENCODER_BUTTON_TREBLE_Pin) == GPIO_PIN_SET)
		{
			Buttons_EncoderTrebleButton_Released();
		}
	}
	else if (GPIO_Pin == ENCODER_BUTTON_MIDDLE_Pin)
	{
		if (HAL_GPIO_ReadPin(ENCODER_BUTTON_MIDDLE_GPIO_Port, ENCODER_BUTTON_MIDDLE_Pin) == GPIO_PIN_RESET)
		{
			Buttons_EncoderMiddleButton_Pressed();
		}
		else if (HAL_GPIO_ReadPin(ENCODER_BUTTON_MIDDLE_GPIO_Port, ENCODER_BUTTON_MIDDLE_Pin) == GPIO_PIN_SET)
		{
			Buttons_EncoderMiddleButton_Released();
		}
	}
	else if (GPIO_Pin == ENCODER_BUTTON_BASS_Pin)
	{
		if (HAL_GPIO_ReadPin(ENCODER_BUTTON_BASS_GPIO_Port, ENCODER_BUTTON_BASS_Pin) == GPIO_PIN_RESET)
		{
			Buttons_EncoderBassButton_Pressed();
		}
		else if (HAL_GPIO_ReadPin(ENCODER_BUTTON_BASS_GPIO_Port, ENCODER_BUTTON_BASS_Pin) == GPIO_PIN_SET)
		{
			Buttons_EncoderBassButton_Released();
		}
	}
	else if (GPIO_Pin == ENCODER_BUTTON_RADIO_Pin)
	{
		if (HAL_GPIO_ReadPin(ENCODER_BUTTON_RADIO_GPIO_Port, ENCODER_BUTTON_RADIO_Pin) == GPIO_PIN_RESET)
		{
			Buttons_EncoderRadioButton_Pressed();
		}
		else if (HAL_GPIO_ReadPin(ENCODER_BUTTON_RADIO_GPIO_Port, ENCODER_BUTTON_RADIO_Pin) == GPIO_PIN_SET)
		{
			Buttons_EncoderRadioButton_Released();
		}
	}
	else if (GPIO_Pin == ENCODER_BUTTON_LOUD_Pin)
	{
		if (HAL_GPIO_ReadPin(ENCODER_BUTTON_LOUD_GPIO_Port, ENCODER_BUTTON_LOUD_Pin) == GPIO_PIN_RESET)
		{
			Buttons_EncoderLoudnessButton_Pressed();
		}
		else if (HAL_GPIO_ReadPin(ENCODER_BUTTON_LOUD_GPIO_Port, ENCODER_BUTTON_LOUD_Pin) == GPIO_PIN_SET)
		{
			Buttons_EncoderLoudnessButton_Released();
		}
	}
	// Used for middle encoder
	else if (GPIO_Pin == ENCODER_MIDDLE_1_Pin)
	{
		if (HAL_GPIO_ReadPin(ENCODER_MIDDLE_1_GPIO_Port, ENCODER_MIDDLE_1_Pin) == GPIO_PIN_RESET)
		{
			HAL_Encoders_Middle_1_FallingEdge();
		}
		else if (HAL_GPIO_ReadPin(ENCODER_MIDDLE_1_GPIO_Port, ENCODER_MIDDLE_1_Pin) == GPIO_PIN_SET)
		{
			HAL_Encoders_Middle_1_RisingEdge();
		}
	}
	// Used for middle encoder
	else if (GPIO_Pin == ENCODER_MIDDLE_2_Pin)
	{
		if (HAL_GPIO_ReadPin(ENCODER_MIDDLE_2_GPIO_Port, ENCODER_MIDDLE_2_Pin) == GPIO_PIN_RESET)
		{
			HAL_Encoders_Middle_2_FallingEdge();
		}
		else if (HAL_GPIO_ReadPin(ENCODER_MIDDLE_2_GPIO_Port, ENCODER_MIDDLE_2_Pin) == GPIO_PIN_SET)
		{
			HAL_Encoders_Middle_2_RisingEdge();
		}
	}
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

void HAL_Buttons_PowerButton_SetPWM(uint8_t pwm_value)
{
	//__HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, pwm_value);
}

void HAL_Buttons_PowerButton_Init(void)
{
	//HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);
	//__HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, 0);
}

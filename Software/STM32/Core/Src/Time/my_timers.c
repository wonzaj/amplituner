#include "my_timers.h"

extern uint32_t SysTick_MiliSeconds;
extern uint32_t SysTick_Seconds;
extern encoder_t encoderVolFront;
extern encoder_t encoderVolBack;

/* Timer output compare callback
 * Is used for:
 * - TIM14 - if set, changing screen state back to saved screen - configured for 2 seconds
 * - TIM16 - PWM timer for power LED
 * - TIM17 - Used for debouncing all buttons
 */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM14)
	{
		AppDisplay_SetDisplayState(AppDisplay_GetSavedDisplayState());
		HAL_TIM_OC_Stop_IT(htim, TIM_CHANNEL_1);
	}
	else if (htim->Instance == TIM13)
	{
		set_saved_display(htim);
	}
}

void HAL_Timers_Init(void)
{
	//HAL_Timers_OC_Start(&htim14, TIM_CHANNEL_1);
}

void HAL_Timers_OC_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
	HAL_TIM_OC_Stop_IT(htim, Channel);
}

void HAL_Timers_OC_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
	HAL_TIM_OC_Start_IT(htim, Channel);
}

uint32_t HAL_Timers_SysTick_GetMiliseconds(void)
{
	return SysTick_MiliSeconds;
}

uint32_t HAL_Timers_SysTick_GetSeconds(void)
{
	return SysTick_Seconds;
}

void HAL_Timers_RefreshTimer(TIM_HandleTypeDef *htim, uint32_t Channel)
{
	if (TIM_CHANNEL_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_READY)
	{
		HAL_TIM_OC_Start_IT(htim, Channel);

	}
	if (TIM_CHANNEL_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_BUSY)
	{
		htim->Instance->CNT = 1U;

	}
}

void set_saved_display(TIM_HandleTypeDef *htim)
{
	//encoderVolFront.audioOutputState = MASTER;
	//encoderVolBack.audioOutputState = NORMAL;
	////AppDisplay_SetDisplayState(AppDisplay_GetSavedDisplayState());
	////AppDisplay_SetSavedDisplayState(0);
	//HAL_TIM_OC_Stop_IT(&htim13, TIM_CHANNEL_1);
	htim->Instance->CNT = 1;
}

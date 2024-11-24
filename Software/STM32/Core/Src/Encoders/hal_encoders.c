//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "hal_encoders.h"
//--------------------------------------------------------------
// Defines
//--------------------------------------------------------------

//--------------------------------------------------------------
// Static variables
//--------------------------------------------------------------
static int32_t Encoder_Middle_Rotate_Value = 0;

//--------------------------------------------------------------
// Function definitions
//--------------------------------------------------------------

/* Time input capture callback
 * Is called whenever any encoder change it's position (expect for radio encoder)
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	HAL_Encoders_CheckVolumeRanges();

	/* Volume front encoder */
	if (htim->Instance == ENCODER_VOL_FRONT_TIMER)
	{
		AppEncoders_EncoderVolFront_Rotated();
	}
	/* Volume back encoder */
	if (htim->Instance == ENCODER_VOL_BACK_TIMER)
	{
		AppEncoders_EncoderVolBack_Rotated();
	}
	/* Treble encoder */
	if (htim->Instance == ENCODER_VOL_TREBLE_TIMER)
	{
		AppEncoders_EncoderTreble_Rotated();
	}
	/* Bass encoder */
	if (htim->Instance == ENCODER_VOL_BASS_TIMER)
	{
		AppEncoders_EncoderBass_Rotated();
	}
	/* Radio encoder */
	if (htim->Instance == ENCODER_VOL_RADIO_TIMER)
	{
		AppEncoders_EncoderRadio_Rotated();
	}
	/* Loudness  encoder */
	if (htim->Instance == ENCODER_VOL_LOUD_TIMER)
	{
		AppEncoders_EncoderLoudness_Rotated();
	}
}

// Is called only when system starts
void HAL_Encoders_Init(void)
{
	HAL_TIM_Encoder_Start_IT(&htim1, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start_IT(&htim4, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start_IT(&htim5, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start_IT(&htim8, TIM_CHANNEL_ALL);
	// Turn on IRQ from encoder middle
}

void HAL_Encoders_Deinit(void)
{
	HAL_TIM_Encoder_Stop_IT(&htim1, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Stop_IT(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Stop_IT(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Stop_IT(&htim4, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Stop_IT(&htim5, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Stop_IT(&htim8, TIM_CHANNEL_ALL);
	// Turn off IRQ from encoder middle
}

void HAL_Encoder_SingleEncoderStop(TIM_HandleTypeDef *htim)
{
	HAL_TIM_Encoder_Stop_IT(htim, TIM_CHANNEL_ALL);
}

void HAL_Encoder_SingleEncoderStart(TIM_HandleTypeDef *htim)
{
	HAL_TIM_Encoder_Start_IT(htim, TIM_CHANNEL_ALL);
}

void HAL_Encoders_Middle_1_RisingEdge(void)
{

}
void HAL_Encoders_Middle_1_FallingEdge(void)
{
	if (HAL_GPIO_ReadPin(ENCODER_MIDDLE_2_GPIO_Port, ENCODER_MIDDLE_2_Pin) == GPIO_PIN_SET)
	{
		// Encoder middle rotated clockwise
		Encoder_Middle_Rotate_Value++;
		AppEncoders_EncoderMiddle_Rotated();
	}
	else if (HAL_GPIO_ReadPin(ENCODER_MIDDLE_2_GPIO_Port, ENCODER_MIDDLE_2_Pin) == GPIO_PIN_RESET)
	{
		// Encoder middle rotated counter clockwise
		Encoder_Middle_Rotate_Value--;
		AppEncoders_EncoderMiddle_Rotated();
	}
}

void HAL_Encoders_Middle_2_RisingEdge(void)
{

}
void HAL_Encoders_Middle_2_FallingEdge(void)
{

}

int32_t HAL_Encoders_Middle_GetRotateValue(void)
{
	return Encoder_Middle_Rotate_Value;
}

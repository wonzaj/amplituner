//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "app_encoders.h"
#include "hal_encoders.h"
#include "TDA7719.h"
#include "cmsis_os2.h"
//--------------------------------------------------------------
// Defines
//--------------------------------------------------------------
#define OFFSET_VOLUME 						79
#define ENCODER_MAX_VOLUME_FRONT_MASTER		94
#define ENCODER_MAX_VOLUME_FRONT_LEFTRIGHT	79
#define ENCODER_MAX_VOLUME_BACK_LEFTRIGHT	79
//--------------------------------------------------------------
// Variables
//--------------------------------------------------------------
extern osMessageQueueId_t refreshTimerQueueHandle;
static uint32_t RefreshTimer = 0;

savedUserSettings_t savedUserSettings =
{

};

Device_Cfg_Audio_t Device_Cfg_Audio =
{
		.VolFront.volumeMaster 		= OFFSET_VOLUME,
		.VolFront.volumeLeftRight 	= OFFSET_VOLUME,
		.VolFront.volumeLeft 		= OFFSET_VOLUME,
		.VolFront.volumeRight 		= OFFSET_VOLUME,
		.VolFront.audioOutputState 	= MASTER,

		.VolBack.audioOutputState 	= NORMAL,
		.VolBack.volumeLeftRight 	= OFFSET_VOLUME,
		.VolBack.volumeLeft 		= OFFSET_VOLUME,
		.VolBack.volumeRight 		= OFFSET_VOLUME,

		.Loudness.buttonControl = SET_GAIN,
		.Loudness.gain = 0,
		.Loudness.centerFreq 	= 0,
		.Loudness.soft_step 	= 0,
		.Loudness.high_boost	= 0,

		.Treble.gain = 0,
		.Treble.buttonControl = SET_GAIN,

		.Middle.gain = 0,
		.Middle.buttonControl = SET_GAIN,

};
//--------------------------------------------------------------
// external variables
//--------------------------------------------------------------
static void Check_Volume_Range_Front(volatile int8_t *const volume, const uint8_t maxVolume);
static void Check_Volume_Range_Back(volatile int8_t *const volume, const uint8_t maxVolume);
static void Check_Loudness_Param_Range(volatile int8_t *const gain, const uint8_t maxGain);
static void Check_Bass_Param_Range(volatile int8_t *const gain, const uint8_t maxGain);
static void Check_Middle_Param_Range(volatile int8_t *const gain, const uint8_t maxGain);
static void Check_Treble_Param_Range(volatile int8_t *const gain, const uint8_t maxGain);
//--------------------------------------------------------------
// Function definition
//--------------------------------------------------------------
void SetSavedDisplay_StartTimer(void)
{
	/* Check for available space */
	if(osMessageQueueGetSpace(refreshTimerQueueHandle) == 0U)
	{
		return;
	}

	osMessageQueuePut(refreshTimerQueueHandle, &RefreshTimer, 1U, 0U);
}


void AppEncoders_EncoderVolFront_Rotated(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_VOLUME_FRONT);
	SetSavedDisplay_StartTimer();

	switch (Device_Cfg_Audio.VolFront.audioOutputState)
	{
	case MASTER:
		Check_Volume_Range_Front(&Device_Cfg_Audio.VolFront.volumeMaster, ENCODER_MAX_VOLUME_FRONT_MASTER);
		if (Device_Cfg_Audio.VolFront.volumeMaster >= ENCODER_MAX_VOLUME_FRONT_LEFTRIGHT + 1)
			Device_Cfg_Audio.volumeMasterFlag = 1;
		else
			Device_Cfg_Audio.volumeMasterFlag = 0;
		TDA7719_SetVolume_Master(Device_Cfg_Audio.tempVolFrontLeft, Device_Cfg_Audio.tempVolFrontRight, Device_Cfg_Audio.tempVolBackLeft, Device_Cfg_Audio.tempVolBackRight);
		break;
	case MUTE:
		//jak wyłącze przerwanie on enkodera to teraz nie bedzie tutaj kod wchodził
		//więc może jakieś zerowanie rejestru CNT.
		break;
	case MASTER_V2:
		Check_Volume_Range_Front(&Device_Cfg_Audio.VolFront.volumeMaster, ENCODER_MAX_VOLUME_FRONT_MASTER);
		if (Device_Cfg_Audio.VolFront.volumeMaster >= 80)
			Device_Cfg_Audio.volumeMasterFlag = 1;
		else
			Device_Cfg_Audio.volumeMasterFlag = 0;
		TDA7719_SetVolume_Master(Device_Cfg_Audio.tempVolFrontLeft, Device_Cfg_Audio.tempVolFrontRight, Device_Cfg_Audio.tempVolBackLeft, Device_Cfg_Audio.tempVolBackRight);
		break;
	case NORMAL:
		Check_Volume_Range_Front(&Device_Cfg_Audio.VolFront.volumeLeftRight, ENCODER_MAX_VOLUME_FRONT_LEFTRIGHT);
		TDA7719_SetVolumeFront_LeftRight(Device_Cfg_Audio.tempVolFrontLeft, Device_Cfg_Audio.tempVolFrontRight);
		break;
	case ATTE_LEFT:
		Check_Volume_Range_Front(&Device_Cfg_Audio.VolFront.volumeLeft, ENCODER_MAX_VOLUME_FRONT_LEFTRIGHT);
		TDA7719_SetVolume_LeftFront(Device_Cfg_Audio.tempVolFrontLeft, 0); //0 - 79
		break;
	case ATTE_RIGHT:
		Check_Volume_Range_Front(&Device_Cfg_Audio.VolFront.volumeRight, ENCODER_MAX_VOLUME_FRONT_LEFTRIGHT);
		TDA7719_SetVolume_RightFront(Device_Cfg_Audio.tempVolFrontRight, 0);
		break;
	default:
		break;
	}
}

void AppEncoders_EncoderVolBack_Rotated(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_VOLUME_BACK);
	SetSavedDisplay_StartTimer();

	switch (Device_Cfg_Audio.VolBack.audioOutputState)
	{
	case NORMAL:
		Check_Volume_Range_Back(&Device_Cfg_Audio.VolBack.volumeLeftRight, ENCODER_MAX_VOLUME_BACK_LEFTRIGHT);
		TDA7719_SetVolumeBack_LeftRight(Device_Cfg_Audio.tempVolBackLeft, Device_Cfg_Audio.tempVolBackRight);
		break;
	case MUTE:
		//no action for encoder
		break;
	case NORMAL_V2:
		Check_Volume_Range_Back(&Device_Cfg_Audio.VolBack.volumeLeftRight, ENCODER_MAX_VOLUME_BACK_LEFTRIGHT);
		TDA7719_SetVolumeBack_LeftRight(Device_Cfg_Audio.tempVolBackLeft, Device_Cfg_Audio.tempVolBackRight);
		break;
	case ATTE_LEFT:
		Check_Volume_Range_Back(&Device_Cfg_Audio.VolBack.volumeLeft, ENCODER_MAX_VOLUME_BACK_LEFTRIGHT);
		TDA7719_SetVolume_LeftRear(Device_Cfg_Audio.tempVolBackLeft, 0);
		break;
	case ATTE_RIGHT:
		Check_Volume_Range_Back(&Device_Cfg_Audio.VolBack.volumeRight, ENCODER_MAX_VOLUME_BACK_LEFTRIGHT);
		TDA7719_SetVolume_RightRear(Device_Cfg_Audio.tempVolBackLeft, 0);
		break;
	default:
		break;
	}
}

void AppEncoders_EncoderTreble_Rotated(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_TREBLE);
	SetSavedDisplay_StartTimer();

	switch (Device_Cfg_Audio.Treble.buttonControl)
	{
	case SET_GAIN:
		Check_Treble_Param_Range(&Device_Cfg_Audio.Treble.gain, 30);
		break;
	case SET_CENTER_FREQ:
		Check_Treble_Param_Range(&Device_Cfg_Audio.Treble.centerFreq, 3);
		break;
	case SET_SOFT_STEP:
		Check_Treble_Param_Range(&Device_Cfg_Audio.Treble.soft_step, 1);
		break;
	default:
		break;
	}
	TDA7719_SetTreble(Device_Cfg_Audio.Treble.gain - 16, Device_Cfg_Audio.Treble.centerFreq, Device_Cfg_Audio.Treble.soft_step);
}

void AppEncoders_EncoderBass_Rotated(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_BASS);
	SetSavedDisplay_StartTimer();

	switch (Device_Cfg_Audio.Bass.buttonControl)
	{
	case SET_GAIN:
		Check_Bass_Param_Range(&Device_Cfg_Audio.Bass.gain, 30);
		break;
	case SET_CENTER_FREQ:
		Check_Bass_Param_Range(&Device_Cfg_Audio.Bass.centerFreq, 3);
		break;
	case SET_SOFT_STEP:
		Check_Bass_Param_Range(&Device_Cfg_Audio.Bass.soft_step, 1);
		break;
	default:
		Device_Cfg_Audio.Bass.buttonControl = SET_GAIN;
		break;
	}
	TDA7719_SetBass(Device_Cfg_Audio.Bass.gain - 16, Device_Cfg_Audio.Bass.centerFreq, Device_Cfg_Audio.Bass.soft_step);
}

void AppEncoders_EncoderMiddle_Rotated(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_MIDDLE);
	SetSavedDisplay_StartTimer();

	switch (Device_Cfg_Audio.Middle.buttonControl)
	{
	case SET_GAIN:
		Check_Middle_Param_Range(&Device_Cfg_Audio.Middle.gain, 30);
		break;
	case SET_CENTER_FREQ:
		Check_Middle_Param_Range(&Device_Cfg_Audio.Middle.centerFreq, 3);
		break;
	case SET_SOFT_STEP:
		Check_Middle_Param_Range(&Device_Cfg_Audio.Middle.soft_step, 1);
		break;
	default:
		break;
	}
	TDA7719_SetMiddle(Device_Cfg_Audio.Middle.gain - 16, Device_Cfg_Audio.Middle.centerFreq, Device_Cfg_Audio.Middle.soft_step);
}

void AppEncoders_EncoderRadio_Rotated(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_RADIO);
	SetSavedDisplay_StartTimer();
}

void AppEncoders_EncoderLoudness_Rotated(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_LOUDNESS);
	SetSavedDisplay_StartTimer();

	switch (Device_Cfg_Audio.Loudness.buttonControl)
	{
	case SET_GAIN:
		Check_Loudness_Param_Range(&Device_Cfg_Audio.Loudness.gain, 15);
		break;
	case SET_CENTER_FREQ:
		Check_Loudness_Param_Range(&Device_Cfg_Audio.Loudness.centerFreq, 3);
		break;
	case SET_SOFT_STEP:
		Check_Loudness_Param_Range(&Device_Cfg_Audio.Loudness.soft_step, 1);
		break;
	case SET_HIGH_BOOST:
		Check_Loudness_Param_Range(&Device_Cfg_Audio.Loudness.high_boost, 1);
		break;
	default:
		Device_Cfg_Audio.Loudness.buttonControl = SET_GAIN;
		break;
	}
	TDA7719_SetLoudness(Device_Cfg_Audio.Loudness.gain, Device_Cfg_Audio.Loudness.centerFreq, Device_Cfg_Audio.Loudness.high_boost, Device_Cfg_Audio.Loudness.soft_step);
}

void AppEncoders_SingleEncoderStop(TIM_HandleTypeDef *htim)
{
	HAL_Encoder_SingleEncoderStop(htim);
}

void AppEncoders_SingleEncoderStart(TIM_HandleTypeDef *htim)
{
	HAL_Encoder_SingleEncoderStart(htim);
}

void HAL_Encoders_CheckVolumeRanges(void)
{
	if (Device_Cfg_Audio.volumeMasterFlag == 1)
	{
		Device_Cfg_Audio.tempVolFrontLeft 	= (Device_Cfg_Audio.VolFront.volumeLeft - OFFSET_VOLUME) 	+ (Device_Cfg_Audio.VolFront.volumeLeftRight - OFFSET_VOLUME);
		Device_Cfg_Audio.tempVolFrontRight 	= (Device_Cfg_Audio.VolFront.volumeRight - OFFSET_VOLUME) + (Device_Cfg_Audio.VolFront.volumeLeftRight - OFFSET_VOLUME);
		Device_Cfg_Audio.tempVolBackLeft 		= (Device_Cfg_Audio.VolBack.volumeLeft - OFFSET_VOLUME) 	+ (Device_Cfg_Audio.VolBack.volumeLeftRight - OFFSET_VOLUME);
		Device_Cfg_Audio.tempVolBackRight 	= (Device_Cfg_Audio.VolBack.volumeRight - OFFSET_VOLUME) 	+ (Device_Cfg_Audio.VolBack.volumeLeftRight - OFFSET_VOLUME);
	}
	else if (Device_Cfg_Audio.volumeMasterFlag == 0)
	{
		Device_Cfg_Audio.tempVolFrontLeft 	= (Device_Cfg_Audio.VolFront.volumeMaster - OFFSET_VOLUME) + (Device_Cfg_Audio.VolFront.volumeLeft - OFFSET_VOLUME) + (Device_Cfg_Audio.VolFront.volumeLeftRight - OFFSET_VOLUME);
		Device_Cfg_Audio.tempVolFrontRight 	= (Device_Cfg_Audio.VolFront.volumeMaster - OFFSET_VOLUME) + (Device_Cfg_Audio.VolFront.volumeRight - OFFSET_VOLUME) + (Device_Cfg_Audio.VolFront.volumeLeftRight - OFFSET_VOLUME);
		Device_Cfg_Audio.tempVolBackLeft 		= (Device_Cfg_Audio.VolFront.volumeMaster - OFFSET_VOLUME) + (Device_Cfg_Audio.VolBack.volumeLeft - OFFSET_VOLUME) + (Device_Cfg_Audio.VolBack.volumeLeftRight - OFFSET_VOLUME);
		Device_Cfg_Audio.tempVolBackRight 	= (Device_Cfg_Audio.VolFront.volumeMaster - OFFSET_VOLUME) + (Device_Cfg_Audio.VolBack.volumeRight - OFFSET_VOLUME) + (Device_Cfg_Audio.VolBack.volumeLeftRight - OFFSET_VOLUME);
	}

	if (Device_Cfg_Audio.tempVolFrontLeft 	<= -OFFSET_VOLUME) 		Device_Cfg_Audio.tempVolFrontLeft  = -OFFSET_VOLUME;
	if (Device_Cfg_Audio.tempVolFrontRight 	<= -OFFSET_VOLUME) 		Device_Cfg_Audio.tempVolFrontRight = -OFFSET_VOLUME;
	if (Device_Cfg_Audio.tempVolBackLeft 		<= -OFFSET_VOLUME)		Device_Cfg_Audio.tempVolBackLeft   = -OFFSET_VOLUME;
	if (Device_Cfg_Audio.tempVolBackRight 	<= -OFFSET_VOLUME)		Device_Cfg_Audio.tempVolBackRight  = -OFFSET_VOLUME;
}

// Checks if given value (volume) is given range
// It also increments or decrements value depending on CNT register upgraded by volume front encoder
static void Check_Volume_Range_Front(volatile int8_t *const volume, const uint8_t maxVolume)
{
	static int16_t TimerDiff1;
	static uint16_t LastTimerCounter1;

	TimerDiff1 = __HAL_TIM_GET_COUNTER(ENCODER_VOL_FRONT) - LastTimerCounter1;
	if (TimerDiff1 >= 4 || TimerDiff1 <= -4)
	{
		TimerDiff1 /= 4;
		(*volume) += (int8_t) TimerDiff1;
		if ((*volume) > maxVolume)
			(*volume) = maxVolume;
		if ((*volume) < 0)
			(*volume) = 0;
		LastTimerCounter1 = __HAL_TIM_GET_COUNTER(ENCODER_VOL_FRONT);

	}
}

// Checks if given value (volume) is given range
// It also increments or decrements value depending on CNT register upgraded by volume back encoder
static void Check_Volume_Range_Back(volatile int8_t *const volume, const uint8_t maxVolume)
{
	static int16_t TimerDiff2;
	static uint16_t LastTimerCounter2;

	TimerDiff2 = __HAL_TIM_GET_COUNTER(ENCODER_VOL_BACK) - LastTimerCounter2;
	if (TimerDiff2 >= 4 || TimerDiff2 <= -4)
	{
		TimerDiff2 /= 4;
		(*volume) += (int8_t) TimerDiff2;
		if ((*volume) > maxVolume)
			(*volume) = maxVolume;
		if ((*volume) < 0)
			(*volume) = 0;
		LastTimerCounter2 = __HAL_TIM_GET_COUNTER(ENCODER_VOL_BACK);
	}
}

// Checks if given value (loudness attenuator, center freqency, soft step, high boost) is given range
// It also increments or decrements value depending on CNT register upgraded by loudness encoder
static void Check_Loudness_Param_Range(volatile int8_t *const gain, const uint8_t maxGain)
{
	static int16_t TimerDiff3;
	static uint16_t LastTimerCounter3;

	TimerDiff3 = __HAL_TIM_GET_COUNTER(ENCODER_VOL_LOUD) - LastTimerCounter3;
	if (TimerDiff3 >= 4 || TimerDiff3 <= -4)
	{
		TimerDiff3 /= 4;
		(*gain) += (int8_t) TimerDiff3;
		if ((*gain) >= maxGain)
			(*gain) = maxGain;
		if ((*gain) <= 0)
			(*gain) = 0;
		LastTimerCounter3 = __HAL_TIM_GET_COUNTER(ENCODER_VOL_LOUD);
	}
}

// Checks if given value (loudness, Bass Q Factor, soft step) is given range
// It also increments or decrements value depending on CNT register upgraded by bass encoder
static void Check_Bass_Param_Range(volatile int8_t *const gain, const uint8_t maxGain)
{
	static int16_t TimerDiff3;
	static uint16_t LastTimerCounter3;

	TimerDiff3 = __HAL_TIM_GET_COUNTER(ENCODER_VOL_BASS) - LastTimerCounter3;
	if (TimerDiff3 >= 4 || TimerDiff3 <= -4)
	{
		TimerDiff3 /= 4;
		(*gain) += (int8_t) TimerDiff3;
		if ((*gain) >= maxGain)
			(*gain) = maxGain;
		if ((*gain) <= 0)
			(*gain) = 0;
		LastTimerCounter3 = __HAL_TIM_GET_COUNTER(ENCODER_VOL_BASS);
	}
}

// Checks if given value (loudness, middle Q Factor, soft step) is given range
// It also increments or decrements value depending on CNT register upgraded by middle encoder
static void Check_Middle_Param_Range(volatile int8_t *const gain, const uint8_t maxGain)
{
	static int16_t TimerDiff3;
	static uint16_t LastTimerCounter3;

	TimerDiff3 = HAL_Encoders_Middle_GetRotateValue() - LastTimerCounter3;
	if(TimerDiff3 >= 4 || TimerDiff3 <= -4)
	{
		// TimerDiff3 /= 4;
		(*gain) += (int8_t) TimerDiff3;
		if ((*gain) >= maxGain)
			(*gain) = maxGain;
		if ((*gain) <= 0)
			(*gain) = 0;
		LastTimerCounter3 = HAL_Encoders_Middle_GetRotateValue();
	}
}

// Checks if given value (loudness, treble Q Factor, soft step) is given range
// It also increments or decrements value depending on CNT register upgraded by treble encoder
static void Check_Treble_Param_Range(volatile int8_t *const gain, const uint8_t maxGain)
{
	static int16_t TimerDiff3;
	static uint16_t LastTimerCounter3;

	TimerDiff3 = __HAL_TIM_GET_COUNTER(ENCODER_VOL_TREBLE) - LastTimerCounter3;
	if (TimerDiff3 >= 4 || TimerDiff3 <= -4)
	{
		TimerDiff3 /= 4;
		(*gain) += (int8_t) TimerDiff3;
		if ((*gain) >= maxGain)
			(*gain) = maxGain;
		if ((*gain) <= 0)
			(*gain) = 0;
		LastTimerCounter3 = __HAL_TIM_GET_COUNTER(ENCODER_VOL_TREBLE);
	}
}

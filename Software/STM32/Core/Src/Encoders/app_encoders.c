//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "app_encoders.h"

//--------------------------------------------------------------
// Defines
//--------------------------------------------------------------

//--------------------------------------------------------------
// Variables
//--------------------------------------------------------------
encoderFilter_t encoderFilterTreble;
encoderFilter_t encoderFilterMiddle;
encoderFilter_t encoderFilterBass;
encoderFilter_t encoderFilterLoudness;
encoder_t encoderVolFront;
encoder_t encoderVolBack;

uint8_t volumeMasterFlag;
int16_t tempVolFrontLeft;
int16_t tempVolFrontRight;
int16_t tempVolBackLeft;
int16_t tempVolBackRight;

int32_t licznik = 0;

savedUserSettings_t savedUserSettings;

//--------------------------------------------------------------
// external variables
//--------------------------------------------------------------
extern uint8_t volumeMasterFlag;

static void Check_and_Set_Volume_back(void);
static void Check_Volume_Range_Front(volatile int8_t *const volume, const uint8_t maxVolume);
static void Check_Volume_Range_Back(volatile int8_t *const volume, const uint8_t maxVolume);
static void Check_Loudness_Param_Range(volatile int8_t *const gain, const uint8_t maxGain);
static void Check_Bass_Param_Range(volatile int8_t *const gain, const uint8_t maxGain);
static void Check_Middle_Param_Range(volatile int8_t *const gain, const uint8_t maxGain);
static void Check_Treble_Param_Range(volatile int8_t *const gain, const uint8_t maxGain);
//--------------------------------------------------------------
// Function definition
//--------------------------------------------------------------
void AppEncoders_EncoderVolFront_Rotated(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_VOLUME_FRONT);

	switch (encoderVolFront.audioOutputState)
	{
	case MASTER:
		Check_Volume_Range_Front(&encoderVolFront.volumeMaster, 94);
		if (encoderVolFront.volumeMaster >= 80)
			volumeMasterFlag = 1;
		else
			volumeMasterFlag = 0;
		TDA7719_SetVolume_Master(tempVolFrontLeft, tempVolFrontRight, tempVolBackLeft, tempVolBackRight);
		break;
	case MUTE:
		//jak wyłącze przerwanie on enkodera to teraz nie bedzie tutaj kod wchodził
		//więc może jakieś zerowanie rejestru CNT.
		break;
	case MASTER_V2:
		Check_Volume_Range_Front(&encoderVolFront.volumeMaster, 94);
		if (encoderVolFront.volumeMaster >= 80)
			volumeMasterFlag = 1;
		else
			volumeMasterFlag = 0;
		TDA7719_SetVolume_Master(tempVolFrontLeft, tempVolFrontRight, tempVolBackLeft, tempVolBackRight);
		break;
	case NORMAL:
		Check_Volume_Range_Front(&encoderVolFront.volumeLeftRight, 79);
		TDA7719_SetVolumeFront_LeftRight(tempVolFrontLeft, tempVolFrontRight);
		break;
	case ATTE_LEFT:
		Check_Volume_Range_Front(&encoderVolFront.volumeLeft, 79);
		TDA7719_SetVolume_LeftFront(tempVolFrontLeft, 0); //0 - 79
		break;
	case ATTE_RIGHT:
		Check_Volume_Range_Front(&encoderVolFront.volumeRight, 79);
		TDA7719_SetVolume_RightFront(tempVolFrontRight, 0);
		break;
	default:
		break;
	}
}

void AppEncoders_EncoderVolBack_Rotated(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_VOLUME_BACK);
	//HAL_Timers_RefreshTimer(&htim14, TIM_CHANNEL_1);

	switch (encoderVolBack.audioOutputState)
	{
	case NORMAL:
		Check_Volume_Range_Back(&encoderVolBack.volumeLeftRight, 79);
		TDA7719_SetVolumeBack_LeftRight(tempVolBackLeft, tempVolBackRight);
		break;
	case MUTE:
		//no action for encoder
		break;
	case NORMAL_V2:
		Check_Volume_Range_Back(&encoderVolBack.volumeLeftRight, 79);
		TDA7719_SetVolumeBack_LeftRight(tempVolBackLeft, tempVolBackRight);
		break;
	case ATTE_LEFT:
		Check_Volume_Range_Back(&encoderVolBack.volumeLeft, 79);
		TDA7719_SetVolume_LeftRear(tempVolBackLeft, 0);
		break;
	case ATTE_RIGHT:
		Check_Volume_Range_Back(&encoderVolBack.volumeRight, 79);
		TDA7719_SetVolume_RightRear(tempVolBackLeft, 0);
		break;
	default:
		break;
	}
}

void AppEncoders_EncoderTreble_Rotated(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_TREBLE);
	//HAL_Timers_RefreshTimer(&htim14, TIM_CHANNEL_1);

	switch (encoderFilterTreble.buttonControl)
	{
	case SET_GAIN:
		Check_Treble_Param_Range(&encoderFilterTreble.gain, 30);
		break;
	case SET_CENTER_FREQ:
		Check_Treble_Param_Range(&encoderFilterTreble.centerFreq, 3);
		break;
	case SET_SOFT_STEP:
		Check_Treble_Param_Range(&encoderFilterTreble.soft_step, 1);
		break;
	default:
		break;
	}
	TDA7719_SetTreble(encoderFilterTreble.gain - 16, encoderFilterTreble.centerFreq, encoderFilterTreble.soft_step);
}

void AppEncoders_EncoderBass_Rotated(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_BASS);
	//HAL_Timers_RefreshTimer(&htim14, TIM_CHANNEL_1);

	switch (encoderFilterBass.buttonControl)
	{
	case SET_GAIN:
		Check_Bass_Param_Range(&encoderFilterBass.gain, 30);
		break;
	case SET_CENTER_FREQ:
		Check_Bass_Param_Range(&encoderFilterBass.centerFreq, 3);
		break;
	case SET_SOFT_STEP:
		Check_Bass_Param_Range(&encoderFilterBass.soft_step, 1);
		break;
	default:
		encoderFilterBass.buttonControl = SET_GAIN;
		break;
	}
	TDA7719_SetBass(encoderFilterBass.gain - 16, encoderFilterBass.centerFreq, encoderFilterBass.soft_step);
}

void AppEncoders_EncoderMiddle_Rotated(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_MIDDLE);
	//HAL_Timers_RefreshTimer(&htim14, TIM_CHANNEL_1);

	switch (encoderFilterMiddle.buttonControl)
	{
	case SET_GAIN:
		Check_Middle_Param_Range(&encoderFilterMiddle.gain, 30);
		break;
	case SET_CENTER_FREQ:
		Check_Middle_Param_Range(&encoderFilterMiddle.centerFreq, 3);
		break;
	case SET_SOFT_STEP:
		Check_Middle_Param_Range(&encoderFilterMiddle.soft_step, 1);
		break;
	default:
		break;
	}
	TDA7719_SetMiddle(encoderFilterMiddle.gain - 16, encoderFilterMiddle.centerFreq, encoderFilterMiddle.soft_step);
}

void AppEncoders_EncoderRadio_Rotated(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_RADIO);
	//HAL_Timers_RefreshTimer(&htim14, TIM_CHANNEL_1);

}

void AppEncoders_EncoderLoudness_Rotated(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_LOUDNESS);
	//HAL_Timers_RefreshTimer(&htim14, TIM_CHANNEL_1);

	switch (encoderFilterLoudness.buttonControl)
	{
	case SET_GAIN:
		Check_Loudness_Param_Range(&encoderFilterLoudness.gain, 15);
		break;
	case SET_CENTER_FREQ:
		Check_Loudness_Param_Range(&encoderFilterLoudness.centerFreq, 3);
		break;
	case SET_SOFT_STEP:
		Check_Loudness_Param_Range(&encoderFilterLoudness.soft_step, 1);
		break;
	case SET_HIGH_BOOST:
		Check_Loudness_Param_Range(&encoderFilterLoudness.high_boost, 1);
		break;
	default:
		encoderFilterLoudness.buttonControl = SET_GAIN;
		break;
	}
	TDA7719_SetLoudness(encoderFilterLoudness.gain, encoderFilterLoudness.centerFreq, encoderFilterLoudness.high_boost, encoderFilterLoudness.soft_step);
}

void AppEncoders_SingleEncoderStop(TIM_HandleTypeDef *htim)
{
	HAL_Encoder_SingleEncoderStop(htim);
}

void AppEncoders_SingleEncoderStart(TIM_HandleTypeDef *htim)
{
	HAL_Encoder_SingleEncoderStart(htim);
}

void check_volumes_ranges(void)
{
	if (volumeMasterFlag == 1)
	{
		tempVolFrontLeft 	= (encoderVolFront.volumeLeft - 79) 	+ (encoderVolFront.volumeLeftRight - 79);
		tempVolFrontRight = (encoderVolFront.volumeRight - 79) 	+ (encoderVolFront.volumeLeftRight - 79);
		tempVolBackLeft 	= (encoderVolBack.volumeLeft - 79) 	+ (encoderVolBack.volumeLeftRight - 79);
		tempVolBackRight 	= (encoderVolBack.volumeRight - 79) 	+ (encoderVolBack.volumeLeftRight - 79);
	}
	else if (volumeMasterFlag == 0)
	{
		tempVolFrontLeft = (encoderVolFront.volumeMaster - 79) + (encoderVolFront.volumeLeft - 79) + (encoderVolFront.volumeLeftRight - 79);
		tempVolFrontRight = (encoderVolFront.volumeMaster - 79) + (encoderVolFront.volumeRight - 79) + (encoderVolFront.volumeLeftRight - 79);
		tempVolBackLeft = (encoderVolFront.volumeMaster - 79) + (encoderVolBack.volumeLeft - 79) + (encoderVolBack.volumeLeftRight - 79);
		tempVolBackRight = (encoderVolFront.volumeMaster - 79) + (encoderVolBack.volumeRight - 79) + (encoderVolBack.volumeLeftRight - 79);
	}

	if (tempVolFrontLeft <= -79) 		tempVolFrontLeft  = -79;
	if (tempVolFrontRight <= -79) 	tempVolFrontRight = -79;
	if (tempVolBackLeft <= -79)		tempVolBackLeft   = -79;
	if (tempVolBackRight <= -79)		tempVolBackRight  = -79;
}

// Checks if given value (volume) is given range
// It also increments or decrements value depending on CNT register upgraded by volume front encoder
void Check_Volume_Range_Front(volatile int8_t *const volume, const uint8_t maxVolume)
{
	static int16_t TimerDiff1;
	static uint16_t LastTimerCounter1;

	TimerDiff1 = htim5.Instance->CNT - LastTimerCounter1;
	if (TimerDiff1 >= 4 || TimerDiff1 <= -4)
	{
		TimerDiff1 /= 4;
		(*volume) += (int8_t) TimerDiff1;
		if ((*volume) > maxVolume)
			(*volume) = maxVolume;
		if ((*volume) < 0)
			(*volume) = 0;
		LastTimerCounter1 = htim5.Instance->CNT;
	}
}

// Checks if given value (volume) is given range
// It also increments or decrements value depending on CNT register upgraded by volume back encoder
void Check_Volume_Range_Back(volatile int8_t *const volume, const uint8_t maxVolume)
{
	static int16_t TimerDiff2;
	static uint16_t LastTimerCounter2;

	TimerDiff2 = htim3.Instance->CNT - LastTimerCounter2;
	if (TimerDiff2 >= 4 || TimerDiff2 <= -4)
	{
		TimerDiff2 /= 4;
		(*volume) += (int8_t) TimerDiff2;
		if ((*volume) > maxVolume)
			(*volume) = maxVolume;
		if ((*volume) < 0)
			(*volume) = 0;
		LastTimerCounter2 = htim3.Instance->CNT;
	}
}

// Checks if given value (loudness attenuator, center freqency, soft step, high boost) is given range
// It also increments or decrements value depending on CNT register upgraded by loudness encoder
void Check_Loudness_Param_Range(volatile int8_t *const gain, const uint8_t maxGain)
{
	static int16_t TimerDiff3;
	static uint16_t LastTimerCounter3;

	TimerDiff3 = htim8.Instance->CNT - LastTimerCounter3;
	if (TimerDiff3 >= 4 || TimerDiff3 <= -4)
	{
		TimerDiff3 /= 4;
		(*gain) += (int8_t) TimerDiff3;
		if ((*gain) >= maxGain)
			(*gain) = maxGain;
		if ((*gain) <= 0)
			(*gain) = 0;
		LastTimerCounter3 = htim8.Instance->CNT;
	}
}

// Checks if given value (loudness, Bass Q Factor, soft step) is given range
// It also increments or decrements value depending on CNT register upgraded by bass encoder
void Check_Bass_Param_Range(volatile int8_t *const gain, const uint8_t maxGain)
{
	static int16_t TimerDiff3;
	static uint16_t LastTimerCounter3;

	TimerDiff3 = htim2.Instance->CNT - LastTimerCounter3;
	if (TimerDiff3 >= 4 || TimerDiff3 <= -4)
	{
		TimerDiff3 /= 4;
		(*gain) += (int8_t) TimerDiff3;
		if ((*gain) >= maxGain)
			(*gain) = maxGain;
		if ((*gain) <= 0)
			(*gain) = 0;
		LastTimerCounter3 = htim2.Instance->CNT;
	}
}

// Checks if given value (loudness, middle Q Factor, soft step) is given range
// It also increments or decrements value depending on CNT register upgraded by middle encoder
void Check_Middle_Param_Range(volatile int8_t *const gain, const uint8_t maxGain)
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
void Check_Treble_Param_Range(volatile int8_t *const gain, const uint8_t maxGain)
{
	static int16_t TimerDiff3;
	static uint16_t LastTimerCounter3;

	TimerDiff3 = htim1.Instance->CNT - LastTimerCounter3;
	if (TimerDiff3 >= 4 || TimerDiff3 <= -4)
	{
		TimerDiff3 /= 4;
		(*gain) += (int8_t) TimerDiff3;
		if ((*gain) >= maxGain)
			(*gain) = maxGain;
		if ((*gain) <= 0)
			(*gain) = 0;
		LastTimerCounter3 = htim1.Instance->CNT;
	}
}

//--------------------------------------------------------------
/* encoders.c
 * contains operations for every used encoder for the device
*/
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "Encoders/encoders.h"



//--------------------------------------------------------------
// Function declarations
//--------------------------------------------------------------



// Is called only when system starts
void encoders_init(void)
    {
	HAL_TIM_Encoder_Start_IT(&htim1, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
//	HAL_TIM_Encoder_Start_IT(&htim4, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start_IT(&htim5, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start_IT(&htim8, TIM_CHANNEL_ALL);
    }

//
void volume_front_encoder_used(void)
    {
	save_current_display_state(SSD1322_SCREEN_ENCODER_VOLUME_FRONT);
	refresh_timer_state();

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

//
void volume_back_encoder_used(void)
    {
	save_current_display_state(SSD1322_SCREEN_ENCODER_VOLUME_BACK);
	refresh_timer_state();
	//dodać też to kiedy wciśnięty będzie przycisk
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

//
void loudness_encoder_used(void)
    {
	save_current_display_state(SSD1322_SCREEN_ENCODER_LOUDNESS);
	refresh_timer_state();

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
	TDA7719_SetLoudness(encoderFilterLoudness.gain, encoderFilterLoudness.centerFreq,
		encoderFilterLoudness.high_boost, encoderFilterLoudness.soft_step);
    }

//
void bass_encoder_used(void)
    {
	save_current_display_state(SSD1322_SCREEN_ENCODER_BASS);
	refresh_timer_state();

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

//
void middle_encoder_used(void)
    {
	save_current_display_state(SSD1322_SCREEN_ENCODER_MIDDLE);
	refresh_timer_state();
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

//
void treble_encoder_used(void)
    {
	save_current_display_state(SSD1322_SCREEN_ENCODER_TREBLE);
	refresh_timer_state();

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

//
void radio_encoder_used(void)
    {

    }

//--------------------------------------------------------------
/* buttons.c
 * contains operations on every button for the device
 *
*/
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include <Display_OLED/SSD1322_API.h>
#include "Display_OLED/draws_display.h"
#include "buttons/buttons.h"



//--------------------------------------------------------------
// Static variables
//--------------------------------------------------------------
static uint8_t refresh_time = 1;
static char letter = 'a';
static uint8_t inc = 0; //97 - 'a'

//--------------------------------------------------------------
// typedefs
//--------------------------------------------------------------
FFT_channel_source_e 		FFT_channel_source = 1;
SettingsUserMenu_t 	 	SettingsUserMenu;
extern SSD1322_Screen_t 	SSD1322_Screen_State;
extern Clock_Data_Change_t 	Clock_Data_Time;
extern AlarmDataChange_t 		AlarmDataChange;
//--------------------------------------------------------------
// Static functions declarations
//--------------------------------------------------------------
static void Save_Station_Freq_1(void);
static void Save_Station_Freq_2(void);
static void Change_FFT_source_Down(void);
static void Change_FFT_source_Up(void);
static void Change_Down_Settings(void);
static void Change_Up_Settings(void);
static void Change_Down_Input(void);
static void Change_Up_Input(void);
static void Change_selected_setting(void);
static void Read_Set_TimeAndDate(void);

//--------------------------------------------------------------
// Global functions for encoders
//--------------------------------------------------------------

// Volume master takes into account attenuator of each channel and front and back volume aswell
void TDA7719_SetVolume_Master(const int16_t VolFrontLeft, const int16_t VolFrontRight, const int16_t VolBackLeft, const int16_t VolBackRight)
    {
	//write to diffrent TDA7719 register depending on value
	if ((encoderVolFront.volumeMaster <= 94) && (encoderVolFront.volumeMaster >= 80))
	    {
		TDA7719_SetVolume((encoderVolFront.volumeMaster) - 79, 0, 0);
	    }
	else if ((encoderVolFront.volumeMaster >= 0 ) && (encoderVolFront.volumeMaster <= 79))
	    {
		TDA7719_SetVolume_LeftFront ((VolFrontLeft) , 0);
		TDA7719_SetVolume_RightFront((VolFrontRight), 0);
		TDA7719_SetVolume_LeftRear  ((VolBackLeft)  , 0);
		TDA7719_SetVolume_RightRear ((VolBackRight) , 0);
	    }
    }

// Sets volume just for both front channels
void TDA7719_SetVolumeFront_LeftRight(const int16_t VolFrontLeft, const int16_t VolFrontRight)
    {

	if (VolFrontLeft <= -79)
		TDA7719_SetVolume_LeftFront(VOLUME_MUTE, 0); //0 - mute, 79 - max_volume
	else 	TDA7719_SetVolume_LeftFront(VolFrontLeft, 0);

	if(VolFrontRight <= -79)
		TDA7719_SetVolume_RightFront(VOLUME_MUTE, 0); //0 - mute, 79 - max_volume
	else 	TDA7719_SetVolume_RightFront(VolFrontRight, 0);

    }

// Checks if given value (volume) is given range
// It also increments or decrements value depending on CNT register upgraded by volume front encoder
void Check_Volume_Range_Front(volatile int8_t *const volume, const uint8_t maxVolume)
    {
	static int16_t  TimerDiff1;
	static uint16_t LastTimerCounter1;

	TimerDiff1 = htim2.Instance->CNT - LastTimerCounter1;
	if(TimerDiff1 >= 4 || TimerDiff1 <= -4)
	{
	    TimerDiff1 /= 4;
	    (*volume) += (int8_t) TimerDiff1;
	    if((*volume) > maxVolume) (*volume) = maxVolume;
	    if((*volume) < 0)  (*volume) = 0;
	    LastTimerCounter1 = htim2.Instance->CNT;
	}
    }

// Sets volume just for both back channels
void TDA7719_SetVolumeBack_LeftRight(const int16_t VolBackLeft, const int16_t VolBackRight)
    {
	if (VolBackLeft <= -79)
		TDA7719_SetVolume_LeftRear (VOLUME_MUTE, 0); //0 - mute, 79 - max_volume
	else 	TDA7719_SetVolume_LeftRear (VolBackLeft, 0);

	if(VolBackRight <= -79)
		TDA7719_SetVolume_RightRear(VOLUME_MUTE, 0); //0 - mute, 79 - max_volume
	else 	TDA7719_SetVolume_RightRear(VolBackRight, 0);

    }

// Checks if given value (volume) is given range
// It also increments or decrements value depending on CNT register upgraded by volume back encoder
void Check_Volume_Range_Back(volatile int8_t *const volume, const uint8_t maxVolume)
    {
	static int16_t  TimerDiff2;
	static uint16_t LastTimerCounter2;

	TimerDiff2 = htim8.Instance->CNT - LastTimerCounter2;
	if(TimerDiff2 >= 4 || TimerDiff2 <= -4)
	{
	    TimerDiff2 /= 4;
	    (*volume) += (int8_t) TimerDiff2;
	    if((*volume) > maxVolume) (*volume) = maxVolume;
	    if((*volume) < 0)  (*volume) = 0;
	    LastTimerCounter2 = htim8.Instance->CNT;
	}
    }

// Checks if given value (loudness attenuator, center freqency, soft step, high boost) is given range
// It also increments or decrements value depending on CNT register upgraded by loudness encoder
void Check_Loudness_Param_Range(volatile int8_t *const gain, const uint8_t maxGain)
    {
	static int16_t  TimerDiff3;
	static uint16_t LastTimerCounter3;

	TimerDiff3 = htim1.Instance->CNT - LastTimerCounter3;
	if(TimerDiff3 >= 4 || TimerDiff3 <= -4)
	{
	    TimerDiff3 /= 4;
	    (*gain) += (int8_t) TimerDiff3;
	    if((*gain) >= maxGain) (*gain) = maxGain;
	    if((*gain) <= 0)       (*gain) = 0;
	    LastTimerCounter3 = htim1.Instance->CNT;
	}
    }

// Checks if given value (loudness, Bass Q Factor, soft step) is given range
// It also increments or decrements value depending on CNT register upgraded by bass encoder
void Check_Bass_Param_Range(volatile int8_t *const gain, const uint8_t maxGain)
    {
	static int16_t  TimerDiff3;
	static uint16_t LastTimerCounter3;

	TimerDiff3 = htim1.Instance->CNT - LastTimerCounter3;
	if(TimerDiff3 >= 4 || TimerDiff3 <= -4)
	{
	    TimerDiff3 /= 4;
	    (*gain) += (int8_t) TimerDiff3;
	    if((*gain) >= maxGain) (*gain) = maxGain;
	    if((*gain) <= 0) 	   (*gain) = 0;
	    LastTimerCounter3 = htim1.Instance->CNT;
	}
    }

// Checks if given value (loudness, middle Q Factor, soft step) is given range
// It also increments or decrements value depending on CNT register upgraded by middle encoder
void Check_Middle_Param_Range(volatile int8_t *const gain, const uint8_t maxGain)
    {
	static int16_t  TimerDiff3;
	static uint16_t LastTimerCounter3;

	TimerDiff3 = htim3.Instance->CNT - LastTimerCounter3;
	if(TimerDiff3 >= 4 || TimerDiff3 <= -4)
	{
	    TimerDiff3 /= 4;
	    (*gain) += (int8_t) TimerDiff3;
	    if((*gain) >= maxGain) (*gain) = maxGain;
	    if((*gain) <= 0)  	   (*gain) = 0;
	    LastTimerCounter3 = htim3.Instance->CNT;
	}
    }

// Checks if given value (loudness, treble Q Factor, soft step) is given range
// It also increments or decrements value depending on CNT register upgraded by treble encoder
void Check_Treble_Param_Range(volatile int8_t *const gain, const uint8_t maxGain)
    {
	static int16_t  TimerDiff3;
	static uint16_t LastTimerCounter3;

	TimerDiff3 = htim5.Instance->CNT - LastTimerCounter3;
	if(TimerDiff3 >= 4 || TimerDiff3 <= -4)
	{
	    TimerDiff3 /= 4;
	    (*gain) += (int8_t) TimerDiff3;
	    if((*gain) >= maxGain) (*gain) = maxGain;
	    if((*gain) <= 0)	   (*gain) = 0;
	    LastTimerCounter3 = htim5.Instance->CNT;
	}
    }

//
void any_buttons_pressed_routine(void)
    {
	//if(SysTick_CTRL->ENABLE == true)
	if (is_display_on_standby_flag == true)
	    {
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		SSD1322_API_sleep_off();
		saved_seconds = sTime.Seconds;
		saved_minutes = sTime.Minutes;
	    }

	button_debou_state = true;
	HAL_TIM_OC_Stop_IT(&htim17, TIM_CHANNEL_1);
//		HAL_TIM_OC_Start_IT(&htim16, TIM_CHANNEL_1);
	switch (gGPIO_Pin)
	    {
	    case POWER_BUTTON_Pin:
		Power_button_pressed();

		break;
	    case USER_BUTTON_1_Pin:
		if (HAL_GPIO_ReadPin(USER_BUTTON_1_GPIO_Port, USER_BUTTON_1_Pin) == GPIO_PIN_RESET)
		    {
			SSD1322_Screen_State++;
			if (ENUM_MAX_USER_DISPLAY == SSD1322_Screen_State) SSD1322_Screen_State = SSD1322_SCREEN_TIME;

			Button_pressed_change_screen();
		    }
		break;
	    case USER_BUTTON_2_Pin:
		if (HAL_GPIO_ReadPin(USER_BUTTON_2_GPIO_Port, USER_BUTTON_2_Pin) == GPIO_PIN_RESET)
		    {
			Button_pressed_accept();
		    }
		break;

	    case USER_BUTTON_3_Pin:
		// if (HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin) == GPIO_PIN_RESET)
		{
		    Button_pressed_set_down();
		}
		break;

	    case USER_BUTTON_4_Pin:
		//if (HAL_GPIO_ReadPin(USER_BUTTON_4_GPIO_Port, USER_BUTTON_4_Pin) == GPIO_PIN_RESET)
		{
		    Button_pressed_set_up();
		}
		break;
	    case ENCODER_BUTTON_VOLUME_FRONT_Pin:
		if (HAL_GPIO_ReadPin(ENCODER_BUTTON_VOLUME_FRONT_GPIO_Port, ENCODER_BUTTON_VOLUME_FRONT_Pin) == GPIO_PIN_RESET)
		    {
			Encoder_button_volume_front();
		    }
		break;
	    case ENCODER_BUTTON_VOLUME_BACK_Pin:
		if (HAL_GPIO_ReadPin(ENCODER_BUTTON_VOLUME_BACK_GPIO_Port, ENCODER_BUTTON_VOLUME_BACK_Pin) == GPIO_PIN_RESET)
		    {
			Encoder_button_volume_back();
		    }
		break;
	    case ENCODER_BUTTON_LOUDNESS_Pin:
		if (HAL_GPIO_ReadPin(ENCODER_BUTTON_LOUDNESS_GPIO_Port, ENCODER_BUTTON_LOUDNESS_Pin) == GPIO_PIN_RESET)
		    {
			Encoder_button_loudness();
			//ENCODER_BUTTON_BASS();
		    }
		break;
//		    case ENCODER_BUTTON_BASS_Pin:
//			if (HAL_GPIO_ReadPin(ENCODER_BUTTON_BASS_GPIO_Port, ENCODER_BUTTON_BASS_Pin) == GPIO_PIN_RESET)
//			    {
//				Encoder_button_treble();
//			    }
//			break;
//		    case ENCODER_BUTTON_MIDDLE_Pin:
//			if (HAL_GPIO_ReadPin(ENCODER_BUTTON_MIDDLE_GPIO_Port, ENCODER_BUTTON_MIDDLE_Pin) == GPIO_PIN_RESET)
//			    {
//				Encoder_button_middle();
//			    }
//			break;
//		    case ENCODER_BUTTON_TREBLE_Pin:
//			if (HAL_GPIO_ReadPin(ENCODER_BUTTON_TREBLE_GPIO_Port, ENCODER_BUTTON_TREBLE_Pin) == GPIO_PIN_RESET)
//			    {
//				Encoder_button_bass();
//			    }
//			break;
//		    default:
//			break;
	    }
    }

//
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
    {
	// after debouncing time go to timer output compare callback
//	if (states1 == true) // zabezpieczenie przed kolejnym wywołaniem przed zakończeniem timera
	    {
		HAL_TIM_OC_Start_IT(&htim17, TIM_CHANNEL_1);
		button_debou_state = false;
		gGPIO_Pin = GPIO_Pin;
	    }

    }
//--------------------------------------------------------------
/* Global Functions for handling buttons
 *
 * There are four buttons:
 * - Change screen
 * - Accept
 * - Set value up
 * - Set value down
*/
//--------------------------------------------------------------


// -_-
void Button_pressed_change_screen(void)
    {
	if(ADC_IS_ON_flag == true)
	    {
		HAL_TIM_Base_Stop(&htim6);
		HAL_ADC_Stop_DMA(&hadc1);
		ADC_IS_ON_flag = false;
	    }


	switch (SSD1322_Screen_State)
	    {
	    case SSD1322_SCREEN_TIME:

		break;
	    case SSD1322_SCREEN_RADIO:

		break;
	    case SSD1322_SCREEN_FFT:
		HAL_TIM_Base_Start(&htim6);
		HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_SamplesTEST, UV_meter_numb_of_chan);
		ADC_IS_ON_flag = true;
		break;
	    case SSD1322_SCREEN_UVMETER:
		HAL_TIM_Base_Start(&htim6);
		HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_SamplesTEST, UV_meter_numb_of_chan);
		ADC_IS_ON_flag = true;
		//set flag indicating that adc is on
		break;
	    case SSD1322_SCREEN_SETCLOCK:

		break;
	    case SSD1322_SCREEN_SETALARM:

		break;
	    case SSD1322_SCREEN_SETINPUT:

		break;
	    case SSD1322_SCREEN_SETTINGS:

		break;
	    case SSD1322_SCREEN_USB:

		break;
	    case SSD1322_SCREEN_TIME_BOUNCING:

		break;
	    default:
		if (ENUM_MAX_USER_DISPLAY == SSD1322_Screen_State)
		    {
			SSD1322_Screen_State = SSD1322_SCREEN_TIME;
		    }
		break;
	    }

    }

//
void Button_pressed_accept(void)
    {
	switch (SSD1322_Screen_State)
	    {
	    case SSD1322_SCREEN_TIME:
		break;
	    case SSD1322_SCREEN_RADIO:
		RDA5807_SeekUp();
		break;
	    case SSD1322_SCREEN_WakeUp:
		//po wcisnięciu budzika można ustawić czy się wyłącza muzyka
		break;
	    case SSD1322_SCREEN_FFT:
		break;
	    case SSD1322_SCREEN_UVMETER:
		break;
	    case SSD1322_SCREEN_OFF:
		break;
	    case SSD1322_SCREEN_GoodBye:
		break;
	    case SSD1322_SCREEN_SETCLOCK:
		Clock_Data_Time++;
		if (9 == Clock_Data_Time)
		    {
			Clock_Data_Time = 1;
		    }
		break;
	    case SSD1322_SCREEN_SETALARM:
		AlarmDataChange++;
		if (7 == AlarmDataChange)
		    {
			AlarmDataChange = 1;
		    }
		break;
	    case SSD1322_SCREEN_SETTINGS:
		Change_selected_setting();
		break;
	    case SSD1322_SCREEN_USB:
		//tutaj akcept wejście w folder albo otworzenie pliku
		//jeżeli brak podpiętego USB to komunikat żeby podłączyć z formatowanie FatFS
		break;
	    default:
		SSD1322_Screen_State = SSD1322_SCREEN_TIME;
		break;
	    }
    }

//
void Button_pressed_set_down(void)
    {
	switch (SSD1322_Screen_State)
	    {
	    case SSD1322_SCREEN_TIME:
		break;
	    case SSD1322_SCREEN_RADIO:
		Save_Station_Freq_1();
		break;
	    case SSD1322_SCREEN_WakeUp:
		break;
	    case SSD1322_SCREEN_FFT:
		Change_FFT_source_Down();
		break;
	    case SSD1322_SCREEN_UVMETER:
		if (HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin) == GPIO_PIN_RESET)
		    {
			UV_meter_front_back = UV_METER_BACK;
		    }
		break;
	    case SSD1322_SCREEN_OFF:
		break;
	    case SSD1322_SCREEN_GoodBye:
		break;
	    case SSD1322_SCREEN_SETCLOCK:
		if (HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin) == GPIO_PIN_RESET)
		    {
			switch_change_time(Clock_Data_Time, 0);
		    }
		break;
	    case SSD1322_SCREEN_SETALARM:
		if (HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin) == GPIO_PIN_RESET)
		    {
			switch_change_alarm(AlarmDataChange, 0);
		    }
		break;
	    case SSD1322_SCREEN_SETTINGS:
		Change_Down_Settings();
		break;
	    case SSD1322_SCREEN_USB:
		break;
	    case SSD1322_SCREEN_SETINPUT:
		Change_Down_Input();
		break;
	    default:
		break;
	    }
    }

//
void Button_pressed_set_up(void)
    {
	switch (SSD1322_Screen_State)
	    {
	    case SSD1322_SCREEN_TIME:
		break;
	    case SSD1322_SCREEN_RADIO:
		Save_Station_Freq_2();
		break;
	    case SSD1322_SCREEN_WakeUp:
		break;
	    case SSD1322_SCREEN_FFT:
		Change_FFT_source_Up();
		break;
	    case SSD1322_SCREEN_UVMETER:
		if (HAL_GPIO_ReadPin(USER_BUTTON_4_GPIO_Port, USER_BUTTON_4_Pin) == GPIO_PIN_RESET)
		    {
			UV_meter_front_back = UV_METER_FRONT;
		    }
		break;
	    case SSD1322_SCREEN_OFF:
		break;
	    case SSD1322_SCREEN_GoodBye:
		break;
	    case SSD1322_SCREEN_SETCLOCK:
		Read_Set_TimeAndDate();
		break;
	    case SSD1322_SCREEN_SETALARM:
		if (HAL_GPIO_ReadPin(USER_BUTTON_4_GPIO_Port, USER_BUTTON_4_Pin) == GPIO_PIN_RESET)
		    {
			switch_change_alarm(AlarmDataChange, 1);
		    }
		break;
	    case SSD1322_SCREEN_SETTINGS:
		Change_Up_Settings();
		break;
	    case SSD1322_SCREEN_USB:
		break;
	    case SSD1322_SCREEN_SETINPUT:
		Change_Up_Input();
		break;
	    default:
		break;
	    }
    }



//--------------------------------------------------------------
/* Global Functions for handling buttons
 *
 * There are seven buttons from seven encoders
 * - Volume front
 * - Volume back
 * - Loudness
 * - Treble
 * - Middle
 * - Bass
 * - Radio Seek
*/
//--------------------------------------------------------------

//
void Encoder_button_volume_front(void)
    {
	save_current_display_state(SSD1322_SCREEN_ENCODER_VOLUME_FRONT);
	refresh_timer_state();


	switch (encoderVolFront.audioOutputState)
	    {
	    case MASTER:
		encoderVolFront.audioOutputState = MUTE;
		TDA7719_SetVolume_LeftFront (VOLUME_MUTE, 0);
		TDA7719_SetVolume_RightFront(VOLUME_MUTE, 0);
		TDA7719_SetVolume_LeftRear  (VOLUME_MUTE, 0);
		TDA7719_SetVolume_RightRear (VOLUME_MUTE, 0);
		HAL_TIM_Encoder_Stop_IT(&htim2, TIM_CHANNEL_ALL);
		break;
	    case MUTE:
		encoderVolFront.audioOutputState = MASTER_V2;
		TDA7719_SetVolume_LeftFront (tempVolFrontLeft, 0);
		TDA7719_SetVolume_RightFront(tempVolFrontRight, 0);
		TDA7719_SetVolume_LeftRear  (tempVolBackLeft, 0);
		TDA7719_SetVolume_RightRear (tempVolBackLeft, 0);
		TDA7719_SetVolume_Master(tempVolFrontLeft, tempVolFrontRight, tempVolBackLeft, tempVolBackLeft);
		HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_ALL);
//		HAL_TIM_OC_Start_IT(&htim15, TIM_CHANNEL_2);
		break;
	    case MASTER_V2:
		encoderVolFront.audioOutputState = NORMAL;
		TDA7719_SetVolume_Master(tempVolFrontLeft, tempVolFrontRight, tempVolBackLeft, tempVolBackRight);
//		HAL_TIM_OC_Stop_IT(&htim15, TIM_CHANNEL_2);	//zmienić na 5 - 10 sekund
		break;
	    case NORMAL:
		encoderVolFront.audioOutputState = ATTE_LEFT;
		break;
	    case ATTE_LEFT:
		encoderVolFront.audioOutputState = ATTE_RIGHT;
		break;
	    case ATTE_RIGHT:
		encoderVolFront.audioOutputState = MASTER;
		break;
	    default:
		encoderVolFront.audioOutputState = MASTER;
		break;
	    }

    }
//
void Encoder_button_volume_back(void)
    {
	switch (encoderVolBack.audioOutputState)
	    {
	    case NORMAL:
		encoderVolBack.audioOutputState = MUTE;
		TDA7719_SetVolume_LeftRear (VOLUME_MUTE, 0);
		TDA7719_SetVolume_RightRear(VOLUME_MUTE, 0);
		HAL_TIM_Encoder_Stop_IT(&htim8, TIM_CHANNEL_ALL);
		break;
	    case MUTE:
		encoderVolBack.audioOutputState = NORMAL_V2;
		TDA7719_SetVolume_LeftRear (tempVolBackLeft, 0);
		TDA7719_SetVolume_RightRear(tempVolBackRight, 0);
		HAL_TIM_Encoder_Start_IT(&htim8, TIM_CHANNEL_ALL);
		break;
	    case NORMAL_V2:
		encoderVolBack.audioOutputState = ATTE_LEFT;
		TDA7719_SetVolume_LeftRear (tempVolBackLeft, 0);
		TDA7719_SetVolume_RightRear(tempVolBackRight, 0);
		//HAL_TIM_OC_Start_IT(&htim16, TIM_CHANNEL_1);
		break;
	    case ATTE_LEFT:
		encoderVolBack.audioOutputState = ATTE_RIGHT;
		break;
	    case ATTE_RIGHT:
		encoderVolBack.audioOutputState = NORMAL;
		break;
	    default:
		encoderVolBack.audioOutputState = NORMAL;
		break;
	    }
    }
//
void Encoder_button_loudness(void)
    {
//	save_display_state(SSD1322_SCREEN_ENCODER_LOUDNESS);
//	TIM_state_Cos_tam();

	switch (encoderFilterLoudness.buttonControl)
	    {
	    case SET_GAIN:
		encoderFilterLoudness.buttonControl = SET_CENTER_FREQ;
		break;
	    case SET_CENTER_FREQ:
		encoderFilterLoudness.buttonControl = SET_SOFT_STEP;
		break;
	    case SET_SOFT_STEP:
		encoderFilterLoudness.buttonControl = SET_HIGH_BOOST;
		break;
	    case SET_HIGH_BOOST:
		encoderFilterLoudness.buttonControl = SET_GAIN;
		break;
	    default:
		encoderFilterLoudness.buttonControl = SET_GAIN;
		break;
	    }
    }
//
void Encoder_button_treble(void)
    {

    }
//
void Encoder_button_middle(void)
    {

    }
//
void Encoder_button_bass(void)
    {
	//	save_display_state(SSD1322_SCREEN_ENCODER_LOUDNESS);
	//	TIM_state_Cos_tam();

	switch (encoderFilterBass.buttonControl)
	    {
	    case SET_GAIN:
		encoderFilterBass.buttonControl = SET_CENTER_FREQ;
		break;
	    case SET_CENTER_FREQ:
		encoderFilterBass.buttonControl = SET_SOFT_STEP;
		break;
	    case SET_SOFT_STEP:
		encoderFilterBass.buttonControl = SET_GAIN;
		break;
	    default:
		encoderFilterBass.buttonControl = SET_GAIN;
		break;
	    }
    }
//
void Power_button_pressed(void)
    {
	//stopniowo zmniejszać głośność przez kilka sekund - opcja do włączenia w settings
	//w między czasie albo na końcu odpalić ekran pożegnalny

	/* routine after power button is pressed*/
	if(HAL_GPIO_ReadPin(POWER_BUTTON_GPIO_Port, POWER_BUTTON_Pin) == GPIO_PIN_RESET)
	    {
		/* if power button is pressed while device is ON*/
		if(POWER_device_state_flag == 1)
		    {
			//change global flag
			POWER_device_state_flag = 0;
			//turn off display
			SSD1322_API_sleep_on();
			//turn off radio
			RDA5807_PowerOff();
			//turn off preamp
			TDA7719_SetSoftMute(0, 1); //change to deinit TDA
			//change power led mode
			SettingsUserMenu.Power_LED = 1;
			//save to eeprom
			EEPROM_Save_FilterSettings(&encoderFilterTreble, &encoderFilterMiddle, &encoderFilterBass, &encoderFilterLoudness);
			EEPROM_Save_UserSetting(&savedUserSettings);
			EEPROM_Save_VolumeSettings(&encoderVolFront, &encoderVolBack);
			//turn off interupts from encoders and buttons (expect from power button)
//			__NVIC_DisableIRQ(IRQn);
			//turn off relay
			//HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
		    }

		else if(POWER_device_state_flag == 0) //schować w funkcje, która będzie wywoływana przy starcie systemu, albo przy starcie będzie inny startup routine
		    {
			POWER_device_state_flag = 1;
			//turn on display
			SSD1322_API_sleep_on();
			//turn on radio
			RDA5807_PowerOn();
			//RDA5807_Init();
			//turn off preamp
			//TDA7719_init();
			TDA7719_SetSoftMute(1, 1);
			//change power led mode
			SettingsUserMenu.Power_LED = 1;
			//read from eeprom settings
			EEPROM_Read_FilterSettings(&encoderFilterTreble, &encoderFilterMiddle, &encoderFilterBass, &encoderFilterLoudness);
			EEPROM_Read_UserSetting(&savedUserSettings);
			EEPROM_Read_VolumeSettings(&encoderVolFront, &encoderVolBack);
			//turn on interupts from encoders and buttons (expect from power button)
			//__NVIC_EnableIRQ(IRQn);
			//turn on relay
			//HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);

		    }
	    }

	/* routine after power button is realesed*/
	if(HAL_GPIO_ReadPin(POWER_BUTTON_GPIO_Port, POWER_BUTTON_Pin) == GPIO_PIN_SET)
	    {


	    }
    }


//--------------------------------------------------------------
// Static functions definictions
//--------------------------------------------------------------

//
static void Save_Station_Freq_2(void)
    {
	static uint32_t button_tim;
	static uint32_t button_tim1;

	if (HAL_GPIO_ReadPin(USER_BUTTON_4_GPIO_Port, USER_BUTTON_4_Pin) == GPIO_PIN_RESET)
	    {
		button_tim = HAL_GetTick();
	    }

	if (HAL_GPIO_ReadPin(USER_BUTTON_4_GPIO_Port, USER_BUTTON_4_Pin) == GPIO_PIN_SET)
	    {
		button_tim1 = HAL_GetTick();

		if (button_tim + 2000 <= button_tim1) //if holded for more than 2 secs
		    {
			savedUserSettings.stationSaved_2 = RDA5807_GetFrequency();
		    }

		if (button_tim + 2000 >= button_tim1) //if holed for less than 2 secs
		    {
			RDA5807_SetFrequency(savedUserSettings.stationSaved_2);
		    }
	    }
    }

//
static void Save_Station_Freq_1(void)
    {
	static uint32_t button_timer;
	static uint32_t button_timer1;

	if (HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin) == GPIO_PIN_RESET)
	    {
		button_timer = HAL_GetTick();
	    }

	if (HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin) == GPIO_PIN_SET)
	    {
		button_timer1 = HAL_GetTick();

		if (button_timer + 2000 <= button_timer1) //if holded for more than 2 secs
		    {
			savedUserSettings.stationSaved_1 = RDA5807_GetFrequency();
			//EEPROM_Save_UserSetting(&savedUserSettings);
		    }

		if (button_timer + 2000 >= button_timer1) //if holed for less than 2 secs
		    {
			RDA5807_SetFrequency(savedUserSettings.stationSaved_1);
		    }
	    }
    }

//
static void Change_selected_setting(void)
    {
	switch (SettingsUserMenu.SETTINGS_USER_MENU)
	    {
	    case REFRESH_SCREEN_TIME:
		SettingsUserMenu.SETTINGS_USER_MENU = ALARM_SOURCE_A;
		break;
	    case ALARM_SOURCE_A:
		SettingsUserMenu.SETTINGS_USER_MENU = ALARM_SOURCE_B;
		break;
	    case ALARM_SOURCE_B:
		SettingsUserMenu.SETTINGS_USER_MENU = USER_NAME;
		break;
	    case USER_NAME:
		SettingsUserMenu.SETTINGS_USER_MENU = DISPLAY_MODE_ON_OFF;
		settings_page = PAGE_SETTINGS_2;
		break;
	    case DISPLAY_MODE_ON_OFF:
		SettingsUserMenu.SETTINGS_USER_MENU = POWER_LED;
		break;
	    case POWER_LED:
		SettingsUserMenu.SETTINGS_USER_MENU = REFRESH_SCREEN_TIME;
		settings_page = PAGE_SETTINGS_1;
		break;
	    default:
		break;
	    }
    }

//
static void Change_Down_Settings(void)
    {
	if (HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin) == GPIO_PIN_RESET)
	    {
	switch (SettingsUserMenu.SETTINGS_USER_MENU)
	    {
	    case ALARM_SOURCE_A:
		SettingsUserMenu.AlarmSource_A ++;
		if(SettingsUserMenu.AlarmSource_A == 3) SettingsUserMenu.AlarmSource_A = RADIO;
		if(SettingsUserMenu.AlarmSource_A == 7) SettingsUserMenu.AlarmSource_A = JACK_1;
		break;
	    case ALARM_SOURCE_B:
		SettingsUserMenu.AlarmSource_B ++;
		if(SettingsUserMenu.AlarmSource_B == 3) SettingsUserMenu.AlarmSource_B = RADIO;
		if(SettingsUserMenu.AlarmSource_B == 7) SettingsUserMenu.AlarmSource_B = JACK_1;
		break;
	    case REFRESH_SCREEN_TIME:
		{
		    if (SettingsUserMenu.Display_mode == Disp_normal)
			{
			    SettingsUserMenu.RefreshScreenTime = 65535;
			    break;
			}
		    refresh_time--;
		    if(refresh_time == 0) refresh_time = 5;
		    SettingsUserMenu.RefreshScreenTime = refresh_time_values[refresh_time];
		    set_change_time_of_display(SettingsUserMenu.RefreshScreenTime);
		}
		break;
	    case USER_NAME:
		{
		    inc++;
		    if(inc == 10) inc = 0;
		}
		break;
	    case DISPLAY_MODE_ON_OFF:
		{
		    //go to sleep mode after no actions is taken after 3 sec
		    SettingsUserMenu.Display_mode++;
		    if(SettingsUserMenu.Display_mode == 5) SettingsUserMenu.Display_mode = 1;
		    if(SettingsUserMenu.Display_mode == Disp_normal) SettingsUserMenu.RefreshScreenTime = 65535;
		    if(SettingsUserMenu.Display_mode == Disp_standby)
			{
			    is_display_on_standby_flag = true;
			    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
			    saved_seconds = sTime.Seconds;
			    saved_minutes = sTime.Minutes;
			}
		    else
			{
			    is_display_on_standby_flag = false;
			}
		}
		break;
	    case POWER_LED:
		{
		    SettingsUserMenu.Power_LED++;
		    if(SettingsUserMenu.Power_LED == 6) SettingsUserMenu.Power_LED = 1;
		}
		break;
	    default:
		break;
	    }
	    }
    }

//
static void Change_Up_Settings(void)
    {
	if (HAL_GPIO_ReadPin(USER_BUTTON_4_GPIO_Port, USER_BUTTON_4_Pin) == GPIO_PIN_RESET)
	    {
		switch (SettingsUserMenu.SETTINGS_USER_MENU)
		    {
		    case ALARM_SOURCE_A:
			SettingsUserMenu.AlarmSource_A--;
			if (SettingsUserMenu.AlarmSource_A == 4)
			    SettingsUserMenu.AlarmSource_A = JACK_2;
			if (SettingsUserMenu.AlarmSource_A == 255)
			    SettingsUserMenu.AlarmSource_A = MICROPHONE;
			break;
		    case ALARM_SOURCE_B:
			SettingsUserMenu.AlarmSource_B--;
			if (SettingsUserMenu.AlarmSource_B == 4)
			    SettingsUserMenu.AlarmSource_B = JACK_2;
			if (SettingsUserMenu.AlarmSource_B == 255)
			    SettingsUserMenu.AlarmSource_B = MICROPHONE;
			break;
		    case REFRESH_SCREEN_TIME:
			{
			    if (SettingsUserMenu.Display_mode == Disp_normal)
				{
				    SettingsUserMenu.RefreshScreenTime = 65535;
				    break;
				}
			    refresh_time++;
			    if (refresh_time == 6)refresh_time = 1;
			    SettingsUserMenu.RefreshScreenTime = refresh_time_values[refresh_time];
			    set_change_time_of_display(SettingsUserMenu.RefreshScreenTime);
			}
			break;
		    case USER_NAME:
			    letter++;
			    if(letter == '{') letter = 'a';
			    user_name[inc] = letter;
			break;
		    case DISPLAY_MODE_ON_OFF:
			    SettingsUserMenu.Display_mode--;
			    if(SettingsUserMenu.Display_mode == 0) SettingsUserMenu.Display_mode = 4;
			    if(SettingsUserMenu.Display_mode == Disp_normal) SettingsUserMenu.RefreshScreenTime = 65535;
			    if(SettingsUserMenu.Display_mode == Disp_standby)
				{
				    is_display_on_standby_flag = true;
				    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
				    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
				    saved_seconds = sTime.Seconds;
				    saved_minutes = sTime.Minutes;
				}
			    else
				{
				    is_display_on_standby_flag = false;
				}
			break;
		    case POWER_LED:
			    SettingsUserMenu.Power_LED--;
			    if(SettingsUserMenu.Power_LED == 0) SettingsUserMenu.Power_LED = 5;
			break;
		    default:
			break;
		    }
	    }
    }

//
static void Change_Down_Input(void)
    {
	if (HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin) == GPIO_PIN_RESET)
	    {
		switch (TDA7719_config.set_input_front)
		    {
		    case BLUETOOTH:
			TDA7719_config.set_input_front = JACK_1;
			break;
		    case JACK_1:
			TDA7719_config.set_input_front = JACK_2;
			break;
		    case JACK_2:
			TDA7719_config.set_input_front = RADIO;
			RADIO_IS_ON_front_flag = true;
			if (RADIO_IS_ON_front_flag == true && RADIO_IS_ON_back_flag == false)
			    {
				RDA5807_Init();
				RDA5807_PowerOn();
				RDA5807_SetFrequency(savedUserSettings.radio_freq);
				break;
			    }
			else
			    break;
		    case RADIO:
			TDA7719_config.set_input_front = MICROPHONE;
			RADIO_IS_ON_front_flag = false;
			if (RADIO_IS_ON_front_flag == false && RADIO_IS_ON_back_flag == false)
			    {
				RDA5807_PowerOff();
			    }
			break;
		    case MICROPHONE:
			TDA7719_config.set_input_front = BLUETOOTH;
			break;
		    default:
			break;
		    }
		TDA7719_SetMainInput(TDA7719_config.set_input_front);
	    }
    }

//
static void Change_Up_Input(void)
    {
	if (HAL_GPIO_ReadPin(USER_BUTTON_4_GPIO_Port, USER_BUTTON_4_Pin) == GPIO_PIN_RESET)
	    {
		switch (TDA7719_config.set_input_back)
		    {
		    case BLUETOOTH:
			TDA7719_config.set_input_back = JACK_1;
			break;
		    case JACK_1:
			TDA7719_config.set_input_back = JACK_2;
			break;
		    case JACK_2:
			TDA7719_config.set_input_back = RADIO;
			RADIO_IS_ON_back_flag = true;
			if (RADIO_IS_ON_back_flag == true && RADIO_IS_ON_front_flag == false)
			    {
				RDA5807_Init();
				RDA5807_PowerOn();
				RDA5807_SetFrequency(savedUserSettings.radio_freq);
				break;
			    }
			break;
		    case RADIO:
			TDA7719_config.set_input_back = MICROPHONE;
			RADIO_IS_ON_back_flag = false;
			if (RADIO_IS_ON_back_flag == false && RADIO_IS_ON_front_flag == false)
			    {
				RDA5807_PowerOff();
			    }
			break;
		    case MICROPHONE:
			TDA7719_config.set_input_back = BLUETOOTH;
			break;
		    default:
			break;
		    }
		TDA7719_SetSecondInput(TDA7719_config.set_input_back);
	    }
    }

//
static void Change_FFT_source_Up(void)
    {
	if (HAL_GPIO_ReadPin(USER_BUTTON_4_GPIO_Port, USER_BUTTON_4_Pin) == GPIO_PIN_RESET)
	    {
		FFT_channel_source--;
		if (FFT_channel_source == FFT_ch_src_enum_MIN)
		    {
			FFT_channel_source = FFT_back_right;
		    }
	    }
    }

//
static void Change_FFT_source_Down(void)
    {
	if (HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin) == GPIO_PIN_RESET)
	    {
		FFT_channel_source++;
		if (FFT_channel_source >= FFT_ch_src_enum_MAX)
		    {
			FFT_channel_source = FFT_front_left;
		    }
	    }
    }

//
static void Read_Set_TimeAndDate(void)
    {
	if (HAL_GPIO_ReadPin(USER_BUTTON_4_GPIO_Port, USER_BUTTON_4_Pin) == GPIO_PIN_RESET)
	    {
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
		switch_change_time(Clock_Data_Time, 1);
		HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	    }
    }


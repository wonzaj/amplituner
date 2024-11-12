//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "app_buttons.h"
//--------------------------------------------------------------
// Defines
//--------------------------------------------------------------

//--------------------------------------------------------------
// Static variables
//--------------------------------------------------------------
static uint8_t refresh_time = 1;
static char letter = 'a';
static uint8_t inc = 0; //97 - 'a'

//--------------------------------------------------------------
// typedefs
//--------------------------------------------------------------
FFT_channel_source_e FFT_channel_source = 1;
SettingsUserMenu_t SettingsUserMenu;
extern Clock_Data_Change_t Clock_Data_Time;
extern AlarmDataChange_t AlarmDataChange;

extern TDA7719_config_t TDA7719_config;
extern savedUserSettings_t savedUserSettings;
extern encoderFilter_t encoderFilterTreble;
extern encoderFilter_t encoderFilterMiddle;
extern encoderFilter_t encoderFilterBass;
extern encoderFilter_t encoderFilterLoudness;
extern encoder_t encoderVolFront;
extern encoder_t encoderVolBack;

extern uint8_t volumeMasterFlag;
extern int16_t tempVolFrontLeft;
extern int16_t tempVolFrontRight;
extern int16_t tempVolBackLeft;
extern int16_t tempVolBackRight;

extern uint16_t ADC_SamplesTEST[4];
extern uint16_t gGPIO_Pin;
extern _Bool volatile button_debou_state;
//extern ADC_HandleTypeDef 	 hadc1;
//extern ADC_HandleTypeDef 	 hadc2;
//extern DMA_HandleTypeDef 	 hdma_adc1;
//extern TIM_HandleTypeDef 	 htim6;

extern volatile uint8_t ADC_IS_ON_flag;
extern volatile uint8_t RADIO_IS_ON_back_flag;
extern volatile uint8_t RADIO_IS_ON_front_flag;

extern const uint16_t refresh_time_values[7];
extern uint8_t settings_page;
extern uint8_t saved_seconds;
extern uint8_t saved_minutes;
extern char user_name[10];
extern volatile uint8_t POWER_device_state_flag;

extern uint8_t UV_meter_front_back;
extern uint8_t UV_meter_numb_of_chan;

extern uint32_t SysTick_MiliSeconds;
extern uint32_t SysTick_Seconds;

volatile uint8_t POWER_device_state_flag = 0;
volatile uint8_t ADC_IS_ON_flag = 0;
volatile uint8_t RADIO_IS_ON_front_flag;
volatile uint8_t RADIO_IS_ON_back_flag;
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
/* Global Functions for handling buttons
 *
 * There are four buttons:
 * - Change screen
 * - Accept
 * - Set value up
 * - Set value down
 */
//--------------------------------------------------------------
//Power button
void Buttons_PowerButton_Pressed(void)
{
	//stopniowo zmniejszać głośność przez kilka sekund - opcja do włączenia w settings
	//w między czasie albo na końcu odpalić ekran pożegnalny

	/* if power button is pressed while device is ON*/
	if (POWER_device_state_flag == 1)
	{
		//change global flag
		POWER_device_state_flag = 0;
		//turn off display
		SSD1322_API_Sleep_On();
		//turn off radio
		RDA5807_PowerOff();
		//turn off preamp
		TDA7719_SetSoftMute(0, 1);     //change to deinit TDA
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

	else if (POWER_device_state_flag == 0) //schować w funkcje, która będzie wywoływana przy starcie systemu, albo przy starcie będzie inny startup routine
	{
		POWER_device_state_flag = 1;
		//turn on display
		SSD1322_API_Sleep_On();
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

void Buttons_PowerButton_Released(void)
{

}

GPIO_PinState Buttons_PowerButton_GetState(void)
{
	return HAL_GPIO_ReadPin(POWER_BUTTON_GPIO_Port, POWER_BUTTON_Pin);
}

//User button 1
void Buttons_UserButton1_Pressed(void)
{
	ScreenState_t Screen_State_temp = AppDisplay_GetDisplayState();

	if ((Screen_State_temp + 1) == ENUM_MAX_USER_DISPLAY)
	{
		AppDisplay_SetDisplayState(SCREEN_TIME);
	}
	else
	{
		AppDisplay_SetDisplayState(Screen_State_temp + 1);
	}

	if (ADC_IS_ON_flag == true)
	{
		//HAL_TIM_Base_Stop(&htim6);
		//HAL_ADC_Stop_DMA(&hadc2);
		ADC_IS_ON_flag = false;
	}

	switch (Screen_State_temp)
	{
	case SCREEN_TIME:

		break;
	case SCREEN_RADIO:

		break;
	case SCREEN_FFT:
		//HAL_TIM_Base_Start(&htim6);
		//HAL_ADC_Start_DMA(&hadc2, (uint32_t *)ADC_SamplesTEST, UV_meter_numb_of_chan);
		ADC_IS_ON_flag = true;
		break;
	case SCREEN_UVMETER:
		//HAL_TIM_Base_Start(&htim6);
		//HAL_ADC_Start_DMA(&hadc2, (uint32_t *)ADC_SamplesTEST, UV_meter_numb_of_chan);
		ADC_IS_ON_flag = true;
		//set flag indicating that adc is on
		break;
	case SCREEN_SETCLOCK:

		break;
	case SCREEN_SETALARM:

		break;
	case SCREEN_SETINPUT:

		break;
	case SCREEN_SETTINGS:

		break;
	case SCREEN_USB:

		break;
	case SCREEN_TIME_BOUNCING:

		break;
	default:
		AppDisplay_SetDisplayState(SCREEN_TIME);
		break;
	}
}

void Buttons_UserButton1_Released(void)
{

}

GPIO_PinState Buttons_UserButton1_GetState(void)
{
	return HAL_GPIO_ReadPin(USER_BUTTON_1_GPIO_Port, USER_BUTTON_1_Pin);
}

//User button 2
void Buttons_UserButton2_Pressed(void)
{
	ScreenState_t SSD1322_Screen_State_temp =
			AppDisplay_GetDisplayState();

	switch (SSD1322_Screen_State_temp)
	{
	case SCREEN_TIME:
		break;
	case SCREEN_RADIO:
		RDA5807_SeekUp();
		break;
	case SCREEN_WAKEUP:
		//po wcisnięciu budzika można ustawić czy się wyłącza muzyka
		break;
	case SCREEN_FFT:
		break;
	case SCREEN_UVMETER:
		break;
	case SCREEN_OFF:
		break;
	case SCREEN_GOODBYTE:
		break;
	case SCREEN_SETCLOCK:
		Clock_Data_Time++;
		if (9 == Clock_Data_Time)
		{
			Clock_Data_Time = 1;
		}
		break;
	case SCREEN_SETALARM:
		AlarmDataChange++;
		if (7 == AlarmDataChange)
		{
			AlarmDataChange = 1;
		}
		break;
	case SCREEN_SETTINGS:
		Change_selected_setting();
		break;
	case SCREEN_USB:
		//tutaj akcept wejście w folder albo otworzenie pliku
		//jeżeli brak podpiętego USB to komunikat żeby podłączyć z formatowanie FatFS
		break;
	case SCREEN_SETINPUT:

		break;
	default:
		AppDisplay_SetDisplayState(SCREEN_TIME);
		break;
	}
}

void Buttons_UserButton2_Released(void)
{

}

GPIO_PinState Buttons_UserButton2_GetState(void)
{
	return HAL_GPIO_ReadPin(USER_BUTTON_2_GPIO_Port, USER_BUTTON_2_Pin);
}

//User button 3
void Buttons_UserButton3_Pressed(void)
{
	ScreenState_t SSD1322_Screen_State_temp =
			AppDisplay_GetDisplayState();

	switch (SSD1322_Screen_State_temp)
	{
	case SCREEN_TIME:
		break;
	case SCREEN_RADIO:
		Save_Station_Freq_1(); // zmienić nazwe, wrzucić do odpowiedniego modulu i tutaj do przeniesienia do released button
		break;
	case SCREEN_WAKEUP:
		break;
	case SCREEN_FFT:
		Change_FFT_source_Down();
		break;
	case SCREEN_UVMETER:
		UV_meter_front_back = UV_METER_BACK; // zmienić nazwe, wrzucić do odpowiedniego modulu wrzucić do funkcji do odpowiedniego modułu
		break;
	case SCREEN_OFF:
		break;
	case SCREEN_GOODBYTE:
		break;
	case SCREEN_SETCLOCK:
		switch_change_time(Clock_Data_Time, 0); //zmienic nazwe i wrzucic do odpowiedniego modułu
		break;
	case SCREEN_SETALARM:
		switch_change_alarm(AlarmDataChange, 0); //zmienic nazwe i wrzucic do odpowiedniego modułu
		break;
	case SCREEN_SETTINGS:
		Change_Down_Settings(); //zmienic nazwe i wrzucic do odpowiedniego modułu
		break;
	case SCREEN_USB:
		break;
	case SCREEN_SETINPUT:
		Change_Down_Input();   //zmienic nazwe i wrzucic do odpowiedniego modułu
		break;
	default:
		break;
	}
}

void Buttons_UserButton3_Released(void)
{

}

GPIO_PinState Buttons_UserButton3_GetState(void)
{
	return HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin);
}

//User button 4
void Buttons_UserButton4_Pressed(void)
{
	ScreenState_t SSD1322_Screen_State_temp =
			AppDisplay_GetDisplayState();

	switch (SSD1322_Screen_State_temp)
	{
	case SCREEN_TIME:
		break;
	case SCREEN_RADIO:
		Save_Station_Freq_2(); // zmienić nazwe, wrzucić do odpowiedniego modulu i tutaj do przeniesienia do released button
		break;
	case SCREEN_WAKEUP:
		break;
	case SCREEN_FFT:
		Change_FFT_source_Up();	// zmienić nazwe, wrzucić do odpowiedniego modulu
		break;
	case SCREEN_UVMETER:
		UV_meter_front_back = UV_METER_FRONT;// zmienić nazwe, wrzucić do odpowiedniego modulu wrzucić do funkcji do odpowiedniego modułu
		break;
	case SCREEN_OFF:
		break;
	case SCREEN_GOODBYTE:
		break;
	case SCREEN_SETCLOCK:
		Read_Set_TimeAndDate();	//zmienic nazwe i wrzucic do odpowiedniego modułus
		break;
	case SCREEN_SETALARM:
		switch_change_alarm(AlarmDataChange, 1);//zmienic nazwe i wrzucic do odpowiedniego modułu
		break;
	case SCREEN_SETTINGS:
		Change_Up_Settings();//zmienic nazwe i wrzucic do odpowiedniego modułu
		break;
	case SCREEN_USB:
		break;
	case SCREEN_SETINPUT:
		Change_Up_Input();	//zmienic nazwe i wrzucic do odpowiedniego modułu
		break;
	default:
		break;
	}
}

void Buttons_UserButton4_Released(void)
{

}

GPIO_PinState Buttons_UserButton4_GetState(void)
{
	return HAL_GPIO_ReadPin(USER_BUTTON_4_GPIO_Port, USER_BUTTON_4_Pin);
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
// Encoder volume front
void Buttons_EncoderVolFrontButton_Pressed(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_VOLUME_FRONT);
	//HAL_Timers_RefreshTimer(&htim14, TIM_CHANNEL_1);

	switch (encoderVolFront.audioOutputState)
	{
	case MASTER:
		encoderVolFront.audioOutputState = MUTE;
		TDA7719_SetVolume_LeftFront(VOLUME_MUTE, 0);
		TDA7719_SetVolume_RightFront(VOLUME_MUTE, 0);
		TDA7719_SetVolume_LeftRear(VOLUME_MUTE, 0);
		TDA7719_SetVolume_RightRear(VOLUME_MUTE, 0);
		AppEncoders_SingleEncoderStop(ENCODER_VOL_FRONT);
		break;
	case MUTE:
		encoderVolFront.audioOutputState = MASTER_V2;
		TDA7719_SetVolume_LeftFront(tempVolFrontLeft, 0);
		TDA7719_SetVolume_RightFront(tempVolFrontRight, 0);
		TDA7719_SetVolume_LeftRear(tempVolBackLeft, 0);
		TDA7719_SetVolume_RightRear(tempVolBackLeft, 0);
		TDA7719_SetVolume_Master(tempVolFrontLeft, tempVolFrontRight,
				tempVolBackLeft, tempVolBackLeft);
		AppEncoders_SingleEncoderStart(ENCODER_VOL_FRONT);
		//HAL_TIM_OC_Start_IT(&htim13, TIM_CHANNEL_1);
		break;
	case MASTER_V2:
		encoderVolFront.audioOutputState = NORMAL;
		TDA7719_SetVolume_Master(tempVolFrontLeft, tempVolFrontRight, tempVolBackLeft, tempVolBackRight);
		//HAL_TIM_OC_Stop_IT(&htim13, TIM_CHANNEL_1);	//zmienić na 5 - 10 sekund
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

void Buttons_EncoderVolFrontButton_Released(void)
{

}

GPIO_PinState Buttons_EncoderVolFrontButton_GetState(void)
{
	return HAL_GPIO_ReadPin(ENCODER_BUTTON_VOL_FRONT_GPIO_Port,
			ENCODER_BUTTON_VOL_FRONT_Pin);
}

// Encoder volume back
void Buttons_EncoderVolBackButton_Pressed(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_VOLUME_BACK);
	//HAL_Timers_RefreshTimer(&htim14, TIM_CHANNEL_1);

	switch (encoderVolBack.audioOutputState)
	{
	case NORMAL:
		encoderVolBack.audioOutputState = MUTE;
		TDA7719_SetVolume_LeftRear(VOLUME_MUTE, 0);
		TDA7719_SetVolume_RightRear(VOLUME_MUTE, 0);
		//HAL_TIM_Encoder_Stop_IT(&htim8, TIM_CHANNEL_ALL);
		break;
	case MUTE:
		encoderVolBack.audioOutputState = NORMAL_V2;
		TDA7719_SetVolume_LeftRear(tempVolBackLeft, 0);
		TDA7719_SetVolume_RightRear(tempVolBackRight, 0);
		//HAL_TIM_Encoder_Start_IT(&htim8, TIM_CHANNEL_ALL);
		break;
	case NORMAL_V2:
		encoderVolBack.audioOutputState = ATTE_LEFT;
		TDA7719_SetVolume_LeftRear(tempVolBackLeft, 0);
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

void Buttons_EncoderVolBackButton_Released(void)
{

}

GPIO_PinState Buttons_EncoderVolBackButton_GetState(void)
{
	return HAL_GPIO_ReadPin(ENCODER_BUTTON_VOL_BACK_GPIO_Port,
			ENCODER_BUTTON_VOL_BACK_Pin);
}

// Encoder volume treble
void Buttons_EncoderTrebleButton_Pressed(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_TREBLE);
	//HAL_Timers_RefreshTimer(&htim14, TIM_CHANNEL_1);

	switch (encoderFilterTreble.buttonControl)
	{
	case SET_GAIN:
		encoderFilterTreble.buttonControl = SET_CENTER_FREQ;
		break;
	case SET_CENTER_FREQ:
		encoderFilterTreble.buttonControl = SET_SOFT_STEP;
		break;
	case SET_SOFT_STEP:
		encoderFilterTreble.buttonControl = SET_HIGH_BOOST;
		break;
	case SET_HIGH_BOOST:
		encoderFilterTreble.buttonControl = SET_GAIN;
		break;
	default:
		encoderFilterTreble.buttonControl = SET_GAIN;
		break;
	}
}

void Buttons_EncoderTrebleButton_Released(void)
{

}

GPIO_PinState Buttons_EncoderTrebleButton_GetState(void)
{
	return HAL_GPIO_ReadPin(ENCODER_BUTTON_TREBLE_GPIO_Port,
			ENCODER_BUTTON_TREBLE_Pin);
}

// Encoder volume bass
void Buttons_EncoderBassButton_Pressed(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_BASS);
	//HAL_Timers_RefreshTimer(&htim14, TIM_CHANNEL_1);

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

void Buttons_EncoderBassButton_Released(void)
{

}

GPIO_PinState Buttons_EncoderBassButton_GetState(void)
{
	return HAL_GPIO_ReadPin(ENCODER_BUTTON_BASS_GPIO_Port,
			ENCODER_BUTTON_BASS_Pin);
}

// Encoder volume middle
void Buttons_EncoderMiddleButton_Pressed(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_MIDDLE);
	//HAL_Timers_RefreshTimer(&htim14, TIM_CHANNEL_1);

	switch (encoderFilterMiddle.buttonControl)
	{
	case SET_GAIN:
		encoderFilterMiddle.buttonControl = SET_CENTER_FREQ;
		break;
	case SET_CENTER_FREQ:
		encoderFilterMiddle.buttonControl = SET_SOFT_STEP;
		break;
	case SET_SOFT_STEP:
		encoderFilterMiddle.buttonControl = SET_HIGH_BOOST;
		break;
	case SET_HIGH_BOOST:
		encoderFilterMiddle.buttonControl = SET_GAIN;
		break;
	default:
		encoderFilterMiddle.buttonControl = SET_GAIN;
		break;
	}
}

void Buttons_EncoderMiddleButton_Released(void)
{

}

GPIO_PinState Buttons_EncoderMiddleButton_GetState(void)
{
	return HAL_GPIO_ReadPin(ENCODER_BUTTON_MIDDLE_GPIO_Port,
			ENCODER_BUTTON_MIDDLE_Pin);
}

// Encoder volume radio
void Buttons_EncoderRadioButton_Pressed(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_RADIO);
	//HAL_Timers_RefreshTimer(&htim14, TIM_CHANNEL_1);
}

void Buttons_EncoderRadioButton_Released(void)
{

}

GPIO_PinState Buttons_EncoderRadioButton_GetState(void)
{
	return HAL_GPIO_ReadPin(ENCODER_BUTTON_RADIO_GPIO_Port,
			ENCODER_BUTTON_RADIO_Pin);
}

// Encoder volume loudness
void Buttons_EncoderLoudnessButton_Pressed(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_LOUDNESS);
	//HAL_Timers_RefreshTimer(&htim14, TIM_CHANNEL_1);

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

void Buttons_EncoderLoudnessButton_Released(void)
{

}

GPIO_PinState Buttons_EncoderLoudnessButton_GetState(void)
{
	return HAL_GPIO_ReadPin(ENCODER_BUTTON_LOUD_GPIO_Port,
			ENCODER_BUTTON_LOUD_Pin);
}

// Volume master takes into account attenuator of each channel and front and back volume aswell
void TDA7719_SetVolume_Master(const int16_t VolFrontLeft,
		const int16_t VolFrontRight, const int16_t VolBackLeft,
		const int16_t VolBackRight)
{
	//write to diffrent TDA7719 register depending on value
	if ((encoderVolFront.volumeMaster <= 94)
			&& (encoderVolFront.volumeMaster >= 80))
	{
		TDA7719_SetVolume((encoderVolFront.volumeMaster) - 79, 0, 0);
	}
	else if ((encoderVolFront.volumeMaster >= 0)
			&& (encoderVolFront.volumeMaster <= 79))
	{
		TDA7719_SetVolume_LeftFront((VolFrontLeft), 0);
		TDA7719_SetVolume_RightFront((VolFrontRight), 0);
		TDA7719_SetVolume_LeftRear((VolBackLeft), 0);
		TDA7719_SetVolume_RightRear((VolBackRight), 0);
	}
}

// Sets volume just for both front channels
void TDA7719_SetVolumeFront_LeftRight(const int16_t VolFrontLeft,
		const int16_t VolFrontRight)
{

	if (VolFrontLeft <= -79)
		TDA7719_SetVolume_LeftFront(VOLUME_MUTE, 0); //0 - mute, 79 - max_volume
	else
		TDA7719_SetVolume_LeftFront(VolFrontLeft, 0);

	if (VolFrontRight <= -79)
		TDA7719_SetVolume_RightFront(VOLUME_MUTE, 0); //0 - mute, 79 - max_volume
	else
		TDA7719_SetVolume_RightFront(VolFrontRight, 0);

}

// Checks if given value (volume) is given range
// It also increments or decrements value depending on CNT register upgraded by volume front encoder
void Check_Volume_Range_Front(volatile int8_t *const volume,
		const uint8_t maxVolume)
{
	static int16_t TimerDiff1;
	static uint16_t LastTimerCounter1;

	//TimerDiff1 = htim5.Instance->CNT - LastTimerCounter1;
	if (TimerDiff1 >= 4 || TimerDiff1 <= -4)
	{
		TimerDiff1 /= 4;
		(*volume) += (int8_t) TimerDiff1;
		if ((*volume) > maxVolume)
			(*volume) = maxVolume;
		if ((*volume) < 0)
			(*volume) = 0;
		//LastTimerCounter1 = htim5.Instance->CNT;
	}
}

// Sets volume just for both back channels
void TDA7719_SetVolumeBack_LeftRight(const int16_t VolBackLeft,
		const int16_t VolBackRight)
{
	if (VolBackLeft <= -79)
		TDA7719_SetVolume_LeftRear(VOLUME_MUTE, 0); //0 - mute, 79 - max_volume
	else
		TDA7719_SetVolume_LeftRear(VolBackLeft, 0);

	if (VolBackRight <= -79)
		TDA7719_SetVolume_RightRear(VOLUME_MUTE, 0); //0 - mute, 79 - max_volume
	else
		TDA7719_SetVolume_RightRear(VolBackRight, 0);

}

// Checks if given value (volume) is given range
// It also increments or decrements value depending on CNT register upgraded by volume back encoder
void Check_Volume_Range_Back(volatile int8_t *const volume,
		const uint8_t maxVolume)
{
	static int16_t TimerDiff2;
	static uint16_t LastTimerCounter2;

	//TimerDiff2 = htim3.Instance->CNT - LastTimerCounter2;
	if (TimerDiff2 >= 4 || TimerDiff2 <= -4)
	{
		TimerDiff2 /= 4;
		(*volume) += (int8_t) TimerDiff2;
		if ((*volume) > maxVolume)
			(*volume) = maxVolume;
		if ((*volume) < 0)
			(*volume) = 0;
		//LastTimerCounter2 = htim3.Instance->CNT;
	}
}

// Checks if given value (loudness attenuator, center freqency, soft step, high boost) is given range
// It also increments or decrements value depending on CNT register upgraded by loudness encoder
void Check_Loudness_Param_Range(volatile int8_t *const gain,
		const uint8_t maxGain)
{
	static int16_t TimerDiff3;
	static uint16_t LastTimerCounter3;

	//TimerDiff3 = htim8.Instance->CNT - LastTimerCounter3;
	if (TimerDiff3 >= 4 || TimerDiff3 <= -4)
	{
		TimerDiff3 /= 4;
		(*gain) += (int8_t) TimerDiff3;
		if ((*gain) >= maxGain)
			(*gain) = maxGain;
		if ((*gain) <= 0)
			(*gain) = 0;
		//LastTimerCounter3 = htim8.Instance->CNT;
	}
}

// Checks if given value (loudness, Bass Q Factor, soft step) is given range
// It also increments or decrements value depending on CNT register upgraded by bass encoder
void Check_Bass_Param_Range(volatile int8_t *const gain, const uint8_t maxGain)
{
	static int16_t TimerDiff3;
	static uint16_t LastTimerCounter3;

	//TimerDiff3 = htim2.Instance->CNT - LastTimerCounter3;
	if (TimerDiff3 >= 4 || TimerDiff3 <= -4)
	{
		TimerDiff3 /= 4;
		(*gain) += (int8_t) TimerDiff3;
		if ((*gain) >= maxGain)
			(*gain) = maxGain;
		if ((*gain) <= 0)
			(*gain) = 0;
		//LastTimerCounter3 = htim2.Instance->CNT;
	}
}

// Checks if given value (loudness, middle Q Factor, soft step) is given range
// It also increments or decrements value depending on CNT register upgraded by middle encoder
void Check_Middle_Param_Range(volatile int8_t *const gain,
		const uint8_t maxGain)
{
	static int16_t TimerDiff3;
	static uint16_t LastTimerCounter3;

	TimerDiff3 = HAL_Encoders_Middle_GetRotateValue() - LastTimerCounter3;
	//if(TimerDiff3 >= 4 || TimerDiff3 <= -4)
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
void Check_Treble_Param_Range(volatile int8_t *const gain,
		const uint8_t maxGain)
{
	static int16_t TimerDiff3;
	static uint16_t LastTimerCounter3;

	//TimerDiff3 = htim1.Instance->CNT - LastTimerCounter3;
	if (TimerDiff3 >= 4 || TimerDiff3 <= -4)
	{
		TimerDiff3 /= 4;
		(*gain) += (int8_t) TimerDiff3;
		if ((*gain) >= maxGain)
			(*gain) = maxGain;
		if ((*gain) <= 0)
			(*gain) = 0;
		//LastTimerCounter3 = htim1.Instance->CNT;
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

	if (HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin)
			== GPIO_PIN_RESET)
	{
		button_timer = HAL_GetTick();
	}

	if (HAL_GPIO_ReadPin(USER_BUTTON_3_GPIO_Port, USER_BUTTON_3_Pin)
			== GPIO_PIN_SET)
	{
		button_timer1 = HAL_GetTick();

		if (button_timer + 2000 <= button_timer1) //if holded for more than 2 secs
		{
			savedUserSettings.stationSaved_1 = RDA5807_GetFrequency();
			EEPROM_Save_UserSetting(&savedUserSettings);
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
			SettingsUserMenu.AlarmSource_A++;
			if (SettingsUserMenu.AlarmSource_A == 3)
				SettingsUserMenu.AlarmSource_A = RADIO;
			if (SettingsUserMenu.AlarmSource_A == 7)
				SettingsUserMenu.AlarmSource_A = JACK_1;
			break;
		case ALARM_SOURCE_B:
			SettingsUserMenu.AlarmSource_B++;
			if (SettingsUserMenu.AlarmSource_B == 3)
				SettingsUserMenu.AlarmSource_B = RADIO;
			if (SettingsUserMenu.AlarmSource_B == 7)
				SettingsUserMenu.AlarmSource_B = JACK_1;
			break;
		case REFRESH_SCREEN_TIME:
		{
			if (SettingsUserMenu.Display_mode == DISPLAY_NORMAL_MODE)
			{
				SettingsUserMenu.RefreshScreenTime = 65535;
				break;
			}
			refresh_time--;
			if (refresh_time == 0)
				refresh_time = 5;
			SettingsUserMenu.RefreshScreenTime =
					refresh_time_values[refresh_time];
			//set_change_time_of_display(SettingsUserMenu.RefreshScreenTime);
		}
			break;
		case USER_NAME:
		{
			inc++;
			if (inc == 10)
				inc = 0;
		}
			break;
		case DISPLAY_MODE_ON_OFF:
		{
			//go to sleep mode after no actions is taken after 3 sec
			SettingsUserMenu.Display_mode++;
			if (SettingsUserMenu.Display_mode == 5)
				SettingsUserMenu.Display_mode = 1;
			if (SettingsUserMenu.Display_mode == DISPLAY_NORMAL_MODE)
				SettingsUserMenu.RefreshScreenTime = 65535;
			if (SettingsUserMenu.Display_mode == DISPLAY_STANDBY)
			{
				Display_Controls.OnStandbyMode_flag = true;
				HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
				HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
				saved_seconds = sTime.Seconds;
				saved_minutes = sTime.Minutes;
			}
			else
			{
				Display_Controls.OnStandbyMode_flag = false;
			}
		}
			break;
		case POWER_LED:
		{
			SettingsUserMenu.Power_LED++;
			if (SettingsUserMenu.Power_LED == 6)
				SettingsUserMenu.Power_LED = 1;
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
				SettingsUserMenu.AlarmSource_A = MICROPHONE_USB;
			break;
		case ALARM_SOURCE_B:
			SettingsUserMenu.AlarmSource_B--;
			if (SettingsUserMenu.AlarmSource_B == 4)
				SettingsUserMenu.AlarmSource_B = JACK_2;
			if (SettingsUserMenu.AlarmSource_B == 255)
				SettingsUserMenu.AlarmSource_B = MICROPHONE_USB;
			break;
		case REFRESH_SCREEN_TIME:
		{
			if (SettingsUserMenu.Display_mode == DISPLAY_NORMAL_MODE)
			{
				SettingsUserMenu.RefreshScreenTime = 65535;
				break;
			}
			refresh_time++;
			if (refresh_time == 6)
				refresh_time = 1;
			SettingsUserMenu.RefreshScreenTime =
					refresh_time_values[refresh_time];
			//set_change_time_of_display(SettingsUserMenu.RefreshScreenTime);
		}
			break;
		case USER_NAME:
			letter++;
			if (letter == '{')
				letter = 'a';
			user_name[inc] = letter;
			break;
		case DISPLAY_MODE_ON_OFF:
			SettingsUserMenu.Display_mode--;
			if (SettingsUserMenu.Display_mode == 0)
				SettingsUserMenu.Display_mode = 4;
			if (SettingsUserMenu.Display_mode == DISPLAY_NORMAL_MODE)
				SettingsUserMenu.RefreshScreenTime = 65535;
			if (SettingsUserMenu.Display_mode == DISPLAY_STANDBY)
			{
				Display_Controls.OnStandbyMode_flag = true;
				HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
				HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
				saved_seconds = sTime.Seconds;
				saved_minutes = sTime.Minutes;
			}
			else
			{
				Display_Controls.OnStandbyMode_flag = false;
			}
			break;
		case POWER_LED:
			SettingsUserMenu.Power_LED--;
			if (SettingsUserMenu.Power_LED == 0)
				SettingsUserMenu.Power_LED = 5;
			break;
		default:
			break;
		}
	}
}

//
static void Change_Down_Input(void)
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
		TDA7719_config.set_input_front = MICROPHONE_USB;
		RADIO_IS_ON_front_flag = false;
		if (RADIO_IS_ON_front_flag == false && RADIO_IS_ON_back_flag == false)
		{
			RDA5807_PowerOff();
		}
		break;
	case MICROPHONE_USB:
		TDA7719_config.set_input_front = BLUETOOTH;
		break;
	default:
		break;
	}
	TDA7719_SetMainInput(TDA7719_config.set_input_front);

}

//
static void Change_Up_Input(void)
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
		TDA7719_config.set_input_back = MICROPHONE_USB;
		RADIO_IS_ON_back_flag = false;
		if (RADIO_IS_ON_back_flag == false && RADIO_IS_ON_front_flag == false)
		{
			RDA5807_PowerOff();
		}
		break;
	case MICROPHONE_USB:
		TDA7719_config.set_input_back = BLUETOOTH;
		break;
	default:
		break;
	}
	TDA7719_SetSecondInput(TDA7719_config.set_input_back);

}

//
static void Change_FFT_source_Up(void)
{
	FFT_channel_source--;
	if (FFT_channel_source == FFT_ch_src_enum_MIN)
	{
		FFT_channel_source = FFT_back_right;
	}

}

//
static void Change_FFT_source_Down(void)
{
	FFT_channel_source++;
	if (FFT_channel_source >= FFT_ch_src_enum_MAX)
	{
		FFT_channel_source = FFT_front_left;
	}

}

//
static void Read_Set_TimeAndDate(void)
{
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	switch_change_time(Clock_Data_Time, 1);
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

}

void AppButtons_PowerButton_SetPWMValue(uint8_t pwm_value)
{
	HAL_Buttons_PowerButton_SetPWM(pwm_value);
}

void AppButtons_PowerButton_StateUpdate(void)
{
	static uint32_t miliseconds = 0;
	_Bool countingUp_or_countingDown = false;

	if ((miliseconds + 10) <= SysTick_MiliSeconds)
	{
		miliseconds = SysTick_MiliSeconds;

		switch (SettingsUserMenu.Power_LED)
		{
		case POWER_OFF:
			if (countingUp_or_countingDown)
			{
				AppButtons_PowerButton_SetPWMValue(AppButtons_ConvertPWM(miliseconds % POWER_BUTTON_PWM_MAX_VALUE));
			}
			else
			{
				AppButtons_PowerButton_SetPWMValue(AppButtons_ConvertPWM(miliseconds % POWER_BUTTON_PWM_MAX_VALUE));
			}
			break;
		case POWER_ON:
			AppButtons_PowerButton_SetPWMValue(AppButtons_ConvertPWM(miliseconds % POWER_BUTTON_PWM_MAX_VALUE));
			break;
		case Always_OFF:
			AppButtons_PowerButton_SetPWMValue(POWER_BUTTON_PWM_MIN_VALUE);
			break;
		case ALWAYS_ON:
			AppButtons_PowerButton_SetPWMValue(POWER_BUTTON_PWM_MAX_VALUE);
			break;
		case CHANGE_BRIGHTNESS:
			AppButtons_PowerButton_SetPWMValue(AppButtons_ConvertPWM(miliseconds % POWER_BUTTON_PWM_MAX_VALUE));
			break;
			//może case gdzie użytkownik podaje parametr określający jasność
		default:
			break;
		}
	}
}

void AppButtons_PowerButton_Init(void)
{
	HAL_Buttons_PowerButton_Init();
}

// change linear scale to more eye-friendly
float AppButtons_ConvertPWM(float val)
{
	const float k = 0.06f;
	const float x0 = 100.0f;
	return 200.0f / (1.0f + exp(-k * (val - x0)));
}

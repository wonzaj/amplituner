/**
 ********************************************************************************
 * @file    app_buttons.c
 * @author  macie
 * @date    Nov 16, 2024
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "app_buttons.h"

#include "cmsis_os2.h"
/************************************
 * EXTERN VARIABLES
 ************************************/
extern osMessageQueueId_t buttonHandlerQueueHandle;
/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
static char Current_letter = 'a';
static uint8_t inc = 0; //97 - 'a'
SettingsUserMenu_t SettingsUserMenu;
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
extern uint32_t SysTick_MiliSeconds;
extern uint32_t SysTick_Seconds;
extern uint16_t ADC_SamplesTEST[4];
extern volatile uint8_t RADIO_IS_ON_back_flag;
extern volatile uint8_t RADIO_IS_ON_front_flag;
extern uint8_t settings_page;
extern uint8_t saved_seconds;
extern uint8_t saved_minutes;
extern char user_name[10];
extern volatile uint8_t POWER_device_state_flag;
extern uint8_t UV_meter_front_back;
extern uint8_t UV_meter_numb_of_chan;
volatile uint8_t POWER_device_state_flag = 0;
volatile uint8_t RADIO_IS_ON_front_flag;
volatile uint8_t RADIO_IS_ON_back_flag;
/************************************
 * GLOBAL VARIABLES
 ************************************/
Device_config_t Device_Config =
{
		.isTurnedOn = TURNED_ON,
		.isFFTtoCalculate = 0,
};
/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
static void Buttons_PowerButton_Pressed(void);
static void Buttons_PowerButton_Released(void);
static void Buttons_UserButton1_Pressed(void);
static void Buttons_UserButton1_Released(void);
static void Buttons_UserButton2_Pressed(void);
static void Buttons_UserButton2_Released(void);
static void Buttons_UserButton3_Pressed(void);
static void Buttons_UserButton3_Released(void);
static void Buttons_UserButton4_Pressed(void);
static void Buttons_UserButton4_Released(void);
static void Buttons_EncoderVolFrontButton_Pressed(void);
static void Buttons_EncoderVolFrontButton_Released(void);
static void Buttons_EncoderVolBackButton_Pressed(void);
static void Buttons_EncoderVolBackButton_Released(void);
static void Buttons_EncoderTrebleButton_Pressed(void);
static void Buttons_EncoderTrebleButton_Released(void);
static void Buttons_EncoderBassButton_Pressed(void);
static void Buttons_EncoderBassButton_Released(void);
static void Buttons_EncoderMiddleButton_Pressed(void);
static void Buttons_EncoderMiddleButton_Released(void);
static void Buttons_EncoderRadioButton_Pressed(void);
static void Buttons_EncoderRadioButton_Released(void);
static void Buttons_EncoderLoudnessButton_Pressed(void);
static void Buttons_EncoderLoudnessButton_Released(void);

static void Save_Station_Freq_1(void);
static void Save_Station_Freq_2(void);
static void SettingsChange_Down(void);
static void SettingsChange_Up(void);
static void Change_Down_Input(void);
static void Change_Up_Input(void);
static void SettingsChange_Selected(void);
/************************************
 * STATIC FUNCTIONS
 ************************************/
static void Buttons_PowerButton_Pressed(void)
{
	//stopniowo zmniejszać głośność przez kilka sekund - opcja do włączenia w settings
	//w między czasie albo na końcu odpalić ekran pożegnalny

	/* if power button is pressed while device is ON*/
	if (Device_Config.isTurnedOn == TURNED_ON)
	{
		Device_Config.isTurnedOn = TURNED_OFF;
		SSD1322_API_Sleep_On();
		RDA5807_PowerOff();
		TDA7719_SetSoftMute(0, 1);
		EEPROM_Save_FilterSettings(&encoderFilterTreble, &encoderFilterMiddle, &encoderFilterBass, &encoderFilterLoudness);
		EEPROM_Save_UserSetting(&savedUserSettings);
		EEPROM_Save_VolumeSettings(&encoderVolFront, &encoderVolBack);
		HAL_Buttons_IRQ_TurnOff();
		// Mute high power amplituner section
	}

	else if (Device_Config.isTurnedOn == TURNED_OFF)
	{
		Device_Config.isTurnedOn = TURNED_ON;
		SSD1322_API_Sleep_On();
		RDA5807_PowerOn();
		RDA5807_Init();
		TDA7719_init();
		TDA7719_SetSoftMute(1, 1);
		EEPROM_Read_FilterSettings(&encoderFilterTreble, &encoderFilterMiddle, &encoderFilterBass, &encoderFilterLoudness);
		EEPROM_Read_UserSetting(&savedUserSettings);
		EEPROM_Read_VolumeSettings(&encoderVolFront, &encoderVolBack);
		HAL_Buttons_IRQ_TurnOn();
	}
}

static void Buttons_PowerButton_Released(void)
{

}

//User button 1
static void Buttons_UserButton1_Pressed(void)
{
	ScreenState_t Current_Screen_State = AppDisplay_GetDisplayState();

	if ((Current_Screen_State + 1) == SCREEN_STATE_ENUM_MAX_USER_DISPLAY)
	{
		AppDisplay_SetDisplayState(SCREEN_TIME);
	}
	else
	{
		AppDisplay_SetDisplayState(Current_Screen_State + 1);
	}

	switch (Current_Screen_State)
	{
	case SCREEN_TIME:

		break;
	case SCREEN_RADIO:

		break;
	case SCREEN_FFT:
		FFT_Start_Measurements();
		break;
	case SCREEN_UVMETER:
		AudioVis_Start_Measurements();
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

static void Buttons_UserButton1_Released(void)
{

}

//User button 2
static void Buttons_UserButton2_Pressed(void)
{
	ScreenState_t Current_Screen_State = AppDisplay_GetDisplayState();

	switch (Current_Screen_State)
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
		// Change scaling factor in y or x on fft graph
		break;
	case SCREEN_UVMETER:
		break;
	case SCREEN_OFF:
		break;
	case SCREEN_GOODBYTE:
		break;
	case SCREEN_SETCLOCK:
		Time_ChangeSelected_TimeComponent();
		break;
	case SCREEN_SETALARM:
		Time_ChangeSelected_AlarmComponent();
		break;
	case SCREEN_SETTINGS:
		SettingsChange_Selected();
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

static void Buttons_UserButton2_Released(void)
{

}

//User button 3
static void Buttons_UserButton3_Pressed(void)
{
	ScreenState_t Current_Screen_State = AppDisplay_GetDisplayState();

	switch (Current_Screen_State)
	{
	case SCREEN_TIME:
		break;
	case SCREEN_RADIO:
		Save_Station_Freq_1(); // zmienić nazwe, wrzucić do odpowiedniego modulu i tutaj do przeniesienia do released button
		break;
	case SCREEN_WAKEUP:
		break;
	case SCREEN_FFT:
		FFT_ChangeSource_Down();
		break;
	case SCREEN_UVMETER:
		AudioVis_ChangeSource_Front();
	case SCREEN_OFF:
		break;
	case SCREEN_GOODBYTE:
		break;
	case SCREEN_SETCLOCK:
		Time_ReadAndSet_TimeAndDate(CHANGE_VALUE_UP);
		break;
	case SCREEN_SETALARM:
		Time_ReadAndSet_Alarms(CHANGE_VALUE_UP);
		break;
	case SCREEN_SETTINGS:
		SettingsChange_Down();
		break;
	case SCREEN_USB:
		break;
	case SCREEN_SETINPUT:
		Change_Down_Input();
		break;
	default:
		break;
	}
}

static void Buttons_UserButton3_Released(void)
{

}

//User button 4
static void Buttons_UserButton4_Pressed(void)
{
	ScreenState_t Current_Screen_State = AppDisplay_GetDisplayState();

	switch (Current_Screen_State)
	{
	case SCREEN_TIME:
		break;
	case SCREEN_RADIO:
		Save_Station_Freq_2();
		break;
	case SCREEN_WAKEUP:
		break;
	case SCREEN_FFT:
		FFT_ChangeSource_Up();
		break;
	case SCREEN_UVMETER:
		AudioVis_ChangeSource_Back();
		break;
	case SCREEN_OFF:
		break;
	case SCREEN_GOODBYTE:
		break;
	case SCREEN_SETCLOCK:
		Time_ReadAndSet_TimeAndDate(CHANGE_VALUE_DOWN);
		break;
	case SCREEN_SETALARM:
		Time_ReadAndSet_Alarms(CHANGE_VALUE_DOWN);
		break;
	case SCREEN_SETTINGS:
		SettingsChange_Up();
		break;
	case SCREEN_USB:
		break;
	case SCREEN_SETINPUT:
		Change_Up_Input();
		break;
	default:
		break;
	}
}

static void Buttons_UserButton4_Released(void)
{

}

static void Buttons_EncoderVolFrontButton_Pressed(void)
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
		TDA7719_SetVolume_Master(tempVolFrontLeft, tempVolFrontRight,tempVolBackLeft, tempVolBackLeft);
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

static void Buttons_EncoderVolFrontButton_Released(void)
{

}

// Encoder volume back
static void Buttons_EncoderVolBackButton_Pressed(void)
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

static void Buttons_EncoderVolBackButton_Released(void)
{

}

// Encoder volume treble
static void Buttons_EncoderTrebleButton_Pressed(void)
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

static void Buttons_EncoderTrebleButton_Released(void)
{

}

// Encoder volume bass
static void Buttons_EncoderBassButton_Pressed(void)
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

static void Buttons_EncoderBassButton_Released(void)
{

}

// Encoder volume middle
static void Buttons_EncoderMiddleButton_Pressed(void)
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

static void Buttons_EncoderMiddleButton_Released(void)
{

}

// Encoder volume radio
static void Buttons_EncoderRadioButton_Pressed(void)
{
	AppDisplay_SaveCurrentDisplayState();
	AppDisplay_SetDisplayState(SCREEN_ENCODER_RADIO);
	//HAL_Timers_RefreshTimer(&htim14, TIM_CHANNEL_1);
}

static void Buttons_EncoderRadioButton_Released(void)
{

}

// Encoder volume loudness
static void Buttons_EncoderLoudnessButton_Pressed(void)
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

static void Buttons_EncoderLoudnessButton_Released(void)
{

}
/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void AppButtons_PowerButton_Init(void)
{

}

void AppButtons_EventHandler(void)
{
	Queue_ButtonEvent_t Queue_ButtonEvent = {0};

	if(osMessageQueueGet(buttonHandlerQueueHandle, &Queue_ButtonEvent, 0U, osWaitForever) == osOK)
	{
		if(Queue_ButtonEvent.State == Button_Pressed)
		{
			switch (Queue_ButtonEvent.GPIO_Pin)
			{
				case POWER_BUTTON_Pin:
					Buttons_PowerButton_Pressed();
					break;
				case USER_BUTTON_1_Pin:
					Buttons_UserButton1_Pressed();
					break;
				case USER_BUTTON_2_Pin:
					Buttons_UserButton2_Pressed();
					break;
				case USER_BUTTON_3_Pin:
					Buttons_UserButton3_Pressed();
					break;
				case USER_BUTTON_4_Pin:
					Buttons_UserButton4_Pressed();
					break;
				case ENCODER_BUTTON_VOL_FRONT_Pin:
					Buttons_EncoderVolFrontButton_Pressed();
					break;
				case ENCODER_BUTTON_VOL_BACK_Pin:
					Buttons_EncoderVolBackButton_Pressed();
					break;
				case ENCODER_BUTTON_LOUD_Pin:
					Buttons_EncoderLoudnessButton_Pressed();
					break;
				case ENCODER_BUTTON_TREBLE_Pin:
					Buttons_EncoderTrebleButton_Pressed();
					break;
				case ENCODER_BUTTON_BASS_Pin:
					Buttons_EncoderBassButton_Pressed();
					break;
				case ENCODER_BUTTON_MIDDLE_Pin:
					Buttons_EncoderMiddleButton_Pressed();
					break;
				case ENCODER_BUTTON_RADIO_Pin:
					Buttons_EncoderRadioButton_Pressed();
					break;
				default:
					break;
			}
		}
		else if(Queue_ButtonEvent.State == Button_Released)
		{
			switch (Queue_ButtonEvent.GPIO_Pin)
			{
				case POWER_BUTTON_Pin:
					Buttons_PowerButton_Released();
					break;
				case USER_BUTTON_1_Pin:
					Buttons_UserButton1_Released();
					break;
				case USER_BUTTON_2_Pin:
					Buttons_UserButton2_Released();
					break;
				case USER_BUTTON_3_Pin:
					Buttons_UserButton3_Released();
					break;
				case USER_BUTTON_4_Pin:
					Buttons_UserButton4_Released();
					break;
				case ENCODER_BUTTON_VOL_FRONT_Pin:
					Buttons_EncoderVolFrontButton_Released();
					break;
				case ENCODER_BUTTON_VOL_BACK_Pin:
					Buttons_EncoderVolBackButton_Released();
					break;
				case ENCODER_BUTTON_LOUD_Pin:
					Buttons_EncoderLoudnessButton_Released();
					break;
				case ENCODER_BUTTON_TREBLE_Pin:
					Buttons_EncoderTrebleButton_Released();
					break;
				case ENCODER_BUTTON_BASS_Pin:
					Buttons_EncoderBassButton_Released();
					break;
				case ENCODER_BUTTON_MIDDLE_Pin:
					Buttons_EncoderMiddleButton_Released();
					break;
				case ENCODER_BUTTON_RADIO_Pin:
					Buttons_EncoderRadioButton_Released();
					break;
				default:
					break;
			}
		}
	}
}

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
			EEPROM_Save_UserSetting(&savedUserSettings);
		}

		if (button_timer + 2000 >= button_timer1) //if holed for less than 2 secs
		{
			RDA5807_SetFrequency(savedUserSettings.stationSaved_1);
		}
	}
}

//
static void SettingsChange_Selected(void)
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
static void SettingsChange_Down(void)
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

//
static void SettingsChange_Up(void)
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
	}
		break;
	case USER_NAME:
		Current_letter++;
		if (Current_letter == '{')
			Current_letter = 'a';
		user_name[inc] = Current_letter;
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

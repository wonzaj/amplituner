#ifndef INC_DISPLAY_OLED_DRAWS_DISPLAY_H_
#define INC_DISPLAY_OLED_DRAWS_DISPLAY_H_

#include "rtc.h"
#include "string.h"
//#include "rng.h"
#include "stdbool.h"
#include "stdlib.h"

#include "app_buttons.h"
#include "TDA7719.h"
#include "RDA5807m.h"
#include "app_encoders.h"
#include "hal_buttons.h"
#include "SSD1322_API.h"
#include "SSD1322_GFX.h"
#include "fft.h"

//--------------------------------------------------------------
// Defines
//--------------------------------------------------------------
#define DISPLAY_FPS				5	/* Too high value may affect systems's responsiveness */
#define DISPLAY_REFRESH_TIME_HZ (1000/DISPLAY_FPS)

#define PRZECINEK 44
#define ASCII_DWUKROPEK 58
#define ASCII_DIGIT_OFFSET 48
#define ASCII_NULL 0
#define ASCII_MINUS 45
#define ASCII_PLUS 43
#define RADIO_MAX_FREQ 1095
#define RADIO_MIN_FREQ 885
#define RADIO_100MHZ_FREQ 1000
#define DISPLAY_BLACK 0
#define DISPLAY_WHITE 1

#define REFRESH_TIME_240s  55001
#define REFRESH_TIME_120s  55000
#define REFRESH_TIME_60s   27500
#define REFRESH_TIME_30s   13750
#define REFRESH_TIME_10s   4580

#define PAGE_SETTINGS_1 0
#define PAGE_SETTINGS_2 1

//--------------------------------------------------------------
// extern variables
//--------------------------------------------------------------
extern RTC_TimeTypeDef 	 sTime;
extern RTC_DateTypeDef 	 sDate;
extern RTC_DateTypeDef 	 pDate;
extern RTC_AlarmTypeDef  Alarm_A;
extern RTC_AlarmTypeDef  Alarm_B;
extern RTC_AlarmTypeDef  Alarm;
//extern RNG_HandleTypeDef hrng;

//extern encoderFilter_t 	 encoderFilterTreble;
//extern encoderFilter_t	 encoderFilterMiddle;
//extern encoderFilter_t	 encoderFilterBass;
//extern encoderFilter_t	 encoderFilterLoudness;
//extern encoder_t 	 encoderVolFront;
//extern encoder_t	 encoderVolBack;
extern volatile uint8_t RADIO_IS_ON_front_flag;
extern volatile uint8_t RADIO_IS_ON_back_flag;
// buffer for display
extern uint8_t 		DisplayOLEDBuffer[];

extern uint8_t UV_meter_front_back;
extern uint32_t ADC_SamplesSUM[4];
extern volatile uint8_t FFT_calc_done;
extern uint8_t OutFreqArray[21];

extern _Bool alarm_set_A_or_B;
extern _Bool PreviewAlarm;
extern char AlarmMode[25];
//--------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------


//--------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------

/* Most important enum. Declares display state */
typedef enum
{
    SCREEN_TIME = 1,
    SCREEN_RADIO,
    SCREEN_FFT,
    SCREEN_UVMETER,
    SCREEN_TIME_BOUNCING,
    SCREEN_SETINPUT,
    SCREEN_SETCLOCK,
    SCREEN_SETALARM,
    SCREEN_SETTINGS,
    ENUM_MAX_USER_DISPLAY,

    SCREEN_SNAKE,	//not used atm
    SCREEN_USB, 	//not used atm

    SCREEN_WAKEUP = 20,
    SCREEN_WELCOME,
    SCREEN_OFF,
    SCREEN_GOODBYTE,
    ENUM_MAX_INVIS_DISPLAY,

    SCREEN_ENCODER_VOLUME_FRONT = 30,
    SCREEN_ENCODER_VOLUME_BACK,
    SCREEN_ENCODER_LOUDNESS,
    SCREEN_ENCODER_TREBLE,
    SCREEN_ENCODER_MIDDLE,
    SCREEN_ENCODER_BASS,
    SCREEN_ENCODER_RADIO,
    ENUM_MAX

}ScreenState_t;

typedef enum
{
    POWER_OFF = 1,
    POWER_ON,
    Always_OFF,
    ALWAYS_ON,
    CHANGE_BRIGHTNESS

} led_power_state_t;

typedef enum
{
	DISPLAY_NORMAL_MODE = 1,
    DISPLAY_CHANGING,
    DISPLAY_STANDBY,
    DISPLAY_TIME,

}display_mode_t;

typedef struct
{
	ScreenState_t 	Screen_State;
	ScreenState_t 	Screen_State_Saved;
	uint32_t 		Refresh_Hz;

	_Bool OnStandbyMode_flag;

}Display_Controls_t;

extern Display_Controls_t Display_Controls;

ScreenState_t AppDisplay_GetDisplayState(void);
void AppDisplay_SetDisplayState(ScreenState_t ScreenState);

ScreenState_t AppDisplay_GetSavedDisplayState(void);
void AppDisplay_SetSavedDisplayState(ScreenState_t ScreenState);
void AppDisplay_OnInitTask(void);
void AppDisplay_RefreshDisplayTask(void);
void AppDisplay_RefreshDisplay(const ScreenState_t SSD1322_Screen_State);
void AppDisplay_SaveCurrentDisplayState();
//void set_saved_display(TIM_HandleTypeDef *htim);
//void change_display_state(TIM_HandleTypeDef *htim);

#endif /* INC_DISPLAY_OLED_DRAWS_DISPLAY_H_ */

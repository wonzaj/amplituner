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
// Fonts
#include "FreeMono9pt7b.h"
#include "Maciek.h"
#include "FreeSerifItalic24pt7b.h"
#include "FreeSerifItalic9pt7b.h"
#include "FreeSerifBold9pt7b.h"
#include "custom_chars.h"
#include "FreeSerifBoldItalic24pt7b.h"
#include "FreeSerifBoldItalic9pt7b.h"
#include "FreeSerif9pt7b.h"
#include "TomThumb.h"
#include "FreeMonoOblique9pt7b.h"
#include "FreeSansBold9pt7b.h"
#include "FreeSans9pt7b.h"
#include "FreeSerif24pt7b.h"

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

typedef uint16_t UV_meter_t;

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
    SCREEN_SNAKE,
    ENUM_MAX_USER_DISPLAY,

    SCREEN_USB, //not used atm

    SCREEN_WakeUp = 20,
    SCREEN_Welcome,
    SCREEN_OFF,
    SCREEN_GoodBye,
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

}Display_Controls_t;

extern Display_Controls_t Display_Controls;

ScreenState_t AppDisplay_GetDisplayState(void);
void AppDisplay_SetDisplayState(ScreenState_t ScreenState);

ScreenState_t AppDisplay_GetSavedDisplayState(void);
void AppDisplay_SetSavedDisplayState(ScreenState_t ScreenState);
//--------------------------------------------------------------
// Possible displayed screens
//--------------------------------------------------------------
void AppDisplay_OnInitTask(void);
void AppDisplay_RefreshDisplayTask(void);
void AppDisplay_RefreshDisplay(const ScreenState_t SSD1322_Screen_State);
void SSD1322_Screen_Welcome(uint8_t *const buffer);
void SSD1322_Screen_Time(uint8_t *const buffer);//, RTC_TimeTypeDef *RtcTime, RTC_DateTypeDef *RtcDate);
void SSD1322_Screen_Radio(uint8_t *const buffer);
void SSD1322_Screen_WakeUp(uint8_t *const buffer);
void SSD1322_Screen_FFT(uint8_t *const buffer, uint8_t *const FFT_out_buffer);
void SSD1322_Screen_UVMeter(uint8_t *const buffer, UV_meter_t left_channel, UV_meter_t right_channel, const uint8_t mode);
void SSD1322_Screen_OFF(uint8_t *const buffer);
void SSD1322_Screen_GoodBye(uint8_t *const buffer);
void SSD1322_Screen_SetClock(uint8_t *const buffer);
void SSD1322_Screen_SetAlarm(uint8_t *const buffer);
void SSD1322_Screen_Settings(uint8_t *const buffer);
void SSD1322_Screen_Snake(uint8_t *const buffer);
void SSD1322_Screen_USB(uint8_t *const buffer);
void SSD1322_Screen_Time_Bouncing(uint8_t *const buffer);
void SSD1322_Screen_SetInput(uint8_t *const buffer);
void SSD1322_Screen_Encoder_Volume_Front(uint8_t *const buffer);
void SSD1322_Screen_Encoder_Volume_Back(uint8_t *const buffer);
void SSD1322_Screen_Encoder_Loudness(uint8_t *const buffer);
void SSD1322_Screen_Encoder_Treble(uint8_t *const buffer);
void SSD1322_Screen_Encoder_Middle(uint8_t *const buffer);
void SSD1322_Screen_Encoder_Bass(uint8_t *const buffer);

void draw_encoder_volume_front_scale(uint8_t *const buffer);
void draw_encoder_volume_back_scale(uint8_t *const buffer);
void draw_gain_and_freq_loudness(uint8_t *const buffer);
void draw_gain_and_freq(int8_t gainValue);
void draw_UV_rectangle_scale(uint8_t *const buffer, const UV_meter_t left_channel, const UV_meter_t right_channel);
void draw_UV_lines_scale(uint8_t *const buffer, const UV_meter_t left_channel, const UV_meter_t right_channel);

void draw_nutka1(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y);
void draw_nutka2(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y);
void draw_nutka3(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y);
void draw_nutka4(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y);
void draw_nutka5(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y);
void draw_speaker_left(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y);
void draw_speaker_right(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y);
void draw_freq_scale(uint8_t *const buffer, const uint16_t freq_scaled);
void draw_alarmsSource(uint8_t *const buffer);
void draw_pointer(uint8_t *const buffer);
void draw_refreshTime(uint8_t *const buffer);
void draw_powerLED(uint8_t *const buffer);
void draw_displayMode(uint8_t *const buffer);

//--------------------------------------------------------------
// Other functions declarations
//--------------------------------------------------------------
void AppDisplay_SaveCurrentDisplayState();
//void set_saved_display(TIM_HandleTypeDef *htim);
//void change_display_state(TIM_HandleTypeDef *htim);

void ChangeDateToArrayChar(uint16_t frq);
void ChangeDateToArrayCharTime(char *arrayChar,uint8_t hours, uint8_t minutes, uint8_t seconds,uint8_t mode);
void ConvertDateToBuffer(uint16_t Year, uint8_t Month, uint8_t WeekDay, uint8_t Date);
int16_t SplitNumberToDignits(int16_t position, int16_t number);
uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max );

_Bool prepare_RDS_text(char *RDS_text);
_Bool  get_random_coords(uint32_t *random_x, uint32_t *random_y);
void make_array(uint8_t *frame_buffer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t brightness);

#endif /* INC_DISPLAY_OLED_DRAWS_DISPLAY_H_ */

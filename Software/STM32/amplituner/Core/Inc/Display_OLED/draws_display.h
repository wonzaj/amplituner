#ifndef INC_DISPLAY_OLED_DRAWS_DISPLAY_H_
#define INC_DISPLAY_OLED_DRAWS_DISPLAY_H_


#include "main.h"
#include "rtc.h"
#include "string.h"
#include "stdlib.h"
#include "rng.h"
#include "stdbool.h"

#include "TDA7719/TDA7719.h"
#include "buttons/buttons.h"
#include "radio/RDA5807m.h"
#include <Display_OLED/SSD1322_API.h>
#include <Display_OLED/SSD1322_HW_Driver.h>

//--------------------------------------------------------------
// Defines
//--------------------------------------------------------------
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
extern RNG_HandleTypeDef hrng;

extern encoderFilter_t 	 encoderFilterTreble;
extern encoderFilter_t	 encoderFilterMiddle;
extern encoderFilter_t	 encoderFilterBass;
extern encoderFilter_t	 encoderFilterLoudness;
extern encoder_t 	 encoderVolFront;
extern encoder_t	 encoderVolBack;

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

_Bool IS_ALARM_SET_A;
_Bool IS_ALARM_SET_B;


//--------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------

typedef uint16_t UV_meter_t;

//
typedef enum
{
    ONLY_ONE_TIME_ALARM = 1,
    ONLY_WEEKENDS_ALARM,
    EVERYDAY_ALARM,
    MON_to_FRI_ALARM,
    EXACT_DATE_ALRAM
}RTC_typeOfAlarm_t;

/* Most important enum. Declares display state */
typedef enum
{
    SSD1322_SCREEN_TIME = 1,		//duże ładne litery
    SSD1322_SCREEN_RADIO,
    SSD1322_SCREEN_FFT,
    SSD1322_SCREEN_UVMETER,
    SSD1322_SCREEN_TIME_BOUNCING,
    SSD1322_SCREEN_SETINPUT,
    SSD1322_SCREEN_SETCLOCK,
    SSD1322_SCREEN_SETALARM,
    SSD1322_SCREEN_SETTINGS,
    ENUM_MAX_USER_DISPLAY,

    SSD1322_SCREEN_USB, //not used atm

    SSD1322_SCREEN_WakeUp = 20,
    SSD1322_SCREEN_Welcome,
    SSD1322_SCREEN_OFF,
    SSD1322_SCREEN_GoodBye,
    ENUM_MAX_INVIS_DISPLAY,

    SSD1322_SCREEN_ENCODER_VOLUME_FRONT = 30,
    SSD1322_SCREEN_ENCODER_VOLUME_BACK,
    SSD1322_SCREEN_ENCODER_LOUDNESS,
    SSD1322_SCREEN_ENCODER_TREBLE,
    SSD1322_SCREEN_ENCODER_MIDDLE,
    SSD1322_SCREEN_ENCODER_BASS,
    ENUM_MAX

}SSD1322_Screen_t;


typedef enum
{
    HOUR = 1,
    MINUTE = 2,
    DAYWEEK = 3,
    DAY = 4,
    MODE = 5,
    SECOND = 6,
    MONTH = 7,
    YEAR = 8
} Clock_Data_Change_t;


typedef enum
{
    HOUR_ALARM = 1,
    MINUTE_ALARM = 2,
    DAYWEEK_ALARM = 3,
    MODE_ALARM = 4,
    SET_ALARM = 5,
    PREVIEW_ALARM
} AlarmDataChange_t;


typedef enum
{
    POWER_OFF = 1,
    POWER_ON,
    Always_OFF,
    Always_ON,
    Change_brigh

} led_power_state_t;


typedef enum
{
    Disp_normal = 1,
    Disp_changing,
    Disp_standby,
    Disp_time,

}display_mode_t;


//--------------------------------------------------------------
// Possible displayed screens
//--------------------------------------------------------------
void Refresh_display(const SSD1322_Screen_t SSD1322_Screen_State);
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
// Fonts
//--------------------------------------------------------------
extern const GFXfont FreeMono24pt7b;
extern const GFXfont FreeMono9pt7b;
extern const GFXfont FreeMonoOblique9pt7b;
extern const GFXfont FreeMonoOblique24pt7b;
extern const GFXfont FreeSerif9pt7b;
extern const GFXfont FreeSerif24pt7b;
extern const GFXfont FreeMonoBold24pt7b;
extern const GFXfont FreeMonoBold9pt7b;
extern const GFXfont FreeMonoBoldOblique24pt7b;
extern const GFXfont FreeMonoBoldOblique9pt7b;
extern const GFXfont FreeSans24pt7b;
extern const GFXfont FreeSans9pt7b;
extern const GFXfont FreeSansBold24pt7b;
extern const GFXfont FreeSansBold9pt7b;
extern const GFXfont FreeSansBoldOblique24pt7b;
extern const GFXfont FreeSansBoldOblique9pt7b;
extern const GFXfont FreeSansOblique24pt7b;
extern const GFXfont FreeSansOblique9pt7b;
extern const GFXfont FreeSerifBold24pt7b;
extern const GFXfont FreeSerifBold9pt7b;
extern const GFXfont FreeSerifBoldItalic24pt7b;
extern const GFXfont FreeSerifBoldItalic9pt7b;
extern const GFXfont FreeSerifItalic24pt7b;
extern const GFXfont FreeSerifItalic9pt7b;
extern const GFXfont Org_01;
extern const GFXfont Picopixel;
extern const GFXfont Tiny3x3a2pt7b;
extern const GFXfont TomThumb;
extern const GFXfont MACIEK_FONT;
extern const GFXfont Custon_chars;

//--------------------------------------------------------------
// Other functions declarations
//--------------------------------------------------------------
void save_current_display_state(SSD1322_Screen_t Screen_State);
void refresh_timer_state(void);
void set_saved_display(TIM_HandleTypeDef *htim);
void change_display_state(TIM_HandleTypeDef *htim);

void ChangeDateToArrayChar(uint16_t frq);
void ChangeDateToArrayCharTime(char *arrayChar,uint8_t hours, uint8_t minutes, uint8_t seconds,uint8_t mode);
void ConvertDateToBuffer(uint16_t Year, uint8_t Month, uint8_t WeekDay, uint8_t Date);

_Bool prepare_RDS_text(char *RDS_text);
_Bool  get_random_coords(uint32_t *random_x, uint32_t *random_y);
void make_array(uint8_t *frame_buffer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t brightness);

extern void Set_Alarm_Mode(RTC_typeOfAlarm_t typeOfAlarm);
extern uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max );
extern int16_t SplitNumberToDignits(int16_t position, int16_t number);
extern void switch_change_time(Clock_Data_Change_t Clock , _Bool add_subb);
extern void switch_change_alarm(AlarmDataChange_t AlarmDataChange, _Bool add_subb);

#endif /* INC_DISPLAY_OLED_DRAWS_DISPLAY_H_ */

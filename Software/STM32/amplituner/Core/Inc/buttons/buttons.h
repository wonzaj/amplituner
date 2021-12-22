#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_
//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "main.h"
#include "string.h"
#include "stdbool.h"
#include "rtc.h"
#include "tim.h"
#include "float.h"
#include "EEPROM/eeprom.h"
#include "TDA7719/TDA7719.h"
#include "radio/RDA5807m.h"

//--------------------------------------------------------------
// Defines
//--------------------------------------------------------------
#define VOLUME_MUTE 96
#define UV_METER_FRONT 0
#define UV_METER_BACK  1
//--------------------------------------------------------------
// Extern variables
//--------------------------------------------------------------
extern	int16_t tempVolFrontLeft;
extern	int16_t tempVolFrontRight;
extern	int16_t tempVolBackLeft;
extern	int16_t tempVolBackRight;

extern uint16_t ADC_SamplesTEST[4];
extern uint16_t gGPIO_Pin;
extern _Bool volatile button_debou_state;
extern ADC_HandleTypeDef 	 hadc1;
extern ADC_HandleTypeDef 	 hadc2;
extern DMA_HandleTypeDef 	 hdma_adc1;
extern TIM_HandleTypeDef 	 htim6;
extern volatile TDA7719_config_t TDA7719_config;

extern volatile uint8_t ADC_IS_ON_flag;
extern volatile uint8_t RADIO_IS_ON_back_flag;
extern volatile uint8_t RADIO_IS_ON_front_flag;

extern const uint16_t   refresh_time_values[7];
extern uint8_t 		settings_page;
extern uint8_t 		saved_seconds;
extern uint8_t 		saved_minutes;
extern char user_name[10];
extern volatile _Bool   is_display_on_standby_flag;
extern volatile uint8_t POWER_device_state_flag;

extern uint8_t UV_meter_front_back;
extern uint8_t UV_meter_numb_of_chan;

//--------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------

//Defines source for FFT calculation
//May be changed by button
typedef enum
{
    FFT_ch_src_enum_MIN = 0,
    FFT_front_left  	= 1,
    FFT_front_right 	= 2,
    FFT_back_left   	= 3,
    FFT_back_right  	= 4,
    FFT_ch_src_enum_MAX = 5

}FFT_channel_source_e;

typedef struct
{

    uint16_t RefreshScreenTime;
    uint8_t AlarmSource_A;
    uint8_t AlarmSource_B;
    uint8_t Display_mode;
    uint8_t Power_LED;
    char UserName[10];

    enum
    {
	ALARM_SOURCE_A,
	ALARM_SOURCE_B,
	REFRESH_SCREEN_TIME,
	USER_NAME,
	DISPLAY_MODE_ON_OFF,
	POWER_LED

    } SETTINGS_USER_MENU;

}SettingsUserMenu_t;

//--------------------------------------------------------------
// Normal buttons
//--------------------------------------------------------------
void Button_pressed_change_screen(void);
void Button_pressed_accept(void);
void Button_pressed_set_up(void);
void Button_pressed_set_down(void);

//--------------------------------------------------------------
// Encoders buttons
//--------------------------------------------------------------
void Encoder_button_volume_front(void);
void Encoder_button_volume_back(void);
void Encoder_button_loudness(void);
void Encoder_button_treble(void);
void Encoder_button_middle(void);
void Encoder_button_bass(void);


void Check_and_Set_Volume_back(void);
void TDA7719_SetVolume_Master(const int16_t VolFrontLeft, const int16_t VolFrontRight, const int16_t VolBackLeft, const int16_t VolBackRight);
void Check_Volume_Range_Front(volatile int8_t *const volume, const uint8_t maxVolume);
void Check_Volume_Range_Back(volatile int8_t *const volume, const uint8_t maxVolume);
void TDA7719_SetVolumeBack_LeftRight(const int16_t VolBackLeft, const int16_t VolBackRight);
void TDA7719_SetVolumeFront_LeftRight(const int16_t VolFrontLeft,const int16_t VolFrontRight);
void Check_Loudness_Param_Range(volatile int8_t *const gain, const uint8_t maxGain);
void Check_Bass_Param_Range(volatile int8_t *const gain, const uint8_t maxGain);
void Check_Middle_Param_Range(volatile int8_t *const gain, const uint8_t maxGain);
void Check_Treble_Param_Range(volatile int8_t *const gain, const uint8_t maxGain);
void Power_button_pressed(void);

extern void set_change_time_of_display(const uint16_t counter_period);
void any_buttons_pressed_routine(void);
#endif /* INC_BUTTONS_H_ */

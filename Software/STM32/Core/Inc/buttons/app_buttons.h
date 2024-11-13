#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_
//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "hal_buttons.h"
#include "eeprom.h"
#include "TDA7719.h"
#include "RDA5807m.h"
#include "app_encoders.h"
#include "SSD1322_API.h"
#include "Display_API.h"
#include "config.h"
#include "my_timers.h"
#include "time.h"
#include "math.h"
#include "rtc.h"

//--------------------------------------------------------------
// Defines
//--------------------------------------------------------------
#define VOLUME_MUTE 96
#define UV_METER_FRONT 0
#define UV_METER_BACK  1

#define POWER_BUTTON_PWM_MAX_VALUE 200
#define POWER_BUTTON_PWM_MIN_VALUE 0

//--------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------

//Defines source for FFT calculation
//May be changed by button

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

extern SettingsUserMenu_t SettingsUserMenu;

void Buttons_PowerButton_Pressed(void);
void Buttons_PowerButton_Released(void);
GPIO_PinState Buttons_PowerButton_GetState(void);
void Buttons_UserButton1_Pressed(void);
void Buttons_UserButton1_Released(void);
GPIO_PinState Buttons_UserButton1_GetState(void);
void Buttons_UserButton2_Pressed(void);
void Buttons_UserButton2_Released(void);
GPIO_PinState Buttons_UserButton2_GetState(void);
void Buttons_UserButton3_Pressed(void);
void Buttons_UserButton3_Released(void);
GPIO_PinState Buttons_UserButton3_GetState(void);
void Buttons_UserButton4_Pressed(void);
void Buttons_UserButton4_Released(void);
GPIO_PinState Buttons_UserButton4_GetState(void);
void Buttons_EncoderVolFrontButton_Pressed(void);
void Buttons_EncoderVolFrontButton_Released(void);
GPIO_PinState Buttons_EncoderVolFrontButton_GetState(void);
void Buttons_EncoderVolBackButton_Pressed(void);
void Buttons_EncoderVolBackButton_Released(void);
GPIO_PinState Buttons_EncoderVolBackButton_GetState(void);
void Buttons_EncoderTrebleButton_Pressed(void);
void Buttons_EncoderTrebleButton_Released(void);
GPIO_PinState Buttons_EncoderTrebleButton_GetState(void);
void Buttons_EncoderBassButton_Pressed(void);
void Buttons_EncoderBassButton_Released(void);
GPIO_PinState Buttons_EncoderBassButton_GetState(void);
void Buttons_EncoderMiddleButton_Pressed(void);
void Buttons_EncoderMiddleButton_Released(void);
GPIO_PinState Buttons_EncoderMiddleButton_GetState(void);
void Buttons_EncoderRadioButton_Pressed(void);
void Buttons_EncoderRadioButton_Released(void);
GPIO_PinState Buttons_EncoderRadioButton_GetState(void);
void Buttons_EncoderLoudnessButton_Pressed(void);
void Buttons_EncoderLoudnessButton_Released(void);
GPIO_PinState Buttons_EncoderLoudnessButton_GetState(void);

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

extern void set_change_time_of_display(const uint16_t counter_period);

void AppButtons_PowerButton_SetPWMValue(uint8_t pwm_value);
void AppButtons_PowerButton_StateUpdate(void);
void AppButtons_PowerButton_Init(void);
float AppButtons_ConvertPWM(float val);


#endif /* INC_BUTTONS_H_ */

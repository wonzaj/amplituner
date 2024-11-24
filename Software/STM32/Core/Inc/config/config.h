#ifndef INC_CONFIG_CONFIG_H_
#define INC_CONFIG_CONFIG_H_

#include "string.h"
#include "stdlib.h"
#include "stdint.h"

//--------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------

//Defines state of volume for volume encoders - front and back
typedef enum
{
    MASTER,
    MUTE,
    MASTER_V2,
    NORMAL,
    NORMAL_V2,
    ATTE_LEFT,
    ATTE_RIGHT,
}audioOutputState_e;

//Defines state of buttons encoders - loud, bass, middle, treble
typedef enum
{
    SET_GAIN,
    SET_CENTER_FREQ,
    SET_SOFT_STEP,
    SET_HIGH_BOOST,
    CONFIG
}buttonControl_e;

//
typedef struct
{
    uint8_t input_gain_front;     // = 0;
    uint8_t input_gain_back;     // = 0;
    uint8_t loudness;
    uint8_t volume;
    uint8_t treble;
    uint8_t middle;
    uint8_t bass;

}TDA7719_Control_t;


//
typedef struct
{
    uint8_t set_input_front;     // = 0;
    uint8_t set_input_back;     // = 1;
    uint8_t input_confi;     // = 2;
    uint8_t set_mix_source;
    uint8_t set_mix_source_Attenuator;
    enum
    {
	JACK_1 = 0,
	JACK_2 = 4,
	DAC_LR = 1,
	RADIO = 5,
	BLUETOOTH = 2,
	MICROPHONE_USB = 6
    }audio_source;

}TDA7719_config_t;

//Hold values which are saved to EEPROM
typedef struct
{
    uint16_t radio_freq;
    uint16_t stationSaved_1;
    uint16_t stationSaved_2;
    uint16_t stationSaved_3;
    uint16_t displayRefreshTime;
    uint16_t displayFont;
//dodać częstotliwość aktualna
//ustawienie budzenia się do zapisanej stacji
}savedUserSettings_t;

//
typedef enum
{
    TDA7719_MIX_TO_FRONT,
    TDA7719_MIX_TO_BACK
}TDA7719_SetMixSourceTo_t;

//Holds values for volume encoders - front and back
typedef struct
{
    int8_t volumeMaster;
    int8_t volumeLeftRight;
    int8_t volumeLeft;
    int8_t volumeRight;
    int16_t speed;

    audioOutputState_e audioOutputState;   //maybe add state - mute_both, front and back

}encoder_t;

//Hold values for filters settings - bass, middle, treble, loudness
typedef struct
{
    buttonControl_e buttonControl;

    int8_t gain;
    int8_t centerFreq;
    int8_t soft_step;
    int8_t high_boost;

}encoderFilter_t;

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
    	ALARM_SOURCE_A 		= 1,
		ALARM_SOURCE_B		= 2,
		REFRESH_SCREEN_TIME	= 3,
		USER_NAME			= 4,
		DISPLAY_MODE_ON_OFF	= 5,
		POWER_LED			= 6,
		SETTINGS_USER_MENU_ENUM_MAX

    } SETTINGS_USER_MENU;

}SettingsUserMenu_t;

#endif /* INC_CONFIG_CONFIG_H_ */

/**
 ********************************************************************************
 * @file    app_buttons.h
 * @author  macie
 * @date    Nov 16, 2024
 * @brief   
 ********************************************************************************
 */

#ifndef INC_BUTTONS_APP_BUTTONS_H_
#define INC_BUTTONS_APP_BUTTONS_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
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
/************************************
 * MACROS AND DEFINES
 ************************************/
#define VOLUME_MUTE 96

#define POWER_BUTTON_PWM_MAX_VALUE 200
#define POWER_BUTTON_PWM_MIN_VALUE 0
/************************************
 * TYPEDEFS
 ************************************/
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

enum
{
	TURNED_ON,
	TURNED_OFF
};

typedef struct
{
	uint8_t isTurnedOn;
	uint8_t isFFTtoCalculate;

}Device_config_t;
/************************************
 * EXPORTED VARIABLES
 ************************************/
extern SettingsUserMenu_t SettingsUserMenu;
/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void AppButtons_EventHandler(void);
void AppButtons_PowerButton_Init(void);

#ifdef __cplusplus
}
#endif

#endif 

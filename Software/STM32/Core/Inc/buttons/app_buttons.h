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

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void AppButtons_EventHandler(void);
void AppButtons_PowerButton_Init(void);

#ifdef __cplusplus
}
#endif

#endif 

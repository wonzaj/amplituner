/**
 ********************************************************************************
 * @file    butt_pwr_LEDs_effects.h
 * @author  macie
 * @date    Nov 19, 2024
 * @brief   
 ********************************************************************************
 */

#ifndef INC_BUTT_PWR_LEDS_EFFECTS_H_
#define INC_BUTT_PWR_LEDS_EFFECTS_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "stdint.h"
/************************************
 * MACROS AND DEFINES
 ************************************/
#define NBR_OF_REFRESH_STYLES 8

#define UPANDDOWN_REFRESHSTYLE		0
#define EXPONENTIAL_REFRESHSTYLE		1
#define SINUSOIDAL_REFRESHSTYLE		2
#define BREATHING_REFRESHSTYLE		3
#define LOGARITHM_REFRESHSTYLE		4
#define STEPDIM_REFRESHSTYLE			5
#define ZIGZAC_REFRESHSTYLE			6
#define LINEARDIM_REFRESHSTYLE		7
/************************************
 * TYPEDEFS
 ************************************/
typedef uint8_t(*RefreshLEDStyle)(uint8_t);
/************************************
 * EXPORTED VARIABLES
 ************************************/
extern const RefreshLEDStyle RefreshLEDStyles[NBR_OF_REFRESH_STYLES];
/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void Butt_pwr_LEDs_effect_refresh_Red(void);
void Butt_pwr_LEDs_effect_refresh_Yellow(void);
void Butt_pwr_LEDs_effect_refresh_Green(void);

#ifdef __cplusplus
}
#endif

#endif 

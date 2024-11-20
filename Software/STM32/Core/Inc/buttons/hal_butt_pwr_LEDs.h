/**
 ********************************************************************************
 * @file    hal_butt_pwr_LEDs.h
 * @author  macie
 * @date    Nov 16, 2024
 * @brief   
 ********************************************************************************
 */

#ifndef INC_BUTTONS_HAL_BUTT_PWR_LEDS_H_
#define INC_BUTTONS_HAL_BUTT_PWR_LEDS_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "stdint.h"
#include "tim.h"
/************************************
 * MACROS AND DEFINES
 ************************************/
#define TIM_HANDLER_RED		&htim13
#define TIM_HANDLER_GREEN	&htim11
#define TIM_HANDLER_YELLOW	&htim10

#define LED_TURNED_ON 1
#define LED_TURNED_OFF 0
/************************************
 * TYPEDEFS
 ************************************/
typedef enum
{
	POWER_LEDS_RED,
	POWER_LEDS_GREEN,
	POWER_LEDS_YELLOW
}PowerLEDs;

typedef struct
{
	uint8_t Red_PWM_Val;
	uint8_t Red_IsTurnedOn;
	uint8_t Red_Refresh_Time;
	uint8_t (*Red_RefreshStyle_Cb)(uint8_t);

	uint8_t Yellow_PWM_Val;
	uint8_t Yellow_IsTurnedOn;
	uint8_t Yellow_Refresh_Time;
	uint8_t (*Yellow_RefreshStyle_Cb)(uint8_t);

	uint8_t Green_PWM_Val;
	uint8_t Green_IsTurnedOn;
	uint8_t Green_Refresh_Time;
	uint8_t (*Green_RefreshStyle_Cb)(uint8_t);

	uint32_t Delay_Task_ms;
}PowerLEDs_Config_t;
/************************************
 * EXPORTED VARIABLES
 ************************************/
extern PowerLEDs_Config_t PowerLEDs_Config;
/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void HAL_PowerButtonLEDs_Task(void);
void HAL_PowerButtonLEDs_Init(void);
void HAL_PowerButtonLEDs_TurnAllOff(void);
void HAL_PowerButtonLEDs_TurnAllOn(void);
void HAL_PowerButtonLEDs_TurnLEDOn(PowerLEDs LED);
void HAL_PowerButtonLEDs_TurnLEDOff(PowerLEDs LED);

#ifdef __cplusplus
}
#endif

#endif 

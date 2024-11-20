/**
 ********************************************************************************
 * @file    hal_butt_pwr_LEDs.c
 * @author  macie
 * @date    Nov 16, 2024
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "hal_butt_pwr_LEDs.h"
#include "hal_buttons.h"
#include "butt_pwr_LEDs_effects.h"
/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define REFRESH_POWER_LED_TASK_AT_INIT 10

#define RED_REFRESH_TIME_AT_INIT		10
#define GREEN_REFRESH_TIME_AT_INIT 	10
#define YELLOW_REFRESH_TIME_AT_INIT	10
/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/

/************************************
 * GLOBAL VARIABLES
 ************************************/
PowerLEDs_Config_t PowerLEDs_Config =
{
		.Delay_Task_ms = REFRESH_POWER_LED_TASK_AT_INIT,

		.Green_RefreshStyle_Cb 	= NULL,
		.Yellow_RefreshStyle_Cb = NULL,
		.Red_RefreshStyle_Cb 	= NULL,

		.Green_IsTurnedOn 	= LED_TURNED_OFF,
		.Red_IsTurnedOn 	= LED_TURNED_OFF,
		.Yellow_IsTurnedOn 	= LED_TURNED_OFF,


		.Green_Refresh_Time 	= GREEN_REFRESH_TIME_AT_INIT,
		.Red_Refresh_Time 		= RED_REFRESH_TIME_AT_INIT,
		.Yellow_Refresh_Time 	= YELLOW_REFRESH_TIME_AT_INIT
};
/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void HAL_PowerButtonLEDs_Task(void)
{
	Butt_pwr_LEDs_effect_refresh_Red();
	Butt_pwr_LEDs_effect_refresh_Yellow();
	Butt_pwr_LEDs_effect_refresh_Green();
}

void HAL_PowerButtonLEDs_Init(void)
{
	HAL_TIM_PWM_Start(TIM_HANDLER_RED, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(TIM_HANDLER_GREEN, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(TIM_HANDLER_YELLOW, TIM_CHANNEL_1);

	HAL_PowerButtonLEDs_TurnLEDOn(POWER_LEDS_RED);
	PowerLEDs_Config.Red_RefreshStyle_Cb = RefreshLEDStyles[UPANDDOWN_REFRESHSTYLE];
}

void HAL_PowerButtonLEDs_TurnAllOff(void)
{
	PowerLEDs_Config.Red_IsTurnedOn = 0;
	PowerLEDs_Config.Green_IsTurnedOn = 0;
	PowerLEDs_Config.Yellow_IsTurnedOn = 0;
}

void HAL_PowerButtonLEDs_TurnAllOn(void)
{
	PowerLEDs_Config.Red_IsTurnedOn = 1;
	PowerLEDs_Config.Green_IsTurnedOn = 1;
	PowerLEDs_Config.Yellow_IsTurnedOn = 1;
}

void HAL_PowerButtonLEDs_TurnLEDOn(PowerLEDs LED)
{
	switch (LED)
	{
		case POWER_LEDS_RED:
			PowerLEDs_Config.Red_IsTurnedOn = 1;
			break;
		case POWER_LEDS_GREEN:
			PowerLEDs_Config.Green_IsTurnedOn = 1;
			break;
		case POWER_LEDS_YELLOW:
			PowerLEDs_Config.Yellow_IsTurnedOn = 1;
			break;
		default:
			break;
	}
}

void HAL_PowerButtonLEDs_TurnLEDOff(PowerLEDs LED)
{
	switch (LED)
	{
		case POWER_LEDS_RED:
			PowerLEDs_Config.Red_IsTurnedOn = 0;
			break;
		case POWER_LEDS_GREEN:
			PowerLEDs_Config.Green_IsTurnedOn = 0;
			break;
		case POWER_LEDS_YELLOW:
			PowerLEDs_Config.Yellow_IsTurnedOn = 0;
			break;
		default:
			break;
	}
}

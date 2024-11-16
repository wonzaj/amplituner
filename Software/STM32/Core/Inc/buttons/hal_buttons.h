/**
 ********************************************************************************
 * @file    hal_buttons.h
 * @author  macie
 * @date    Nov 15, 2024
 * @brief   
 ********************************************************************************
 */

#ifndef INC_BUTTONS_HAL_BUTTONS_H_
#define INC_BUTTONS_HAL_BUTTONS_H_

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

/************************************
 * TYPEDEFS
 ************************************/
typedef enum
{
	Button_Pressed,
	Button_Released
}Button_State_t;

typedef struct
{
	uint16_t GPIO_Pin;
	Button_State_t State;
}Queue_ButtonEvent_t;

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void HAL_Buttons_IRQ_TurnOff(void);
void HAL_Buttons_IRQ_TurnOn(void);
void HAL_Buttons_PowerButton_SetPWM(uint8_t pwm_value);
void HAL_Buttons_PowerButton_Init(void);

#ifdef __cplusplus
}
#endif

#endif

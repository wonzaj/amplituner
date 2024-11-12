#ifndef INC_BUTTONS_HAL_BUTTONS_H_
#define INC_BUTTONS_HAL_BUTTONS_H_

#include "config.h"
#include "main.h"
#include "app_buttons.h"
//#include "hal_encoders.h"

void HAL_Buttons_IRQ_TurnOff(void);
void HAL_Buttons_IRQ_TurnOn(void);
void HAL_Buttons_PowerButton_SetPWM(uint8_t pwm_value);
void HAL_Buttons_PowerButton_Init(void);

#endif /* INC_BUTTONS_HAL_BUTTONS_H_ */

#ifndef INC_TIME_MY_TIMERS_H_
#define INC_TIME_MY_TIMERS_H_

#include "main.h"
//#include "tim.h"
#include "config.h"
#include "Display_API.h"

void HAL_Timers_Init(void);
void HAL_Timers_OC_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);
void HAL_Timers_OC_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
uint32_t HAL_Timers_SysTick_GetMiliseconds(void);
uint32_t HAL_Timers_SysTick_GetSeconds(void);
void HAL_Timers_RefreshTimer(TIM_HandleTypeDef *htim, uint32_t Channel);
void set_saved_display(TIM_HandleTypeDef *htim);

#endif /* INC_TIME_MY_TIMERS_H_ */

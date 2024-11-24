#ifndef INC_ENCODERS_HAL_ENCODERS_H_
#define INC_ENCODERS_HAL_ENCODERS_H_

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------

#include "config.h"
#include "tim.h"
#include "main.h"

#include "app_buttons.h"
#include "Display_API.h"
#include "TDA7719.h"
#include "app_encoders.h"

//--------------------------------------------------------------
// Defines
//--------------------------------------------------------------

#define ENCODER_VOL_FRONT_TIMER 		TIM5
#define ENCODER_VOL_BACK_TIMER 		TIM3
#define ENCODER_VOL_TREBLE_TIMER 	TIM1
#define ENCODER_VOL_BASS_TIMER 		TIM2
#define ENCODER_VOL_RADIO_TIMER 		TIM4
#define ENCODER_VOL_LOUD_TIMER 		TIM8

#define ENCODER_VOL_FRONT 		&htim5
#define ENCODER_VOL_BACK 			&htim3
#define ENCODER_VOL_TREBLE		&htim1
#define ENCODER_VOL_BASS 			&htim2
#define ENCODER_VOL_RADIO 		&htim4
#define ENCODER_VOL_LOUD			&htim8

//--------------------------------------------------------------
// Variables
//--------------------------------------------------------------

//--------------------------------------------------------------
// Function declarations
//--------------------------------------------------------------
void HAL_Encoders_Init(void);
void HAL_Encoders_Middle_1_RisingEdge(void);
void HAL_Encoders_Middle_1_FallingEdge(void);
void HAL_Encoders_Middle_2_RisingEdge(void);
void HAL_Encoders_Middle_2_FallingEdge(void);
int32_t HAL_Encoders_Middle_GetRotateValue(void);

void HAL_Encoder_SingleEncoderStop(TIM_HandleTypeDef *htim);
void HAL_Encoder_SingleEncoderStart(TIM_HandleTypeDef *htim);

#endif /* INC_ENCODERS_HAL_ENCODERS_H_ */


//            CW --->
//  A (IRQ11)  ¯|___|¯¯¯¯|___|¯¯¯¯
//  Interrupts  ^   ^    ^   ^
//  B (IRQ 10) ¯¯¯|___|¯¯¯¯¯|___|¯
//  Interrupts    ^   ^     ^   ^
//            CCW <---

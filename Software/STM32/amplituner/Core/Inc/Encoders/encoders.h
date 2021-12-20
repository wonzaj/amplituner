#ifndef INC_ENCODERS_ENCODERS_H_
#define INC_ENCODERS_ENCODERS_H_

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "Display_OLED/draws_display.h"
#include "TDA7719/TDA7719.h"


//--------------------------------------------------------------
// Variables
//--------------------------------------------------------------
extern void save_display_state(SSD1322_Screen_t  Screen_State);
extern void TIM_state_Cos_tam();

extern uint8_t volumeMasterFlag;



//--------------------------------------------------------------
// Function declarations
//--------------------------------------------------------------
void encoders_init(void);

void volume_front_encoder_used(void);
void volume_back_encoder_used(void);
void loudness_encoder_used(void);
void middle_encoder_used(void);
void treble_encoder_used(void);
void radio_encoder_used(void);
void bass_encoder_used(void);

#endif /* INC_ENCODERS_ENCODERS_H_ */


//            CW --->
//  A (IRQ11)  ¯|___|¯¯¯¯|___|¯¯¯¯
//  Interrupts  ^   ^    ^   ^
//  B (IRQ 10) ¯¯¯|___|¯¯¯¯¯|___|¯
//  Interrupts    ^   ^     ^   ^
//            CCW <---

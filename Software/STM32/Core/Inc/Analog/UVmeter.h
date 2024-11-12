#ifndef INC_ANALOG_UVMETER_H_
#define INC_ANALOG_UVMETER_H_

//--------------------------------------------------------------
// Defines
//--------------------------------------------------------------
#define ADC_CHANNEL_LEFT_FRONT 1
#define ADC_CHANNEL_RIGHT_FRONT 2
#define ADC_CHANNEL_LEFT_BACK 3
#define ADC_CHANNEL_RIGHT_BACK 0


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
//#include "adc.h"
#include "Display/Display_API.h"
#include <buttons/app_buttons.h>

//--------------------------------------------------------------
// Function declarations
//--------------------------------------------------------------
//void ADC_calc_UV_value(uint8_t adc_channel_left, uint8_t adc_channel_right);
void calc_UVmeter_scale(void);
void ADC_calc_UV_value(uint8_t adc_channel_left, uint8_t adc_channel_right);

#endif /* INC_ANALOG_UVMETER_H_ */

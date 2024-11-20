#ifndef INC_AUDIO_VISUALIZER_UVMETER_H_
#define INC_AUDIO_VISUALIZER_UVMETER_H_

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
#include "adc.h"
#include "Display_API.h"
#include "app_buttons.h"
#include "fft.h"

#define UV_METER_FRONT 0
#define UV_METER_BACK  1

typedef uint16_t UV_meter_t;

extern
//--------------------------------------------------------------
// Function declarations
//--------------------------------------------------------------
void AudioVis_Start_Measurements(void);
void AudioVis_Stop_ADC_Measurements(void);
void AudioVis_ChangeSource_Front(void);
void AudioVis_ChangeSource_Back(void);
uint8_t AudioVis_GetSource(void);
void AudioVis_GetValuesForDisplay(UV_meter_t *left_channel, UV_meter_t *right_channel);
//void ADC_calc_UV_value(uint8_t adc_channel_left, uint8_t adc_channel_right);
void calc_UVmeter_scale(void);
void ADC_calc_UV_value(uint8_t adc_channel_left, uint8_t adc_channel_right);

#endif /* INC_AUDIO_VISUALIZER_UVMETER_H_ */

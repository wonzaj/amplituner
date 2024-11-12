#ifndef INC_ANALOG_FFT_H_
#define INC_ANALOG_FFT_H_

//--------------------------------------------------------------
// Defines
//--------------------------------------------------------------
#define FFT_SAMPLES 2048


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "math.h"
#include "stdint.h"
#include "app_buttons.h"
#include "app_encoders.h"
//#include "Adc/arm_math.h"
#include "float.h"

typedef enum
{
    FFT_ch_src_enum_MIN = 0,
    FFT_front_left  	= 1,
    FFT_front_right 	= 2,
    FFT_back_left   	= 3,
    FFT_back_right  	= 4,
    FFT_ch_src_enum_MAX = 5

}FFT_channel_source_e;

//--------------------------------------------------------------
// extern variables
//--------------------------------------------------------------
extern volatile uint8_t			Samples_ready;
extern FFT_channel_source_e 	FFT_channel_source;
extern uint16_t 				ADC_In[FFT_SAMPLES];
extern uint16_t					ADC_SamplesTEST[4];
extern uint8_t 					OutFreqArray[21];
extern float 					FFTInBuffer[FFT_SAMPLES];
extern volatile uint8_t 		FFT_calc_done;
extern float 					FFTOutBuffer[FFT_SAMPLES];
extern volatile uint8_t FFT_calc_done;
//extern arm_rfft_fast_instance_f32 FFTHandler;

//--------------------------------------------------------------
// Functions declarations
//--------------------------------------------------------------
void calc_FFT_scale(void);
void check_FFT_flag(void);
void CalculateFFT(void);
float complexABS(float real, float compl);


#endif /* INC_ANALOG_FFT_H_ */

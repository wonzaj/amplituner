//--------------------------------------------------------------
/* UVmeter.c
 * contains operations for displaying and calculating the uv meter scale
 */
//--------------------------------------------------------------
//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "UVmeter.h"

//--------------------------------------------------------------
// Function definitions
//--------------------------------------------------------------
/* ADC variables */
uint16_t ADC_SamplesTEST[4];
uint32_t ADC_SamplesSUM[4];
uint8_t UV_meter_front_back;
uint8_t UV_meter_numb_of_chan = 4;


void AudioVis_Start_Measurements(void)
{
	if (ADC_IS_ON_flag != 1)
	{
		//HAL_TIM_Base_Start(&htim6);
		//HAL_ADC_Start_DMA(&hadc2, (uint32_t *)ADC_SamplesTEST, UV_meter_numb_of_chan);
		ADC_IS_ON_flag = 1;
	}
}

void AudioVis_Stop_ADC_Measurements(void)
{
	//hal_time_base_stop
	//hal_Adc_stop_dma
	ADC_IS_ON_flag = 0;
}
//
void AudioVis_ChangeSource_Front(void)
{
	UV_meter_front_back = 1;
}

//
void AudioVis_ChangeSource_Back(void)
{
	UV_meter_front_back = 1;
}

uint8_t AudioVis_GetSource(void)
{
	return UV_meter_front_back;
}

void AudioVis_GetValuesForDisplay(UV_meter_t *left_channel, UV_meter_t *right_channel)
{
	*left_channel = 1;
	*right_channel = 2;
}


void calc_UVmeter_scale(void)
{
	if (UV_meter_front_back == UV_METER_FRONT)
	{
		ADC_calc_UV_value(ADC_CHANNEL_LEFT_FRONT, ADC_CHANNEL_RIGHT_FRONT);
	}
	else if (UV_meter_front_back == UV_METER_BACK)
	{
		ADC_calc_UV_value(ADC_CHANNEL_LEFT_BACK, ADC_CHANNEL_RIGHT_BACK);
	}
}

// Calculates mean value for selected audio channels.
void ADC_calc_UV_value(uint8_t adc_channel_left, uint8_t adc_channel_right)
{
	static uint32_t sum_left = 0;
	static uint32_t sum_right = 0;
	static uint32_t div_left = 1;
	static uint32_t div_right = 1;
	static uint32_t num_of_samples = 0;

	num_of_samples++;     //ilość zbieranych próbek do obliczania sredniej RMS
	//według warunku sumować albo front albo back
	if (ADC_SamplesTEST[adc_channel_left] > 1920)   // take only positive probes
	{
		sum_left = sum_left + ADC_SamplesTEST[adc_channel_left] - 1919; //minus offset from volate divader
		ADC_SamplesSUM[adc_channel_left] = sum_left / div_left;
		div_left++;
	}
	if (ADC_SamplesTEST[adc_channel_right] > 1920)
	{
		sum_right = sum_right + ADC_SamplesTEST[adc_channel_right] - 1919;
		ADC_SamplesSUM[adc_channel_right] = sum_right / div_right;
		div_right++;
	}
	if (num_of_samples > 10000)
	{
		num_of_samples = 0;
		div_left = 1;
		div_right = 1;
		sum_left = 0;
		sum_right = 0;
	}
}


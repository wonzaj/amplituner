//--------------------------------------------------------------
/* fft.c
 * contains operations for FFT
*/
//--------------------------------------------------------------



//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include <audio_visualizer/fft.h>

uint8_t OutFreqArray[21];
//arm_rfft_fast_instance_f32 FFTHandler;
uint16_t ADC_In[FFT_SAMPLES];
FFT_channel_source_e FFT_channel_source = 1;
float FFTInBuffer[FFT_SAMPLES];
float FFTOutBuffer[FFT_SAMPLES];
volatile uint8_t Samples_ready;
volatile uint8_t FFT_calc_done = 0;
volatile uint8_t ADC_IS_ON_flag = 0;

//--------------------------------------------------------------
// Functions definitions
//--------------------------------------------------------------
void FFT_Start_Measurements(void)
{
	if (ADC_IS_ON_flag != 1)
	{
		//HAL_TIM_Base_Start(&htim6);
		//HAL_ADC_Start_DMA(&hadc2, (uint32_t *)ADC_SamplesTEST, UV_meter_numb_of_chan);
		ADC_IS_ON_flag = 1;
	}
}

void FFT_Stop_ADC_Measurements(void)
{
	//hal_time_base_stop
	//hal_Adc_stop_dma
	ADC_IS_ON_flag = 0;
}
//
void FFT_ChangeSource_Up(void)
{
	FFT_channel_source--;
	if (FFT_channel_source == FFT_ch_src_enum_MIN)
	{
		FFT_channel_source = FFT_back_right;
	}
}

//
void FFT_ChangeSource_Down(void)
{
	FFT_channel_source++;
	if (FFT_channel_source >= FFT_ch_src_enum_MAX)
	{
		FFT_channel_source = FFT_front_left;
	}
}
//
void calc_FFT_scale(void)
    {
	static uint16_t i = 0;

	switch(FFT_channel_source)
	{
	    case FFT_front_left:
		ADC_In[i] = ADC_SamplesTEST[1];
	    break;
	    case FFT_front_right:
		ADC_In[i] = ADC_SamplesTEST[2];
	    break;
	    case FFT_back_left:
		ADC_In[i] = ADC_SamplesTEST[3];
	    break;
	    case FFT_back_right:
		ADC_In[i] = ADC_SamplesTEST[0];
	    break;
	    default:
		break;
	}

	i++;

	if(i == FFT_SAMPLES)
	{
	    i = 0;
	    Samples_ready = 1;
	}
    }


// calculate abs
float complexABS(float real, float compl)
    {
	return sqrtf(real * real + compl * compl);
    }

//
void CalculateFFT(void)
    {
	FFT_calc_done = 1;

	//arm_rfft_fast_f32(&FFTHandler , FFTInBuffer , FFTOutBuffer , 0);

	int Freqs[FFT_SAMPLES];
	int FreqPoint = 0;
	int Offset = 60;     // variable noise floor offset

	// calculate abs values and linear-to-dB
	for(int i = 0; i < FFT_SAMPLES; i = i + 2)
	{
	    Freqs[FreqPoint] = (int) (20 * log10f(complexABS(FFTOutBuffer[i] , FFTOutBuffer[i + 1]))) - Offset;

	    if(Freqs[FreqPoint] < 0)
	    {
		Freqs[FreqPoint] = 0;
	    }
	    FreqPoint++;
	}
	//liczenie średniej z każdego przedziału
	//	for (uint32_t k = 0;  k < 1024;  k++)
	//	    {
	//
	//	    }

	OutFreqArray[0] = (uint8_t) Freqs[1];     // 20 Hz
	OutFreqArray[1] = (uint8_t) Freqs[3];     // 50 Hz
	OutFreqArray[2] = (uint8_t) Freqs[4];     // 75 Hz
	OutFreqArray[3] = (uint8_t) Freqs[6];     // 125 Hz
	OutFreqArray[4] = (uint8_t) Freqs[10];     // 200 Hz
	OutFreqArray[5] = (uint8_t) Freqs[17];     // 1000 Hz
	OutFreqArray[6] = (uint8_t) Freqs[24];     // 2200 Hz
	OutFreqArray[7] = (uint8_t) Freqs[35];     // 4500 Hz
	OutFreqArray[8] = (uint8_t) Freqs[47];     // 9000 Hz
	OutFreqArray[9] = (uint8_t) Freqs[59];     // 15000 Hz
	OutFreqArray[10] = (uint8_t) Freqs[82];     //
	OutFreqArray[11] = (uint8_t) Freqs[117];     // 63 Hz
	OutFreqArray[12] = (uint8_t) Freqs[186];     // 125 Hz
	OutFreqArray[13] = (uint8_t) Freqs[256];     // 250 Hz
	OutFreqArray[14] = (uint8_t) Freqs[326];     // 500 Hz
	OutFreqArray[15] = (uint8_t) Freqs[372];     // 1000 Hz
	OutFreqArray[16] = (uint8_t) Freqs[465];     // 2200 Hz
	OutFreqArray[17] = (uint8_t) Freqs[558];     // 4500 Hz
	OutFreqArray[18] = (uint8_t) Freqs[651];     // 9000 Hz
	OutFreqArray[19] = (uint8_t) Freqs[836];     // 15000 Hz
	OutFreqArray[20] = (uint8_t) Freqs[929];     // 15000 Hz
    }

//
void check_FFT_flag(void)
    {
	if(Samples_ready == 1)
	{
	    Samples_ready = 0;

	    for(uint32_t i = 0; i < FFT_SAMPLES; i++)
	    {
		FFTInBuffer[i] = (float) ADC_In[i];
	    }

	    CalculateFFT();
	}
    }

/**
 ********************************************************************************
 * @file    butt_pwr_LEDs_effects.c
 * @author  macie
 * @date    Nov 19, 2024
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "butt_pwr_LEDs_effects.h"
#include "hal_butt_pwr_LEDs.h"
#include "tim.h"
#include "math.h"
#include "stdlib.h"
/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
static uint8_t UpAndDownRefreshStyle(uint8_t pwm_val);
static uint8_t RefreshExponentialDim(uint8_t pwm_val);
static uint8_t RefreshSinusoidalBrighten(uint8_t pwm_val);
static uint8_t RefreshBreathingEffect(uint8_t pwm_val);
static uint8_t RefreshLogarithmicBrighten(uint8_t pwm_val);
static uint8_t RefreshStepDim(uint8_t pwm_val);
static uint8_t RefreshZigzagEffect(uint8_t pwm_val);
static uint8_t RefreshLinearDimSlow(uint8_t pwm_val);

const RefreshLEDStyle RefreshLEDStyles[NBR_OF_REFRESH_STYLES] =
{
		UpAndDownRefreshStyle,
		RefreshExponentialDim,
		RefreshSinusoidalBrighten,
		RefreshBreathingEffect,
		RefreshLogarithmicBrighten,
		RefreshStepDim,
		RefreshZigzagEffect,
		RefreshLinearDimSlow
};
/************************************
 * STATIC FUNCTIONS
 ************************************/
static uint8_t UpAndDownRefreshStyle(uint8_t pwm_val)
{
	static uint8_t direction = 1;

	if (direction)
	{
		pwm_val++;
		if (pwm_val >= 255)
		{
			direction = 0;
		}
	}
	else
	{
		pwm_val--;
		if (pwm_val == 0)
		{
			direction = 1;
		}
	}
	return pwm_val;
}

static uint8_t RefreshExponentialDim(uint8_t pwm_val)
{
    static uint8_t intensity = 255;

    if (intensity > 0)
    {
        intensity -= (intensity / 8) + 1;
    }
    else
    {
        intensity = 0;
    }

    pwm_val = intensity;
    return pwm_val;
}

static uint8_t RefreshSinusoidalBrighten(uint8_t pwm_val)
{
    static float phase = 0.0f;

    pwm_val = (uint8_t)(255 * (0.5f * (1.0f + sinf(phase))));
    phase += 0.1f;

    if (phase >= 2 * M_PI)
    {
        phase -= 2 * M_PI;
    }

    return pwm_val;
}

static uint8_t RefreshBreathingEffect(uint8_t pwm_val)
{
    static float phase = 0.0f;

    pwm_val = (uint8_t)(255 * (0.5f * (1.0f + sinf(phase))));
    phase += 0.05f;

    if (phase >= 2 * M_PI) {
        phase -= 2 * M_PI;
    }
    return pwm_val;
}

static uint8_t RefreshLogarithmicBrighten(uint8_t pwm_val)
{
    static uint8_t step = 0;

    if (step <= 255) {
        PowerLEDs_Config.Red_PWM_Val = (uint8_t)(logf(1 + step) / logf(256) * 255);
        step++;
    } else {
        step = 255;
    }
    return pwm_val;
}

static uint8_t RefreshStepDim(uint8_t pwm_val)
{
    static uint8_t intensity = 255;

    if (intensity > 0) {
        intensity -= (rand() % 20) + 1; // Losowe zmniejszenie jasności
        if (intensity > 255) {
            intensity = 0; // Zapobiegaj przepełnieniu
        }
    } else {
        intensity = 0;
    }

    pwm_val = intensity;
    return pwm_val;
}

static uint8_t RefreshZigzagEffect(uint8_t pwm_val)
{
    static int8_t direction = 1;
    static uint8_t intensity = 0;

    intensity += direction * 5;
    if (intensity >= 255) {
        intensity = 255;
        direction = -1;
    } else if (intensity <= 0) {
        intensity = 0;
        direction = 1;
    }

    pwm_val = intensity;
    return pwm_val;
}

static uint8_t RefreshLinearDimSlow(uint8_t pwm_val)
{
    static uint8_t intensity = 255;

    if (intensity > 0) {
        intensity--; // Wolne zmniejszanie jasności
    } else {
        intensity = 0; // Osiągnięto zero
    }

    pwm_val = intensity;
    return pwm_val;
}
/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void Butt_pwr_LEDs_effect_refresh_Red(void)
{
   static uint32_t Refresh_counter = 0;

   Refresh_counter++;

	if (Refresh_counter == PowerLEDs_Config.Red_Refresh_Time)
	{
		if (PowerLEDs_Config.Red_IsTurnedOn && PowerLEDs_Config.Red_RefreshStyle_Cb)
		{
			PowerLEDs_Config.Red_PWM_Val = PowerLEDs_Config.Red_RefreshStyle_Cb(PowerLEDs_Config.Red_PWM_Val);
			__HAL_TIM_SET_COMPARE(TIM_HANDLER_RED, TIM_CHANNEL_1, PowerLEDs_Config.Red_PWM_Val);
		}
	}
}

void Butt_pwr_LEDs_effect_refresh_Yellow(void)
{
	static uint32_t Refresh_counter = 0;

	Refresh_counter++;

	if (Refresh_counter == PowerLEDs_Config.Yellow_Refresh_Time)
	{
		if (PowerLEDs_Config.Yellow_IsTurnedOn && PowerLEDs_Config.Yellow_RefreshStyle_Cb)
		{
			PowerLEDs_Config.Yellow_PWM_Val = PowerLEDs_Config.Yellow_RefreshStyle_Cb(PowerLEDs_Config.Yellow_PWM_Val);
			__HAL_TIM_SET_COMPARE(TIM_HANDLER_YELLOW, TIM_CHANNEL_1, PowerLEDs_Config.Yellow_PWM_Val);
		}
	}
}

void Butt_pwr_LEDs_effect_refresh_Green(void)
{
	static uint32_t Refresh_counter = 0;

	Refresh_counter++;

	if (Refresh_counter == PowerLEDs_Config.Green_Refresh_Time)
	{
		if (PowerLEDs_Config.Green_IsTurnedOn && PowerLEDs_Config.Green_RefreshStyle_Cb)
		{
			PowerLEDs_Config.Green_PWM_Val = PowerLEDs_Config.Green_RefreshStyle_Cb(PowerLEDs_Config.Green_PWM_Val);
			__HAL_TIM_SET_COMPARE(TIM_HANDLER_GREEN, TIM_CHANNEL_1, PowerLEDs_Config.Green_PWM_Val);
		}
	}
}

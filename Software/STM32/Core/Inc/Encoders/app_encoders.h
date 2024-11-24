/**
 ********************************************************************************
 * @file    app_encoders1.h
 * @author  macie
 * @date    Nov 24, 2024
 * @brief   
 ********************************************************************************
 */

#ifndef INC_ENCODERS_APP_ENCODERS_H_
#define INC_ENCODERS_APP_ENCODERS_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "stdint.h"
#include "config.h"
#include "tim.h"
/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/
typedef struct
{
	uint8_t volumeMasterFlag;
	int16_t tempVolFrontLeft;
	int16_t tempVolFrontRight;
	int16_t tempVolBackLeft;
	int16_t tempVolBackRight;

}Device_config_Volumes_t;
/************************************
 * EXPORTED VARIABLES
 ************************************/
extern encoderFilter_t encoderFilterTreble;
extern encoderFilter_t encoderFilterMiddle;
extern encoderFilter_t encoderFilterBass;
extern encoderFilter_t encoderFilterLoudness;
extern encoder_t encoderVolFront;
extern encoder_t encoderVolBack;
/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void AppEncoders_EncoderVolFront_Rotated(void);
void AppEncoders_EncoderVolBack_Rotated(void);
void AppEncoders_EncoderTreble_Rotated(void);
void AppEncoders_EncoderBass_Rotated(void);
void AppEncoders_EncoderMiddle_Rotated(void);
void AppEncoders_EncoderRadio_Rotated(void);
void AppEncoders_EncoderLoudness_Rotated(void);

void AppEncoders_SingleEncoderStop(TIM_HandleTypeDef *htim);
void AppEncoders_SingleEncoderStart(TIM_HandleTypeDef *htim);
void check_volumes_ranges(void);

#ifdef __cplusplus
}
#endif

#endif 

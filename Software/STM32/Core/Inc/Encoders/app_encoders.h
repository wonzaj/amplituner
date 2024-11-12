#ifndef INC_ENCODERS_APP_ENCODERS_H_
#define INC_ENCODERS_APP_ENCODERS_H_

#include "hal_encoders.h"
#include "TDA7719.h"
#include "config.h"
#include "tim.h"

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

#endif /* INC_ENCODERS_APP_ENCODERS_H_ */

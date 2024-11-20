/**
 ********************************************************************************
 * @file    Display_Draws.h
 * @author  macie
 * @date    Nov 13, 2024
 * @brief   
 ********************************************************************************
 */

#ifndef INC_DISPLAY_DISPLAY_DRAWS_H_
#define INC_DISPLAY_DISPLAY_DRAWS_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "stdint.h"
#include "stdlib.h"

#include "config.h"
#include "const_grafics.h"
#include "app_buttons.h"

// Fonts
#include "FreeMono9pt7b.h"
#include "Maciek.h"
#include "FreeSerifItalic24pt7b.h"
#include "FreeSerifItalic9pt7b.h"
#include "FreeSerifBold9pt7b.h"
#include "custom_chars.h"
#include "FreeSerifBoldItalic24pt7b.h"
#include "FreeSerifBoldItalic9pt7b.h"
#include "FreeSerif9pt7b.h"
#include "TomThumb.h"
#include "FreeMonoOblique9pt7b.h"
#include "FreeSansBold9pt7b.h"
#include "FreeSans9pt7b.h"
#include "FreeSerif24pt7b.h"

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void draw_encoder_volume_front_scale(uint8_t *const buffer);
void draw_encoder_volume_back_scale(uint8_t *const buffer);
void draw_gain_and_freq_loudness(uint8_t *const buffer, char *input_string);
void draw_gain_and_freq(int8_t gainValue, char *input_string);
void draw_UV_rectangle_scale(uint8_t *const buffer, const UV_meter_t left_channel, const UV_meter_t right_channel);
void draw_UV_lines_scale(uint8_t *const buffer, const UV_meter_t left_channel, const UV_meter_t right_channel);

void draw_nutka1(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y);
void draw_nutka2(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y);
void draw_nutka3(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y);
void draw_nutka4(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y);
void draw_nutka5(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y);
void draw_speaker_left(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y);
void draw_speaker_right(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y);
void draw_freq_scale(uint8_t *const buffer, const uint16_t freq_scaled);
void draw_alarmsSource(uint8_t *const buffer);
void draw_pointer(uint8_t *const buffer);
void draw_refreshTime(uint8_t *const buffer);
void draw_powerLED(uint8_t *const buffer);
void draw_displayMode(uint8_t *const buffer);

void ChangeDateToArrayChar(char *input_string, uint16_t frq);
void ChangeDateToArrayCharTime(char *arrayChar,uint8_t hours, uint8_t minutes, uint8_t seconds,uint8_t mode);
void ConvertDateToBuffer(char *input_string, uint16_t Year, uint8_t Month, uint8_t WeekDay, uint8_t Date);
int16_t SplitNumberToDignits(int16_t position, int16_t number);
uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);

_Bool prepare_RDS_text(char *RDS_text);
_Bool  get_random_coords(uint32_t *random_x, uint32_t *random_y);
void make_array(uint8_t *frame_buffer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t brightness);

#ifdef __cplusplus
}
#endif

#endif 

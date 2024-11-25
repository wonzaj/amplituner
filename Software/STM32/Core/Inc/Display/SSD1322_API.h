#ifndef SSD1322_API_H
#define SSD1322_API_H

//--------------------------------------------------------------
// Includes
//
#include "SSD1322_HW_Driver.h"
#include "SSD1322_GFX.h"
//#include <Display_OLED/draws_display.h>
//#include "tim.h"
//#include "TDA7719/TDA7719.h"

//--------------------------------------------------------------
// Commands defines
//--------------------------------------------------------------
#define ENABLE_GRAYSCALE_TABLE     0x00
#define SET_COLUMN_ADDR            0x15
#define ENABLE_RAM_WRITE           0x5C
#define SET_ROW_ADDR               0x75
#define SET_REMAP_AND_DUAL_COM     0xA0
#define SET_DISP_START_LINE        0xA1
#define SET_DISP_OFFSET            0xA2

#define SET_DISP_MODE_OFF          0xA4
#define SET_DISP_MODE_ON           0xA5
#define SET_DISP_MODE_NORMAL       0xA6
#define SET_DISP_MODE_INVERTED     0xA7

#define ENABLE_PARTIAL_MODE        0xA8
#define DISABLE_PARTIAL_MODE       0xA9

#define SLEEP_MODE_ON              0xAE
#define SLEEP_MODE_OFF             0xAF

#define SET_PHASE_LENGTH           0xB1
#define SET_FRONT_CLOCK_DIV        0xB3
#define DISP_ENCHANCEMENT          0xB4
#define SET_2ND_PRECHARGE_PERIOD   0xB6
#define SET_GRAYSCALE_TABLE        0xB8
#define SET_DEFAULT_GRAYSCALE_TAB  0xB9
#define SET_PRECHARGE_VOLTAGE      0xBB
#define SET_V_COMH                 0xBE
#define SET_CONTRAST_CURRENT       0xC1
#define MASTER_CONTRAST_CURRENT    0xC7
#define SET_MUX_RATIO              0xCA
#define SET_COMMANDS_LOCK          0xFD

#define ACTIVATE_SCROLL 		0x2F
#define DEACTIVATE_SCROLL 		0x2E
#define SET_VERTICAL_SCROLL_AREA 	0xA3
#define RIGHT_HORIZONTAL_SCROLL 	0x26
#define LEFT_HORIZONTAL_SCROLL 		0x27
#define VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

enum SSD1322_mode_e
{
	SSD1322_MODE_ON, SSD1322_MODE_OFF, SSD1322_MODE_NORMAL, SSD1322_MODE_INVERTED
};

//extern TIM_HandleTypeDef htim;
//extern const GFXfont 	 MACIEK_FONT;
//--------------------------------------------------------------
// Function declarations
//--------------------------------------------------------------

void SSD1322_API_Send_Command(uint8_t command);
void SSD1322_API_Send_Data(uint8_t data);
void DisplayDriver_API_Init(void);
void SSD1322_API_Sleep_On(void);
void SSD1322_API_Sleep_Off(void);
void SSD1322_API_Set_DisplayMode(enum SSD1322_mode_e mode);
void SSD1322_API_Set_Contrast(uint8_t contrast);
void SSD1322_API_Set_Brightness(uint8_t brightness);
void SSD1322_API_Set_Window(uint8_t start_column, uint8_t end_column, uint8_t start_row, uint8_t end_row);
uint8_t SSD1322_API_Set_CustomGrayscale(uint8_t* grayscale_tab);
void SSD1322_API_Set_DefaultGrayscale(void);
void SSD1322_API_Send_Buffer(uint8_t* buffer, uint32_t buffer_size);

#endif /* SSD1322_API_H */

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "SSD1322_API.h"

extern uint8_t saved_seconds;
extern uint8_t saved_minutes;
extern volatile _Bool is_display_on_standby_flag;
//extern RTC_TimeTypeDef sTime;
//extern RTC_DateTypeDef sDate;
//====================== command ========================//
/**
 *  @brief Sends command byte to SSD1322
 */
void SSD1322_API_Send_Command(uint8_t command)
{
	SSD1322_HW_SetPin_CS_LOW();
	SSD1322_HW_SetPin_DC_LOW();
	SSD1322_HW_SPI_TX_Byte(command);
	SSD1322_HW_SetPin_CS_HIGH();
}

//====================== data ========================//
/**
 *  @brief Sends data byte to SSD1322
 */
void SSD1322_API_Send_Data(uint8_t data)
{
	SSD1322_HW_SetPin_CS_LOW();
	SSD1322_HW_SetPin_DC_HIGH();
	SSD1322_HW_SPI_TX_Byte(data);
	SSD1322_HW_SetPin_CS_HIGH();
}

//====================== initialization sequence ========================//
/**
 *  @brief Initializes SSD1322 OLED display.
 */
void DisplayDriver_API_Init()
{
	SSD1322_HW_SetPin_RESET_LOW();  			//Reset pin low
	SSD1322_HW_msDelay(1);                  	//1ms delay
	SSD1322_HW_SetPin_RESET_HIGH(); 			//Reset pin high
	SSD1322_HW_msDelay(50);                 	//50ms delay
	SSD1322_API_Send_Command(0xFD);     		//set Command unlock
	SSD1322_API_Send_Data(0x12);
	SSD1322_API_Send_Command(0xAE);     		//set display off
	SSD1322_API_Send_Command(0xB3);     		//set display clock divide ratio
	SSD1322_API_Send_Data(0x91);
	SSD1322_API_Send_Command(0xCA);     		//set multiplex ratio
	SSD1322_API_Send_Data(0x3F);
	SSD1322_API_Send_Command(0xA2);  		 //set display offset to 0
	SSD1322_API_Send_Data(0x00);
	SSD1322_API_Send_Command(0xA1);   		//start display start line to 0
	SSD1322_API_Send_Data(0x00);
	SSD1322_API_Send_Command(0xA0);   		//set remap and dual COM Line Mode
	SSD1322_API_Send_Data(0x14);
	SSD1322_API_Send_Data(0x11);
	SSD1322_API_Send_Command(0xB5);   		//disable IO input
	SSD1322_API_Send_Data(0x00);
	SSD1322_API_Send_Command(0xAB);   		//function select
	SSD1322_API_Send_Data(0x01);
	SSD1322_API_Send_Command(0xB4);   		//enable VSL extern
	SSD1322_API_Send_Data(0xA0);
	SSD1322_API_Send_Data(0xFD);
	SSD1322_API_Send_Command(0xC1);   		//set contrast current
	SSD1322_API_Send_Data(0xFF);
	SSD1322_API_Send_Command(0xC7);   		//set master contrast current
	SSD1322_API_Send_Data(0x0F);
	SSD1322_API_Send_Command(0xB9);   		//default grayscale
	SSD1322_API_Send_Command(0xB1);   		//set phase length
	SSD1322_API_Send_Data(0xE2);
	SSD1322_API_Send_Command(0xD1);   		//enhance driving scheme capability
	SSD1322_API_Send_Data(0x82);
	SSD1322_API_Send_Data(0x20);
	SSD1322_API_Send_Command(0xBB);   		//first pre charge voltage
	SSD1322_API_Send_Data(0x1F);
	SSD1322_API_Send_Command(0xB6);   		//second pre charge voltage
	SSD1322_API_Send_Data(0x08);
	SSD1322_API_Send_Command(0xBE);   		//VCOMH
	SSD1322_API_Send_Data(0x07);
	SSD1322_API_Send_Command(0xA6);   		//set normal display mode
	SSD1322_API_Send_Command(0xA9);   		//no partial mode
	SSD1322_HW_msDelay(10);               		//stabilize VDD
	SSD1322_API_Send_Command(0xAF);   		//display on
	SSD1322_HW_msDelay(50);               	//stabilize VDD
}

//====================== go to sleep ========================//
/**
 *  @brief Go to sleep mode.
 */
void SSD1322_API_Sleep_On()
{
	SSD1322_API_Send_Command(SLEEP_MODE_ON);
}

//====================== wake up from sleep ========================//
/**
 *  @brief Wake up from sleep mode.
 */
void SSD1322_API_Sleep_Off()
{
	SSD1322_API_Send_Command(SLEEP_MODE_OFF);
}

//====================== normal/inversion ========================//
/**
 *  @brief Sets display mode to normal/inverted/off/on
 *
 *  When mode is set to ON or OFF, display ignores GRAM data and is always on or off.
 */

void SSD1322_API_Set_DisplayMode(enum SSD1322_mode_e mode)
{
	switch (mode)
	{
	case SSD1322_MODE_NORMAL:
		SSD1322_API_Send_Command(SET_DISP_MODE_NORMAL);
		break;
	case SSD1322_MODE_INVERTED:
		SSD1322_API_Send_Command(SET_DISP_MODE_INVERTED);
		break;
	case SSD1322_MODE_ON:
		SSD1322_API_Send_Command(SET_DISP_MODE_ON);
		break;
	case SSD1322_MODE_OFF:
		SSD1322_API_Send_Command(SET_DISP_MODE_OFF);
		break;
	}
}

//====================== contrast ========================//
/**
 *  @brief Sets contrast between brightest and darkest pixels.
 */
void SSD1322_API_Set_Contrast(uint8_t contrast)
{
	SSD1322_API_Send_Command(SET_CONTRAST_CURRENT);
	SSD1322_API_Send_Data(contrast);
}

//====================== brightness ========================//
/**
 *  @brief Should set brightness, but actual effect is similar to setting contrast.
 */
void SSD1322_API_Set_Brightness(uint8_t brightness)
{
	SSD1322_API_Send_Command(MASTER_CONTRAST_CURRENT);
	SSD1322_API_Send_Data(0x0F & brightness);        //first 4 bits have to be 0
}

//====================== window to draw into ========================//
/**
 *  @brief Sets range of pixels to write to.
 *
 *  @param[in] start_column
 *  @param[in] end_column
 *  @param[in] start_row
 *  @param[in] end_row
 */
void SSD1322_API_Set_Window(uint8_t start_column, uint8_t end_column, uint8_t start_row, uint8_t end_row)
{
	SSD1322_API_Send_Command(SET_COLUMN_ADDR);  //set columns range
	SSD1322_API_Send_Data(28 + start_column);
	SSD1322_API_Send_Data(28 + end_column);
	SSD1322_API_Send_Command(SET_ROW_ADDR);  //set rows range
	SSD1322_API_Send_Data(start_row);
	SSD1322_API_Send_Data(end_row);
}
//====================== custom grayscale ========================//
/**
 *  @brief Upload custom grayscale table to SSD1322.
 *
 *  Uploads exact values for 4-bit grayscale levels. Grayscale levels can have values from 0 to 180
 *  and have to meet following condition:
 *
 *  G0 < G1 < G2 < ... < G14 < G15
 *
 *  Where Gx is value of grayscale level
 *
 *  @param[in] grayscale_tab array of 16 brightness values
 *
 *  @return 0 when levels are out of range, 1 if function has ended correctly
 */
uint8_t SSD1322_API_Set_CustomGrayscale(uint8_t *grayscale_tab)
{
	SSD1322_API_Send_Command(SET_GRAYSCALE_TABLE);
	for (int i = 0; i < 16; i++)
	{
		if (grayscale_tab[i] > 180)
			return 0;
		SSD1322_API_Send_Data(grayscale_tab[i]);
	}
	SSD1322_API_Send_Command(ENABLE_GRAYSCALE_TABLE);
	return 1;
}

//====================== default grayscale ========================//
/**
 *  @brief Reset grayscale levels to default (linear)
 */
void SSD1322_API_Set_DefaultGrayscale()
{
	SSD1322_API_Send_Command(SET_DEFAULT_GRAYSCALE_TAB);
}

//====================== send pixel data to display ========================//
/**
 *  @brief Sends pixels buffer to SSD1322 GRAM memory.
 *
 *  This function should be always preceded by SSD1322_API_set_window() to specify range of rows and columns.
 *
 *  @param[in] buffer array of pixel values
 *  @param[in] buffer_size amount of bytes in the array
 */
void SSD1322_API_Send_Buffer(uint8_t *buffer, uint32_t buffer_size)
{
	SSD1322_API_Send_Command(ENABLE_RAM_WRITE);  //enable write of pixels
	SSD1322_HW_SetPin_CS_LOW();
	SSD1322_HW_SetPin_DC_HIGH();
	SSD1322_HW_SPI_TX_Array(buffer, buffer_size);
	SSD1322_HW_SetPin_CS_HIGH();
}

//====================== set change time of display ========================//
/**
 *  @brief sets time of refresh between displays states
 */
//void set_change_time_of_display(const uint16_t counter_period)
//    {
//	htim15.Instance->CCR1 = counter_period;
//    }
//====================== display on standby mode routine ========================//
/**
 *  @brief
 */
//void display_on_standby_mode(void)
//    {
//	if (Display_Controls.OnStandbyMode_flag == true)
//	    {
//		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
//		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
//		SSD1322_API_sleep_off();
//		saved_seconds = sTime.Seconds;
//		saved_minutes = sTime.Minutes;
//	    }
//    }
//====================== send frame buffer to OLED ========================//
/**
 *  @brief Sends frame buffer to OLED display.
 *
 *  Transmits frame buffer to OLED display.
 *
 *  If your frame buffer size is equal to OLED size, leave start_x and start_y at 0.
 *  If you use bigger frame buffer, you can use this parameters to choose region of your frame buffer
 *  that will be displayed on OLED. This is particularly useful for scrolling.
 *
 *  @param[in] frame_buffer
 *             array of pixel values
 *  @param[in] start_x
 *             x position of frame buffer part that will be displayed on OLED. Useful for horizontal scrolling.
 *  @param[in] start_y
 *             y position of frame buffer part that will be displayed on OLED Useful for vertical scrolling.

 */
void DisplayDriver_TX_ImageBuff(uint8_t *frame_buffer, uint16_t start_x, uint16_t start_y)
{
	SSD1322_API_Set_Window(0, 63, 0, 255);
	SSD1322_API_Send_Buffer(frame_buffer + (start_y * DISPLAY_WIDTH / 2) + start_x, 8192);
}

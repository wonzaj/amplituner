/**
 ********************************************************************************
 * @file    Display_Draws.c
 * @author  macie
 * @date    Nov 13, 2024
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "Display_Draws.h"
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

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void draw_encoder_volume_front_scale(uint8_t *const buffer)
{
	static uint16_t Volume;
	static uint16_t Volume1;
	static uint16_t Volume2;
	static uint16_t Volume3;
	static uint8_t i;

	switch (encoderVolFront.audioOutputState)
	{
	case MASTER:
		Volume = map(encoderVolFront.volumeMaster, 0, 94, 5, 230);
		DisplayGFX_DrawText(buffer, "Volume Master", 70, 15, 5);
		draw_rect_filled(buffer, 5, 56, Volume, 62, 5);
		draw_speaker_left(buffer, 3, 20);
		draw_speaker_right(buffer, 243, 20);
		draw_speaker_left(buffer, 10, 43);
		draw_speaker_right(buffer, 236, 43);
		draw_nutka1(buffer, 21, 10);
//		draw_nutka3(buffer, 26, 18);
		draw_nutka2(buffer, 31, 8);
		draw_nutka4(buffer, 32, 25);
		for (i = Volume; i < 230; i++)
		{
			if (i % 5 == 0)
				draw_vline(buffer, i, 56, 62, 3);
		}
		break;
	case MASTER_V2:
		Volume = map(encoderVolFront.volumeMaster, 0, 94, 5, 230);
		DisplayGFX_DrawText(buffer, "Volume Master", 70, 15, 5);
		draw_rect_filled(buffer, 5, 56, Volume, 62, 5);
		draw_speaker_left(buffer, 3, 20);
		draw_speaker_right(buffer, 243, 20);
		draw_speaker_left(buffer, 10, 43);
		draw_speaker_right(buffer, 236, 43);
		for (i = Volume; i < 230; i++)
		{
			if (i % 5 == 0)
				draw_vline(buffer, i, 56, 62, 3);
		}
		break;
	case NORMAL:
		Volume3 = map(encoderVolFront.volumeLeftRight, 0, 94, 5, 230);
		DisplayGFX_DrawText(buffer, "Volume front", 70, 15, 5);
		draw_rect_filled(buffer, 5, 56, Volume3, 62, 5);
		draw_speaker_left(buffer, 3, 20);
		draw_speaker_right(buffer, 243, 20);
		for (i = Volume3; i < 230; i++)
		{
			if (i % 5 == 0)
				draw_vline(buffer, i, 56, 62, 3);
		}
		break;
	case MUTE:
		DisplayGFX_DrawText(buffer, "Volume MUTED", 70, 15, 5);
		draw_rect_filled(buffer, 5, 56, 6, 62, 5);
		for (i = 0; i < 230; i++)
		{
			if (i % 5 == 0)
				draw_vline(buffer, i, 56, 62, 3);
		}
		break;
	case ATTE_LEFT:
		Volume1 = map(encoderVolFront.volumeLeft, 0, 94, 5, 230);
		DisplayGFX_DrawText(buffer, "Volume front left", 70, 15, 5);
		draw_rect_filled(buffer, 5, 56, Volume1, 62, 5);
		draw_speaker_left(buffer, 3, 20);
		for (i = Volume1; i < 230; i++)
		{
			if (i % 5 == 0)
				draw_vline(buffer, i, 56, 62, 3);
		}
		break;
	case ATTE_RIGHT:
		Volume2 = map(encoderVolFront.volumeRight, 0, 94, 5, 230);
		DisplayGFX_DrawText(buffer, "Volume front right", 70, 15, 5);
		draw_rect_filled(buffer, 5, 56, Volume2, 62, 5);
		draw_speaker_right(buffer, 243, 20);
		for (i = Volume2; i < 230; i++)
		{
			if (i % 5 == 0)
				draw_vline(buffer, i, 56, 62, 3);
		}
		break;
	default:
		break;
	}

}

void draw_encoder_volume_back_scale(uint8_t *const buffer)
{
	static uint16_t Volume1;
	static uint16_t Volume2;
	static uint16_t Volume3;
	static uint8_t i;

	switch (encoderVolBack.audioOutputState)
	{
	case NORMAL:
		Volume3 = map(encoderVolBack.volumeLeftRight, 0, 94, 5, 230);
		DisplayGFX_DrawText(buffer, "Volume back", 70, 15, 5);
		draw_rect_filled(buffer, 5, 56, Volume3, 62, 5);
		draw_speaker_left(buffer, 10, 43);
		draw_speaker_right(buffer, 236, 43);
		for (i = Volume3; i < 230; i++)
		{
			if (i % 5 == 0)
				draw_vline(buffer, i, 56, 62, 3);
		}
		break;
	case NORMAL_V2:
		Volume3 = map(encoderVolBack.volumeLeftRight, 0, 94, 5, 230);
		DisplayGFX_DrawText(buffer, "Volume back", 70, 15, 5);
		draw_rect_filled(buffer, 5, 56, Volume3, 62, 5);
		draw_speaker_left(buffer, 10, 43);
		draw_speaker_right(buffer, 236, 43);
		for (i = Volume3; i < 230; i++)
		{
			if (i % 5 == 0)
				draw_vline(buffer, i, 56, 62, 3);
		}
		break;
	case MUTE:
		DisplayGFX_DrawText(buffer, "Volume MUTED", 70, 15, 5);
		draw_rect_filled(buffer, 5, 56, 6, 62, 5);
		for (i = 0; i < 230; i++)
		{
			if (i % 5 == 0)
				draw_vline(buffer, i, 56, 62, 3);
		}
		break;
	case ATTE_LEFT:
		Volume1 = map(encoderVolBack.volumeLeft, 0, 94, 5, 230);
		DisplayGFX_DrawText(buffer, "Volume back left", 70, 15, 5);
		draw_rect_filled(buffer, 5, 56, Volume1, 62, 5);
		draw_speaker_left(buffer, 10, 43);
		for (i = Volume1; i < 230; i++)
		{
			if (i % 5 == 0)
				draw_vline(buffer, i, 56, 62, 3);
		}
		break;
	case ATTE_RIGHT:
		Volume2 = map(encoderVolBack.volumeRight, 0, 94, 5, 230);
		DisplayGFX_DrawText(buffer, "Volume Back right", 70, 15, 5);
		draw_rect_filled(buffer, 5, 56, Volume2, 62, 5);
		draw_speaker_right(buffer, 236, 43);
		for (i = Volume2; i < 230; i++)
		{
			if (i % 5 == 0)
				draw_vline(buffer, i, 56, 62, 3);
		}
		break;
	default:
		break;
	}

}

void draw_gain_and_freq_loudness(uint8_t *const buffer, char *input_string)
{
	if ((encoderFilterLoudness.gain > 0))
	{
		if (encoderFilterLoudness.gain > 10)
		{
			input_string[0] = ASCII_PLUS;
			input_string[1] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(1, encoderFilterLoudness.gain);
			input_string[2] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, encoderFilterLoudness.gain);
		}
		else
		{
			input_string[0] = ASCII_PLUS;
			input_string[1] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, encoderFilterLoudness.gain);
		}
	}
	else 	//gain == 0
	{
		input_string[0] = ASCII_DIGIT_OFFSET;
	}

	strcat(input_string, " dB");
	DisplayGFX_DrawText(buffer, (char*) input_string, 100, 63, 5);

	switch (encoderFilterLoudness.centerFreq)
	{
	case 0:		//00 - flat
		DisplayGFX_DrawText(buffer, "Flat", 100, 40, 5);
		break;
	case 1:		//01  - 400 Hz
		DisplayGFX_DrawText(buffer, "400 Hz", 100, 40, 5);
		break;
	case 2:		//10  - 800 Hz
		DisplayGFX_DrawText(buffer, "800 Hz", 100, 40, 5);
		break;
	case 3:		//11  - 2400 Hz
		DisplayGFX_DrawText(buffer, "2400 Hz", 100, 40, 5);
		break;
	default:
		break;
	}
}

void draw_gain_and_freq(int8_t gainValue, char *input_string)
{
	if (gainValue < 0)
	{
		gainValue = abs(gainValue);
		if (gainValue > 10)
		{
			input_string[0] = ASCII_MINUS;
			input_string[1] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(1, gainValue);
			input_string[2] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, gainValue);
		}
		else
		{
			input_string[0] = ASCII_MINUS;
			input_string[1] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, gainValue);
		}
	}
	else if ((gainValue > 0))
	{
		if (gainValue > 10)
		{
			input_string[0] = ASCII_PLUS;
			input_string[1] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(1, gainValue);
			input_string[2] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, gainValue);
		}
		else
		{
			input_string[0] = ASCII_PLUS;
			input_string[1] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, gainValue);
		}
	}
	else 	//gain == 0
	{
		input_string[0] = ASCII_DIGIT_OFFSET;
	}

	strcat(input_string, " dB");
}

//heigh - 6 pixels, width - 8 pixels
void draw_nutka1(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y)
{
	draw_rect_filled(buffer, begin_x, begin_y, begin_x + 1, begin_y + 1, 5); // x = 19   y = 19
	draw_rect_filled(buffer, begin_x + 4, begin_y, begin_x + 5, begin_y + 1, 5);
	draw_hline(buffer, begin_y - 5, begin_x + 2, begin_x + 7, 5);
	draw_vline(buffer, begin_x + 1, begin_y - 1, begin_y - 4, 5);
	draw_vline(buffer, begin_x + 5, begin_y - 1, begin_y - 4, 5);
	draw_pixel(buffer, begin_x + 2, begin_y - 4, 5);
}

//heigh - 5 pixels, width - 4 pixels
void draw_nutka2(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y)
{
	draw_rect_filled(buffer, begin_x, begin_y, begin_x + 1, begin_y + 1, 5);
	draw_vline(buffer, begin_x + 1, begin_y - 1, begin_y - 3, 5);
	draw_hline(buffer, begin_y - 4, begin_x + 1, begin_x + 4, 5);
	draw_pixel(buffer, begin_x + 4, begin_y - 3, 5);
}

//heigh - 7 pixels, width - 6 pixels
void draw_nutka3(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y)
{
	draw_rect_filled(buffer, begin_x, begin_y, begin_x + 2, begin_y + 2, 5);
	draw_vline(buffer, begin_x + 2, begin_y - 1, begin_y - 6, 5);
	draw_hline(buffer, begin_y - 6, begin_x + 2, begin_x + 5, 5);
	draw_pixel(buffer, begin_x + 5, begin_y - 4, 5);
	draw_pixel(buffer, begin_x + 5, begin_y - 5, 5);
}

//heigh - 8 pixels, width - 11 pixels
void draw_nutka4(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y)
{
	draw_rect_filled(buffer, begin_x + 1, begin_y, begin_x + 2, begin_y + 2, 5);
	draw_rect_filled(buffer, begin_x + 6, begin_y, begin_x + 8, begin_y + 2, 5);
	draw_vline(buffer, begin_x + 2, begin_y - 1, begin_y - 6, 5);
	draw_vline(buffer, begin_x + 8, begin_y - 1, begin_y - 7, 5);
	draw_hline(buffer, begin_y - 7, begin_x + 3, begin_x + 10, 5);
	draw_pixel(buffer, begin_x + 3, begin_y - 6, 5);
	draw_pixel(buffer, begin_x + 10, begin_y - 6, 5);
	draw_pixel(buffer, begin_x, begin_y + 1, 5);
	draw_pixel(buffer, begin_x, begin_y + 2, 5);
}

//heigh - 8 pixels, width - 6 pixels
void draw_nutka5(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y)

{
	draw_rect_filled(buffer, begin_x, begin_y, begin_x + 2, begin_y + 2, 5);
	draw_vline(buffer, begin_x + 2, begin_y - 1, begin_y - 6, 5);
	draw_hline(buffer, begin_y - 6, begin_x + 2, begin_x + 4, 5);
	draw_hline(buffer, begin_y - 5, begin_x + 4, begin_x + 5, 5);

}

//heigh - 11 pixels, width - 19 pixels
void draw_speaker_left(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y)
{
	draw_hline(buffer, begin_y, begin_x + 2, begin_x + 11, 5);
	draw_hline(buffer, begin_y - 16, begin_x + 3, begin_x + 11, 5);
	draw_vline(buffer, begin_x + 3, begin_y, begin_y - 16, 5);
	draw_vline(buffer, begin_x + 12, begin_y, begin_y - 16, 5);
	draw_pixel(buffer, begin_x + 1, begin_y - 1, 5);
	draw_vline(buffer, begin_x, begin_y - 2, begin_y - 19, 5);
	draw_hline(buffer, begin_y - 19, begin_x, begin_x + 9, 5);
	draw_pixel(buffer, begin_x + 10, begin_y - 18, 5);
	draw_pixel(buffer, begin_x + 11, begin_y - 17, 5);
	draw_pixel(buffer, begin_x + 1, begin_y - 18, 5);
	draw_pixel(buffer, begin_x + 2, begin_y - 17, 5);
	draw_circle(buffer, begin_x + 8, begin_y - 5, 2, 5);
	draw_circle(buffer, begin_x + 8, begin_y - 12, 1, 5);
}

//heigh - 11 pixels, width - 19 pixels
void draw_speaker_right(uint8_t *const buffer, const int16_t begin_x, const int16_t begin_y)
{
	draw_hline(buffer, begin_y, begin_x, begin_x + 8, 5);
	draw_hline(buffer, begin_y - 16, begin_x, begin_x + 8, 5);
	draw_vline(buffer, begin_x, begin_y, begin_y - 16, 5);
	draw_vline(buffer, begin_x + 9, begin_y, begin_y - 16, 5);
	draw_pixel(buffer, begin_x + 1, begin_y - 17, 5);
	draw_pixel(buffer, begin_x + 2, begin_y - 18, 5);
	draw_hline(buffer, begin_y - 19, begin_x + 3, begin_x + 11, 5);
	draw_pixel(buffer, begin_x + 9, begin_y - 17, 5);
	draw_pixel(buffer, begin_x + 10, begin_y - 18, 5);
	draw_vline(buffer, begin_x + 12, begin_y - 3, begin_y - 19, 5);
	draw_pixel(buffer, begin_x + 10, begin_y - 1, 5);
	draw_pixel(buffer, begin_x + 11, begin_y - 2, 5);
	draw_circle(buffer, begin_x + 4, begin_y - 5, 2, 5);
	draw_circle(buffer, begin_x + 4, begin_y - 12, 1, 5);
}

void draw_UV_rectangle_scale(uint8_t *const buffer, const UV_meter_t left_channel, const UV_meter_t right_channel)
{
	for (uint16_t a = 20; a < 254; a = a + 18)
	{
		if (left_channel < (a + 18))
		{
			draw_rect_filled(buffer, a, 16, a + 14, 26, 1);
		}
		else
		{
			draw_rect_filled(buffer, a, 16, a + 14, 26, 10);
		}

		if (right_channel < (a + 18))
		{
			draw_rect_filled(buffer, a, 40, a + 14, 50, 1);
		}
		else
		{
			draw_rect_filled(buffer, a, 40, a + 14, 50, 10);
		}
	}
}

void draw_UV_lines_scale(uint8_t *const buffer, const UV_meter_t left_channel, const UV_meter_t right_channel)
{
	for (uint16_t b = 20; b < 254; b = b + 3)
	{
		if (left_channel < b)
		{
			draw_vline(buffer, b, 10, 3, 1);
		}
		else
		{
			draw_vline(buffer, b, 10, 3, 10);
		}
		if (right_channel < b)
		{

			draw_vline(buffer, b, 63, 56, 1);
		}
		else
		{

			draw_vline(buffer, b, 63, 56, 10);
		}
	}
}

void draw_freq_scale(uint8_t *const buffer, uint16_t freq_scaled)
{
	for (uint8_t i = 40; i < 220; i++)
	{
		if (i % 5 == 0)
		{
			draw_vline(buffer, i, 63, 60, 5);
		}

		if (i % 15 == 0)
		{
			draw_vline(buffer, i, 57, 63, 5);
		}
		if (freq_scaled == i - 20)
		{
			//draw_vline(buffer, i, 53, 63, 5);
			draw_rect_filled(buffer, i, 52, i + 1, 63, 10);
		}
	}

	//select_font(&TomThumb);
	DisplayGFX_DrawText(buffer, "87,0", 33, 51, 5);
	DisplayGFX_DrawText(buffer, "108,0", 210, 51, 5);
}

void draw_alarmsSource(uint8_t *const buffer)
{

	DisplayGFX_SelectFont(&FreeMonoOblique9pt7b);
	switch (SettingsUserMenu.AlarmSource_A)
	{
	case JACK_1:
		DisplayGFX_DrawText(buffer, (char*) Jack_1, 110, 30, 5);
		break;
	case JACK_2:
		DisplayGFX_DrawText(buffer, (char*) Jack_2, 110, 30, 5);
		break;
	case RADIO:
		DisplayGFX_DrawText(buffer, (char*) Radio, 110, 30, 5);
		break;
	case BLUETOOTH:
		DisplayGFX_DrawText(buffer, (char*) Bluetooth, 110, 30, 5);
		break;
	case MICROPHONE_USB:
		DisplayGFX_DrawText(buffer, (char*) Microphone, 110, 30, 5);
		break;
	default:
		break;
	}

	DisplayGFX_SelectFont(&FreeMonoOblique9pt7b);
	switch (SettingsUserMenu.AlarmSource_B)
	{
	case JACK_1:
		DisplayGFX_DrawText(buffer, (char*) Jack_1, 110, 45, 5);
		break;
	case JACK_2:
		DisplayGFX_DrawText(buffer, (char*) Jack_2, 110, 45, 5);
		break;
	case RADIO:
		DisplayGFX_DrawText(buffer, (char*) Radio, 110, 45, 5);
		break;
	case BLUETOOTH:
		DisplayGFX_DrawText(buffer, (char*) Bluetooth, 110, 45, 5);
		break;
	case MICROPHONE_USB:
		DisplayGFX_DrawText(buffer, (char*) Microphone, 110, 45, 5);
		break;
	default:
		break;
	}
}

void draw_pointer(uint8_t *const buffer)
{

	DisplayGFX_SelectFont(&FreeSerif24pt7b);
	switch (SettingsUserMenu.SETTINGS_USER_MENU)
	{
	case REFRESH_SCREEN_TIME:
		DisplayGFX_DrawText(buffer, ".", 2, 11, 5);
		break;
	case ALARM_SOURCE_A:
		DisplayGFX_DrawText(buffer, ".", 2, 26, 5);
		break;
	case ALARM_SOURCE_B:
		DisplayGFX_DrawText(buffer, ".", 2, 41, 5);
		break;
	case USER_NAME:
		DisplayGFX_DrawText(buffer, ".", 2, 56, 5);
		break;
	case DISPLAY_MODE_ON_OFF:
		DisplayGFX_DrawText(buffer, ".", 2, 11, 5);
		break;
	case POWER_LED:
		DisplayGFX_DrawText(buffer, ".", 2, 26, 5);
		break;
	default:
		break;
	}
}

void draw_refreshTime(uint8_t *const buffer)
{
	switch (SettingsUserMenu.RefreshScreenTime)
	{
	case REFRESH_TIME_240s:
		DisplayGFX_DrawText(buffer, "240 s", 160, 15, 5);
		//also set double cnt flag
		break;
	case REFRESH_TIME_120s:
		DisplayGFX_DrawText(buffer, "120 s", 160, 15, 5);
		break;
	case REFRESH_TIME_60s:
		DisplayGFX_DrawText(buffer, "60 s", 160, 15, 5);
		break;
	case REFRESH_TIME_30s:
		DisplayGFX_DrawText(buffer, "30 s", 160, 15, 5);
		break;
	case REFRESH_TIME_10s:
		DisplayGFX_DrawText(buffer, "10 s", 160, 15, 5);
		break;
	case 65535:
		DisplayGFX_DrawText(buffer, "OFF", 160, 15, 5);
		break;
	default:
		break;
	}
}

void draw_powerLED(uint8_t *const buffer)
{
	DisplayGFX_SelectFont(&FreeMonoOblique9pt7b);
	switch (SettingsUserMenu.Power_LED)
	{
	case POWER_OFF:
		DisplayGFX_DrawText(buffer, (char*) Power_OFF_str, 125, 30, 5);
		break;
	case POWER_ON:
		DisplayGFX_DrawText(buffer, (char*) Power_ON_str, 125, 30, 5);
		break;
	case Always_OFF:
		DisplayGFX_DrawText(buffer, (char*) Always_OFF_str, 125, 30, 5);
		break;
	case ALWAYS_ON:
		DisplayGFX_DrawText(buffer, (char*) Always_ON_str, 125, 30, 5);
		break;
	case CHANGE_BRIGHTNESS:
		DisplayGFX_DrawText(buffer, (char*) Change_brigh_str, 125, 30, 5);
		break;
	default:
		break;
	}
}

void draw_displayMode(uint8_t *const buffer)
{
	DisplayGFX_SelectFont(&FreeMonoOblique9pt7b);
	switch (SettingsUserMenu.Display_mode)
	{
	case DISPLAY_NORMAL_MODE:
		DisplayGFX_DrawText(buffer, (char*) Disp_normal_str, 160, 15, 5);
		break;
	case DISPLAY_CHANGING:
		DisplayGFX_DrawText(buffer, (char*) Disp_changing_str, 160, 15, 5);
		break;
	case DISPLAY_STANDBY:
		DisplayGFX_DrawText(buffer, (char*) Disp_standby_str, 160, 15, 5);
		break;
	case DISPLAY_TIME:
		DisplayGFX_DrawText(buffer, (char*) Disp_time_str, 160, 15, 5);
		break;
	default:
		break;
	}
}

void ChangeDateToArrayChar(char *input_string, uint16_t frq)
{
	if (frq >= RADIO_100MHZ_FREQ && frq <= RADIO_MAX_FREQ)
	{
		input_string[4] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, frq);
		input_string[3] = PRZECINEK;
		input_string[2] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(1, frq);
		input_string[1] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(2, frq);
		input_string[0] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(3, frq);
		input_string[5] = ASCII_NULL;
	}
	else if (frq < RADIO_100MHZ_FREQ && frq >= RADIO_MIN_FREQ)
	{
		input_string[3] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, frq);
		input_string[2] = PRZECINEK;
		input_string[1] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(1, frq);
		input_string[0] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(2, frq);
		input_string[4] = ASCII_NULL;
	}
	else //Invalid frq value
	{

	}
}

void ChangeDateToArrayCharTime(char *arrayChar, uint8_t hours, uint8_t minutes, uint8_t seconds, uint8_t mode)

{
	if (hours < 9)
	{
		arrayChar[0] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, hours);
		arrayChar[1] = ASCII_DWUKROPEK;
		arrayChar[2] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(1, minutes);
		arrayChar[3] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, minutes);
		if (0 == mode)
		{
			arrayChar[4] = ASCII_DWUKROPEK;
			arrayChar[5] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(1, seconds);
			arrayChar[6] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, seconds);
			arrayChar[7] = ASCII_NULL;
		}
		else if (1 == mode)
		{
			arrayChar[4] = ASCII_NULL;
		}

	}
	else if (hours <= 24 && hours >= 10)
	{
		arrayChar[0] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(1, hours);
		arrayChar[1] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, hours);
		arrayChar[2] = ASCII_DWUKROPEK;
		arrayChar[3] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(1, minutes);
		arrayChar[4] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, minutes);
		if (0 == mode)
		{
			arrayChar[5] = ASCII_DWUKROPEK;
			arrayChar[6] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(1, seconds);
			arrayChar[7] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, seconds);
			arrayChar[8] = ASCII_NULL;
		}
		else if (1 == mode)
		{
			arrayChar[5] = ASCII_NULL;
		}
	}

}

//
int16_t SplitNumberToDignits(int16_t position, int16_t number)
{
	while (position--)
		number /= 10;
	return number % 10;
}

//
uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void ConvertDateToBuffer(char *input_string, uint16_t Year, uint8_t Month, uint8_t WeekDay, uint8_t Date)
{
	uint8_t temp = 0;

	for (uint8_t i = 0; i < 40; ++i)
	{
		input_string[i] = 0;
	}

	switch (WeekDay)
	{
	case 1:
		strcat(input_string, Poniedzialek);
		break;
	case 2:
		strcat(input_string, Wtorek);
		break;
	case 3:
		strcat(input_string, Sroda);
		break;
	case 4:
		strcat(input_string, Czwartek);
		break;
	case 5:
		strcat(input_string, Piatek);
		break;
	case 6:
		strcat(input_string, Sobota);
		break;
	case 7:
		strcat(input_string, Niedziela);
		break;
	default:
		break;
	}

	temp = strlen(input_string);
	input_string[temp] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(1, Date);
	input_string[temp + 1] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, Date);
	input_string[temp + 2] = PRZECINEK;

	switch (Month)
	{
	case 1:
		strcat(input_string, Styczen);
		break;
	case 2:
		strcat(input_string, Luty);
		break;
	case 3:
		strcat(input_string, Marzec);
		break;
	case 4:
		strcat(input_string, Kwiecien);
		break;
	case 5:
		strcat(input_string, Maj);
		break;
	case 6:
		strcat(input_string, Czerwiec);
		break;
	case 7:
		strcat(input_string, Lipiec);
		break;
	case 8:
		strcat(input_string, Sierpien);
		break;
	case 9:
		strcat(input_string, Wrzesien);
		break;
	case 10:
		strcat(input_string, Pazdziernik);
		break;
	case 11:
		strcat(input_string, Listopad);
		break;
	case 12:
		strcat(input_string, Grudzien);
		break;
	default:
		break;
	}
	temp = strlen(input_string);
	input_string[temp] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(3, Year);
	input_string[temp + 1] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(2, Year);
	input_string[temp + 2] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(1, Year);
	input_string[temp + 3] = ASCII_DIGIT_OFFSET + SplitNumberToDignits(0, Year);
	input_string[temp + 4] = ASCII_NULL;
}

_Bool prepare_RDS_text(char *RDS_text)
{

	//if first chars are empty spaces then its not a good string to display
	if ((RDStextbuffer[0] == ' ') && (RDStextbuffer[1] == ' ') && (RDStextbuffer[2] == ' ') && (RDStextbuffer[3] == ' '))
	{
		return false;
	}
	if ((RDStextbuffer[0] == 0x00) && (RDStextbuffer[1] == 0x00) && (RDStextbuffer[2] == 0x00) && (RDStextbuffer[3] == 0x00))
	{
		return false;
	}
	//checks for end of the string
	for (uint8_t i = 0; i < 66; i++)
	{
		if (RDStextbuffer[i] == ' ' && RDStextbuffer[i + 1] == ' ' && RDStextbuffer[i + 2] == ' ' && RDStextbuffer[i + 3] == ' ' && RDStextbuffer[i + 4] == ' ' && RDStextbuffer[i + 5] == ' ')
		{

			RDStextbuffer[i] = 0x00; //add end of the string
			if (i > 40)
			{
				RDS_text[i - 39] = RDStextbuffer[i];
			}
		}

	}

	return true;
}

_Bool get_random_coords(uint32_t *random_x, uint32_t *random_y)
{
	uint32_t temp;
	static uint32_t random2;

//	if (hrng.ErrorCode == 2) //dać inny warunek, uniwersalny
	{
//		HAL_RNG_DeInit(&hrng);
//		HAL_RNG_Init(&hrng);
	}

//	HAL_RNG_GenerateRandomNumber(&hrng, &random2);
//	if (hrng.Instance->SR == 1) //new random data ready
	{
//		temp = hrng.RandomNumber;
		*random_x = (uint8_t) temp; 	   //get 8 bits
		*random_y = (uint8_t) (temp >> 26); //get other 6 bits
		//don't let the clock be outside of display
		if (*random_x >= 195)
		{
			*random_x -= 59;
		}
		if (*random_y <= 13)
		{
			*random_y += 13;
		}
		return true;
	}
	return false;
}

void make_array(uint8_t *frame_buffer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t brightness)
{
	char temp_array[10] = {0};
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);


	if (steep == 1)
	{
		uint16_t tmp = y0;
		y0 = x0;
		x0 = tmp;
		tmp = y1;
		y1 = x1;
		x1 = tmp;
	}

	if (x0 > x1) //określa kierunek rysowania linii
	{
		uint16_t tmp = x0;
		x0 = x1;
		x1 = tmp;
		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1)
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}

	for (; x0 <= x1; x0++)
	{
		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

		if (steep)
		{
			if (Display_Controls.Screen_State != SCREEN_TIME_BOUNCING) return;

			DisplayDriver_FillBufferWithValue(frame_buffer, 0);
			HAL_Delay(50);
			ChangeDateToArrayCharTime(temp_array, sTime.Hours, sTime.Minutes, sTime.Seconds, 0);
			DisplayGFX_DrawText(frame_buffer, (char*) temp_array, y0, x0, brightness);
			DisplayDriver_TX_ImageBuff(frame_buffer, 0, 0);
		}
		else
		{
			if (Display_Controls.Screen_State != SCREEN_TIME_BOUNCING) return;

			DisplayDriver_FillBufferWithValue(frame_buffer, 0);
			HAL_Delay(50);
			ChangeDateToArrayCharTime(temp_array, sTime.Hours, sTime.Minutes, sTime.Seconds, 0);
			DisplayGFX_DrawText(frame_buffer, (char*) temp_array, x0, y0, brightness);
			DisplayDriver_TX_ImageBuff(frame_buffer, 0, 0);
			//można dodać flagę volatile która jest sprawdzana jeżeli zmieniono ekran
		}
		err -= dy;
		if (err < 0)
		{
			y0 += ystep;
			err += dx;
		}
	}
}

//--------------------------------------------------------------
/* Display_API.c
 * contains operations that draw on the display
 */
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "Display_API.h"
#include "const_grafics.h"
#include "Display_Draws.h"

extern SettingsUserMenu_t SettingsUserMenu;
extern Device_Cfg_Audio_t Device_Cfg_Audio;

//--------------------------------------------------------------
// Typedefs
//--------------------------------------------------------------
uint8_t Display_Buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 2];	//divided by 2 because one pixel takes 4 bits

Display_Controls_t Display_Controls =
{
		.Screen_State 				= SCREEN_TIME,
		.Screen_State_Saved 		= SCREEN_WELCOME,
		.Refresh_Hz 				= DISPLAY_REFRESH_TIME_HZ,
		.AutoChangeScreenTime_ms 	= DISPLAY_CHANGE_TO_NEXT_SCREEN_TIME,
		.AutoChangeToSavedTime_ms	= DISPLAY_BACK_FROM_SAVED_TIME_MS,
		.OnStandbyMode_flag 		= false,
		.CurrentDisplayMode 		= DISPLAY_NORMAL_MODE,
};

//--------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------
_Bool IS_ALARM_SET_A;
_Bool IS_ALARM_SET_B;

uint8_t settings_page = 0;
uint8_t saved_seconds = 0;
uint8_t saved_minutes = 0;
char user_name[10];

//--------------------------------------------------------------
// Static functions
//--------------------------------------------------------------
static void Display_Init(void);
static void Screen_Welcome(uint8_t *const buffer);
static void Screen_Time(uint8_t *const buffer);
static void Screen_Radio(uint8_t *const buffer);
static void Screen_WakeUp(uint8_t *const buffer);
static void Screen_FFT(uint8_t *const buffer, uint8_t *const FFT_out_buffer);
static void Screen_UVMeter(uint8_t *const buffer);
static void Screen_OFF(uint8_t *const buffer);
static void Screen_GoodBye(uint8_t *const buffer);
static void Screen_SetClock(uint8_t *const buffer);
static void Screen_SetAlarm(uint8_t *const buffer);
static void Screen_Settings(uint8_t *const buffer);
static void Screen_Snake(uint8_t *const buffer);
static void Screen_USB(uint8_t *const buffer);
static void Screen_SetInput(uint8_t *const buffer);
static void Screen_TimeBouncing(uint8_t *const buffer);
static void Screen_Encoder_VolumeFront(uint8_t *const buffer);
static void Screen_Encoder_Volume_Back(uint8_t *const buffer);
static void Screen_Encoder_Loudness(uint8_t *const buffer);
static void Screen_Encoder_Treble(uint8_t *const buffer);
static void Screen_Encoder_Middle(uint8_t *const buffer);
static void Screen_Encoder_Bass(uint8_t *const buffer);
/**
 *  @brief init display api and drivers
 */
static void Display_Init(void)
{
	DisplayDriver_API_Init();
	DisplayGFX_SelectFont(&MACIEK_FONT);
	DisplayDriver_FillBufferWithValue(Display_Buffer, 0);
	DisplayDriver_TX_ImageBuff(Display_Buffer, 0, 0);
}

//--------------------------------------------------------------
// Possible displayed screens
//--------------------------------------------------------------
// ??
static void Screen_Welcome(uint8_t *const buffer)
{
	DisplayGFX_DrawText(buffer, "Przyjemnosc ze sluchania ", 20, 15, 15);
	DisplayGFX_DrawText(buffer, "zapewnia ", 20, 36, 15);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
	HAL_Delay(200);
	draw_char(buffer, 'M', 20, 55, 15);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
	HAL_Delay(200);
	draw_char(buffer, 'A', 38, 55, 15);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
	HAL_Delay(200);
	draw_char(buffer, 'C', 53, 55, 15);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
	HAL_Delay(200);
	draw_char(buffer, 'I', 68, 55, 15);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
	HAL_Delay(200);
	draw_char(buffer, 'E', 78, 55, 15);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
	HAL_Delay(200);
	draw_char(buffer, 'J', 93, 55, 15);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
	HAL_Delay(1000);
	DisplayDriver_FillBufferWithValue(buffer, 0);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
// ??
static void Screen_Time(uint8_t *const buffer)
{
	char temp_array_time[10] = {0};
	char temp_array_date[40] = {0};

	// do zegara dodac wybor roznych czcionek
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	DisplayDriver_FillBufferWithValue(buffer, DISPLAY_BLACK);
	DisplayGFX_SelectFont(&FreeSerifItalic24pt7b);
	//wywoływać tylko co sekunde i nie sprawdzac nie potrzebne innych wartości

	/* Drawing Time */
	ChangeDateToArrayCharTime(temp_array_time, sTime.Hours, sTime.Minutes, sTime.Seconds, 0);
	DisplayGFX_DrawText(buffer, (char*) temp_array_time, 2, 32, 5);
	/* Drawing Date */
	DisplayGFX_SelectFont(&FreeSerifItalic9pt7b);
	ConvertDateToBuffer(temp_array_date, (2023 + sDate.Year), sDate.Month, sDate.WeekDay, sDate.Date);
	DisplayGFX_DrawText(buffer, (char*) temp_array_date, 2, 60, 5);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_Radio(uint8_t *const buffer)
{
	static uint16_t freq_scaled = 0;
	static uint16_t rssi_scaled = 0;
	static char RDStext[32];
	char temp_array_freq[6] = {0};

	if (RADIO_IS_ON_back_flag == true || RADIO_IS_ON_front_flag == true)
	{
		RDA5807_Read();
	}
	else
	{
		RDA5807_RDSinit();
	}

	freq_scaled = map(RDA5807_GetFrequency(), RADIO_MIN_FREQ, RADIO_MAX_FREQ, 20, 200);
	DisplayDriver_FillBufferWithValue(buffer, DISPLAY_BLACK);
	DisplayGFX_SelectFont(&FreeSerifBold9pt7b);

	ChangeDateToArrayChar(temp_array_freq, RDA5807_GetFrequency());
	DisplayGFX_DrawText(buffer, (char*) temp_array_freq, 20, 13, 5);

	/* draw radio info */
	DisplayGFX_SelectFont(&FreeSerifBold9pt7b);
	DisplayGFX_DrawText(buffer, (char *)StationName, 150, 13, 5);
	DisplayGFX_SelectFont(&MACIEK_FONT);
	if (prepare_RDS_text((char*) RDStext) == true)
	{
		DisplayGFX_DrawText(buffer, (char *)RDStextbuffer, 20, 32, 5); //zwraca stringa
		//if(another string avaible == true)
		{
			//draw in the same place after 2 sec
			//zrobić wtedy zmianę co 2 sec
		}
	}

	/* Scale for frequency */
	draw_freq_scale(buffer, freq_scaled);
	/* Draw RSSI wskaźnik */
	rssi_scaled = map(RDA5807_GetRSSI(), 0, 63, 0, 63);
	draw_rect_filled(buffer, 0, 62 - rssi_scaled, 10, 63, 5);
	/* Draw antenna */
	DisplayGFX_SelectFont(&Custon_chars);
	draw_char(buffer, '!', 0, 16, 5); // ! - anntena in custom chars

	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_WakeUp(uint8_t *const buffer)
{
	char temp_array[10] = {0};

	//zwiększanie głośności podczas budzenia
	DisplayDriver_FillBufferWithValue(buffer, DISPLAY_BLACK);
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	uint8_t Hours = sTime.Hours;
	uint8_t Minutes = sTime.Minutes;
	uint8_t Seconds = sTime.Seconds;

	uint8_t Mode = 0;

	DisplayGFX_SelectFont(&FreeSerifBoldItalic9pt7b);
	DisplayGFX_DrawText(buffer, "WSTAWAJ !!!", 2, 58, 5);
	ChangeDateToArrayCharTime(temp_array, Hours, Minutes, Seconds, Mode);
	DisplayGFX_SelectFont(&FreeSerifBoldItalic24pt7b);
	DisplayGFX_DrawText(buffer, (char*) temp_array, 2, 33, 5);
	//dodać budzik który bedzie sie ruszal, czyli odswiezac i zmieniac go dwa razy na sekunde
	//poprzez togglowanie flagi
	//albo usunac napis wstawaj i dac tylko czas i animacje budzika
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_FFT(uint8_t *const buffer, uint8_t *const FFT_out_buffer)
{
	if (FFT_calc_done == 1)
	{
		FFT_calc_done = 0;
		DisplayGFX_SelectFont(&FreeSerif9pt7b);
		DisplayDriver_FillBufferWithValue(buffer, 0);
		switch (FFT_channel_source)
			{
			case FFT_front_left:
				DisplayGFX_DrawText(buffer, (char*)FFT_front_left_str, 80, 12, 5);
			break;
			case FFT_front_right:
				DisplayGFX_DrawText(buffer, (char*)FFT_front_right_str, 80, 12, 5);
			break;
			case FFT_back_left:
				DisplayGFX_DrawText(buffer, (char*)FFT_back_left_str, 80, 12, 5);
			break;
			case FFT_back_right:
				DisplayGFX_DrawText(buffer, (char*)FFT_back_right_str, 80, 12, 5);
			break;
			default:
			break;
			}
		uint8_t j = 10; //begin of first prążek

		for (uint8_t i = 0; i < 22; i++)
		{
			if ((63 - OutFreqArray[i]) < 13) //set max heigh of prążek
			{
				draw_rect_filled(buffer, j, 10, j + 5, 63, 5);
			}
			else
			{
				draw_rect_filled(buffer, j, 60 - OutFreqArray[i], j + 5, 63, 5);
			}
			j = j + 10;
		}

		DisplayGFX_SelectFont(&TomThumb);
		DisplayGFX_DrawText(buffer, "50", 10, 5, 5);
		DisplayGFX_DrawText(buffer, "200", 50, 5, 5);
		DisplayGFX_DrawText(buffer, "2k", 140, 5, 5);
		DisplayGFX_DrawText(buffer, "20k", 220, 5, 5);

		DisplayDriver_TX_ImageBuff(buffer, 0, 0);
	}
}
//
static void Screen_UVMeter(uint8_t *const buffer)
{
	uint8_t Audio_Value_Source 	= AudioVis_GetSource();
	UV_meter_t Audio_LeftChannel 	= 0;
	UV_meter_t Audio_RightChannel 	= 0;

	AudioVis_GetValuesForDisplay(&Audio_LeftChannel, &Audio_RightChannel);

	DisplayGFX_SelectFont(&FreeSerifItalic9pt7b);
	DisplayDriver_FillBufferWithValue(buffer, 0);
	Audio_LeftChannel = map(Audio_LeftChannel, 20, 2100, 25, 254);
	Audio_RightChannel = map(Audio_RightChannel, 20, 2100, 25, 254);

	if(Audio_Value_Source == UV_METER_FRONT)
	{
		//draw front label
	}
	else if(Audio_Value_Source == UV_METER_BACK)
	{
		//draw back label
	}

	draw_UV_rectangle_scale(buffer, Audio_LeftChannel, Audio_RightChannel);
	draw_UV_lines_scale(buffer, Audio_LeftChannel, Audio_RightChannel);

	draw_char(buffer, 'L', 2, 23, 10); // powinien być wyświetlany czy to jest lewy front czy prawy front
	draw_char(buffer, 'P', 2, 55, 10);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_OFF(uint8_t *const buffer)
{
	DisplayGFX_SelectFont(&FreeSerif9pt7b);
	DisplayDriver_FillBufferWithValue(buffer, 0);
	DisplayGFX_DrawText(buffer, "Screen_OFF  (:", 5, 32, 5);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_GoodBye(uint8_t *const buffer)
{
	//dodać argument z wyświetlanym imieniem użytkownika i pożegnanie do niego
	DisplayDriver_FillBufferWithValue(buffer, 0);
	DisplayGFX_SelectFont(&FreeSerif9pt7b);
	DisplayGFX_DrawText(buffer, "GoodBye  (:", 5, 32, 5);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_SetClock(uint8_t *const buffer)
{
	DisplayDriver_FillBufferWithValue(buffer, 0);
	DisplayGFX_SelectFont(&FreeSerif9pt7b);
	DisplayGFX_DrawText(buffer, "SetClock  (:", 5, 32, 5);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_SetAlarm(uint8_t *const buffer)
{
	char temp_array[10] = {0};

	/* Alaways reset display buffer to zero*/
	DisplayDriver_FillBufferWithValue(buffer, DISPLAY_BLACK);
	DisplayGFX_SelectFont(&FreeSerifItalic9pt7b);

	/* Checks if display mode is set to preview set alarms or modify them  */
	if (PreviewAlarm == true)
	{
		/* Drawing a pointer to selected ALARM.  */
		if (alarm_set_A_or_B == true)
			DisplayGFX_DrawText(buffer, "--->", 135, 25, 5);
		if (alarm_set_A_or_B == false)
			DisplayGFX_DrawText(buffer, "--->", 135, 55, 5);

		/* Drawing filled rectangle if alarm A is SET otherwise just a rectangle */
		if (IS_ALARM_SET_A == false)
			draw_rect(buffer, 240, 15, 250, 25, 5);
		if (IS_ALARM_SET_A == true)
			draw_rect_filled(buffer, 240, 15, 250, 25, 5); //może zmienić na X
		//draw_rect	(buffer, x0, y0, x1, y1, 15);

		/* Drawing filled rectangle if alarm B is SET otherwise just a rectangle */
		if (IS_ALARM_SET_B == false)
			draw_rect(buffer, 240, 45, 250, 55, 5);
		else
			draw_rect_filled(buffer, 240, 45, 250, 55, 5); //może zmienić na X

		/* Drawing a pointer to selected ALARM.  */
		DisplayGFX_DrawText(buffer, "Alarm A", 172, 25, 5);
		DisplayGFX_DrawText(buffer, "Alarm B", 172, 55, 5);

		/* Drawing a pointer to selected ALARM.  */
		DisplayGFX_SelectFont(&FreeSerifItalic24pt7b);
		ChangeDateToArrayCharTime(temp_array, Alarm.AlarmTime.Hours, Alarm.AlarmTime.Minutes, 0, 1);
		DisplayGFX_DrawText(buffer, (char*) temp_array, 5, 32, 5);

		/* Drawing alarm mode */
		DisplayGFX_SelectFont(&FreeSerifItalic9pt7b);
		Set_Alarm_Mode(RTC_typeOfAlarm);
		DisplayGFX_DrawText(buffer, (char*) AlarmMode, 5, 60, 5);
	}
	else if (PreviewAlarm == false)
	{
		if ((IS_ALARM_SET_A == false) && (IS_ALARM_SET_B == false))
		{
			//select_font(&FreeMonoOblique24pt7b);
			DisplayGFX_DrawText(buffer, "ALARMY", 2, 30, 5);
			DisplayGFX_DrawText(buffer, "OFF", 2, 62, 5);
		}
		else
		{	//tutaj zmienic czcionkę na ładną
			if (IS_ALARM_SET_A == true)
			{
				DisplayGFX_SelectFont(&FreeSerif9pt7b);
				ChangeDateToArrayCharTime(temp_array, Alarm_A.AlarmTime.Hours, Alarm_A.AlarmTime.Minutes, 0, 1);
				DisplayGFX_DrawText(buffer, (char*) buffer, 2, 30, 5);
				Set_Alarm_Mode(RTC_typeOfAlarm_A);
				DisplayGFX_DrawText(buffer, (char*) AlarmMode, 55, 30, 5);
			}
			else if (IS_ALARM_SET_A == false)
			{
				DisplayGFX_DrawText(buffer, "OFF", 2, 30, 5);
			}
			if (IS_ALARM_SET_B == true)
			{
				DisplayGFX_SelectFont(&FreeSerif9pt7b);
				ChangeDateToArrayCharTime(temp_array, Alarm_B.AlarmTime.Hours, Alarm_B.AlarmTime.Minutes, 0, 1);
				DisplayGFX_DrawText(buffer, (char*) temp_array, 2, 60, 5);
				Set_Alarm_Mode(RTC_typeOfAlarm_B);
				DisplayGFX_DrawText(buffer, (char*) AlarmMode, 55, 60, 5);
			}
			else if (IS_ALARM_SET_B == false)
			{
				DisplayGFX_DrawText(buffer, "OFF", 2, 60, 5);
			}
			DisplayGFX_DrawText(buffer, "Alarm A", 190, 30, 5);
			DisplayGFX_DrawText(buffer, "Alarm B", 190, 60, 5);
		}
	}

	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_Settings(uint8_t *const buffer)
{
	DisplayGFX_SelectFont(&FreeMonoOblique9pt7b);
	DisplayDriver_FillBufferWithValue(buffer, 0);

	if (settings_page == PAGE_SETTINGS_1)
	{
		DisplayGFX_DrawText(buffer, "Screen time:", 15, 15, 5);
		DisplayGFX_DrawText(buffer, "Alarm A:", 15, 30, 5);
		DisplayGFX_DrawText(buffer, "Alarm B:", 15, 45, 5);
		DisplayGFX_DrawText(buffer, "User Name:", 15, 60, 5);

		draw_refreshTime(buffer);
		draw_alarmsSource(buffer);
		DisplayGFX_DrawText(buffer, (char*) user_name, 125, 60, 5);
	}
	else if (settings_page == PAGE_SETTINGS_2)
	{
		DisplayGFX_DrawText(buffer, "Display mode:", 15, 15, 5);
		DisplayGFX_DrawText(buffer, "Power LED:", 15, 30, 5);
		//another case for Instat power off/slow change volume to zero
		draw_powerLED(buffer);
		draw_displayMode(buffer);
		DisplayGFX_DrawText(buffer, "Alarm B:", 15, 45, 5);
		DisplayGFX_DrawText(buffer, "User Name:", 15, 60, 5);

	}

	draw_pointer(buffer);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_Snake(uint8_t *const buffer)
{
//	const static uint32_t width_screen = 256;
//	const static uint32_t heigh_screen = 64;
//
//	static uint32_t random_x = 0;
//	static uint32_t random_y = 0;
//
//	    int i, zarodek;
	//time_t tt;
	//zarodek = time(&tt);
	//srand(zarodek);   // za zarodek wstawiamy pobrany czas w sekundach

	//random_x = (rand()%100);
	//random_y = (int)(rand() / (RAND_MAX + 1.0) * 100.0);

	DisplayGFX_SelectFont(&FreeMonoOblique9pt7b);
	DisplayDriver_FillBufferWithValue(buffer, 0);

	DisplayGFX_DrawText(buffer, "SNAKE", 15, 15, 5);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_USB(uint8_t *const buffer)
{
	DisplayGFX_SelectFont(&FreeSerif9pt7b);
	DisplayDriver_FillBufferWithValue(buffer, 0);
	DisplayGFX_DrawText(buffer, "USB", 5, 32, 5);
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_SetInput(uint8_t *const buffer)
{
	DisplayGFX_SelectFont(&FreeSerif9pt7b);
	DisplayDriver_FillBufferWithValue(buffer, 0);
	DisplayGFX_DrawText(buffer, "Front input:", 10, 20, 5);
	DisplayGFX_DrawText(buffer, "Back  input:", 10, 50, 5);
	switch (TDA7719_config.set_input_front)
	    {
		case JACK_1:
		    DisplayGFX_DrawText(buffer, (char *)Jack_1, 100, 20, 5);
		break;
		case JACK_2:
		    DisplayGFX_DrawText(buffer, (char *)Jack_2, 100, 20, 5);
		break;
		case RADIO:
		    DisplayGFX_DrawText(buffer, (char *)Radio, 100, 20, 5);
		break;
		case BLUETOOTH:
		    DisplayGFX_DrawText(buffer, (char *)Bluetooth, 100, 20, 5);
		break;
		case MICROPHONE_USB:
		    DisplayGFX_DrawText(buffer, (char *)Microphone, 100, 20, 5);
		break;
		default:
		break;
	    }

	switch (TDA7719_config.set_input_back)
	    {
		case JACK_1:
		    DisplayGFX_DrawText(buffer, (char *)Jack_1, 100, 50, 5);
		break;
		case JACK_2:
		    DisplayGFX_DrawText(buffer, (char *)Jack_2, 100, 50, 5);
		break;
		case RADIO:
		    DisplayGFX_DrawText(buffer, (char *)Radio, 100, 50, 5);
		break;
		case BLUETOOTH:
		    DisplayGFX_DrawText(buffer, (char *)Bluetooth, 100, 50, 5);
		break;
		case MICROPHONE_USB:
		    DisplayGFX_DrawText(buffer, (char *)Microphone, 100, 50, 5);
		break;
		default:
		break;
	    }
	TDA7719_config.set_input_front = TDA7719_config.audio_source;

	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_TimeBouncing(uint8_t *const buffer)
{
	static uint32_t random_x_from = 0; 	//number beetwen 0...254
	static uint32_t random_y_from = 20; 	//nubmer beetwen 0...63
	static uint32_t random_x_to = 0; 	//number beetwen 0...254
	static uint32_t random_y_to = 0; 	//nubmer beetwen 0...63
	//static volatile uint8_t bouncing_flag;

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	DisplayGFX_SelectFont(&FreeSansBold9pt7b);

	if (get_random_coords(&random_x_to, &random_y_to) == true)
	{
		make_array(buffer, random_x_from, random_y_from, random_x_to, random_y_to, 5);
		random_x_from = random_x_to;
		random_y_from = random_y_to;
	}
}
//
//Screen_draw_signal_oscyloscope();
//
static void Screen_Encoder_VolumeFront(uint8_t *const buffer)
{
	DisplayDriver_FillBufferWithValue(buffer, 0);
	DisplayGFX_SelectFont(&TomThumb);
	draw_char(buffer, '0', 195, 53, 5);
	DisplayGFX_DrawText(buffer, "-80", 2, 53, 5);
	DisplayGFX_DrawText(buffer, "+15", 225, 53, 5);
	DisplayGFX_SelectFont(&FreeSerifItalic9pt7b);
	DisplayGFX_DrawText(buffer, "dB", 235, 63, 5);
	draw_encoder_volume_front_scale(buffer);

	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_Encoder_Volume_Back(uint8_t *const buffer)
{
	DisplayDriver_FillBufferWithValue(buffer, DISPLAY_BLACK);
	DisplayGFX_SelectFont(&TomThumb);
	draw_char(buffer, '0', 195, 53, 5);
	DisplayGFX_DrawText(buffer, "-80", 2, 53, 5);
	DisplayGFX_DrawText(buffer, "+15", 225, 53, 5);
	DisplayGFX_SelectFont(&FreeSerifItalic9pt7b);
	DisplayGFX_DrawText(buffer, "dB", 235, 63, 5);
	draw_encoder_volume_back_scale(buffer);

	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_Encoder_Loudness(uint8_t *const buffer)
{
	char temp_array_loudness[10] = {0};

	DisplayGFX_SelectFont(&FreeSerif9pt7b);
	DisplayDriver_FillBufferWithValue(buffer, DISPLAY_BLACK);
	DisplayGFX_DrawText(buffer, "LOUDNESS", 70, 15, 5);
	DisplayGFX_SelectFont(&FreeSans9pt7b);
	DisplayGFX_DrawText(buffer, "Mid freq:", 25, 40, 5);
	DisplayGFX_DrawText(buffer, "Gain:", 50, 63, 5);
	draw_gain_and_freq_loudness(buffer, temp_array_loudness);

	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_Encoder_Treble(uint8_t *const buffer)
{
	char temp_array_treble[10] = {0};

	DisplayGFX_SelectFont(&FreeSerif9pt7b);	//niby można wyświetlać wykres
	DisplayDriver_FillBufferWithValue(buffer, DISPLAY_BLACK);
	DisplayGFX_DrawText(buffer, "TREBLE", 70, 15, 5);
	DisplayGFX_SelectFont(&FreeSans9pt7b);
	DisplayGFX_DrawText(buffer, "Mid freq:", 25, 40, 5);
	DisplayGFX_DrawText(buffer, "Gain:", 50, 63, 5);
	draw_gain_and_freq(Device_Cfg_Audio.Treble.gain - 16, temp_array_treble);
	DisplayGFX_DrawText(buffer, (char*) temp_array_treble, 100, 63, 5);

	switch (Device_Cfg_Audio.Treble.centerFreq)
	{
		case 0:		//00 - flat
			DisplayGFX_DrawText(buffer, "10 kHz", 100, 40, 5);
		break;
		case 1:		//01  - 400 Hz
			DisplayGFX_DrawText(buffer, "12,5 kHz", 100, 40, 5);
		break;
		case 2:		//10  - 800 Hz
			DisplayGFX_DrawText(buffer, "15 kHz", 100, 40, 5);
		break;
		case 3:		//11  - 2400 Hz
			DisplayGFX_DrawText(buffer, "17.5 kHz", 100, 40, 5);
		break;
	default:
		break;
	}
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_Encoder_Middle(uint8_t *const buffer)
{
	char temp_array_middle[10] = {0};

	DisplayGFX_SelectFont(&FreeSerif9pt7b);	//niby można wyświetlać wykres
	DisplayDriver_FillBufferWithValue(buffer, DISPLAY_BLACK);
	DisplayGFX_DrawText(buffer, "MIDDLE", 70, 15, 5);
	DisplayGFX_SelectFont(&FreeSans9pt7b);
	DisplayGFX_DrawText(buffer, "Q fact:", 25, 40, 5);
	DisplayGFX_DrawText(buffer, "Gain:", 50, 63, 5);
	draw_gain_and_freq(Device_Cfg_Audio.Middle.gain - 16, temp_array_middle);
	DisplayGFX_DrawText(buffer, (char*) temp_array_middle, 100, 63, 5);

	switch (Device_Cfg_Audio.Middle.centerFreq)
	    {
		case 0:
		    DisplayGFX_DrawText(buffer, "0.5", 100, 40, 5);
		break;
		case 1:
		    DisplayGFX_DrawText(buffer, "0.75", 100, 40, 5);
		break;
		case 2:
		    DisplayGFX_DrawText(buffer, "1", 100, 40, 5);
		break;
		case 3:
		    DisplayGFX_DrawText(buffer, "1.25", 100, 40, 5);
		break;
		default:
		break;
	    }
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}
//
static void Screen_Encoder_Bass(uint8_t *const buffer)
{
	char temp_array_bass[10] = {0};

	DisplayGFX_SelectFont(&FreeSerif9pt7b);	//niby można wyświetlać wykres
	DisplayDriver_FillBufferWithValue(buffer, 0);
	DisplayGFX_DrawText(buffer, "BASS", 70, 15, 5);
	DisplayGFX_SelectFont(&FreeSans9pt7b);
	DisplayGFX_DrawText(buffer, "Q fact:", 25, 40, 5);
	DisplayGFX_DrawText(buffer, "Gain:", 50, 63, 5);
	draw_gain_and_freq(Device_Cfg_Audio.Bass.gain - 16, temp_array_bass);
	DisplayGFX_DrawText(buffer, (char*) temp_array_bass, 100, 63, 5);

	switch (Device_Cfg_Audio.Bass.centerFreq)
	    {
		case 0:
		    DisplayGFX_DrawText(buffer, "1.0", 100, 40, 5);
		break;
		case 1:
		    DisplayGFX_DrawText(buffer, "1.25", 100, 40, 5);
		break;
		case 2:
		    DisplayGFX_DrawText(buffer, "1.5", 100, 40, 5);
		break;
		case 3:
		    DisplayGFX_DrawText(buffer, "2.0", 100, 40, 5);
		break;
		default:
		break;
	    }
	DisplayDriver_TX_ImageBuff(buffer, 0, 0);
}

//--------------------------------------------------------------
// Possible displayed screens
//--------------------------------------------------------------
void AppDisplay_OnInitTask(void)
{
	Display_Init();
}

void AppDisplay_RefreshDisplayTask(void)
{
	AppDisplay_RefreshDisplay(Display_Controls.Screen_State);

}

void AppDisplay_RefreshDisplay(const ScreenState_t Screen_State)
{
	if (Display_Controls.OnStandbyMode_flag == true)
	{
//		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
//		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

		if ((saved_minutes + 1) == 60)
			saved_minutes = 0; 	// in this case it wait 2 min - work on it

//		if ((saved_seconds == sTime.Seconds) && ((saved_minutes + 1) == sTime.Minutes)) //do it one minute after last action with encoder or button
		{
			SSD1322_API_Sleep_On();
			return;
		}
	}

	switch (Screen_State)
	{
	case SCREEN_WELCOME:
		Screen_Welcome(Display_Buffer);
		break;
	case SCREEN_TIME:
		Screen_Time(Display_Buffer);
		break;
	case SCREEN_RADIO:
		Screen_Radio(Display_Buffer);
		break;
	case SCREEN_WAKEUP:
		Screen_WakeUp(Display_Buffer);
		break;
	case SCREEN_FFT:
		Screen_FFT(Display_Buffer, OutFreqArray);
		break;
	case SCREEN_UVMETER:
		Screen_UVMeter(Display_Buffer);
		break;
	case SCREEN_OFF:
		Screen_OFF(Display_Buffer);
		break;
	case SCREEN_GOODBYTE:
		Screen_GoodBye(Display_Buffer);
		break;
	case SCREEN_SETCLOCK:
		Screen_SetClock(Display_Buffer);
		break;
	case SCREEN_SETALARM:
		Screen_SetAlarm(Display_Buffer);
		break;
	case SCREEN_SETTINGS:
		Screen_Settings(Display_Buffer);
		break;
	case SCREEN_SNAKE:
		Screen_Snake(Display_Buffer);
		break;
	case SCREEN_USB:
		Screen_USB(Display_Buffer);
		break;
	case SCREEN_SETINPUT:
		Screen_SetInput(Display_Buffer);
		break;
	case SCREEN_TIME_BOUNCING:
		Screen_TimeBouncing(Display_Buffer);
		break;
	case SCREEN_ENCODER_VOLUME_FRONT:
		Screen_Encoder_VolumeFront(Display_Buffer);
		break;
	case SCREEN_ENCODER_VOLUME_BACK:
		Screen_Encoder_Volume_Back(Display_Buffer);
		break;
	case SCREEN_ENCODER_LOUDNESS:
		Screen_Encoder_Loudness(Display_Buffer);
		break;
	case SCREEN_ENCODER_TREBLE:
		Screen_Encoder_Treble(Display_Buffer);
		break;
	case SCREEN_ENCODER_MIDDLE:
		Screen_Encoder_Middle(Display_Buffer);
		break;
	case SCREEN_ENCODER_BASS:
		Screen_Encoder_Bass(Display_Buffer);
		break;
	default:
		DisplayDriver_FillBufferWithValue(Display_Buffer, 0);
		DisplayDriver_TX_ImageBuff(Display_Buffer, 0, 0);
		break;
	}
}

ScreenState_t AppDisplay_GetDisplayState(void)
{
	return Display_Controls.Screen_State;
}

void AppDisplay_SetDisplayState(ScreenState_t ScreenState)
{
	Display_Controls.Screen_State = ScreenState;
}

ScreenState_t AppDisplay_GetSavedDisplayState(void)
{
	return Display_Controls.Screen_State_Saved;
}

void AppDisplay_SetSavedDisplayState(ScreenState_t ScreenState)
{
	Display_Controls.Screen_State_Saved = ScreenState;
}

void AppDisplay_SaveCurrentDisplayState(void)
{
	ScreenState_t Current_Screen_State = AppDisplay_GetDisplayState();

	if (Current_Screen_State != AppDisplay_GetSavedDisplayState())
	{
		if((Current_Screen_State != (SCREEN_STATE_ENUM_MAX_USER_DISPLAY)
				&& (Current_Screen_State != SCREEN_WAKEUP)
				&& (Current_Screen_State != SCREEN_WELCOME)
				&& (Current_Screen_State != SCREEN_OFF)
				&& (Current_Screen_State != SCREEN_GOODBYTE)
				&& (Current_Screen_State != SCREEN_STATE_ENUM_MAX_INVIS_DISPLAY)
				&& (Current_Screen_State != SCREEN_ENCODER_VOLUME_FRONT)
				&& (Current_Screen_State != SCREEN_ENCODER_VOLUME_BACK)
				&& (Current_Screen_State != SCREEN_ENCODER_LOUDNESS)
				&& (Current_Screen_State != SCREEN_ENCODER_TREBLE)
				&& (Current_Screen_State != SCREEN_ENCODER_MIDDLE)
				&& (Current_Screen_State != SCREEN_ENCODER_BASS)
				&& (Current_Screen_State != SCREEN_ENCODER_RADIO)
				&& (Current_Screen_State != SCREEN_STATE_ENUM_MAX)))
		{
			AppDisplay_SetSavedDisplayState(Current_Screen_State);
		}
	}

}

void change_display_state(TIM_HandleTypeDef *htim)
{
	ScreenState_t SSD1322_Screen_State_temp = AppDisplay_GetDisplayState();

	if (SettingsUserMenu.RefreshScreenTime != 65535) // change display if timer is set
	{
		if ((SSD1322_Screen_State_temp >= SCREEN_TIME) && (SSD1322_Screen_State_temp <= SCREEN_TIME_BOUNCING))
		{
			AppDisplay_SetDisplayState(SSD1322_Screen_State_temp + 1);

			if (SCREEN_SETINPUT == SSD1322_Screen_State_temp) SSD1322_Screen_State_temp = SCREEN_TIME;
			htim->Instance->CNT = 0;

			/* Check if ADC is needed */
			if ((SSD1322_Screen_State_temp == SCREEN_UVMETER) || (SSD1322_Screen_State_temp == SCREEN_FFT))
			{
				//HAL_TIM_Base_Start(&htim6);
				//HAL_ADC_Start_DMA(&hadc1, (uint32_t*) ADC_SamplesTEST, UV_meter_numb_of_chan);
				//ADC_IS_ON_flag = true;
			}
			else
			{
				//HAL_TIM_Base_Stop(&htim6);
				//HAL_ADC_Stop_DMA(&hadc1);
				//ADC_IS_ON_flag = false;
			}
		}
	}
}


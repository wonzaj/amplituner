/**
 ********************************************************************************
 * @file    eeprom1.c
 * @author  macie
 * @date    Nov 25, 2024
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "eeprom.h"
#include "app_encoders.h"
#include "stdint.h"
#include "math.h"
#include "string.h"

#include "cmsis_os2.h"
#include "FreeRTOS.h"
/************************************
 * EXTERN VARIABLES
 ************************************/
extern savedUserSettings_t 		savedUserSettings;
extern Device_Cfg_Audio_t 		Device_Cfg_Audio;
extern TDA7719_config_t 			TDA7719_config;
/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define EEPROM_ADDR_WRITE 	0xA0
#define EEPROM_ADDR_READ	 	0xA1
#define EEPROM_I2C 	&hi2c2
#define PAGE_SIZE 	16     // in Bytes
#define PAGE_NUM  		32    // number of pages
#define PAGE_0			0
#define PAGE_1  		1
#define PAGE_2  		2
#define PAGE_3  		3
#define PAGE_4  		4
#define PAGE_5  		5
#define Byte_0  		0
#define Byte_1  		1
#define Byte_2  		2
#define Byte_3  		3
#define Byte_4  		4
#define Byte_5  		5
#define Byte_6  		6
#define Byte_7  		7
#define Byte_8  		8
#define Byte_9  		9
#define Byte_10		10
#define Byte_11  		11
#define Byte_12  		12

#define FREQ_OFFSET  870
#define USER_SETTINGS_PAGE_ADDR	10
#define VOL_CFG_PAGE_ADDR		11
#define FILTERS_CFG_PAGE_ADDR	12
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
static void WriteData(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
static void ReadData(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
static void PageErase(uint16_t page);
static void Write_NUM(uint16_t page, uint16_t offset, float fdata);
static float Read_NUM(uint16_t page, uint16_t offset);
static uint16_t bytestowrite(uint16_t size, uint16_t offset);
static float Bytes2float(uint8_t *ftoa_bytes_temp);
/************************************
 * STATIC FUNCTIONS
 ************************************/
// function to determine the remaining bytes
static uint16_t bytestowrite(uint16_t size, uint16_t offset)
{
	if ((size + offset) < PAGE_SIZE)
		return size;
	else
		return PAGE_SIZE - offset;
}

static void float2Bytes(uint8_t *ftoa_bytes_temp, float float_variable)
{
	union
	{
		float a;
		uint8_t bytes[4];
	} thing;

	thing.a = float_variable;

	for (uint8_t i = 0; i < 4; i++)
	{
		ftoa_bytes_temp[i] = thing.bytes[i];
	}

}

static float Bytes2float(uint8_t *ftoa_bytes_temp)
{
	union
	{
		float a;
		uint8_t bytes[4];
	} thing;

	for (uint8_t i = 0; i < 4; i++)
	{
		thing.bytes[i] = ftoa_bytes_temp[i];
	}

	float float_variable = thing.a;
	return float_variable;
}

//====================== Write to eeprom ========================//
/**
 *  @brief Writes to speficfied addres given buffor
 *  T24C256 has 512 pages of 64-bytes each
 *
 *  @param[in] page - number of the start page. Range from 0 to PAGE_NUM-1
 *  @param[in] offset -  start byte offset in the page. Range from 0 to PAGE_SIZE-1
 *  @param[in] data is the pointer to the data to write in bytes
 *  @param[in] size is the size of the data
 */
static void WriteData(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size)
{

	// Find out the number of bit, where the page addressing starts
	int paddrposition = log(PAGE_SIZE) / log(2);

	// calculate the start page and the end page
	uint16_t startPage = page;
	uint16_t endPage = page + ((size + offset) / PAGE_SIZE);

	// number of pages to be written
	uint16_t numofpages = (endPage - startPage) + 1;
	uint16_t pos = 0;

	// write the data
	for (int i = 0; i < numofpages; i++)
	{
		/* calculate the address of the memory location
		 * Here we add the page address with the byte address
		 */
		uint16_t MemAddress = startPage << paddrposition | offset;
		uint16_t bytesremaining = bytestowrite(size, offset); // calculate the remaining bytes to be written

		HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR_WRITE, MemAddress, 2, &data[pos], bytesremaining, 1000); // write the data to the EEPROM

		startPage += 1; // increment the page, so that a new page address can be selected for further write
		offset = 0; // since we will be writing to a new page, so offset will be 0
		size = size - bytesremaining;  // reduce the size of the bytes
		pos += bytesremaining;  // update the position for the data buffer

		osDelay(pdMS_TO_TICKS(5));  // Write cycle delay (5ms)
	}
}

//====================== Ready from eeprom ========================//
/**
 *  @brief Reads from speficfied address given buffor
 *
 *  @param[in] page - number of the start page. Range from 0 to PAGE_NUM-1
 *  @param[in] offset -  start byte offset in the page. Range from 0 to PAGE_SIZE-1
 *  @param[in] data is the pointer to the data to write in bytes
 *  @param[in] size is the size of the data
 */
static void ReadData(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size)
{
	int paddrposition = log(PAGE_SIZE) / log(2);

	uint16_t startPage = page;
	uint16_t endPage = page + ((size + offset) / PAGE_SIZE);

	uint16_t numofpages = (endPage - startPage) + 1;
	uint16_t pos = 0;

	for (int i = 0; i < numofpages; i++)
	{
		uint16_t MemAddress = startPage << paddrposition | offset;
		uint16_t bytesremaining = bytestowrite(size, offset);
		HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR_READ, MemAddress, 2, &data[pos], bytesremaining, 1000);
		startPage += 1;
		offset = 0;
		size = size - bytesremaining;
		pos += bytesremaining;
	}
}

//====================== Clear page ========================//
/**
 *  @brief Clears given eeprom page
 *
 *  @param[in] page - number of the start page. Range from 0 to PAGE_NUM-1
 */
static void PageErase(uint16_t page)
{
	// calculate the memory address based on the page number
	int paddrposition = log(PAGE_SIZE) / log(2);
	uint16_t MemAddress = page << paddrposition;

	// create a buffer to store the reset values
	uint8_t data[PAGE_SIZE];
	memset(data, 0xff, PAGE_SIZE);

	// write the data to the EEPROM
	HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR_WRITE, MemAddress, 2, data, PAGE_SIZE, 10);

	HAL_Delay(5);  // write cycle delay
}

/*Write the Float/Integer values to the EEPROM
 * @page is the number of the start page. Range from 0 to PAGE_NUM-1
 * @offset is the start byte offset in the page. Range from 0 to PAGE_SIZE-1
 * @data is the float/integer value that you want to write
 */

static void Write_NUM(uint16_t page, uint16_t offset, float data)
{
	uint8_t bytes_temp[4] = {0};

	float2Bytes(bytes_temp, data);
	WriteData(page, offset, bytes_temp, 4);
}

/* Reads the single Float/Integer values from the EEPROM
 * @page is the number of the start page. Range from 0 to PAGE_NUM-1
 * @offset is the start byte offset in the page. Range from 0 to PAGE_SIZE-1
 * @returns the float/integer value
 */

static float Read_NUM(uint16_t page, uint16_t offset)
{
	uint8_t buffer[4];

	ReadData(page, offset, buffer, 4);
	return (Bytes2float(buffer));
}
/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void EEPROM_Read_UserSetting(void)
{
	uint8_t Temp_tab[PAGE_SIZE] = {0};

	ReadData(USER_SETTINGS_PAGE_ADDR, Byte_0, Temp_tab, sizeof(Temp_tab));

	savedUserSettings.radio_freq 			= Temp_tab[0 + 1] + FREQ_OFFSET;
	savedUserSettings.displayRefreshTime 	= Temp_tab[1 + 1];
	savedUserSettings.stationSaved_1 		= Temp_tab[2 + 1] + FREQ_OFFSET;
	savedUserSettings.stationSaved_2 		= Temp_tab[3 + 1] + FREQ_OFFSET;
	savedUserSettings.stationSaved_3 		= Temp_tab[4 + 1] + FREQ_OFFSET;
	savedUserSettings.stationSaved_3 		= Temp_tab[5 + 1] + FREQ_OFFSET;
	TDA7719_config.set_input_front			= Temp_tab[6 + 1];
	TDA7719_config.set_input_back			= Temp_tab[7 + 1];
}

void EEPROM_Save_UserSetting(void)
{
	uint8_t Temp_tab[PAGE_SIZE] = {0};

	Temp_tab[0] = savedUserSettings.radio_freq - FREQ_OFFSET;
	Temp_tab[1] = savedUserSettings.displayRefreshTime;
	Temp_tab[2] = savedUserSettings.stationSaved_1 - FREQ_OFFSET;
	Temp_tab[3] = savedUserSettings.stationSaved_2 - FREQ_OFFSET;
	Temp_tab[4] = savedUserSettings.stationSaved_3 - FREQ_OFFSET;
	Temp_tab[5] = savedUserSettings.stationSaved_3 - FREQ_OFFSET;
	Temp_tab[6] = TDA7719_config.set_input_front;
	Temp_tab[7] = TDA7719_config.set_input_back;

	WriteData(USER_SETTINGS_PAGE_ADDR, Byte_0, Temp_tab, sizeof(Temp_tab));
}

void EEPROM_Read_VolumeSettings(void)
{
	uint8_t Temp_tab[PAGE_SIZE] = {0};

	ReadData(VOL_CFG_PAGE_ADDR, Byte_0, Temp_tab, sizeof(Temp_tab));

	Device_Cfg_Audio.VolFront.volumeMaster 		= Temp_tab[0 + 1];
	Device_Cfg_Audio.VolFront.volumeLeftRight		= Temp_tab[1 + 1];
	Device_Cfg_Audio.VolFront.volumeRight 		= Temp_tab[2 + 1];
	Device_Cfg_Audio.VolFront.volumeLeft 			= Temp_tab[3 + 1];
	Device_Cfg_Audio.VolBack.volumeLeftRight 		= Temp_tab[4 + 1];
	Device_Cfg_Audio.VolBack.volumeRight 			= Temp_tab[5 + 1];
	Device_Cfg_Audio.VolBack.volumeLeft 			= Temp_tab[6 + 1];
	Device_Cfg_Audio.VolFront.audioOutputState 	= Temp_tab[7 + 1];
	Device_Cfg_Audio.VolBack.audioOutputState 	= Temp_tab[8 + 1];
	Device_Cfg_Audio.volumeMasterFlag 			= Temp_tab[9 + 1];
	Device_Cfg_Audio.tempVolFrontLeft				= Temp_tab[10 + 1];
	Device_Cfg_Audio.tempVolFrontRight			= Temp_tab[11 + 1];
	Device_Cfg_Audio.tempVolBackLeft				= Temp_tab[12 + 1];
	Device_Cfg_Audio.tempVolBackRight				= Temp_tab[13 + 1];
}

void EEPROM_Save_VolumeSettings(void)
{
	uint8_t Temp_tab[PAGE_SIZE] = {0};

	Temp_tab[0] = Device_Cfg_Audio.VolFront.volumeMaster;
	Temp_tab[1] = Device_Cfg_Audio.VolFront.volumeLeftRight;
	Temp_tab[2] = Device_Cfg_Audio.VolFront.volumeRight;
	Temp_tab[3] = Device_Cfg_Audio.VolFront.volumeLeft;
	Temp_tab[4] = Device_Cfg_Audio.VolBack.volumeLeftRight;
	Temp_tab[5] = Device_Cfg_Audio.VolBack.volumeRight;
	Temp_tab[6] = Device_Cfg_Audio.VolBack.volumeLeft;
	Temp_tab[7] = Device_Cfg_Audio.VolFront.audioOutputState;
	Temp_tab[8] = Device_Cfg_Audio.VolBack.audioOutputState;
	Temp_tab[9] = Device_Cfg_Audio.volumeMasterFlag;
	Temp_tab[10] = Device_Cfg_Audio.tempVolFrontLeft;
	Temp_tab[11] = Device_Cfg_Audio.tempVolFrontRight;
	Temp_tab[12] = Device_Cfg_Audio.tempVolBackLeft;
	Temp_tab[13] = Device_Cfg_Audio.tempVolBackRight;

	WriteData(VOL_CFG_PAGE_ADDR, Byte_0, Temp_tab, sizeof(Temp_tab));
}

void EEPROM_Read_FilterSettings(void)
{
	uint8_t Temp_tab[PAGE_SIZE] = {0};

	ReadData(FILTERS_CFG_PAGE_ADDR, Byte_0, Temp_tab, sizeof(Temp_tab));

	Device_Cfg_Audio.Treble.gain 			= Temp_tab[0 + 1];
	Device_Cfg_Audio.Treble.centerFreq	= Temp_tab[1 + 1];
	Device_Cfg_Audio.Treble.soft_step		= Temp_tab[2 + 1];
	Device_Cfg_Audio.Middle.gain 			= Temp_tab[3 + 1];
	Device_Cfg_Audio.Middle.centerFreq 	= Temp_tab[4 + 1];
	Device_Cfg_Audio.Middle.soft_step		= Temp_tab[5 + 1];
	Device_Cfg_Audio.Bass.gain 			= Temp_tab[6 + 1];
	Device_Cfg_Audio.Bass.centerFreq 		= Temp_tab[7 + 1];
	Device_Cfg_Audio.Bass.soft_step 		= Temp_tab[8 + 1];
	Device_Cfg_Audio.Loudness.gain 		= Temp_tab[9 + 1];
	Device_Cfg_Audio.Loudness.centerFreq	= Temp_tab[10 + 1];
	Device_Cfg_Audio.Loudness.high_boost	= Temp_tab[11 + 1];
	Device_Cfg_Audio.Loudness.soft_step	= Temp_tab[12 + 1];
}

void EEPROM_Save_FilterSettings(void)
{
	uint8_t Temp_tab[PAGE_SIZE] = {0};

	Temp_tab[0] = Device_Cfg_Audio.Treble.gain;
	Temp_tab[1] = Device_Cfg_Audio.Treble.centerFreq;
	Temp_tab[2] = Device_Cfg_Audio.Treble.soft_step;
	Temp_tab[3] = Device_Cfg_Audio.Middle.gain;
	Temp_tab[4] = Device_Cfg_Audio.Middle.centerFreq;
	Temp_tab[5] = Device_Cfg_Audio.Middle.soft_step;
	Temp_tab[6] = Device_Cfg_Audio.Bass.gain;
	Temp_tab[7] = Device_Cfg_Audio.Bass.centerFreq;
	Temp_tab[8] = Device_Cfg_Audio.Bass.soft_step;
	Temp_tab[9] = Device_Cfg_Audio.Loudness.gain;
	Temp_tab[10] = Device_Cfg_Audio.Loudness.centerFreq;
	Temp_tab[11] = Device_Cfg_Audio.Loudness.high_boost;
	Temp_tab[12] = Device_Cfg_Audio.Loudness.soft_step;

	WriteData(FILTERS_CFG_PAGE_ADDR, Byte_0, Temp_tab, sizeof(Temp_tab));
}

void EEPROM_ReadDeviceDataAtStart(void)
{
	EEPROM_Read_UserSetting();
	EEPROM_Read_VolumeSettings();
	EEPROM_Read_FilterSettings();
}

void EEPROM_SaveDeviceDataAtTurnOff(void)
{
	EEPROM_Save_UserSetting();
	EEPROM_Save_VolumeSettings();
	EEPROM_Save_FilterSettings();
}

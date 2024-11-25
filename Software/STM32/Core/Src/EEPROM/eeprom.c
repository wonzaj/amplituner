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
/************************************
 * EXTERN VARIABLES
 ************************************/
extern savedUserSettings_t 		savedUserSettings;
extern Device_Cfg_Audio_t 		Device_Cfg_Audio;
/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define EEPROM_ADDR 	0xA0
#define EEPROM_I2C 	&hi2c2
#define PAGE_SIZE 	64     // in Bytes
#define PAGE_NUM  		512    // number of pages
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

#define FREQ_OFFSET  870
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

		HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, MemAddress, 2, &data[pos], bytesremaining, 1000); // write the data to the EEPROM

		startPage += 1; // increment the page, so that a new page address can be selected for further write
		offset = 0; // since we will be writing to a new page, so offset will be 0
		size = size - bytesremaining;  // reduce the size of the bytes
		pos += bytesremaining;  // update the position for the data buffer

		HAL_Delay(5);  // Write cycle delay (5ms)
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
		HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, MemAddress, 2, &data[pos], bytesremaining, 1000);
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
	HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, MemAddress, 2, data, PAGE_SIZE, 10);

	//HAL_Delay(5);  // write cycle delay
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
	ReadData(PAGE_0, Byte_0, (uint8_t*) &savedUserSettings.stationSaved_1, sizeof(savedUserSettings.stationSaved_1));
	ReadData(PAGE_0, Byte_1, (uint8_t*) &savedUserSettings.stationSaved_2, sizeof(savedUserSettings.stationSaved_2));
	ReadData(PAGE_0, Byte_2, (uint8_t*) &savedUserSettings.stationSaved_3, sizeof(savedUserSettings.stationSaved_3));
	ReadData(PAGE_0, Byte_3, (uint8_t*) &savedUserSettings.displayRefreshTime, sizeof(savedUserSettings.displayRefreshTime));

	savedUserSettings.radio_freq 		= savedUserSettings.radio_freq + FREQ_OFFSET;
	savedUserSettings.stationSaved_1 	= savedUserSettings.stationSaved_1 + FREQ_OFFSET;
	savedUserSettings.stationSaved_2 	= savedUserSettings.stationSaved_2 + FREQ_OFFSET;
}

void EEPROM_Save_UserSetting(void)
{
	PageErase(PAGE_0);

	savedUserSettings.radio_freq 	= savedUserSettings.radio_freq - FREQ_OFFSET;
	savedUserSettings.stationSaved_1 = savedUserSettings.stationSaved_1 - FREQ_OFFSET;
	savedUserSettings.stationSaved_2 = savedUserSettings.stationSaved_2 - FREQ_OFFSET;

	WriteData(PAGE_0, Byte_0, (uint8_t*) &savedUserSettings.stationSaved_1, 1);
	WriteData(PAGE_0, Byte_1, (uint8_t*) &savedUserSettings.stationSaved_2, 1);
	WriteData(PAGE_0, Byte_2, (uint8_t*) &savedUserSettings.stationSaved_3, 1);
	WriteData(PAGE_0, Byte_3, (uint8_t*) &savedUserSettings.displayRefreshTime, 1);
	WriteData(PAGE_0, Byte_4, (uint8_t*) &savedUserSettings.radio_freq, 1);
}

void EEPROM_Read_VolumeSettings(void)
{
	ReadData(PAGE_1, Byte_0, (uint8_t*) &Device_Cfg_Audio.VolFront.volumeMaster, sizeof(Device_Cfg_Audio.VolFront.volumeMaster));
	ReadData(PAGE_1, Byte_1, (uint8_t*) &Device_Cfg_Audio.VolFront.volumeLeftRight, sizeof(Device_Cfg_Audio.VolFront.volumeLeftRight));
	ReadData(PAGE_1, Byte_2, (uint8_t*) &Device_Cfg_Audio.VolFront.volumeRight, sizeof(Device_Cfg_Audio.VolFront.volumeRight));
	ReadData(PAGE_1, Byte_3, (uint8_t*) &Device_Cfg_Audio.VolFront.volumeLeft, sizeof(Device_Cfg_Audio.VolFront.volumeLeft));
	ReadData(PAGE_1, Byte_4, (uint8_t*) &Device_Cfg_Audio.VolBack.volumeLeftRight, sizeof(Device_Cfg_Audio.VolBack.volumeLeftRight));
	ReadData(PAGE_1, Byte_5, (uint8_t*) &Device_Cfg_Audio.VolBack.volumeRight, sizeof(Device_Cfg_Audio.VolBack.volumeRight));
	ReadData(PAGE_1, Byte_6, (uint8_t*) &Device_Cfg_Audio.VolBack.volumeLeft, sizeof(Device_Cfg_Audio.VolBack.volumeLeft));
}

void EEPROM_Save_VolumeSettings(void)
{
	PageErase(PAGE_1);

	WriteData(PAGE_1, Byte_0, (uint8_t*) &Device_Cfg_Audio.VolFront.volumeMaster, sizeof(Device_Cfg_Audio.VolFront.volumeMaster));
	WriteData(PAGE_1, Byte_1, (uint8_t*) &Device_Cfg_Audio.VolFront.volumeLeftRight, sizeof(Device_Cfg_Audio.VolFront.volumeLeftRight));
	WriteData(PAGE_1, Byte_2, (uint8_t*) &Device_Cfg_Audio.VolFront.volumeRight, sizeof(Device_Cfg_Audio.VolFront.volumeRight));
	WriteData(PAGE_1, Byte_3, (uint8_t*) &Device_Cfg_Audio.VolFront.volumeLeft, sizeof(Device_Cfg_Audio.VolFront.volumeLeft));
	WriteData(PAGE_1, Byte_4, (uint8_t*) &Device_Cfg_Audio.VolBack.volumeLeftRight, sizeof(Device_Cfg_Audio.VolBack.volumeLeftRight));
	WriteData(PAGE_1, Byte_5, (uint8_t*) &Device_Cfg_Audio.VolBack.volumeRight, sizeof(Device_Cfg_Audio.VolBack.volumeRight));
	WriteData(PAGE_1, Byte_6, (uint8_t*) &Device_Cfg_Audio.VolBack.volumeLeft, sizeof(Device_Cfg_Audio.VolBack.volumeLeft));
}

void EEPROM_Read_FilterSettings(void)
{
	ReadData(PAGE_2, Byte_0, (uint8_t*) &Device_Cfg_Audio.Treble.gain, sizeof(Device_Cfg_Audio.Treble.gain));
	ReadData(PAGE_2, Byte_1, (uint8_t*) &Device_Cfg_Audio.Treble.centerFreq, sizeof(Device_Cfg_Audio.Treble.centerFreq));
	ReadData(PAGE_2, Byte_2, (uint8_t*) &Device_Cfg_Audio.Middle.gain, sizeof(Device_Cfg_Audio.Middle.gain));
	ReadData(PAGE_2, Byte_3, (uint8_t*) &Device_Cfg_Audio.Middle.centerFreq, sizeof(Device_Cfg_Audio.Middle.centerFreq));
	ReadData(PAGE_2, Byte_4, (uint8_t*) &Device_Cfg_Audio.Bass.gain, sizeof(Device_Cfg_Audio.Bass.gain));
	ReadData(PAGE_2, Byte_5, (uint8_t*) &Device_Cfg_Audio.Bass.centerFreq, sizeof(Device_Cfg_Audio.Bass.centerFreq));
	ReadData(PAGE_2, Byte_6, (uint8_t*) &Device_Cfg_Audio.Loudness.gain, sizeof(Device_Cfg_Audio.Loudness.gain));
	ReadData(PAGE_2, Byte_6, (uint8_t*) &Device_Cfg_Audio.Loudness.centerFreq,sizeof(Device_Cfg_Audio.Loudness.centerFreq));
}

void EEPROM_Save_FilterSettings(void)
{
	PageErase(PAGE_2);

	WriteData(PAGE_2, Byte_0, (uint8_t*) &Device_Cfg_Audio.Treble.gain, sizeof(Device_Cfg_Audio.Treble.gain));
	WriteData(PAGE_2, Byte_1, (uint8_t*) &Device_Cfg_Audio.Treble.centerFreq, sizeof(Device_Cfg_Audio.Treble.centerFreq));
	WriteData(PAGE_2, Byte_2, (uint8_t*) &Device_Cfg_Audio.Middle.gain, sizeof(Device_Cfg_Audio.Middle.gain));
	WriteData(PAGE_2, Byte_3, (uint8_t*) &Device_Cfg_Audio.Middle.centerFreq, sizeof(Device_Cfg_Audio.Middle.centerFreq));
	WriteData(PAGE_2, Byte_4, (uint8_t*) &Device_Cfg_Audio.Bass.gain, sizeof(Device_Cfg_Audio.Bass.gain));
	WriteData(PAGE_2, Byte_5, (uint8_t*) &Device_Cfg_Audio.Bass.centerFreq, sizeof(Device_Cfg_Audio.Bass.centerFreq));
	WriteData(PAGE_2, Byte_6, (uint8_t*) &Device_Cfg_Audio.Loudness.gain, sizeof(Device_Cfg_Audio.Loudness.gain));
	WriteData(PAGE_2, Byte_7, (uint8_t*) &Device_Cfg_Audio.Loudness.centerFreq, sizeof(Device_Cfg_Audio.Loudness.centerFreq));
}

void EEPROM_ReadUserData(void)
{
	EEPROM_Read_UserSetting();
}

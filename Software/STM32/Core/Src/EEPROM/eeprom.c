#include "EEPROM/eeprom.h"
#include "stdint.h"
#include "math.h"
#include "string.h"

/*****************************************************************************************************************************************/
uint8_t bytes_temp[4];

extern savedUserSettings_t 		savedUserSettings;
extern encoderFilter_t 			encoderFilterTreble;
extern encoderFilter_t 			encoderFilterMiddle;
extern encoderFilter_t 			encoderFilterBass;
extern encoderFilter_t 			encoderFilterLoudness;
extern encoder_t 					encoderVolFront;
extern encoder_t 					encoderVolBack;

// function to determine the remaining bytes
uint16_t bytestowrite(uint16_t size, uint16_t offset)
{
	if ((size + offset) < PAGE_SIZE)
		return size;
	else
		return PAGE_SIZE - offset;
}

void float2Bytes(uint8_t *ftoa_bytes_temp, float float_variable)
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

float Bytes2float(uint8_t *ftoa_bytes_temp)
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
void EEPROM_Write(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size)
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
void EEPROM_Read(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size)
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
void EEPROM_PageErase(uint16_t page)
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

void EEPROM_Write_NUM(uint16_t page, uint16_t offset, float data)
{

	float2Bytes(bytes_temp, data);
	EEPROM_Write(page, offset, bytes_temp, 4);
}

/* Reads the single Float/Integer values from the EEPROM
 * @page is the number of the start page. Range from 0 to PAGE_NUM-1
 * @offset is the start byte offset in the page. Range from 0 to PAGE_SIZE-1
 * @returns the float/integer value
 */

float EEPROM_Read_NUM(uint16_t page, uint16_t offset)
{
	uint8_t buffer[4];

	EEPROM_Read(page, offset, buffer, 4);
	return (Bytes2float(buffer));
}

/* Reads the single Float/Integer values from the EEPROM
 * @returns the float/integer value
 */
void EEPROM_Read_UserSetting(savedUserSettings_t *savedUser)
{
	EEPROM_Read(PAGE_0, Byte_0, (uint8_t*) &savedUser->stationSaved_1, 1);
	EEPROM_Read(PAGE_0, Byte_1, (uint8_t*) &savedUser->stationSaved_2, sizeof(savedUser->stationSaved_2));
	EEPROM_Read(PAGE_0, Byte_2, (uint8_t*) &savedUser->stationSaved_3, sizeof(savedUser->stationSaved_3));
	EEPROM_Read(PAGE_0, Byte_3, (uint8_t*) &savedUser->displayRefreshTime, sizeof(savedUser->displayRefreshTime));

	savedUser->radio_freq = savedUser->radio_freq + FREQ_OFFSET;
	savedUser->stationSaved_1 = savedUser->stationSaved_1 + FREQ_OFFSET;
	savedUser->stationSaved_2 = savedUser->stationSaved_2 + FREQ_OFFSET;
}

void EEPROM_Save_UserSetting(savedUserSettings_t *savedUser)
{
	EEPROM_PageErase(PAGE_0);

	savedUser->radio_freq = savedUser->radio_freq - FREQ_OFFSET;
	savedUser->stationSaved_1 = savedUser->stationSaved_1 - FREQ_OFFSET;
	savedUser->stationSaved_2 = savedUser->stationSaved_2 - FREQ_OFFSET;

	EEPROM_Write(PAGE_0, Byte_0, (uint8_t*) &savedUser->stationSaved_1, 1);
	EEPROM_Write(PAGE_0, Byte_1, (uint8_t*) &savedUser->stationSaved_2, 1);
	EEPROM_Write(PAGE_0, Byte_2, (uint8_t*) &savedUser->stationSaved_3, 1);
	EEPROM_Write(PAGE_0, Byte_3, (uint8_t*) &savedUser->displayRefreshTime, 1);
	EEPROM_Write(PAGE_0, Byte_4, (uint8_t*) &savedUser->radio_freq, 1);
}

void EEPROM_Read_VolumeSettings(encoder_t *encoderVolFront, encoder_t *encoderVolBack)
{
	EEPROM_Read(PAGE_1, Byte_0, (uint8_t*) &encoderVolFront->volumeMaster, sizeof(encoderVolFront->volumeMaster));
	EEPROM_Read(PAGE_1, Byte_1, (uint8_t*) &encoderVolFront->volumeLeftRight, sizeof(encoderVolFront->volumeLeftRight));
	EEPROM_Read(PAGE_1, Byte_2, (uint8_t*) &encoderVolFront->volumeRight, sizeof(encoderVolFront->volumeRight));
	EEPROM_Read(PAGE_1, Byte_3, (uint8_t*) &encoderVolFront->volumeLeft, sizeof(encoderVolFront->volumeLeft));
	EEPROM_Read(PAGE_1, Byte_4, (uint8_t*) &encoderVolBack->volumeLeftRight, sizeof(encoderVolBack->volumeLeftRight));
	EEPROM_Read(PAGE_1, Byte_5, (uint8_t*) &encoderVolBack->volumeRight, sizeof(encoderVolBack->volumeRight));
	EEPROM_Read(PAGE_1, Byte_6, (uint8_t*) &encoderVolBack->volumeLeft, sizeof(encoderVolBack->volumeLeft));
}

void EEPROM_Save_VolumeSettings(encoder_t *encoderVolFront, encoder_t *encoderVolBack)
{
	EEPROM_PageErase(PAGE_1);

	EEPROM_Write(PAGE_1, Byte_0, (uint8_t*) &encoderVolFront->volumeMaster, 1);
	EEPROM_Write(PAGE_1, Byte_1, (uint8_t*) &encoderVolFront->volumeLeftRight, 1);
	EEPROM_Write(PAGE_1, Byte_2, (uint8_t*) &encoderVolFront->volumeRight, 1);
	EEPROM_Write(PAGE_1, Byte_3, (uint8_t*) &encoderVolFront->volumeLeft, 1);
	EEPROM_Write(PAGE_1, Byte_4, (uint8_t*) &encoderVolBack->volumeLeftRight, 1);
	EEPROM_Write(PAGE_1, Byte_5, (uint8_t*) &encoderVolBack->volumeRight, 1);
	EEPROM_Write(PAGE_1, Byte_6, (uint8_t*) &encoderVolBack->volumeLeft, 1);
}

void EEPROM_Read_FilterSettings(encoderFilter_t *encoderFilterTreble, encoderFilter_t *encoderFilterMiddle, encoderFilter_t *encoderFilterBass, encoderFilter_t *encoderFilterLoudness)
{
	EEPROM_Read(PAGE_2, Byte_0, (uint8_t*) &encoderFilterTreble->gain, 1);
	EEPROM_Read(PAGE_2, Byte_1, (uint8_t*) &encoderFilterTreble->centerFreq, 1);
	EEPROM_Read(PAGE_2, Byte_2, (uint8_t*) &encoderFilterMiddle->gain, 1);
	EEPROM_Read(PAGE_2, Byte_3, (uint8_t*) &encoderFilterMiddle->centerFreq, 1);
	EEPROM_Read(PAGE_2, Byte_4, (uint8_t*) &encoderFilterBass->gain, 1);
	EEPROM_Read(PAGE_2, Byte_5, (uint8_t*) &encoderFilterBass->centerFreq, 1);
	EEPROM_Read(PAGE_2, Byte_6, (uint8_t*) &encoderFilterLoudness->gain, 1);
	EEPROM_Read(PAGE_2, Byte_6, (uint8_t*) &encoderFilterLoudness->centerFreq,
			1);
}

void EEPROM_Save_FilterSettings(encoderFilter_t *encoderFilterTreble, encoderFilter_t *encoderFilterMiddle, encoderFilter_t *encoderFilterBass, encoderFilter_t *encoderFilterLoudness)
{
	EEPROM_PageErase(PAGE_2);

	EEPROM_Write(PAGE_2, Byte_0, (uint8_t*) &encoderFilterTreble->gain, sizeof(encoderFilterTreble->gain));
	EEPROM_Write(PAGE_2, Byte_1, (uint8_t*) &encoderFilterTreble->centerFreq, sizeof(encoderFilterTreble->centerFreq));
	EEPROM_Write(PAGE_2, Byte_2, (uint8_t*) &encoderFilterMiddle->gain, sizeof(encoderFilterMiddle->gain));
	EEPROM_Write(PAGE_2, Byte_3, (uint8_t*) &encoderFilterMiddle->centerFreq, sizeof(encoderFilterMiddle->centerFreq));
	EEPROM_Write(PAGE_2, Byte_4, (uint8_t*) &encoderFilterBass->gain, sizeof(encoderFilterBass->gain));
	EEPROM_Write(PAGE_2, Byte_5, (uint8_t*) &encoderFilterBass->centerFreq, sizeof(encoderFilterBass->centerFreq));
	EEPROM_Write(PAGE_2, Byte_6, (uint8_t*) &encoderFilterLoudness->gain, sizeof(encoderFilterLoudness->gain));
	EEPROM_Write(PAGE_2, Byte_7, (uint8_t*) &encoderFilterLoudness->centerFreq, sizeof(encoderFilterLoudness->centerFreq));
}

void read_user_settings_on_init(void)
{
	EEPROM_Read_UserSetting(&savedUserSettings);
	EEPROM_Save_VolumeSettings(&encoderVolFront, &encoderVolBack);
	EEPROM_Save_FilterSettings(&encoderFilterTreble, &encoderFilterMiddle, &encoderFilterBass, &encoderFilterLoudness);
}

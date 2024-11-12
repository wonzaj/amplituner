#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "i2c.h"
#include "main.h"
#include "config.h"


#define EEPROM_ADDR 	0xA0
#define EEPROM_I2C 	&hi2c2
#define PAGE_SIZE 	64     // in Bytes
#define PAGE_NUM  		512    // number of pages
#define PAGE_0	0
#define PAGE_1  1
#define PAGE_2  2
#define PAGE_3  3
#define PAGE_4  4
#define PAGE_5  5
#define Byte_0  0
#define Byte_1  1
#define Byte_2  2
#define Byte_3  3
#define Byte_4  4
#define Byte_5  5
#define Byte_6  6
#define Byte_7  7
#define Byte_8  8

#define FREQ_OFFSET  870

/* Functions prot */
void EEPROM_Write(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
void EEPROM_Read(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
void EEPROM_PageErase(uint16_t page);

void EEPROM_Write_NUM(uint16_t page, uint16_t offset, float fdata);
float EEPROM_Read_NUM(uint16_t page, uint16_t offset);
uint16_t bytestowrite(uint16_t size, uint16_t offset);
float Bytes2float(uint8_t *ftoa_bytes_temp);

/* Functions for user */
void EEPROM_Read_UserSetting(savedUserSettings_t *savedUser);
void EEPROM_Save_UserSetting(savedUserSettings_t *savedUser);
void EEPROM_Save_VolumeSettings(encoder_t *encoderVolFront, encoder_t *encoderVolBack);
void EEPROM_Read_VolumeSettings(encoder_t *encoderVolFront, encoder_t *encoderVolBack);
void EEPROM_Save_FilterSettings(encoderFilter_t *encoderFilterTreble, encoderFilter_t *encoderFilterMiddle,encoderFilter_t *encoderFilterBass, encoderFilter_t *encoderFilterLoudness);
void EEPROM_Read_FilterSettings(encoderFilter_t *encoderFilterTreble, encoderFilter_t *encoderFilterMiddle,encoderFilter_t *encoderFilterBass, encoderFilter_t *encoderFilterLoudness);
void read_user_settings_on_init(void);

#endif /* INC_EEPROM_H_ */

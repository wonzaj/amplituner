/**
 ********************************************************************************
 * @file    eeprom1.h
 * @author  macie
 * @date    Nov 25, 2024
 * @brief   
 ********************************************************************************
 */

#ifndef INC_EEPROM_EEPROM_H_
#define INC_EEPROM_EEPROM_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "i2c.h"
#include "main.h"
#include "config.h"
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
void EEPROM_Read_UserSetting(void);
void EEPROM_Save_UserSetting(void);
void EEPROM_Save_VolumeSettings(void);
void EEPROM_Read_VolumeSettings(void);
void EEPROM_Save_FilterSettings(void);
void EEPROM_Read_FilterSettings(void);
void EEPROM_ReadUserData(void);

#ifdef __cplusplus
}
#endif

#endif 

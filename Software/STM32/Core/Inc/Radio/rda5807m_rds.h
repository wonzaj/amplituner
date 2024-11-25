#ifndef RDA5807M_RDS_H
#define RDA5807M_RDS_H

#include <stdint.h>
#include <stdbool.h>

// RDS Buffer Size Definitions
#define RDS_TEXT_SIZE 64
#define RDS_STATION_NAME_SIZE 9

// RDS Data Structure
typedef struct {
    char stationName[RDS_STATION_NAME_SIZE];  // Station name (8 characters + null terminator)
    char radioText[RDS_TEXT_SIZE];           // Radio text (64 characters max)
    bool rdsAvailable;                       // Indicates if RDS data is valid
} RDA5807M_RDS_Data;

// Public Functions
void RDS_Init(void);
void RDS_Process(void);
bool RDS_HasNewData(void);
const char* RDS_GetStationName(void);
const char* RDS_GetRadioText(void);

#endif // RDA5807M_RDS_H

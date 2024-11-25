#include "rda5807m_rds.h"
#include "rda5807m.h"
#include "i2c.h"
#include <string.h>

// Internal Static Variables
static RDA5807M_RDS_Data rdsData =
{ .stationName = "", .radioText = "", .rdsAvailable = false };

// Static Helper Functions
static void RDS_DecodeStationName(uint16_t blockB, uint16_t blockD);
static void RDS_DecodeRadioText(uint16_t blockB, uint16_t blockC,
		uint16_t blockD);

// Public Functions

void RDS_Init(void)
{
	// Clear RDS data
	memset(&rdsData, 0, sizeof(rdsData));
}

void RDS_Process(void)
{
	uint16_t rdsBlocks[4] =
	{ 0 };

	// Read RDS blocks from RDA5807M
	if (HAL_I2C_Master_Receive(&hi2c2, (RDASequential << 1) | 1,
			(uint8_t*) rdsBlocks, sizeof(rdsBlocks), 10) == HAL_OK)
	{
		// Check for valid RDS blocks
		if (rdsBlocks[0] & 0x8000)
		{ // Check RDS Ready flag
			uint16_t blockA = rdsBlocks[0] & 0x7FFF; // Block A
			uint16_t blockB = rdsBlocks[1];
			uint16_t blockC = rdsBlocks[2];
			uint16_t blockD = rdsBlocks[3];

			// Decode station name and radio text
			RDS_DecodeStationName(blockB, blockD);
			RDS_DecodeRadioText(blockB, blockC, blockD);

			rdsData.rdsAvailable = true;
		}
		else
		{
			rdsData.rdsAvailable = false;
		}
	}
}

bool RDS_HasNewData(void)
{
	return rdsData.rdsAvailable;
}

const char* RDS_GetStationName(void)
{
	return rdsData.stationName;
}

const char* RDS_GetRadioText(void)
{
	return rdsData.radioText;
}

// Static Helper Functions

static void RDS_DecodeStationName(uint16_t blockB, uint16_t blockD)
{
	// Extract station name characters from RDS blocks
	uint8_t index = (blockB & 0x03) * 2; // Determine the position in station name
	rdsData.stationName[index] = (blockD >> 8) & 0xFF;     // First character
	rdsData.stationName[index + 1] = blockD & 0xFF;        // Second character

	// Null-terminate the station name
	rdsData.stationName[RDS_STATION_NAME_SIZE - 1] = '\0';
}

static void RDS_DecodeRadioText(uint16_t blockB, uint16_t blockC,
		uint16_t blockD)
{
	// Check if the RDS blocks indicate radio text
	if ((blockB & 0xF800) == 0x2000)
	{ // Group 2A: Radio Text
		uint8_t textSegmentAddress = (blockB & 0x0F) * 4;

		if (textSegmentAddress < RDS_TEXT_SIZE - 4)
		{
			rdsData.radioText[textSegmentAddress] = (blockC >> 8) & 0xFF; // First character
			rdsData.radioText[textSegmentAddress + 1] = blockC & 0xFF; // Second character
			rdsData.radioText[textSegmentAddress + 2] = (blockD >> 8) & 0xFF; // Third character
			rdsData.radioText[textSegmentAddress + 3] = blockD & 0xFF; // Fourth character
		}
	}
}

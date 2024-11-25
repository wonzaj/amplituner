#include "string.h"
#include "stdio.h"
#include "RDA5807M.h"
#include "i2c.h"
//--------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------
int volume;
int RDAstatus = 0;
static uint8_t I2C_BUFF[I2C3_MULTIBYTE_ANZ];

unsigned int RDA5807M_WriteReg[8], RDA5807M_ReadReg[7], RDA5807M_RDS[32];
unsigned int RDA5807M_WriteRegDef[6] =
{ 0xC00D, 0x0000, 0x0C00, 0x84D4, 0x4000, 0x0000 };
// 1100 0000 0000 1101
// 0000 0000 0000 0000
// 0000 1100 0000 0000
// 1000 0100 1101 1011
// 0100 0000 0000 0000
RDA5807m_info_t 			Radio_Info;
RDA5807m_RDS_info_t		Radio_RDS_Info;
RDA5807m_RDS_Config_t 	RDS_Config;

void RDA5807_WriteAllRegs(void)
{
	uint8_t i;
	uint8_t x = 0;

	for (i = 0; i < 12; i = i + 2)
	{
		I2C_BUFF[i] = RDA5807M_WriteReg[x] >> 8;
		x++;
	}

	x = 0;

	for (i = 1; i < 13; i = i + 2)
	{
		I2C_BUFF[i] = RDA5807M_WriteReg[x] & 0xFF;
		x++;
	}

	HAL_I2C_Master_Transmit(&hi2c2, RDASequential << 1, I2C_BUFF, sizeof(I2C_BUFF), 100);

}

void RDA5807_Init(void)
{

	for (uint8_t i = 0; i < 6; i++)
	{
		RDA5807M_WriteReg[i] = RDA5807M_WriteRegDef[i];
	}
	RDA5807_Volume(StartingVolume);
	RDA5807_SetFrequency(DefaultFreq);
}

void RDA5807_PowerOn(void)
{
	RDA5807M_WriteReg[1] = RDA5807M_WriteReg[1] | 0x0010;
	RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | RDA_POWER;
	RDA5807_WriteAllRegs();
	RDA5807M_WriteReg[1] = RDA5807M_WriteReg[1] & 0xFFEF;//Disable tune after PowerOn operation
}

void RDA5807_PowerOff(void)
{
	RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] ^ RDA_POWER;
	RDA5807_WriteAllRegs();
}

void RDA5807_Reset(void)
{
	RDA5807_Init();
	RDA5807_PowerOn();
}

void RDA5807_Volume(int8_t vol)
{
	if (vol > 15)
	{
		vol = 15;			//Volume is already at 15 level
	}
	if (vol < 0)
	{
		vol = 0;			//Volume is already at 0 level
	}
	RDA5807M_WriteReg[3] = (RDA5807M_WriteReg[3] & 0xFFF0) | vol; // Set New Volume
	RDA5807_WriteAllRegs();
}

void RDA5807_BassBoost(void)
{
	if ((RDA5807M_WriteReg[0] & 0x1000) == 0)
	{
		RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | RDA_BASS_ON;
	}
	else
	{
		RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] & RDA_BASS_OFF;
	}
	RDA5807_WriteAllRegs();
}

void RDA5807_Mono(void)
{
	if ((RDA5807M_WriteReg[0] & 0x2000) == 0)
	{
		RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | RDA_MONO_ON;
	}
	else
	{
		RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] & RDA_MONO_OFF;
	}
	RDA5807_WriteAllRegs();
}

void RDA5807_Mute(void)
{
	if ((RDA5807M_WriteReg[0] & 0x8000) == 0)
	{
		RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | 0x8000;
	}
	else
	{
		RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] & 0x7FFF;
	}
	RDA5807_WriteAllRegs();
}

void RDA5807_Softmute(void)
{
	if ((RDA5807M_WriteReg[2] & 0x0200) == 0)
	{
		RDA5807M_WriteReg[2] = RDA5807M_WriteReg[2] | 0x0200;
	}
	else
	{
		RDA5807M_WriteReg[2] = RDA5807M_WriteReg[2] & 0xFDFF;
	}
	RDA5807_WriteAllRegs();
}

void RDA5807_SoftBlend(void)
{
	if ((RDA5807M_WriteReg[6] & 0x0001) == 0)
	{
		RDA5807M_WriteReg[6] = RDA5807M_WriteReg[6] | 0x0001;
	}
	else
	{
		RDA5807M_WriteReg[6] = RDA5807M_WriteReg[6] & 0xFFFE;
	}
	RDA5807_WriteAllRegs();
}

void RDA5807_SeekUp(void)
{
	RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | RDA_SEEK_UP;   // Set Seek Up
	RDA5807_WriteAllRegs();
	RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] & RDA_SEEK_STOP; // Disable Seek
}

void RDA5807_SeekDown(void)
{
	RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | RDA_SEEK_DOWN; // Set Seek Down
	RDA5807_WriteAllRegs();
	RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] & RDA_SEEK_STOP; // Disable Seek
}

void RDA5807_SetFrequency(float Freq)
{
	int Channel;
	Channel = ((Freq / 10) - StartingFreq) / 0.1;
	Channel = Channel & 0x03FF;
	RDA5807M_WriteReg[1] = Channel * 64 + 0x10; // Channel + TUNE-Bit + Band=00(87-108) + Space=00(100kHz)
	RDA5807_WriteAllRegs();
	RDA5807M_WriteReg[1] = RDA5807M_WriteReg[1] & RDA_TUNE_OFF;
}

uint16_t RDA5807_GetFrequency(void)
{
	return (Radio_Info.freq / 100);
}

uint16_t RDA5807_GetRSSI(void)
{
	return Radio_Info.signal_strength;
}

void RDA5807_RDS()
{
	if ((RDA5807M_WriteReg[0] & RDA_RDS_ON) == 0)
	{
		RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] | RDA_RDS_ON;
	}
	else
	{
		RDA5807M_WriteReg[0] = RDA5807M_WriteReg[0] & RDA_RDS_OFF;
	}
	RDA5807_WriteAllRegs();
}

void RDA5807_RDSinit()
{
	strcpy(Radio_RDS_Info.StationName, "        ");
	strcpy(Radio_RDS_Info.PSName, "        ");
	strcpy(Radio_RDS_Info.PSName1, "        ");
	strcpy(Radio_RDS_Info.PSName2, "        ");
	memset(Radio_RDS_Info.RDStext, '\0', sizeof(Radio_RDS_Info.RDStext));
	memset(Radio_RDS_Info.RDStextbuffer, '\0', sizeof(Radio_RDS_Info.RDStextbuffer));
	RDS_Config.lastTextIDX = 0;
	Radio_RDS_Info.mins = 0;
	sprintf(Radio_RDS_Info.CTtime, "CT --:--");
}

// Read all RDA5807 registers
void RDA5807_Read()
{
	RDAstatus = HAL_I2C_Master_Receive(&hi2c2, RDASequential << 1, I2C_BUFF, 2 * 7, 100);

	for (uint32_t i = 0; i < 6; i++)
	{
		RDA5807M_ReadReg[i] = ((I2C_BUFF[i * 2] << 8) | I2C_BUFF[(i * 2) + 1]);
	}

	//RDS Text data blocks
	RDS_Config.block1 = RDA5807M_ReadReg[2];
	RDS_Config.block2 = RDA5807M_ReadReg[3];
	RDS_Config.block3 = RDA5807M_ReadReg[4];
	RDS_Config.block4 = RDA5807M_ReadReg[5];

	Radio_Info.rdsready = RDA5807M_ReadReg[0] & 0x8000;									//if rdsready != 0 rds data are ready
	Radio_Info.tuneok = RDA5807M_ReadReg[0] & 0x4000;						//if tuneok != 0 seek/tune completed
	Radio_Info.nochannel = RDA5807M_ReadReg[0] & 0x2000;					//if nochannel != 0 no channel found
	Radio_Info.rdssynchro = RDA5807M_ReadReg[0] & 0x1000;					//if rdssynchro = 1000 rds decoder syncrhonized
	Radio_Info.stereo = RDA5807M_ReadReg[0] & 0x0400; 					//if stereo = 0 station is mono else stereo
	Radio_Info.freq = (((RDA5807M_ReadReg[0] & 0x03FF) * 100) + 87000);	//return freq ex 102600KHz > 102.6MHz
	Radio_Info.signal_strength = RDA5807M_ReadReg[1] >> 9;				//return signal strength rssi
	Radio_Info.fmready = RDA5807M_ReadReg[1] & 0x0008; 					//if fmready = 8 > fm is ready
	Radio_Info.fmstation = RDA5807M_ReadReg[1] & 0x0100; 					//if fmstation = 100 fm station is true
	Radio_Info.rdsblockerror = RDA5807M_ReadReg[1] & 0x000C;							//check for rds blocks errors
	//00= 0 errors,01= 1~2 errors requiring correction
	//10= 3~5 errors requiring correction
	//11= 6+ errors or error in checkword, correction not possible.

	if (RDS_Config.rdssynchro != 0x1000)
	{  // RDS not synchronised or tuning changed, reset all the RDS info.
		RDA5807_RDSinit();
	}

	// analyzing Block 2
	RDS_Config.rdsGroupType = 0x0A | ((RDS_Config.block2 & 0xF000) >> 8) | ((RDS_Config.block2 & 0x0800) >> 11);
	RDS_Config.rdsTP = (RDS_Config.block2 & 0x0400);
	RDS_Config.rdsPTY = (RDS_Config.block2 & 0x0400);
	switch (RDS_Config.rdsGroupType)
	{
	case 0x0A:
	case 0x0B:
		// The data received is part of the Service Station Name
		RDS_Config.idx = 2 * (RDS_Config.block2 & 0x0003);
		// new data is 2 chars from block 4
		Radio_RDS_Info.c1 = RDS_Config.block4 >> 8;
		Radio_RDS_Info.c2 = RDS_Config.block4 & 0x00FF;
		// check that the data was received successfully twice
		// before sending the station name
		if ((Radio_RDS_Info.PSName1[RDS_Config.idx] == Radio_RDS_Info.c1) && (Radio_RDS_Info.PSName1[RDS_Config.idx + 1] == Radio_RDS_Info.c2))
		{
			// retrieve the text a second time: store to _PSName2
			Radio_RDS_Info.PSName2[RDS_Config.idx] = Radio_RDS_Info.c1;
			Radio_RDS_Info.PSName2[RDS_Config.idx + 1] = Radio_RDS_Info.c2;
			Radio_RDS_Info.PSName2[8] = '\0';
			if (strcmp(Radio_RDS_Info.PSName1, Radio_RDS_Info.PSName2) == 0)
			{
				// populate station name
				int8_t n = 0;
				for (uint32_t i = 0; i < (8); i++)
				{ // remove non-printable error ASCCi characters

					if (Radio_RDS_Info.PSName2[i] > 31 && Radio_RDS_Info.PSName2[i] < 127)
					{
						Radio_RDS_Info.StationName[n] = Radio_RDS_Info.PSName2[i];
						n++;
					}
				}
			}
		}
		if ((Radio_RDS_Info.PSName1[RDS_Config.idx] != Radio_RDS_Info.c1) || (Radio_RDS_Info.PSName1[RDS_Config.idx + 1] != Radio_RDS_Info.c2))
		{
			Radio_RDS_Info.PSName1[RDS_Config.idx] = Radio_RDS_Info.c1;
			Radio_RDS_Info.PSName1[RDS_Config.idx + 1] = Radio_RDS_Info.c2;
			Radio_RDS_Info.PSName1[8] = '\0';
		}
		break;

	case 0x2A:
// RDS text
		RDS_Config.textAB = (RDS_Config.block2 & 0x0010);
		RDS_Config.idx = 4 * (RDS_Config.block2 & 0x000F);
		if (RDS_Config.idx < RDS_Config.lastTextIDX)
		{
			// The existing text might be complete because the index is starting at the beginning again.
			// Populate RDS text array.
			int8_t n = 0;
			for (uint32_t i = 0; i < strlen(Radio_RDS_Info.RDStextbuffer); i++)
			{
				if (Radio_RDS_Info.RDStextbuffer[i] > 31 && Radio_RDS_Info.RDStextbuffer[i] < 127)
				{    // remove non printable error characters
					Radio_RDS_Info.RDStext[n] = Radio_RDS_Info.RDStextbuffer[i];
					n++;
				}
			}
		}
		RDS_Config.lastTextIDX = RDS_Config.idx;
		if (RDS_Config.textAB != RDS_Config.lasttextAB)
		{
			// when this bit is toggled text data has changed, the whole buffer should be cleared.
			RDS_Config.lasttextAB = RDS_Config.textAB;
			memset(Radio_RDS_Info.RDStextbuffer, 0, sizeof(Radio_RDS_Info.RDStextbuffer));
			memset(Radio_RDS_Info.RDStext, '\0', sizeof(Radio_RDS_Info.RDStext));
		}
		if (RDS_Config.rdsblockerror < 4)
		{ // limit RDS data errors as we have no correction code
		  // new data is 2 chars from block 3
			Radio_RDS_Info.RDStextbuffer[RDS_Config.idx] = (RDS_Config.block3 >> 8);
			RDS_Config.idx++;
			Radio_RDS_Info.RDStextbuffer[RDS_Config.idx] = (RDS_Config.block3 & 0x00FF);
			RDS_Config.idx++;
			// new data is 2 chars from block 4
			Radio_RDS_Info.RDStextbuffer[RDS_Config.idx] = (RDS_Config.block4 >> 8);
			RDS_Config.idx++;
			Radio_RDS_Info.RDStextbuffer[RDS_Config.idx] = (RDS_Config.block4 & 0x00FF);
			RDS_Config.idx++;
		}
		break;

	case 0x4A:
// Clock time and date
		if (RDS_Config.rdsblockerror < 3)
		{ // limit RDS data errors as we have no correction code
			RDS_Config.offset = (RDS_Config.block4) & 0x3F; // 6 bits
			Radio_RDS_Info.mins = (RDS_Config.block4 >> 6) & 0x3F; // 6 bits
			Radio_RDS_Info.mins += 60 * (((RDS_Config.block3 & 0x0001) << 4) | ((RDS_Config.block4 >> 12) & 0x0F));
		}
		// adjust offset
		if (RDS_Config.offset & 0x20)
		{
			Radio_RDS_Info.mins -= 30 * (RDS_Config.offset & 0x1F);
		}
		else
		{
			Radio_RDS_Info.mins += 30 * (RDS_Config.offset & 0x1F);
		}

		if (Radio_RDS_Info.mins > 0 && Radio_RDS_Info.mins < 1500)
		{
			sprintf(Radio_RDS_Info.CTtime, "CT %2d:%02d", (Radio_RDS_Info.mins / 60), (Radio_RDS_Info.mins % 60)); // CT time formatted string
		}

		break;

	case 0x6A:
		// IH
		break;

	case 0x8A:
		// TMC
		break;

	case 0xAA:
		// TMC
		break;

	case 0xCA:
		// TMC
		break;

	case 0xEA:
		// IH
		break;

	default:

		break;
	}
}

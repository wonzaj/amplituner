#include "string.h"
#include "stdio.h"
#include "RDA5807M.h"
#include "i2c.h"
//--------------------------------------------------------------
// Local Variables
//--------------------------------------------------------------
int volume;
int RDAstatus = 0;
uint8_t I2C_BUFF[I2C3_MULTIBYTE_ANZ];

unsigned int RDA5807M_WriteRegDef[6] =
{ 0xC00D, 0x0000, 0x0C00, 0x84D4, 0x4000, 0x0000 };
// 1100 0000 0000 1101
// 0000 0000 0000 0000
// 0000 1100 0000 0000
// 1000 0100 1101 1011
// 0100 0000 0000 0000
RDA5807m_RDS_info_t RDA5807m_RDS_info;

int rdsready;
int tuneok;
int nochannel;
int rdssynchro;
int stereo;
int freq;
int signal_strength;
int fmready;
int fmstation;
int rdsblockerror;
unsigned int RDA5807M_WriteReg[8], RDA5807M_ReadReg[7], RDA5807M_RDS[32];

char refresh[70];

// ----- RDS Variables
char StationName[10];    // Station Name. 8 characters
char RDStextbuffer[66];  // RDS text buffer 64 characters
char RDStext[66];        // RDS text message 64 characters
char RDSscrolltext[66];  // RDS scroll text buffer
char CTtime[12];         // CT time string formatted as 'CT hh:mm'
char c1, c2;		    // RDS text characters
char PSName[10];    	    // including trailing '\00' character.
char PSName1[10];	    // Station Name buffers
char PSName2[10];

int rdsblockerror, rdssynchro, rdsready, rds, block1, block2, block3, block4;
int mins;          		// RDS CT time in minutes transmitted on the minute
int textAB, lasttextAB, lastTextIDX;
int rdsGroupType, rdsTP, rdsPTY;
int idx;              // index of rdsText
int offset;           // RDS time offset and sign

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

	HAL_I2C_Master_Transmit(&hi2c2, RDASequential << 1, (void*) &I2C_BUFF, sizeof(I2C_BUFF), 10);

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
	return (freq / 100);
}

uint16_t RDA5807_GetRSSI(void)
{
	return signal_strength;
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
	strcpy(StationName, "        ");
	strcpy(PSName, "        ");
	strcpy(PSName1, "        ");
	strcpy(PSName2, "        ");
	memset(RDStext, '\0', sizeof(RDStext));
	memset(RDStextbuffer, '\0', sizeof(RDStextbuffer));
	lastTextIDX = 0;
	mins = 0;
	sprintf(CTtime, "CT --:--");
}

// Read all RDA5807 registers
void RDA5807_Read()
{
	RDAstatus = HAL_I2C_Master_Receive(&hi2c2, RDASequential << 1,
			(void*) &I2C_BUFF, 2 * 7, 100);
	int i = 0;
	for (i = 0; i < 6; i++)
	{
		RDA5807M_ReadReg[i] = ((I2C_BUFF[i * 2] << 8) | I2C_BUFF[(i * 2) + 1]);
	}

	//RDS Text data blocks
	block1 = RDA5807M_ReadReg[2];
	block2 = RDA5807M_ReadReg[3];
	block3 = RDA5807M_ReadReg[4];
	block4 = RDA5807M_ReadReg[5];

	rdsready = RDA5807M_ReadReg[0] & 0x8000;//if rdsready != 0 rds data are ready
	tuneok = RDA5807M_ReadReg[0] & 0x4000;	//if tuneok != 0 seek/tune completed
	nochannel = RDA5807M_ReadReg[0] & 0x2000;//if nochannel != 0 no channel found
	rdssynchro = RDA5807M_ReadReg[0] & 0x1000;//if rdssynchro = 1000 rds decoder syncrhonized
	stereo = RDA5807M_ReadReg[0] & 0x0400; //if stereo = 0 station is mono else stereo
	freq = (((RDA5807M_ReadReg[0] & 0x03FF) * 100) + 87000);//return freq ex 102600KHz > 102.6MHz
	signal_strength = RDA5807M_ReadReg[1] >> 9;			//return signal strength rssi
	fmready = RDA5807M_ReadReg[1] & 0x0008; 	//if fmready = 8 > fm is ready
	fmstation = RDA5807M_ReadReg[1] & 0x0100; //if fmstation = 100 fm station is true
	rdsblockerror = RDA5807M_ReadReg[1] & 0x000C;//check for rds blocks errors
	//00= 0 errors,01= 1~2 errors requiring correction
	//10= 3~5 errors requiring correction
	//11= 6+ errors or error in checkword, correction not possible.

	if (rdssynchro != 0x1000)
	{  // RDS not synchronised or tuning changed, reset all the RDS info.
		RDA5807_RDSinit();
	}

	// analyzing Block 2
	rdsGroupType = 0x0A | ((block2 & 0xF000) >> 8) | ((block2 & 0x0800) >> 11);
	rdsTP = (block2 & 0x0400);
	rdsPTY = (block2 & 0x0400);
	switch (rdsGroupType)
	{
	case 0x0A:
	case 0x0B:
		// The data received is part of the Service Station Name
		idx = 2 * (block2 & 0x0003);
		// new data is 2 chars from block 4
		c1 = block4 >> 8;
		c2 = block4 & 0x00FF;
		// check that the data was received successfully twice
		// before sending the station name
		if ((PSName1[idx] == c1) && (PSName1[idx + 1] == c2))
		{
			// retrieve the text a second time: store to _PSName2
			PSName2[idx] = c1;
			PSName2[idx + 1] = c2;
			PSName2[8] = '\0';
			if (strcmp(PSName1, PSName2) == 0)
			{
				// populate station name
				int8_t n = 0;
				for (i = 0; i < (8); i++)
				{ // remove non-printable error ASCCi characters

					if (PSName2[i] > 31 && PSName2[i] < 127)
					{
						StationName[n] = PSName2[i];
						n++;
					}
				}
			}
		}
		if ((PSName1[idx] != c1) || (PSName1[idx + 1] != c2))
		{
			PSName1[idx] = c1;
			PSName1[idx + 1] = c2;
			PSName1[8] = '\0';
		}
		break;

	case 0x2A:
// RDS text
		textAB = (block2 & 0x0010);
		idx = 4 * (block2 & 0x000F);
		if (idx < lastTextIDX)
		{
			// The existing text might be complete because the index is starting at the beginning again.
			// Populate RDS text array.
			int8_t n = 0;
			for (i = 0; i < strlen(RDStextbuffer); i++)
			{

				if (RDStextbuffer[i] > 31 && RDStextbuffer[i] < 127)
				{    // remove non printable error characters
					RDStext[n] = RDStextbuffer[i];
					n++;
				}
			}
		}
		lastTextIDX = idx;
		if (textAB != lasttextAB)
		{
			// when this bit is toggled text data has changed, the whole buffer should be cleared.
			lasttextAB = textAB;
			memset(RDStextbuffer, 0, sizeof(RDStextbuffer));
			memset(RDStext, '\0', sizeof(RDStext));
		}
		if (rdsblockerror < 4)
		{ // limit RDS data errors as we have no correction code
		  // new data is 2 chars from block 3
			RDStextbuffer[idx] = (block3 >> 8);
			idx++;
			RDStextbuffer[idx] = (block3 & 0x00FF);
			idx++;
			// new data is 2 chars from block 4
			RDStextbuffer[idx] = (block4 >> 8);
			idx++;
			RDStextbuffer[idx] = (block4 & 0x00FF);
			idx++;
		}
		break;

	case 0x4A:
// Clock time and date
		if (rdsblockerror < 3)
		{ // limit RDS data errors as we have no correction code
			offset = (block4) & 0x3F; // 6 bits
			mins = (block4 >> 6) & 0x3F; // 6 bits
			mins += 60 * (((block3 & 0x0001) << 4) | ((block4 >> 12) & 0x0F));
		}
		// adjust offset
		if (offset & 0x20)
		{
			mins -= 30 * (offset & 0x1F);
		}
		else
		{
			mins += 30 * (offset & 0x1F);
		}

		if (mins > 0 && mins < 1500)
		{
			sprintf(CTtime, "CT %2d:%02d", (mins / 60), (mins % 60)); // CT time formatted string
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

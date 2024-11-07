#ifndef INC_RADIO_RDA5807M_H_
#define INC_RADIO_RDA5807M_H_

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stdint.h"

#define    I2C3_MULTIBYTE_ANZ   16

//--------------------------------------------------------------
// Global Functions
//--------------------------------------------------------------
void RDA5807_Dump(void);
void RDA5807_Init(void);
void RDA5807_RDSinit(void);
void RDA5807_Read(void);
void RDA5807_Reset(void);
void RDA5807_PowerOn(void);
void RDA5807_PowerOff(void);
void RDA5807_RDS(void);
void RDA5807_Mute(void);
void RDA5807_Mono(void);
void RDA5807_Softmute(void);
void RDA5807_SoftBlend(void);
void RDA5807_BassBoost(void);
void RDA5807_Volume(int8_t volume);
void RDA5807_SeekUp(void);
void RDA5807_SeekDown(void);
void RDA5807_SetFrequency(float Freq);
uint16_t RDA5807_GetFrequency(void);
uint16_t RDA5807_GetRSSI(void);
int RDA5807_Channel(void);
void RDA5807_WriteAllRegs(void);
//--------------------------------------------------------------
// Definitions
//--------------------------------------------------------------
/* I2C Address */
#define RDASequential 	0x10  //Current Working Communication
#define RDARandom  	0x11  //To Do Soon

/* Initialization Options */
#define StartingFreq 	87.00
#define EndingFreq 	108.00
#define DefaultFreq	87.00
#define StartingVolume 	0x4

/* RDA5807M Function Code  */
/* Register 0x00H (16Bits) */
#define RDA_CHIP_ID     0x0058
/* Register 0x02H (16Bits) */
#define RDA_DHIZ        0x8000
#define RDA_MUTE	0x8000
#define RDA_MONO_ON     0x2000
#define RDA_MONO_OFF	0xDFFF
#define RDA_BASS_ON     0x1000
#define RDA_BASS_OFF	0xEFFF
#define RDA_RCLK_MODE   0x0800
#define RDA_RCLK_DIRECT 0x0400
#define RDA_SEEK_UP     0x0300
#define RDA_SEEK_DOWN	0x0100
#define RDA_SEEK_STOP	0xFCFF
#define RDA_SEEK_WRAP   0x0080
#define RDA_SEEK_NOWRAP 0xFF7F
#define RDA_CLK_0327    0x0000
#define RDA_CLK_1200    0x0010
#define RDA_CLK_2400    0x0050
#define RDA_CLK_1300    0x0020
#define RDA_CLK_2600    0x0060
#define RDA_CLK_1920    0x0030
#define RDA_CLK_3840    0x0070
#define RDA_RDS_ON      0x0008
#define RDA_RDS_OFF	0xFFF7
#define RDA_NEW         0x0004
#define RDA_RESET       0x0002
#define RDA_POWER       0x0001
/* Register 0x03H (16Bits) */
#define RDA_TUNE_ON	0x0010
#define RDA_TUNE_OFF	0xFFEF

//--------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------

typedef struct
{
	int16_t rdsready;
	int16_t tuneok;
	int16_t nochannel;
	int16_t rdssynchro;
	int16_t stereo;
	int16_t freq;
	int16_t signal;
	int16_t fmready;
	int16_t fmstation;
	int16_t rdsblockerror;

}RDA5807m_RDS_info_t;
//--------------------------------------------------------------

extern char    StationName[10];    // Station Name. 8 characters
extern char    RDStextbuffer[66];  // RDS text buffer 64 characters

#endif /* INC_RADIO_RDA5807M_H_ */

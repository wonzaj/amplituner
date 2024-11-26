//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "TDA7719.h"
#include "app_buttons.h"
//--------------------------------------------------------------
// Defines
//--------------------------------------------------------------
#define TDA7719_Registers_size 21
//--------------------------------------------------------------
// Local function declaraction
//--------------------------------------------------------------
static void I2C_send(uint8_t SubAddress, uint8_t Data);

extern Device_Cfg_Audio_t Device_Cfg_Audio;
//--------------------------------------------------------------
// Local variables
//--------------------------------------------------------------
TDA7719_SetMixSourceTo_t 	SetMixSourceTo;
TDA7719_config_t 				TDA7719_config;
TDA7719_Control_t 			TDA7719_Controls_t;

uint8_t TDA7719_registers[TDA7719_Registers_size];
//--------------------------------------------------------------
// Function definitions
//--------------------------------------------------------------
//Function used for prototyping and initialing preamp on startup of MCU
void TDA7719_init(void)
{
	TDA7719_registers[TDA7719_INPUT] = (TDA7719_registers[TDA7719_INPUT] & ~0x7) | (0b0 & 0x7);		//Main source selector - input IN1
	TDA7719_registers[TDA7719_INPUT] = (TDA7719_registers[TDA7719_INPUT] & ~0x8) | ((0 << 3) & 0x8);	//MD1/2 confg for main selector - MD2
	TDA7719_registers[TDA7719_INPUT] = (TDA7719_registers[TDA7719_INPUT] & ~0x10) | ((0 << 4) & 0x10); 	// main source input gain select - +3dB
	TDA7719_registers[TDA7719_INPUT] = (TDA7719_registers[TDA7719_INPUT] & ~0xE0) | ((0b010 << 5) & 0xE0); 	// CFG2
	I2C_send(TDA7719_INPUT, TDA7719_registers[TDA7719_INPUT]);

	TDA7719_registers[TDA7719_INPUT_2] = (TDA7719_registers[TDA7719_INPUT_2] & ~0x7) | (2 & 0x7);		//second source selector - IN2
	TDA7719_registers[TDA7719_INPUT_2] = (TDA7719_registers[TDA7719_INPUT_2] & ~0x8) | ((0 << 3) & 0x8);	//MD1/2 config for 2nd - MD2
	TDA7719_registers[TDA7719_INPUT_2] = (TDA7719_registers[TDA7719_INPUT_2] & ~0x10) | ((0 << 4) & 0x10);	//2nd Source input gain - +3dB
	TDA7719_registers[TDA7719_INPUT_2] = (TDA7719_registers[TDA7719_INPUT_2] & ~0x20) | ((1 << 5) & 0x20);	//QD2 Bypass (front) - OFF
	TDA7719_registers[TDA7719_INPUT_2] = (TDA7719_registers[TDA7719_INPUT_2] & ~0x40) | ((1 << 6) & 0x40);	//QD3 Bypass (rear) - OFF
	TDA7719_registers[TDA7719_INPUT_2] = (TDA7719_registers[TDA7719_INPUT_2] & ~0x80) | ((1 << 7) & 0x80); 	//QD4 Bypass (Subwoofer) - OFF
	I2C_send(TDA7719_INPUT_2, TDA7719_registers[TDA7719_INPUT_2]);

	TDA7719_registers[TDA7719_MIX_SOUR] = (TDA7719_registers[TDA7719_MIX_SOUR] & ~0x7) | (0 & 0x7); 	//Mixing Source Selector - IN0
	TDA7719_registers[TDA7719_MIX_SOUR] = (TDA7719_registers[TDA7719_MIX_SOUR] & ~0xF8) | ((0<< 3) & 0xF8);	//Mixing attenuator - 0dB
	I2C_send(TDA7719_MIX_SOUR, TDA7719_registers[TDA7719_MIX_SOUR]);

	TDA7719_registers[TDA7719_MIX_CONT] = (TDA7719_registers[TDA7719_MIX_CONT] & ~0x1) | ((1 << 0) & 0x1);	//Mix to Front left - OFF
	TDA7719_registers[TDA7719_MIX_CONT] = (TDA7719_registers[TDA7719_MIX_CONT] & ~0x2) | ((1 << 1) & 0x2);	//Mix to front right - OFF
	TDA7719_registers[TDA7719_MIX_CONT] = (TDA7719_registers[TDA7719_MIX_CONT] & ~0x4) | ((1 << 2) & 0x4);	//Mix to Rear left - OFF
	TDA7719_registers[TDA7719_MIX_CONT] = (TDA7719_registers[TDA7719_MIX_CONT] & ~0x8) | ((1 << 3) & 0x8);	//Mix to Rear right - OFF
	TDA7719_registers[TDA7719_MIX_CONT] = (TDA7719_registers[TDA7719_MIX_CONT] & ~0x10) | ((0 << 4) & 0x10);//Rear speaker input confg - Direct path/2nd source   1 - Main signal
	TDA7719_registers[TDA7719_MIX_CONT] = (TDA7719_registers[TDA7719_MIX_CONT] & ~0x20) | ((1 << 5) & 0x20);//Reference output select - internal Vre 3V3
	TDA7719_registers[TDA7719_MIX_CONT] = (TDA7719_registers[TDA7719_MIX_CONT] & ~0x40) | ((1 << 6) & 0x40); //Level meter reset - RESET
	TDA7719_registers[TDA7719_MIX_CONT] = (TDA7719_registers[TDA7719_MIX_CONT] & ~0x80) | ((1 << 7) & 0x80); //DC offset Detector - DC offset detector
	I2C_send(TDA7719_MIX_CONT, TDA7719_registers[TDA7719_MIX_CONT]);

	TDA7719_registers[TDA7719_MUTE] = (TDA7719_registers[TDA7719_MUTE] & ~0x1) | ((1 << 0) & 0x1);		//Soft mute - OFF
	TDA7719_registers[TDA7719_MUTE] = (TDA7719_registers[TDA7719_MUTE] & ~0x2) | ((0 << 1) & 0x2);		//Pin influence for mute - IIC
	TDA7719_registers[TDA7719_MUTE] = (TDA7719_registers[TDA7719_MUTE] & ~0xC) | ((0 << 2) & 0xC);		//Soft Mute Time - 0.48ms
	TDA7719_registers[TDA7719_MUTE] = (TDA7719_registers[TDA7719_MUTE] & ~0x10) | ((1 << 4) & 0x10);	//Subwoofer input config - Bass Output
	TDA7719_registers[TDA7719_MUTE] = (TDA7719_registers[TDA7719_MUTE] & ~0x20) | ((1 << 5) & 0x20);	//Subwoofer enable (OUTL3 & OUTR3) - OFF
	TDA7719_registers[TDA7719_MUTE] = (TDA7719_registers[TDA7719_MUTE] & ~0x40) | ((1 << 6) & 0x40);	//Fast charge - OFF
	TDA7719_registers[TDA7719_MUTE] = (TDA7719_registers[TDA7719_MUTE] & ~0x80) | ((1 << 7) & 0x80);	//Anti-alias filter - OFF
	I2C_send(TDA7719_MUTE , TDA7719_registers[TDA7719_MUTE]);

	TDA7719_registers[TDA7719_SOFT_1] = (TDA7719_registers[TDA7719_SOFT_1] & ~0x1) | ((1 << 0) & 0x1);	//Loudness soft step - OFF
	TDA7719_registers[TDA7719_SOFT_1] = (TDA7719_registers[TDA7719_SOFT_1] & ~0x2) | ((1 << 1) & 0x2);	//Volume soft step - OFF
	TDA7719_registers[TDA7719_SOFT_1] = (TDA7719_registers[TDA7719_SOFT_1] & ~0x4) | ((1 << 2) & 0x4);	//Treble soft step - OFF
	TDA7719_registers[TDA7719_SOFT_1] = (TDA7719_registers[TDA7719_SOFT_1] & ~0x8) | ((1 << 3) & 0x8);	//Middle soft step - OFF
	TDA7719_registers[TDA7719_SOFT_1] = (TDA7719_registers[TDA7719_SOFT_1] & ~0x10) | ((1 << 4) & 0x10);	//Bass soft step - OFF
	TDA7719_registers[TDA7719_SOFT_1] = (TDA7719_registers[TDA7719_SOFT_1] & ~0x20) | ((1 << 5) & 0x20);	//Speaker LF soft step - OFF
	TDA7719_registers[TDA7719_SOFT_1] = (TDA7719_registers[TDA7719_SOFT_1] & ~0x40) | ((1 << 6) & 0x40);	//Speaker RF soft step - OFF
	TDA7719_registers[TDA7719_SOFT_1] = (TDA7719_registers[TDA7719_SOFT_1] & ~0x80) | ((1 << 7) & 0x80);	//Speaker RF soft step - OFF
	I2C_send(TDA7719_SOFT_1 , TDA7719_registers[TDA7719_SOFT_1]);

	TDA7719_registers[TDA7719_SOFT_2] = (TDA7719_registers[TDA7719_SOFT_2] & ~0x1) | ((1 << 0) & 0x1);	//Speaker RR soft step - OFF
	TDA7719_registers[TDA7719_SOFT_2] = (TDA7719_registers[TDA7719_SOFT_2] & ~0x2) | ((1 << 1) & 0x2);	//Subwoofer left soft step - OFF
	TDA7719_registers[TDA7719_SOFT_2] = (TDA7719_registers[TDA7719_SOFT_2] & ~0x4) | ((1 << 2) & 0x4);	//Subwoofer right soft step - OFF
	TDA7719_registers[TDA7719_SOFT_2] = (TDA7719_registers[TDA7719_SOFT_2] & ~0x8) | ((1 << 3) & 0x8);	//soft step time - 10ms
	TDA7719_registers[TDA7719_SOFT_2] = (TDA7719_registers[TDA7719_SOFT_2] & ~0x30) | ((0 << 4) & 0x30);	//Zero-comparator window size - +100mV
	TDA7719_registers[TDA7719_SOFT_2] = (TDA7719_registers[TDA7719_SOFT_2] & ~0xC0) | ((3 << 6) & 0xC0);	//Spike rejection time constant - 44us
	I2C_send(TDA7719_SOFT_2 , TDA7719_registers[TDA7719_SOFT_2]);


	TDA7719_registers[TDA7719_LOUD] = (TDA7719_registers[TDA7719_LOUD] & ~0x30) | ((0b1111 << 4) & 0x30);	//Loudness attenuation - 0dB
	TDA7719_registers[TDA7719_LOUD] = (TDA7719_registers[TDA7719_LOUD] & ~0x40) | ((0 << 6) & 0x40);	//Center frequency - Flat
	TDA7719_registers[TDA7719_LOUD] = (TDA7719_registers[TDA7719_LOUD] & ~0x80) | ((0 << 7) & 0x80);	//High boost - OFF
	I2C_send(TDA7719_LOUD , TDA7719_registers[TDA7719_LOUD]);


	TDA7719_registers[TDA7719_VOL] = (TDA7719_registers[TDA7719_VOL] & ~0x1F) | ((0b11100 << 0) & 0x1F);	//Volume gain/attenuation - 0dB
	TDA7719_registers[TDA7719_VOL] = (TDA7719_registers[TDA7719_VOL] & ~0x40) | ((0 << 6) & 0x40);		//Output gain - 0dB
	TDA7719_registers[TDA7719_VOL] = (TDA7719_registers[TDA7719_VOL] & ~0x80) | ((1 << 7) & 0x80);		//soft step action - wait
	I2C_send(TDA7719_VOL , TDA7719_registers[TDA7719_VOL]);


	TDA7719_registers[TDA7719_TRBLE] = (TDA7719_registers[TDA7719_TRBLE] & ~0x1F) | ((0b11111 << 0) & 0x1F);//Treble filter gain/attenuation - 0dB
	TDA7719_registers[TDA7719_TRBLE] = (TDA7719_registers[TDA7719_TRBLE] & ~0x60) | ((0b11 << 5) & 0x60);	//treble center freq - 12.5kHz
	TDA7719_registers[TDA7719_TRBLE] = (TDA7719_registers[TDA7719_TRBLE] & ~0x80) | ((1 << 7) & 0x80);	//Soft step action - wait
	I2C_send(TDA7719_TRBLE , TDA7719_registers[TDA7719_TRBLE]);

	TDA7719_registers[TDA7719_MIDDLE] = (TDA7719_registers[TDA7719_MIDDLE] & ~0x1F) | ((0b11111 << 0) & 0x1F);//Middle filter gain/attenuation - 0dB
	TDA7719_registers[TDA7719_MIDDLE] = (TDA7719_registers[TDA7719_MIDDLE] & ~0x60) | ((0b11 << 5) & 0x60);	//middle q factor - 0.75
	TDA7719_registers[TDA7719_MIDDLE] = (TDA7719_registers[TDA7719_MIDDLE] & ~0x80) | ((1 << 7) & 0x80);	//soft step action - wait
	I2C_send(TDA7719_MIDDLE , TDA7719_registers[TDA7719_MIDDLE]);

	TDA7719_registers[TDA7719_BASS] = (TDA7719_registers[TDA7719_BASS] & ~0x1F) | ((0b11111 << 0) & 0x1F);	//Bass filter gain/attenuation - 0dB
	TDA7719_registers[TDA7719_BASS] = (TDA7719_registers[TDA7719_BASS] & ~0x60) | ((0b11 << 5) & 0x60);	//Bass Q factor - 1.25
	TDA7719_registers[TDA7719_BASS] = (TDA7719_registers[TDA7719_BASS] & ~0x80) | ((1 << 7) & 0x80);	//Soft step action - wait
	I2C_send(TDA7719_BASS , TDA7719_registers[TDA7719_BASS]);

	TDA7719_registers[TDA7719_SUB_M_B] = (TDA7719_registers[TDA7719_SUB_M_B] & ~0x3) | ((0 << 0) & 0x3);	//Subwoofer Cut-off frequency - Flat
	TDA7719_registers[TDA7719_SUB_M_B] = (TDA7719_registers[TDA7719_SUB_M_B] & ~0x4) | ((0 << 2) & 0x4);	//Subwoofer output phase - 0deg
	TDA7719_registers[TDA7719_SUB_M_B] = (TDA7719_registers[TDA7719_SUB_M_B] & ~0x18) | ((0 << 3) & 0x18);	//Middle Center Frequency - 500Hz
	TDA7719_registers[TDA7719_SUB_M_B] = (TDA7719_registers[TDA7719_SUB_M_B] & ~0x60) | ((0 << 5) & 0x60);	//Bass center Frequency - 60 Hz
	TDA7719_registers[TDA7719_SUB_M_B] = (TDA7719_registers[TDA7719_SUB_M_B] & ~0x80) | ((0 << 7) & 0x80);	//Bass DC mode - OFF
	I2C_send(TDA7719_SUB_M_B , TDA7719_registers[TDA7719_SUB_M_B]);

	TDA7719_registers[TDA7719_ATT_LF] = (TDA7719_registers[TDA7719_ATT_LF] & ~0x7F) | ((0 << 0) & 0x7F);	//speaker attenuation left front - 0dB
	TDA7719_registers[TDA7719_ATT_LF] = (TDA7719_registers[TDA7719_ATT_LF] & ~0x80) | ((1 << 7) & 0x80);	//Soft step action - wait
	I2C_send(TDA7719_ATT_LF , TDA7719_registers[TDA7719_ATT_LF]);

	TDA7719_registers[TDA7719_ATT_RF] = (TDA7719_registers[TDA7719_ATT_RF] & ~0x7F) | ((0 << 0) & 0x7F);	//speaker attenuation right front - 0dB
	TDA7719_registers[TDA7719_ATT_RF] = (TDA7719_registers[TDA7719_ATT_RF] & ~0x80) | ((1 << 7) & 0x80);	//soft step action - wait
	I2C_send(TDA7719_ATT_RF , TDA7719_registers[TDA7719_ATT_RF]);

	TDA7719_registers[TDA7719_ATT_LR] = (TDA7719_registers[TDA7719_ATT_LR] & ~0x7F) | ((0 << 0) & 0x7F);	//speaker attenuation left rear - 0dB
	TDA7719_registers[TDA7719_ATT_LR] = (TDA7719_registers[TDA7719_ATT_LR] & ~0x80) | ((1 << 7) & 0x80);	//soft step action - wait
	I2C_send(TDA7719_ATT_LR , TDA7719_registers[TDA7719_ATT_LR]);

	TDA7719_registers[TDA7719_ATT_RR] = (TDA7719_registers[TDA7719_ATT_RR] & ~0x7F) | ((0 << 0) & 0x7F);	//speaker attenuation right rear - 0dB
	TDA7719_registers[TDA7719_ATT_RR] = (TDA7719_registers[TDA7719_ATT_RR] & ~0x80) | ((1 << 7) & 0x80);	//soft step action - wait
	I2C_send(TDA7719_ATT_RR , TDA7719_registers[TDA7719_ATT_RR]);

	//subwoofer gain/attenuation - left
	//subwoofer gain/attenuation - right
}

void TDA7719_PowerOff(void)
{

}

// Volume master takes into account attenuator of each channel and front and back volume aswell
void TDA7719_SetVolume_Master(const int16_t VolFrontLeft, const int16_t VolFrontRight, const int16_t VolBackLeft, const int16_t VolBackRight)
{
	//write to diffrent TDA7719 register depending on value
	if ((Device_Cfg_Audio.VolFront.volumeMaster <= 94) && (Device_Cfg_Audio.VolFront.volumeMaster >= 80))
	{
		TDA7719_SetVolume((Device_Cfg_Audio.VolFront.volumeMaster) - 79, 0, 0);
	}
	else if ((Device_Cfg_Audio.VolFront.volumeMaster >= 0) && (Device_Cfg_Audio.VolFront.volumeMaster <= 79))
	{
		TDA7719_SetVolume_LeftFront((VolFrontLeft), 0);
		TDA7719_SetVolume_RightFront((VolFrontRight), 0);
		TDA7719_SetVolume_LeftRear((VolBackLeft), 0);
		TDA7719_SetVolume_RightRear((VolBackRight), 0);
	}
}

// Sets volume just for both front channels
void TDA7719_SetVolumeFront_LeftRight(const int16_t VolFrontLeft, const int16_t VolFrontRight)
{

	if (VolFrontLeft <= -79)
		TDA7719_SetVolume_LeftFront(VOLUME_MUTE, 0); //0 - mute, 79 - max_volume
	else
		TDA7719_SetVolume_LeftFront(VolFrontLeft, 0);

	if (VolFrontRight <= -79)
		TDA7719_SetVolume_RightFront(VOLUME_MUTE, 0); //0 - mute, 79 - max_volume
	else
		TDA7719_SetVolume_RightFront(VolFrontRight, 0);

}

// Sets volume just for both back channels
void TDA7719_SetVolumeBack_LeftRight(const int16_t VolBackLeft, const int16_t VolBackRight)
{
	if (VolBackLeft <= -79)
		TDA7719_SetVolume_LeftRear(VOLUME_MUTE, 0); //0 - mute, 79 - max_volume
	else
		TDA7719_SetVolume_LeftRear(VolBackLeft, 0);

	if (VolBackRight <= -79)
		TDA7719_SetVolume_RightRear(VOLUME_MUTE, 0); //0 - mute, 79 - max_volume
	else
		TDA7719_SetVolume_RightRear(VolBackRight, 0);

}
//====================== Input configuration / main selector ========================//
/**
 *  @brief Sets input for main source
 *
 *  Page 31. Register 0x00
 *
 *  tutaj opis konkretnych ustawień i które wybrano i przypisane
 *
 *  @param[in] input value beetween 0-7
 */
void TDA7719_SetMainInput(int8_t const input)
{
	if(input < 0 || input > 7)return;

	TDA7719_registers[TDA7719_INPUT] = (TDA7719_registers[TDA7719_INPUT] & ~0x7) | (input & 0x7);
	I2C_send(TDA7719_INPUT , TDA7719_registers[TDA7719_INPUT]);
}

//====================== Main source input gain ========================//
/**
 *  @brief Sets input gain main source
 *
 *  Page 31. Register 0x00
 *
 *  @param[in] true +3dB, false 0dB
 */
void TDA7719_SetInputGain(_Bool const input_gain)
{
	TDA7719_registers[TDA7719_INPUT] = (TDA7719_registers[TDA7719_INPUT] & ~0x10) | ((input_gain << 4) & 0x10);
	I2C_send(TDA7719_INPUT , TDA7719_registers[TDA7719_INPUT]);
}

//====================== Input configuration ========================//
/**
 *  @brief Sets Input configuration
 *
 *  Page 31. Register 0x00
 *
 *  tutaj spis konfiguracji
 *
 *  @param[in] config, uint8_t 0...7
 */
void TDA7719_SetInputConfig(int8_t const input_config)
{
	if(input_config < 0 || input_config > 7)return;

	TDA7719_registers[TDA7719_INPUT] = (TDA7719_registers[TDA7719_INPUT] & ~0xE0) | ((input_config << 5) & 0xE0);;
	I2C_send(TDA7719_INPUT , TDA7719_registers[TDA7719_INPUT]);
}




//====================== Set secondary input ========================//
/**
 *  @brief Sets Input configuration
 *
 *  Page 32. Register 0x01
 *
 *  tutaj spis konfiguracji
 *
 *  @param[in] input, uint8_t 0...7
 */
void TDA7719_SetSecondInput(int8_t const input)
{
	if(input < 0 || input > 7)return;

	TDA7719_registers[TDA7719_INPUT_2] = (TDA7719_registers[TDA7719_INPUT_2] & ~0x7) | (input & 0x7);
	I2C_send(TDA7719_INPUT_2 , TDA7719_registers[TDA7719_INPUT_2]);
}

//====================== Second source input gain ========================//
/**
 *  @brief
 *
 *  Page 32. Register 0x01
 *
 *  tutaj spis konfiguracji
 *
 *  @param[in] input_gain, _Bool false +0dB gain, true +3dB
 */
void TDA7719_SetSecondSourceInputGain(_Bool const input_gain)
{
	TDA7719_registers[TDA7719_INPUT_2] = (TDA7719_registers[TDA7719_INPUT_2] & ~0x10) | ((input_gain << 4) & 0x10);
	I2C_send(TDA7719_INPUT_2 , TDA7719_registers[TDA7719_INPUT_2]);
}

//====================== Set bypass ========================//
/**
 *  @brief
 *
 *  Page 32. Register 0x01
 *
 */
void TDA7719_SetBypass(void)
{
	I2C_send(TDA7719_INPUT_2 , TDA7719_registers[TDA7719_INPUT_2]);
}

//====================== Mixing Source Selector ========================//
/**
 *  @brief
 *
 *  Page 33. Register 0x02
 *
 *  tutaj spis konfiguracji
 *
 *  @param[in] uint8_t, 0...7
 */
void TDA7719_SetMixingSource(uint8_t const mix_sell)
{
	TDA7719_registers[TDA7719_MIX_SOUR] = (TDA7719_registers[TDA7719_MIX_SOUR] & ~0x7) | (mix_sell & 0x7);
	I2C_send(TDA7719_MIX_SOUR , TDA7719_registers[TDA7719_MIX_SOUR]);
}


//====================== Mixing Attenuator ========================//
/**
 *  @brief
 *
 *  @param[in] uint8_t, 0...7
 */
void TDA7719_SetMixingAttenuator(int16_t mix_att)
{
	mix_att = abs(mix_att);
	TDA7719_registers[TDA7719_MIX_SOUR] = (TDA7719_registers[TDA7719_MIX_SOUR] & ~0xF8) | ((mix_att << 3) & 0xF8);
	I2C_send(TDA7719_MIX_SOUR , TDA7719_registers[TDA7719_MIX_SOUR]);
}


//====================== Mix control ========================//
/**
 *  @brief
 *
 *  Page 34. Register 0x03
 *
 *  @param[in] uint8_t, 0...7
 */
void TDA7719_SetMixSourceTo(TDA7719_SetMixSourceTo_t SetMixSourceTo)
{
	if(SetMixSourceTo == TDA7719_MIX_TO_FRONT)
	    {
			TDA7719_registers[TDA7719_MIX_CONT] = (TDA7719_registers[TDA7719_MIX_SOUR] & ~0x1) | ((TDA7719_MIX_TO_FRONT << 0) & 0x1);
			TDA7719_registers[TDA7719_MIX_CONT] = (TDA7719_registers[TDA7719_MIX_SOUR] & ~0x2) | ((TDA7719_MIX_TO_FRONT << 1) & 0x2);
	    }
	else if(SetMixSourceTo == TDA7719_MIX_TO_BACK)
	    {
			TDA7719_registers[TDA7719_MIX_CONT] = (TDA7719_registers[TDA7719_MIX_SOUR] & ~0x4) | ((TDA7719_MIX_TO_BACK << 2) & 0x4);
			TDA7719_registers[TDA7719_MIX_CONT] = (TDA7719_registers[TDA7719_MIX_SOUR] & ~0x8) | ((TDA7719_MIX_TO_BACK << 3) & 0x8);
	    }
	I2C_send(TDA7719_MIX_CONT , TDA7719_registers[TDA7719_MIX_CONT]);
}


//====================== Rear Speaker Input Configuration ========================//
/**
 *  @brief
 *
 *  @param[in] _Bool, true - Main sygnal, false - Direct path/secoond source
 */
void TDA7719_SetRearSpeakerInput(_Bool const Source)
{
	TDA7719_registers[TDA7719_MIX_CONT] = (TDA7719_registers[TDA7719_MIX_SOUR] & ~0x10) | ((Source << 4) & 0x10);
	I2C_send(TDA7719_MIX_CONT , TDA7719_registers[TDA7719_MIX_CONT]);
}

//====================== Soft mute ========================//
/**
 *  @brief
 *
 *  0 - 0.48ms, 1 - 0.96ms, 2 - 7.68ms, 3 - 15.36ms
 *
 *  @param[in] Soft Mute - 1 OFF, 0 - ON
 *
 *  @param[in]
 */
void TDA7719_SetSoftMute(_Bool const Mute_ON_OFF, uint8_t const SoftMuteTime)
{
	TDA7719_registers[TDA7719_MUTE] = (TDA7719_registers[TDA7719_MUTE] & ~0x1) | ((Mute_ON_OFF << 0) & 0x1);
	TDA7719_registers[TDA7719_MUTE] = (TDA7719_registers[TDA7719_MUTE] & ~0xC) | ((SoftMuteTime << 2) & 0xC);
	I2C_send(TDA7719_MUTE , TDA7719_registers[TDA7719_MUTE]);
}

//====================== Subwoofer control ========================//
/**
 *  @brief
 *
 *  0 - 0.48ms, 1 - 0.96ms, 2 - 7.68ms, 3 - 15.36ms
 *
 *  @param[in] subwoofer Enable (OUTL3 & OUTR3)
 *
 *  @param[in] Subwoofer Input Configuration 0 - Input Mux, 1 - Bass Output
 */
void TDA7719_SetSubwooferConfig(_Bool const sub_eneble, _Bool const sub_in_conf)
{
	TDA7719_registers[TDA7719_MUTE] = (TDA7719_registers[TDA7719_MUTE] & ~0x10) | ((sub_in_conf << 4) & 0x10);
	TDA7719_registers[TDA7719_MUTE] = (TDA7719_registers[TDA7719_MUTE] & ~0x20) | ((sub_eneble << 5) & 0x20);
	I2C_send(TDA7719_MUTE , TDA7719_registers[TDA7719_MUTE]);
}


//reg 5


//reg 6


//====================== Loudness control ========================//
/**
 *  @brief
 *
 *
 *  @param[in]
 *
 *  @param[in]
 */
void TDA7719_SetLoudness(int8_t loud_att, uint8_t const loud_f, uint8_t const loud_b, const uint8_t loud_s)
{
	loud_att = abs(loud_att);
	TDA7719_registers[TDA7719_LOUD] = (TDA7719_registers[TDA7719_LOUD] & ~0xF) | ((loud_att << 0) & 0xF);
	TDA7719_registers[TDA7719_LOUD] = (TDA7719_registers[TDA7719_LOUD] & ~0x30) | ((loud_f   << 4) & 0x30);
	TDA7719_registers[TDA7719_LOUD] = (TDA7719_registers[TDA7719_LOUD] & ~0x40) | ((loud_b   << 6) & 0x40);
	TDA7719_registers[TDA7719_LOUD] = (TDA7719_registers[TDA7719_LOUD] & ~0x80) | ((loud_s   << 7) & 0x80);

	I2C_send(TDA7719_LOUD , TDA7719_registers[TDA7719_LOUD]);
}

//====================== Volume control ========================//
/**
 *  @brief
 *
 *
 *  @param[in]
 *
 *  @param[in]
 */
void TDA7719_SetVolume(int8_t gain, uint8_t const out_gain, uint8_t const soft_step)
{
	switch(gain)
	{
	    case -15:gain = 0b00000000;break;
	    case -14:gain = 0b00000001;break;
	    case -13:gain = 0b00000010;break;
	    case -12:gain = 0b00000011;break;
	    case -11:gain = 0b00000100;break;
	    case -10:gain = 0b00000101;break;
	    case -9:gain  = 0b00000110;break;
	    case -8:gain  = 0b00000111;break;
	    case -7:gain  = 0b00001000;break;
	    case -6:gain  = 0b00001001;break;
	    case -5:gain  = 0b00001010;break;
	    case -4:gain  = 0b00001011;break;
	    case -3:gain  = 0b00001100;break;
	    case -1:gain  = 0b00001110;break;
	    case 0:gain   = 0b00001111;break;
	    case 1:gain   = 0b00011110;break;
	    case 2:gain   = 0b00011101;break;
	    case 3:gain   = 0b00011100;break;
	    case 4:gain   = 0b00011011;break;
	    case 5:gain   = 0b00011010;break;
	    case 6:gain   = 0b00011001;break;
	    case 7:gain   = 0b00011000;break;
	    case 8:gain   = 0b00010111;break;
	    case 9:gain   = 0b00010110;break;
	    case 10:gain  = 0b00010101;break;
	    case 11:gain  = 0b00010100;break;
	    case 12:gain  = 0b00010011;break;
	    case 13:gain  = 0b00010010;break;
	    case 14:gain  = 0b00010001;break;
	    case 15:gain  = 0b00010000;break;
	}
	TDA7719_registers[TDA7719_VOL] = (TDA7719_registers[TDA7719_VOL] & ~0x1F) | ((gain << 0) & 0x1F);
	TDA7719_registers[TDA7719_VOL] = (TDA7719_registers[TDA7719_VOL] & ~0x40) | ((out_gain << 6) & 0x40);
	TDA7719_registers[TDA7719_VOL] = (TDA7719_registers[TDA7719_VOL] & ~0x80) | ((soft_step << 7) & 0x80);

	I2C_send(TDA7719_VOL , TDA7719_registers[TDA7719_VOL]);
}

//====================== Loudness control ========================//
/**
 *  @brief
 *
 *
 *  @param[in]
 *
 *  @param[in]
 */
void TDA7719_SetTreble(int8_t gain_treb, uint8_t const treb_f, uint8_t const soft_treb)
{
	switch(gain_treb)
	{
	    case -15:gain_treb = 0b00000000;break;
	    case -14:gain_treb = 0b00000001;break;
	    case -13:gain_treb = 0b00000010;break;
	    case -12:gain_treb = 0b00000011;break;
	    case -11:gain_treb = 0b00000100;break;
	    case -10:gain_treb = 0b00000101;break;
	    case -9:gain_treb = 0b00000110;break;
	    case -8:gain_treb = 0b00000111;break;
	    case -7:gain_treb = 0b00001000;break;
	    case -6:gain_treb = 0b00001001;break;
	    case -5:gain_treb = 0b00001010;break;
	    case -4:gain_treb = 0b00001011;break;
	    case -3:gain_treb = 0b00001100;break;
	    case -2:gain_treb = 0b00001101;break;
	    case -1:gain_treb = 0b00001110;break;
	    case 0:gain_treb = 0b00001111;break;
	    case 1:gain_treb = 0b00011110;break;
	    case 2:gain_treb = 0b00011101;break;
	    case 3:gain_treb = 0b00011100;break;
	    case 4:gain_treb = 0b00011011;break;
	    case 5:gain_treb = 0b00011010;break;
	    case 6:gain_treb = 0b00011001;break;
	    case 7:gain_treb = 0b00011000;break;
	    case 8:gain_treb = 0b00010111;break;
	    case 9:gain_treb = 0b00010110;break;
	    case 10:gain_treb = 0b00010101;break;
	    case 11:gain_treb = 0b00010100;break;
	    case 12:gain_treb = 0b00010011;break;
	    case 13:gain_treb = 0b00010010;break;
	    case 14:gain_treb = 0b00010001;break;
	    case 15:gain_treb = 0b00010000;break;
	}
	TDA7719_registers[TDA7719_TRBLE] = (TDA7719_registers[TDA7719_TRBLE] & ~0x1F) | ((gain_treb << 0) & 0x1F);
	TDA7719_registers[TDA7719_TRBLE] = (TDA7719_registers[TDA7719_TRBLE] & ~0x60) | ((treb_f << 5) & 0x60);
	TDA7719_registers[TDA7719_TRBLE] = (TDA7719_registers[TDA7719_TRBLE] & ~0x80) | ((soft_treb << 7) & 0x80);

	I2C_send(TDA7719_TRBLE , TDA7719_registers[TDA7719_TRBLE]);
}

//====================== Loudness control ========================//
/**
 *  @brief
 *
 *
 *  @param[in]
 *
 *  @param[in]
 */
void TDA7719_SetMiddle(int8_t gain_mid,  uint8_t const mid_q,  uint8_t const soft_mid)
{
	switch(gain_mid)
	{
	    case -15:gain_mid = 0b00000000;break;
	    case -14:gain_mid = 0b00000001;break;
	    case -13:gain_mid = 0b00000010;break;
	    case -12:gain_mid = 0b00000011;break;
	    case -11:gain_mid = 0b00000100;break;
	    case -10:gain_mid = 0b00000101;break;
	    case -9:gain_mid = 0b00000110;break;
	    case -8:gain_mid = 0b00000111;break;
	    case -7:gain_mid = 0b00001000;break;
	    case -6:gain_mid = 0b00001001;break;
	    case -5:gain_mid = 0b00001010;break;
	    case -4:gain_mid = 0b00001011;break;
	    case -3:gain_mid = 0b00001100;break;
	    case -2:gain_mid = 0b00001101;break;
	    case -1:gain_mid = 0b00001110;break;
	    case 0:gain_mid = 0b00001111;break;
	    case 1:gain_mid = 0b00011110;break;
	    case 2:gain_mid = 0b00011101;break;
	    case 3:gain_mid = 0b00011100;break;
	    case 4:gain_mid = 0b00011011;break;
	    case 5:gain_mid = 0b00011010;break;
	    case 6:gain_mid = 0b00011001;break;
	    case 7:gain_mid = 0b00011000;break;
	    case 8:gain_mid = 0b00010111;break;
	    case 9:gain_mid = 0b00010110;break;
	    case 10:gain_mid = 0b00010101;break;
	    case 11:gain_mid = 0b00010100;break;
	    case 12:gain_mid = 0b00010011;break;
	    case 13:gain_mid = 0b00010010;break;
	    case 14:gain_mid = 0b00010001;break;
	    case 15:gain_mid = 0b00010000;break;
	}
	TDA7719_registers[TDA7719_MIDDLE] = (TDA7719_registers[TDA7719_MIDDLE] & ~0x1F) | ((gain_mid << 0) & 0x1F);
	TDA7719_registers[TDA7719_MIDDLE] = (TDA7719_registers[TDA7719_MIDDLE] & ~0x60) | ((mid_q << 5) & 0x60);
	TDA7719_registers[TDA7719_MIDDLE] = (TDA7719_registers[TDA7719_MIDDLE] & ~0x80) | ((soft_mid << 7) & 0x80);
	I2C_send(TDA7719_MIDDLE , TDA7719_registers[TDA7719_MIDDLE]);
}

//====================== Loudness control ========================//
/**
 *  @brief
 *
 *
 *  @param[in]
 *
 *  @param[in]
 */
void TDA7719_SetBass(int8_t gain_bass, uint8_t const bass_q, uint8_t const soft_bass)
{
	switch(gain_bass)
	{
	    case -15:gain_bass = 0b00000000;break;
	    case -14:gain_bass = 0b00000001;break;
	    case -13:gain_bass = 0b00000010;break;
	    case -12:gain_bass = 0b00000011;break;
	    case -11:gain_bass = 0b00000100;break;
	    case -10:gain_bass = 0b00000101;break;
	    case -9:gain_bass  = 0b00000110;break;
	    case -8:gain_bass  = 0b00000111;break;
	    case -7:gain_bass  = 0b00001000;break;
	    case -6:gain_bass  = 0b00001001;break;
	    case -5:gain_bass  = 0b00001010;break;
	    case -4:gain_bass  = 0b00001011;break;
	    case -3:gain_bass  = 0b00001100;break;
	    case -2:gain_bass  = 0b00001101;break;
	    case -1:gain_bass  = 0b00001110;break;
	    case 0:gain_bass   = 0b00001111;break;

	    case 1:gain_bass   = 0b00011110;break;
	    case 2:gain_bass   = 0b00011101;break;
	    case 3:gain_bass   = 0b00011100;break;
	    case 4:gain_bass   = 0b00011011;break;
	    case 5:gain_bass   = 0b00011010;break;
	    case 6:gain_bass   = 0b00011001;break;
	    case 7:gain_bass   = 0b00011000;break;
	    case 8:gain_bass   = 0b00010111;break;
	    case 9:gain_bass   = 0b00010110;break;
	    case 10:gain_bass  = 0b00010101;break;
	    case 11:gain_bass  = 0b00010100;break;
	    case 12:gain_bass  = 0b00010011;break;
	    case 13:gain_bass  = 0b00010010;break;
	    case 14:gain_bass  = 0b00010001;break;
	    case 15:gain_bass  = 0b00010000;break;
	}
	TDA7719_registers[TDA7719_BASS] = (TDA7719_registers[TDA7719_BASS] & ~0x1F) | ((gain_bass << 0) & 0x1F);
	TDA7719_registers[TDA7719_BASS] = (TDA7719_registers[TDA7719_BASS] & ~0x60) | ((bass_q << 5) & 0x60);
	TDA7719_registers[TDA7719_BASS] = (TDA7719_registers[TDA7719_BASS] & ~0x80) | ((soft_bass << 7) & 0x80);
	I2C_send(TDA7719_BASS , TDA7719_registers[TDA7719_BASS]);
}

//====================== Loudness control ========================//
/**
 *  @brief
 *
 *
 *  @param[in]
 *
 *  @param[in]
 */
void TDA7719_SetSMB(uint8_t const sub_f, uint8_t const sub_out, uint8_t const mid_f, uint8_t const bass_f, uint8_t const bass_dc)
{
	TDA7719_registers[TDA7719_SUB_M_B] = (TDA7719_registers[TDA7719_SUB_M_B] & ~0x3) | ((sub_f << 0) & 0x3);
	TDA7719_registers[TDA7719_SUB_M_B] = (TDA7719_registers[TDA7719_SUB_M_B] & ~0x4) | ((sub_out << 2) & 0x4);
	TDA7719_registers[TDA7719_SUB_M_B] = (TDA7719_registers[TDA7719_SUB_M_B] & ~0x18) | ((mid_f << 3) & 0x18);
	TDA7719_registers[TDA7719_SUB_M_B] = (TDA7719_registers[TDA7719_SUB_M_B] & ~0x60) | ((bass_f << 5) & 0x60);
	TDA7719_registers[TDA7719_SUB_M_B] = (TDA7719_registers[TDA7719_SUB_M_B] & ~0x80) | ((bass_dc << 7) & 0x80);

	I2C_send(TDA7719_SUB_M_B, TDA7719_registers[TDA7719_SUB_M_B]);
}

//====================== Front left speaker volume control ========================//
/**
 *  @brief
 *
 *
 *  @param[in]
 *
 *  @param[in]
 */
void TDA7719_SetVolume_LeftFront(int8_t LeftFront, uint8_t const soft_lf)
{
	LeftFront = abs(LeftFront) + 0b00010000;
	TDA7719_registers[TDA7719_ATT_LF] = (TDA7719_registers[TDA7719_ATT_LF] & ~0x7F) | ((LeftFront << 0) & 0x7F);
	TDA7719_registers[TDA7719_ATT_LF] = (TDA7719_registers[TDA7719_ATT_LF] & ~0x80) | ((soft_lf << 7) & 0x80);

	I2C_send(TDA7719_ATT_LF, TDA7719_registers[TDA7719_ATT_LF]);
}

//====================== Right left speaker volume control ========================//
/**
 *  @brief
 *
 *
 *  @param[in]
 *
 *  @param[in]
 */
void TDA7719_SetVolume_RightFront(int8_t RightFront, uint8_t const soft_rf)
{
	RightFront = abs(RightFront) + 0b00010000;
	TDA7719_registers[TDA7719_ATT_RF] = (TDA7719_registers[TDA7719_ATT_RF] & ~0x7F) | ((RightFront << 0) & 0x7F);
	TDA7719_registers[TDA7719_ATT_RF] = (TDA7719_registers[TDA7719_ATT_RF] & ~0x80) | ((soft_rf << 7) & 0x80);

	I2C_send(TDA7719_ATT_RF, TDA7719_registers[TDA7719_ATT_RF]);
}

//====================== Rear left speaker volume control ========================//
/**
 *  @brief
 *
 *
 *  @param[in]
 *
 *  @param[in]
 */
void TDA7719_SetVolume_LeftRear(int8_t LeftRear, uint8_t const soft_lr)
{
	LeftRear = abs(LeftRear) + 0b00010000;
	TDA7719_registers[TDA7719_ATT_LR] = (TDA7719_registers[TDA7719_ATT_LR] & ~0x7F) | ((LeftRear << 0) & 0x7F);
	TDA7719_registers[TDA7719_ATT_LR] = (TDA7719_registers[TDA7719_ATT_LR] & ~0x80) | ((soft_lr << 7) & 0x80);

	I2C_send(TDA7719_ATT_LR, TDA7719_registers[TDA7719_ATT_LR]);
}

//====================== Rear Right speaker volume control ========================//
/**
 *  @brief
 *
 *
 *  @param[in]
 *
 *  @param[in]
 */
void TDA7719_SetVolume_RightRear(int8_t RightRear, uint8_t const soft_rr)
{

	RightRear = abs(RightRear) + 0b00010000;
	TDA7719_registers[TDA7719_ATT_RR] = (TDA7719_registers[TDA7719_ATT_RR] & ~0x7F) | ((RightRear << 0) & 0x7F);
	TDA7719_registers[TDA7719_ATT_RR] = (TDA7719_registers[TDA7719_ATT_RR] & ~0x80) | ((soft_rr << 7) & 0x80);

	I2C_send(TDA7719_ATT_RR, TDA7719_registers[TDA7719_ATT_RR]);
}

void TDA7719_SetVol_SUB_L(int8_t sl, uint8_t const soft_sl)
{
	sl = abs(sl) + 0b00010000;
	TDA7719_registers[TDA7719_ATT_SUB_L] = (TDA7719_registers[TDA7719_ATT_SUB_L] & ~0x7F) | ((sl << 0) & 0x7F);
	TDA7719_registers[TDA7719_ATT_SUB_L] = (TDA7719_registers[TDA7719_ATT_SUB_L] & ~0x80) | ((soft_sl << 7) & 0x80);
	I2C_send(TDA7719_ATT_SUB_L, TDA7719_registers[TDA7719_ATT_SUB_L]);
}

void TDA7719_SetVol_SUB_R(int8_t sr, uint8_t const soft_sr)
{
	sr = abs(sr) + 0b00010000;
	TDA7719_registers[TDA7719_ATT_SUB_R] = (TDA7719_registers[TDA7719_ATT_SUB_R] & ~0x7F) | ((sr << 0) & 0x7F);
	TDA7719_registers[TDA7719_ATT_SUB_R] = (TDA7719_registers[TDA7719_ATT_SUB_R] & ~0x80) | ((soft_sr << 7) & 0x80);
	I2C_send(TDA7719_ATT_SUB_R, TDA7719_registers[TDA7719_ATT_SUB_R]);
}

//====================== Communication with TDA7719 device via I2C ========================//
static void I2C_send(uint8_t const SubAddress, uint8_t const Data)
{
	uint8_t tab_to_send[2];
	tab_to_send[0] = SubAddress;
	tab_to_send[1] = Data;

	HAL_I2C_Master_Transmit(&hi2c2, TDA7719_address, tab_to_send, 2, 10);

}

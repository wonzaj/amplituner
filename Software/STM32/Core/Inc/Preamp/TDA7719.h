#ifndef INC_TDA7719_H_
#define INC_TDA7719_H_
//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "config.h"
#include "i2c.h"
#include "main.h"
//#include "tim.h"

//https://www.st.com/resource/en/datasheet/tda7719.pdf


//--------------------------------------------------------------
// Defines
//--------------------------------------------------------------

//Address 7 bit
#define TDA7719_address (0x44<<1)
//Sub addresses of registers
#define TDA7719_INPUT        0x00 // Input Configuration / Main Source Selector
#define TDA7719_INPUT_2      0x01 // 2nd Source Selector / Direct Path
#define TDA7719_MIX_SOUR     0x02 // Mixing Source / Mixing Gain
#define TDA7719_MIX_CONT     0x03 // Mix Control / Level Meter / DC Offset Detector Config
#define TDA7719_MUTE         0x04 // Soft Mute / Others
#define TDA7719_SOFT_1       0x05 // Soft Step I
#define TDA7719_SOFT_2       0x06 // Soft Step II / DC-detector
#define TDA7719_LOUD         0x07 // Loudness
#define TDA7719_VOL          0x08 // Volume / Output Gain
#define TDA7719_TRBLE        0x09 // Treble
#define TDA7719_MIDDLE       0x0A // Middle
#define TDA7719_BASS         0x0B // Bass
#define TDA7719_SUB_M_B      0x0C // Subwoofer / Middle / Bass
#define TDA7719_ATT_LF       0x0D // Speaker Attenuator Left Front
#define TDA7719_ATT_RF       0x0E // Speaker Attenuator Right Front
#define TDA7719_ATT_LR       0x0F // Speaker Attenuator Left Rear
#define TDA7719_ATT_RR       0x10 // Speaker Attenuator Right Rear
#define TDA7719_ATT_SUB_L    0x11 // Subwoofer Attenuator Left
#define TDA7719_ATT_SUB_R    0x12 // Subwoofer Attenuator Right
#define TDA7719_TEST_1       0x13 // Testing Audio Processor 1
#define TDA7719_TEST_2       0x14 // Testing Audio Processor 2

/*
 * INPUT pins configuration
 *
 * QD - Quasi-Differential, SD - Signle-ended input, MD -  mono Differential, FD = Full Differential
 */
#define TDA7719_CFG0 	0b000 // 4x QD				//
#define TDA7719_CFG1 	0b001 // 2x QD + 3x SD			//
#define TDA7719_CFG2 	0b010 // 1x QD + 5x SD			// IN0-SE1, IN1-SE3, IN2-QD3, IN3-NULL, IN4-SE2, IN5-SE4, IN6-SE5
#define TDA7719_CFG3 	0b011 // 1x QD + 3x SE + 2x MD		//
#define TDA7719_CFG4 	0b100 // 3x QD + 1x FD			//
#define TDA7719_CFG5 	0b101 // 3x QD + 2x SE			//
#define TDA7719_CFG6 	0b110 // 1x QD + 2x SE + 1x FD + 1x MD	//
#define TDA7719_CFG7 	0b111 // 1x QD + 3x SE + 1x FD		//

#define TDA7719_IN0 0b000 //bluetooth
#define TDA7719_IN1 0b001 //jack 1
#define TDA7719_IN2 0b010 //jack 2
#define TDA7719_IN3 0b011
#define TDA7719_IN4 0b100
#define TDA7719_IN5 0b101 //radio
#define TDA7719_IN6 0b110 //microphone
#define TDA7719_IN7 0b111

extern TDA7719_SetMixSourceTo_t 		SetMixSourceTo;
extern TDA7719_config_t 				TDA7719_config;
extern TDA7719_Control_t 				TDA7719_Controls_t;
//--------------------------------------------------------------
// Function declaration
//--------------------------------------------------------------
void TDA7719_SetVolume_Master(const int16_t VolFrontLeft, const int16_t VolFrontRight, const int16_t VolBackLeft, const int16_t VolBackRight);
void TDA7719_SetVolumeBack_LeftRight(const int16_t VolBackLeft, const int16_t VolBackRight);
void TDA7719_SetVolumeFront_LeftRight(const int16_t VolFrontLeft,const int16_t VolFrontRight);

void TDA7719_SetMainInput(int8_t const input);
void TDA7719_SetInputGain(_Bool const input_gain);
void TDA7719_SetInputConfig(int8_t const input_config);
void TDA7719_SetSecondInput(int8_t const input);
void TDA7719_SetSecondSourceInputGain(_Bool const input_gain);
void TDA7719_SetBypass(void);
void TDA7719_SetMixingSource(uint8_t const mix_sell);
void TDA7719_SetMixSourceTo(TDA7719_SetMixSourceTo_t SetMixSourceTo);
void TDA7719_SetRearSpeakerInput(_Bool const Source);
void TDA7719_SetSoftMute(_Bool const ON_OFF, uint8_t const SoftMuteTime);
void TDA7719_SetSubwooferConfig(_Bool const sub_eneble, _Bool const sub_in_conf);
void TDA7719_SetVolume(int8_t gain, uint8_t const out_gain, uint8_t const soft_step);
void TDA7719_SetVolume_LeftFront(int8_t LeftFront, uint8_t const soft_lf);
void TDA7719_SetVolume_RightFront(int8_t RightFront, uint8_t const soft_rf);
void TDA7719_SetVolume_LeftRear(int8_t LeftRear, uint8_t const soft_lr);
void TDA7719_SetVolume_RightRear(int8_t RightRear, uint8_t const soft_rr);

void TDA7719_SetBypass(void);
void TDA7719_init(void);
void TDA7719_PowerOff(void);
void TDA7719_SetInput(uint8_t input, uint8_t md, uint8_t input_gain, uint8_t input_conf);
void TDA7719_SetInput_2nd(uint8_t sell, uint8_t md_2, uint8_t input_gain_2, uint8_t bypass_front, uint8_t bypass_rear, uint8_t bypass_sub);
void TDA7719_SetMix_source(uint8_t mix_sell, int8_t mix_att);
void TDA7719_SetMix_cont(uint8_t mix_fl, uint8_t mix_fr, uint8_t mix_rl, uint8_t mix_rr, uint8_t rear_speak,uint8_t ref_out_sell, uint8_t level_metr, uint8_t dc);
void TDA7719_SetMute(uint8_t mute, uint8_t pin_mute, uint8_t time_mute, uint8_t sub_in_conf, uint8_t sub_eneble,uint8_t fast, uint8_t filter);
void TDA7719_SetSoft_1(uint8_t soft_loun, uint8_t soft_vol, uint8_t soft_treb, uint8_t soft_mid, uint8_t soft_bass,uint8_t soft_lf, uint8_t soft_fr, uint8_t soft_lr);
void TDA7719_SetSoft_2(uint8_t soft_rr, uint8_t soft_sub_l, uint8_t soft_sub_r, uint8_t soft_time, uint8_t soft_zero,uint8_t soft_time_cons);
void TDA7719_SetLoudness(int8_t loud_att, uint8_t const loud_f, uint8_t const loud_b, uint8_t const loud_s);
void TDA7719_SetTreble(int8_t gain_treb, uint8_t const treb_f, uint8_t const soft_treb);
void TDA7719_SetMiddle(int8_t gain_mid, uint8_t const mid_q, uint8_t const soft_mid);
void TDA7719_SetBass(int8_t gain_bass, uint8_t const bass_q, uint8_t const soft_bass);
void TDA7719_SetSMB(uint8_t const sub_f, uint8_t const , uint8_t const mid_f, uint8_t const bass_f, uint8_t const bass_dc);
void TDA7719_SetVol_SUB_L(int8_t sl, uint8_t const soft_sl);
void TDA7719_SetVol_SUB_R(int8_t sr, uint8_t const soft_sr);

#endif /* INC_TDA7719_H_ */

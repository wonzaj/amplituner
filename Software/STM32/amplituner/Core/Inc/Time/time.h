#ifndef INC_TIME_TIME_H_
#define INC_TIME_TIME_H_
//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "rtc.h"
#include "Display_OLED/draws_display.h"


//--------------------------------------------------------------
// Variables
//--------------------------------------------------------------
extern RTC_AlarmTypeDef 	Alarm_A;
extern RTC_AlarmTypeDef 	Alarm_B;
extern RTC_AlarmTypeDef 	Alarm;
extern RTC_typeOfAlarm_t 	RTC_typeOfAlarm;
extern RTC_typeOfAlarm_t 	RTC_typeOfAlarm_A;
extern RTC_typeOfAlarm_t 	RTC_typeOfAlarm_B;
extern SSD1322_Screen_t 	SSD1322_Screen_State;


//--------------------------------------------------------------
// Function declarations
//--------------------------------------------------------------
void SetAlarm(RTC_AlarmTypeDef Alarm_);
void RTC_Alarm_activated_routine(void);

void switch_change_time(Clock_Data_Change_t Clock , _Bool add_subb);
void switch_change_alarm(AlarmDataChange_t AlarmDataChange, _Bool add_subb);
void Set_Alarm_Mode(RTC_typeOfAlarm_t typeOfAlarm);


#endif /* INC_TIME_TIME_H_ */

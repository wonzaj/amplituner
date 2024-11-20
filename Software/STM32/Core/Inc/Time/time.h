#ifndef INC_TIME_TIME_H_
#define INC_TIME_TIME_H_
//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "RTC.h"
#include "Display_API.h"

#define CHANGE_VALUE_UP 		1
#define CHANGE_VALUE_DOWN 	2

typedef enum
{
    HOUR 	= 1,
    MINUTE 	= 2,
    DAYWEEK = 3,
    DAY 	= 4,
    MODE 	= 5,
    SECOND 	= 6,
    MONTH 	= 7,
    YEAR 	= 8,
	TIME_PARAMETER_ENUMMAX
} TimeComponent_t;

typedef enum
{
    HOUR_ALARM 		= 1,
    MINUTE_ALARM 	= 2,
    DAYWEEK_ALARM 	= 3,
    MODE_ALARM 		= 4,
    SET_ALARM 		= 5,
    PREVIEW_ALARM 	= 6,
	ALARMTIMECOMPONENT_MAXENUM
} AlarmTimeComponent_t;

typedef enum
{
    ONLY_ONE_TIME_ALARM	= 1,
    ONLY_WEEKENDS_ALARM = 2,
    EVERYDAY_ALARM		= 3,
    MON_to_FRI_ALARM 	= 4,
    EXACT_DATE_ALRAM 	= 5
}RTC_typeOfAlarm_t;
//--------------------------------------------------------------
// Variables
//--------------------------------------------------------------
extern RTC_TimeTypeDef 		sTime;
extern RTC_DateTypeDef 		sDate;
extern RTC_AlarmTypeDef 		Alarm_A;
extern RTC_AlarmTypeDef 		Alarm_B;
extern RTC_AlarmTypeDef 		Alarm;
extern RTC_typeOfAlarm_t 		RTC_typeOfAlarm;
extern RTC_typeOfAlarm_t 		RTC_typeOfAlarm_A;
extern RTC_typeOfAlarm_t 		RTC_typeOfAlarm_B;

extern _Bool IS_ALARM_SET_A;
extern _Bool IS_ALARM_SET_B;
//--------------------------------------------------------------
// Function declarations
//--------------------------------------------------------------
void Time_ChangeSelected_TimeComponent(void);
void Time_ChangeSelected_AlarmComponent(void);
void Time_ReadAndSet_TimeAndDate(uint8_t up_or_down);
void Time_ReadAndSet_Alarms(uint8_t up_or_down);


void SetAlarm(RTC_AlarmTypeDef Alarm_);
void RTC_Alarm_activated_routine(void);

void Set_Alarm_Mode(RTC_typeOfAlarm_t typeOfAlarm);


#endif /* INC_TIME_TIME_H_ */

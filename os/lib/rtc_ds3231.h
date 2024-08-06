#ifndef RTC_DS3231_H
#define RTC_DS3231_H

#include "../../platform.h"
#include "../drivers/I2cMaster.h"
#include "../drivers/I2cSoft.h"

#define RTC3231_INT_ENABLE 	0
#define RTC3231_OUT_ONE 	1
#define RTC3231_SQR_1HZ 	2
#define RTC3231_SQR_4KHZ 	3
#define RTC3231_SQR_8KHZ 	4

#define RTC3231_SQR_32KHZ_DISABLE 0
#define RTC3231_SQR_32KHZ_ENABLE 1

mos_uint8_t rtc3231_begin(const struct i2c_master_driver * i2c_port, mos_uint32_t i2c_baud, \
			mos_uint8_t mode, mos_uint8_t mode_32khz);
mos_uint8_t rtc3231_setTime(mos_uint8_t hour, mos_uint8_t minute, mos_uint8_t second);
mos_uint8_t rtc3231_setDate(mos_uint8_t day, mos_uint8_t date, mos_uint8_t month, mos_uint8_t year);
mos_uint8_t rtc3231_getTime(mos_uint8_t * hour, mos_uint8_t * minute, mos_uint8_t * second);
mos_uint8_t rtc3231_getDate(mos_uint8_t *day, mos_uint8_t * date, mos_uint8_t * month, mos_uint8_t * year);
float rtc3231_getTemperature(void);

//#ifdef RTC3231_ALARM1_ENABLE

#define RTC3231_ALARM1_MODE0 0
#define RTC3231_ALARM1_MODE1 1
#define RTC3231_ALARM1_MODE2 2
#define RTC3231_ALARM1_MODE3 3
#define RTC3231_ALARM1_MODE4 4
#define RTC3231_ALARM1_MODE5 5

mos_uint8_t rtc3231_setAlarm1(mos_uint8_t mode, mos_uint8_t date_day, mos_uint8_t hour, mos_uint8_t minute, mos_uint8_t second);
mos_uint8_t rtc3231_getAlarm1(mos_uint8_t * mode, mos_uint8_t * date_day, mos_uint8_t * hour, mos_uint8_t * minute, mos_uint8_t * second);
mos_uint8_t rtc3231_isActiveAlarm1(void);
mos_uint8_t rtc3231_isExpiredAlarm1(void);
mos_uint8_t rtc3231_resetAlarm1(void);
mos_uint8_t rtc3231_stopAlarm1(void);

//#endif

//#ifdef RTC3231_ALARM2_ENABLE

#define RTC3231_ALARM2_MODE0 0
#define RTC3231_ALARM2_MODE1 1
#define RTC3231_ALARM2_MODE2 2
#define RTC3231_ALARM2_MODE3 3
#define RTC3231_ALARM2_MODE4 4

mos_uint8_t rtc3231_setAlarm2(mos_uint8_t mode, mos_uint8_t date_day, mos_uint8_t hour, mos_uint8_t minute);
mos_uint8_t rtc3231_getAlarm2(mos_uint8_t * mode, mos_uint8_t * date_day, mos_uint8_t * hour, mos_uint8_t * minute);
mos_uint8_t rtc3231_isActiveAlarm2(void);
mos_uint8_t rtc3231_isExpiredAlarm2(void);
mos_uint8_t rtc3231_resetAlarm2(void);
mos_uint8_t rtc3231_stopAlarm2(void);

//#endif

#endif
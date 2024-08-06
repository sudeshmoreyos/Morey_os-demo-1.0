#ifndef RTC_DS1307_H
#define RTC_DS1307_H

#include "../../platform.h"
#include "../drivers/I2cMaster.h"
#include "../drivers/I2cSoft.h"

#define RTC1307_OUT_ZERO 	0
#define RTC1307_OUT_ONE 	1
#define RTC1307_SQR_1HZ 	2
#define RTC1307_SQR_4KHZ 	3
#define RTC1307_SQR_8KHZ 	4
#define RTC1307_SQR_32KHZ 	5

mos_uint8_t rtc1307_begin(const struct i2c_master_driver * i2c_port, mos_uint32_t i2c_baud, mos_uint8_t mode);
mos_uint8_t rtc1307_setTime(mos_uint8_t hour, mos_uint8_t minute, mos_uint8_t second);
mos_uint8_t rtc1307_setDate(mos_uint8_t day, mos_uint8_t date, mos_uint8_t month, mos_uint8_t year);
mos_uint8_t rtc1307_getTime(mos_uint8_t * hour, mos_uint8_t * minute, mos_uint8_t * second);
mos_uint8_t rtc1307_getDate(mos_uint8_t *day, mos_uint8_t * date, mos_uint8_t * month, mos_uint8_t * year);


#endif
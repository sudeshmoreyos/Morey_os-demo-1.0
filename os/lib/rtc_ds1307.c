#include "rtc_ds1307.h"

#ifdef PLATFORM_SUPPORT_I2C

#ifndef RTC1307_I2C_PORT
	#ifdef I2C
		#define RTC1307_I2C_PORT I2C
	#endif
#endif

#ifdef RTC1307_I2C_PORT

#ifndef RTC1307_I2C_BAUD
	#define RTC1307_I2C_BAUD 100000
#endif

#define I2C_PORT RTC1307_I2C_PORT
#define I2C_BAUD RTC1307_I2C_BAUD
#define SLAVE_ADDR 0x68
#define CONTROL_REG_ADDR 0x07
#define TIME_REG_ADDR 0x00
#define DATE_REG_ADDR 0x03
#define TEMP_REG_ADDR 0x11

// Local Function to convert decimal to BCD
static mos_uint8_t dec2bcd(mos_uint8_t dec)
{
	return ((dec/10) << 4) | (dec%10);		
}

// Local Function to convert BCD to Decimal
static mos_uint8_t bcd2dec(mos_uint8_t bcd)
{
	return (bcd >> 4)*10 + (bcd & 0xF);		
}

mos_uint8_t rtc1307_begin(mos_uint8_t mode)
{
	mos_uint8_t data_tx[2] = {CONTROL_REG_ADDR,0x00};
	
	// Initialize I2C
	I2C_PORT.masterBegin(I2C_BAUD);
	
	// Set Control register values as per selected input mode
	if(mode == RTC1307_OUT_ZERO)
		data_tx[1] = 0x00;
	else if(mode == RTC1307_OUT_ONE)
		data_tx[1] = 0x80;
	else if(mode == RTC1307_SQR_1HZ)
		data_tx[1] = 0x10;
	else if(mode == RTC1307_SQR_4KHZ)
		data_tx[1] = 0x11;
	else if(mode == RTC1307_SQR_8KHZ)
		data_tx[1] = 0x12;
	else if(mode == RTC1307_SQR_32KHZ)
		data_tx[1] = 0x13;
	
	// Store Control register
	return I2C_PORT.masterTransfer(SLAVE_ADDR, 2 , data_tx, 0 , data_tx);
}

mos_uint8_t rtc1307_setTime(mos_uint8_t hour, mos_uint8_t minute, mos_uint8_t second)
{
	mos_uint8_t data_tx[4] = {TIME_REG_ADDR,0x00,0x00,0x00};
	

	// check if input value of second is valid, else return error = 1
	if (second > 59)
		return 1;
	
	// check if input value of minute is valid, else return error = 2
	if (minute > 59)
		return 2;
	
	// check if input value of hour is valid, else return error = 3
	if(hour > 23)
		return 3;
	
	// Convert decimal values to bcd values and store in data_tx variable
	data_tx[1] = dec2bcd(second);
	data_tx[2] = dec2bcd(minute);
	data_tx[3] = dec2bcd(hour);
	
	// Store Second, minute, hour, values
	return I2C_PORT.masterTransfer(SLAVE_ADDR, 4 , data_tx, 0 , data_tx);
}

mos_uint8_t rtc1307_setDate(mos_uint8_t day, mos_uint8_t date, mos_uint8_t month, mos_uint8_t year)
{
	mos_uint8_t data_tx[5] = {DATE_REG_ADDR,0x00,0x00,0x00,0x00};
	
	// check if input value of day is valid, else return error = 1
	if(day > 7 || day == 0)
		return 1;
	
	// check if input value of date is valid, else return error = 2
	if (date > 31 || date == 0)
		return 2;
	
	// check if input value of month is valid, else return error = 3
	if (month > 12 || month == 0)
		return 3;
	
	// check if input value of year is valid, else return error = 4
	if (year > 99)
		return 4;
	
	// Convert decimal values to bcd values and store in data_tx variable
	data_tx[1] = dec2bcd(day);
	data_tx[2] = dec2bcd(date);
	data_tx[3] = dec2bcd(month);
	data_tx[4] = dec2bcd(year);
	
	// Store day, date, month, year values
	return I2C_PORT.masterTransfer(SLAVE_ADDR, 5 , data_tx, 0 , data_tx);
}

mos_uint8_t rtc1307_getTime(mos_uint8_t * hour, mos_uint8_t * minute, mos_uint8_t * second)
{
	mos_uint8_t data_tx[1] = {TIME_REG_ADDR}, data_rx[3], status;
	
	// Read second, minute, hour values
	status = I2C_PORT.masterTransfer(SLAVE_ADDR, 1 , data_tx, 3, data_rx);
	
	if(status == 0)
	{
		// convert BCD values to Decimal values and store in second, minute, hour variables
		*second = bcd2dec(data_rx[0]);
		*minute = bcd2dec(data_rx[1]);
		*hour = bcd2dec(data_rx[2]);
	}
	
	return status;
}

mos_uint8_t rtc1307_getDate(mos_uint8_t *day, mos_uint8_t * date, mos_uint8_t * month, mos_uint8_t * year)
{
	mos_uint8_t data_tx[1] = {DATE_REG_ADDR}, data_rx[4], status;
	
	// Read day, date, month, year values
	status = I2C_PORT.masterTransfer(SLAVE_ADDR, 1 , data_tx, 4, data_rx);
	
	if(status == 0)
	{
		// convert BCD values to Decimal values and store in day, date, month, year variables
		*day = bcd2dec(data_rx[0]);
		*date = bcd2dec(data_rx[1]);
		*month = bcd2dec(data_rx[2]);
		*year = bcd2dec(data_rx[3]);
	}
	return status;	
}

#undef I2C_PORT
#undef I2C_BAUD
#undef SLAVE_ADDR
#undef TIME_REG_ADDR
#undef DATE_REG_ADD
#undef TEMP_REG_ADD

#endif
#endif
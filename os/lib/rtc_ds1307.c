#include "rtc_ds1307.h"

#define SLAVE_ADDR 0x68
#define CONTROL_REG_ADDR 0x07
#define TIME_REG_ADDR 0x00
#define DATE_REG_ADDR 0x03
#define TEMP_REG_ADDR 0x11

static const struct i2c_master_driver * ds1307_i2c_port;

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

mos_uint8_t rtc1307_begin(const struct i2c_master_driver * i2c_port, mos_uint32_t i2c_baud, mos_uint8_t mode)
{
	mos_uint8_t data_tx[2] = {CONTROL_REG_ADDR,0x00};
	
	ds1307_i2c_port = i2c_port;
	
	// Initialize I2C
	ds1307_i2c_port->begin(i2c_baud);
	
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
	return ds1307_i2c_port->transfer(SLAVE_ADDR, data_tx, 2, data_tx, 0);
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
	return ds1307_i2c_port->transfer(SLAVE_ADDR, data_tx, 4 , data_tx, 0);
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
	return ds1307_i2c_port->transfer(SLAVE_ADDR, data_tx, 5 , data_tx, 0);
}

mos_uint8_t rtc1307_getTime(mos_uint8_t * hour, mos_uint8_t * minute, mos_uint8_t * second)
{
	mos_uint8_t data_tx[1] = {TIME_REG_ADDR}, data_rx[3], status;
	
	// Read second, minute, hour values
	status = ds1307_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 3);
	
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
	status = ds1307_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx,4);
	
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

#undef SLAVE_ADDR
#undef TIME_REG_ADDR
#undef DATE_REG_ADD
#undef TEMP_REG_ADD
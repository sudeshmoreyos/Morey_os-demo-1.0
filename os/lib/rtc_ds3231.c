#include "rtc_ds3231.h"

#define SLAVE_ADDR 0x68
#define TIME_REG_ADDR 0x00
#define DATE_REG_ADDR 0x03
#define CONTROL_REG_ADDR 0x0E
#define TEMP_REG_ADDR 0x11
#define ALARM1_REG_ADDR 0x07
#define ALARM2_REG_ADDR 0x0B
#define STATUS_REG_ADDR 0x0F

static const struct i2c_master_driver * ds3231_i2c_port;

static mos_uint8_t dec2bcd(mos_uint8_t dec)
{
	return ((dec/10) << 4) | (dec%10);		
}

static mos_uint8_t bcd2dec(mos_uint8_t bcd)
{
	return (bcd >> 4)*10 + (bcd & 0xF);		
}

mos_uint8_t rtc3231_begin(const struct i2c_master_driver * i2c_port, mos_uint32_t i2c_baud, \
			mos_uint8_t mode, mos_uint8_t mode_32khz)
{
	mos_uint8_t data_tx[3] = {CONTROL_REG_ADDR,0x00, 0X00};
	
	ds3231_i2c_port = i2c_port;
	
	// Initialize I2C
	ds3231_i2c_port->begin(i2c_baud);
	
	// Set Control register values as per selected input mode
	if(mode == RTC3231_INT_ENABLE)
		data_tx[1] = 0x04;
	else if(mode == RTC3231_OUT_ONE)
		data_tx[1] = 0x00;
	else if(mode == RTC3231_SQR_1HZ)
		data_tx[1] = 0x40;
	else if(mode == RTC3231_SQR_4KHZ)
		data_tx[1] = 0x48;
	else if(mode == RTC3231_SQR_8KHZ)
		data_tx[1] = 0x50;
	else if(mode == RTC3231_SQR_32KHZ_ENABLE)
		data_tx[1] = 0x58;
	
	// Enable 32 KHz output if selected
	if(mode_32khz == RTC3231_SQR_32KHZ_ENABLE)
		data_tx[2] = 0x08;
	
	// Store Control register
	return ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 3 , data_tx, 0);
}

mos_uint8_t rtc3231_setTime(mos_uint8_t hour, mos_uint8_t minute, mos_uint8_t second)
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
	return ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 4 , data_tx, 0);	
}

mos_uint8_t rtc3231_setDate(mos_uint8_t day, mos_uint8_t date, mos_uint8_t month, mos_uint8_t year)
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
	return ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 5 , data_tx, 0);
		
}

mos_uint8_t rtc3231_getTime(mos_uint8_t * hour, mos_uint8_t * minute, mos_uint8_t * second)
{
	mos_uint8_t data_tx[1] = {TIME_REG_ADDR}, data_rx[3], status;
	
	// Read second, minute, hour values
	status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 3);
	
	if(status == 0)
	{
		// convert BCD values to Decimal values and store in second, minute, hour variables
		*second = bcd2dec(data_rx[0]);
		*minute = bcd2dec(data_rx[1]);
		*hour = bcd2dec(data_rx[2]);
	}
	
	return status;
}

mos_uint8_t rtc3231_getDate(mos_uint8_t *day, mos_uint8_t * date, mos_uint8_t * month, mos_uint8_t * year)
{
	static mos_uint8_t data_tx[1] = {DATE_REG_ADDR}, data_rx[4], status;
	
	// Read day, date, month, year values
	status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 4);
	
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

float rtc3231_getTemperature(void)
{
	mos_uint8_t data_tx[1] = {TEMP_REG_ADDR}, data_rx[2];
	mos_int16_t temp;
	
	// Read temperature values
	ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 2);	
	
	// convert to float format
	temp  = ( ((mos_int16_t)data_rx[0]) << 8 ) |  ( ((mos_int16_t)data_rx[1]) & 0xC0 );
    return (float)temp/ 256.0;	
}

//#ifdef RTC3231_ALARM1_ENABLE

mos_uint8_t rtc3231_setAlarm1(mos_uint8_t mode, mos_uint8_t date_day, mos_uint8_t hour, mos_uint8_t minute, mos_uint8_t second)
{
	static mos_uint8_t data_tx[5] = {ALARM1_REG_ADDR, 0x00, 0x00, 0x00, 0x00}, data_rx[1], status;
	
	// check if input value of second is valid, else return error = 1
	if(second > 59)
		return 1;
	
	// check if input value of minute is valid, else return error = 2
	if(minute > 59 )
		return 2;
	
	// check if input value of hour is valid, else return error = 3
	if(hour > 23 )
		return 3;
	
	// If alarm1 mode is mode4, check if input value of date is valid, else return error = 4
	if(mode == RTC3231_ALARM1_MODE4)
	{
		if(date_day > 31 || date_day == 0)
			return 4;
	}
	
	// If alarm1 mode is mode5, check if input value of day is valid, else return error = 5
	else if(mode == RTC3231_ALARM1_MODE5)
	{
		if(date_day > 7 || date_day == 0)
			return 5;
	}
	
	// store second value in data_tx[1] for subsequent i2c write operation
	data_tx[1] = dec2bcd(second & 0x7F);
	
	// If alarm1 mode is mode0, set the MSB of data_tx[1]
	if(mode == RTC3231_ALARM1_MODE0)
	{
		data_tx[1] |= 1<<7;	
	}
	
	// store minute value in data_tx[2] for subsequent i2c write operation
	data_tx[2] = dec2bcd(minute & 0x7F);
	
	// If alarm1 mode is mode0 or mode1, set the MSB of data_tx[2]
	if(mode <= RTC3231_ALARM1_MODE1)
	{
		data_tx[2] |= 1<<7;	
	}
	
	// store hour value in data_tx[3] for subsequent i2c write operation
	data_tx[3] = dec2bcd(hour & 0x7F);
	
	// If alarm1 mode is mode0 or mode1 or mode2, set the MSB of data_tx[3]
	if(mode <= RTC3231_ALARM1_MODE2)
	{
		data_tx[3] |= 1<<7;	
	}
	
	// store date_day value in data_tx[4] for subsequent i2c write operation
	data_tx[4] = dec2bcd(date_day & 0x3F);
	
	// If alarm1 mode is mode0 or mode1 or mode2 or mode3, set the MSB of data_tx[4]
	if(mode <= RTC3231_ALARM1_MODE3)
	{
		data_tx[4] |= 1<<7;	
	}
	
	// If alarm1 mode is mode5, set the 6th bit of data_tx[4]
	if(mode == RTC3231_ALARM1_MODE5)
	{
		data_tx[4] |= 1<<6;	
	}
	
	// Store Alarm1 seconds, minute, hour, day_date values
	status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 5, data_rx, 0);
	
	// If I2C write is Success
	if(status == 0)
	{		
		// set LSB of control register to enable Alarm1
		data_tx[0] = CONTROL_REG_ADDR;
		
		// Read current value of ds3231 control register
		status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 1);
		
		// If I2C read is Success
		if(status == 0 )
		{
			// Enable Alarm1 by seting LSB bit of ds3231 control register
			data_tx[1] = data_rx[0] | 0x01 ;
			
			// Write updated value of ds3231 control register 
			status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 2, data_rx, 0);
			
			// If I2C write is success, status will be 0, hence 0 is returned
			// If I2C write is failure, return I2C failure status
			return status;
		}
		
		// If I2C read is failure, return I2C failure status
		return status;
	}
	
	// If I2C write is failure, return I2C failure status
	return status;
}

mos_uint8_t rtc3231_getAlarm1(mos_uint8_t * mode, mos_uint8_t * date_day, mos_uint8_t * hour, mos_uint8_t * minute, mos_uint8_t * second)
{
	mos_uint8_t data_tx[1] = {ALARM1_REG_ADDR}, data_rx[4], status;
	mos_uint8_t a1m1=0,a1m2=0,a1m3=0,a1m4=0,a1dydt=0;
	
	// Read Alarm1 seconds, minute, hour, day_date values
	status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 4);
	
	// If I2C read is success
	if(status==0)
	{
		// Read Alarm1 Mask1 value
		if(data_rx[0] & (1<<7))
			a1m1=1;
		// Read Alarm1 Mask2 value
		if(data_rx[1] & (1<<7))
			a1m2=1;
		
		// Read Alarm1 Mask3 value
		if(data_rx[2] & (1<<7))
			a1m3=1;
		
		// Read Alarm1 Mask4 value
		if(data_rx[3] & (1<<7))
			a1m4=1;
		
		// Read Alarm1 DY/DT Mask value
		if(data_rx[3] & (1<<6))
			a1dydt=1;
		
		// Write Alarm1 seconds, minute, hour, day_date values into output variables
		*second = bcd2dec(data_rx[0] & 0x7F);
		*minute = bcd2dec(data_rx[1] & 0x7F);
		*hour = bcd2dec(data_rx[2] & 0x7F);
		*date_day = bcd2dec(data_rx[3] & 0x3F);
		
		// Based on Alarm1 Mask values, write Alarm1 mode value into mode output variable
		if(a1m4==1 && a1m3==1 && a1m2==1 && a1m1==1 )
			*mode = RTC3231_ALARM1_MODE0;
		else if(a1m4==1 && a1m3==1 && a1m2==1 && a1m1==0 )
			*mode = RTC3231_ALARM1_MODE1;
		else if(a1m4==1 && a1m3==1 && a1m2==0 && a1m1==0 )
			*mode = RTC3231_ALARM1_MODE2;
		else if(a1m4==1 && a1m3==0 && a1m2==0 && a1m1==0 )
			*mode = RTC3231_ALARM1_MODE3;
		else if(a1m4==0 && a1m3==0 && a1m2==0 && a1m1==0 && a1dydt==0)
			*mode = RTC3231_ALARM1_MODE4;
		else if(a1m4==0 && a1m3==0 && a1m2==0 && a1m1==0 && a1dydt==1)
			*mode = RTC3231_ALARM1_MODE5;
		
		return rtc3231_isActiveAlarm1();			
	}
	
	// If I2C read is failure, return 0
	else
		return 0;
}

mos_uint8_t rtc3231_isActiveAlarm1(void)
{
	static mos_uint8_t data_tx[1] = {CONTROL_REG_ADDR}, data_rx[1], status;
	
	// Read current value of ds3231 control register
	status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 1);
	if(status == 0)
	{
		// If LSB bit of ds3231 control register is one, means alarm1 is active else not
		// If alarm1 active return 1, else 0
		if(data_rx[0] & (1<<0) )
			return 1;
		else
			return 0;
	}
	// If I2C read is failure, return 0
	else
		return 0;	
}

mos_uint8_t rtc3231_isExpiredAlarm1(void)
{
	static mos_uint8_t data_tx[1] = {STATUS_REG_ADDR}, data_rx[1], status;
	
	// Read current value of ds3231 status register
	status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 1);
	if(status == 0)
	{
		// If LSB bit of ds3231 status register is one, means alarm1 is expired else not
		// If alarm1 expired return 1, else 0
		if(data_rx[0] & (1<<0) )
			return 1;
		else
			return 0;
	}
	// If I2C read is failure, return 0
	else
		return 0;	
}

mos_uint8_t rtc3231_resetAlarm1( void )
{
	static mos_uint8_t data_tx[2] = {STATUS_REG_ADDR,0x00}, data_rx[1], status;
	
	// Read current value of ds3231 status register
	status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 1);
	
	// If I2C read success
	if(status == 0 )
	{
		// Reset Alarm1 by reseting LSB bit of ds3231 status register
		data_tx[1] = data_rx[0] & 0xFE ;
		
		// Store updated value of ds3231 status register
		status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 2, data_rx, 0);
		
		// If I2C write is success return 1 else 0
		if(status == 0)
			return 1;
		else
			return 0;
	}
	
	// If I2C read is failure, return 0
	else
		return 0;
	
}

mos_uint8_t rtc3231_stopAlarm1( void )
{
	static mos_uint8_t data_tx[2] = {CONTROL_REG_ADDR,0x00}, data_rx[1], status;
	
	// Read current value of ds3231 control register
	status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 1);
	
	// If I2C read success
	if(status == 0 )
	{
		// Disable Alarm1 by reseting LSB bit of ds3231 control register
		data_tx[1] = data_rx[0] & 0xFE ;
		
		// Store updated value of ds3231 control register
		status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 2, data_rx, 0);
		
		// If I2C write is success return 1 else 0
		if(status == 0)
			return 1;
		else
			return 0;
	}
	
	// If I2C read is failure, return 0
	else
		return 0;
	
}

//#endif

//#ifdef RTC3231_ALARM2_ENABLE

mos_uint8_t rtc3231_setAlarm2(mos_uint8_t mode, mos_uint8_t date_day, mos_uint8_t hour, mos_uint8_t minute)
{
	static mos_uint8_t data_tx[4] = {ALARM2_REG_ADDR, 0x00, 0x00, 0x00}, data_rx[1], status;
	
	// check if input value of minute is valid, else return error = 1
	if(minute > 59 )
		return 1;
	
	// check if input value of hour is valid, else return error = 2
	if(hour > 23 )
		return 2;
	
	// If alarm2 mode is mode3, check if input value of date is valid, else return error = 3
	if(mode == RTC3231_ALARM2_MODE3)
	{
		if(date_day > 31 || date_day == 0)
			return 3;
	}
	
	// If alarm2 mode is mode4, check if input value of day is valid, else return error = 4
	else if(mode == RTC3231_ALARM2_MODE4)
	{
		if(date_day > 7 || date_day == 0)
			return 4;
	}
	
	// store minute value in data_tx[1] for subsequent i2c write operation
	data_tx[1] = dec2bcd(minute & 0x7F);
	
	// If alarm2 mode is mode0, set the MSB of data_tx[1]
	if(mode == RTC3231_ALARM2_MODE0)
	{
		data_tx[1] |= 1<<7;	
	}
	
	// store hour value in data_tx[2] for subsequent i2c write operation
	data_tx[2] = dec2bcd(hour & 0x7F);
	
	// If alarm2 mode is mode0 or mode1, set the MSB of data_tx[2]
	if(mode <= RTC3231_ALARM2_MODE1)
	{
		data_tx[2] |= 1<<7;	
	}
	
	// store date_day value in data_tx[3] for subsequent i2c write operation
	data_tx[3] = dec2bcd(date_day & 0x3F);
	
	// If alarm2 mode is mode0 or mode1 or mode2, set the MSB of data_tx[3]
	if(mode <= RTC3231_ALARM2_MODE2)
	{
		data_tx[3] |= 1<<7;	
	}
	
	// If alarm2 mode is mode4, set the 6th bit of data_tx[3]
	if(mode == RTC3231_ALARM2_MODE4)
	{
		data_tx[3] |= 1<<6;	
	}
	
	// Store Alarm2 minute, hour, day_date values
	status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 4, data_rx, 0);
	
	// If I2C write is Success
	if(status == 0)
	{		
		// set 1st bit of control register to enable Alarm2
		data_tx[0] = CONTROL_REG_ADDR;
		
		// Read current value of ds3231 control register
		status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 1);

		// If I2C read is Success
		if(status == 0 )
		{
			// Enable Alarm2 by seting 1st bit of ds3231 control register
			data_tx[1] = data_rx[0] | 0x02 ;
			
			// Write updated value of ds3231 control register 
			status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 2, data_rx, 0);
			
			// If I2C write is success, status will be 0, hence 0 is returned
			// If I2C write is failure, return I2C failure status
			return status;
		}
		
		// If I2C read is failure, return I2C failure status
		return status;
	}
	
	// If I2C write is failure, return I2C failure status
	return status;
}

mos_uint8_t rtc3231_getAlarm2(mos_uint8_t * mode, mos_uint8_t * date_day, mos_uint8_t * hour, mos_uint8_t * minute)
{
	mos_uint8_t data_tx[1] = {ALARM2_REG_ADDR}, data_rx[3], status;
	mos_uint8_t a2m2=0,a2m3=0,a2m4=0,a2dydt=0;
	
	// Read Alarm2 minute, hour, day_date values
	status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 3);
	
	// If I2C read is success
	if(status==0)
	{
		// Read Alarm2 Mask2 value
		if(data_rx[0] & (1<<7))
			a2m2=1;
		
		// Read Alarm2 Mask3 value
		if(data_rx[1] & (1<<7))
			a2m3=1;
		
		// Read Alarm2 Mask4 value
		if(data_rx[2] & (1<<7))
			a2m4=1;
		
		// Read Alarm2 DY/DT Mask value
		if(data_rx[2] & (1<<6))
			a2dydt=1;
		
		// Write Alarm2 minute, hour, day_date values into output variables
		*minute = bcd2dec(data_rx[0] & 0x7F);
		*hour = bcd2dec(data_rx[1] & 0x7F);
		*date_day = bcd2dec(data_rx[2] & 0x3F);
		
		// Based on Alarm2 Mask values, write Alarm2 mode value into mode output variable
		if(a2m4==1 && a2m3==1 && a2m2==1)
			*mode = RTC3231_ALARM2_MODE0;
		else if(a2m4==1 && a2m3==1 && a2m2==0)
			*mode = RTC3231_ALARM2_MODE1;
		else if(a2m4==1 && a2m3==0 && a2m2==0)
			*mode = RTC3231_ALARM2_MODE2;
		else if(a2m4==0 && a2m3==0 && a2m2==0 && a2dydt==0)
			*mode = RTC3231_ALARM2_MODE3;
		else if(a2m4==0 && a2m3==0 && a2m2==0 && a2dydt==1)
			*mode = RTC3231_ALARM2_MODE4;
		
		return rtc3231_isActiveAlarm2();			
	}
	
	// If I2C read is failure, return 0
	else
		return 0;
}

mos_uint8_t rtc3231_isActiveAlarm2(void)
{
	static mos_uint8_t data_tx[1] = {CONTROL_REG_ADDR}, data_rx[1], status;
	
	// Read current value of ds3231 control register
	status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 1);
	if(status == 0)
	{
		// If 1st bit of ds3231 control register is one, means alarm2 is active else not
		// If alarm2 is active return 1, else 0
		if(data_rx[0] & (1<<1) )
			return 1;
		else
			return 0;
	}
	// If I2C read is failure, return 0
	else
		return 0;	
}

mos_uint8_t rtc3231_isExpiredAlarm2(void)
{
	static mos_uint8_t data_tx[1] = {STATUS_REG_ADDR}, data_rx[1], status;
	
	// Read current value of ds3231 status register
	status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 1);
	if(status == 0)
	{
		// If 1st bit of ds3231 status register is one, means alarm2 is expired else not
		// If alarm2 expired return 1, else 0
		if(data_rx[0] & (1<<1) )
			return 1;
		else
			return 0;
	}
	// If I2C read is failure, return 0
	else
		return 0;	
}

mos_uint8_t rtc3231_resetAlarm2( void )
{
	static mos_uint8_t data_tx[2] = {STATUS_REG_ADDR,0x00}, data_rx[1], status;
	
	// Read current value of ds3231 status register
	status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 1);
	
	// If I2C read success
	if(status == 0 )
	{
		// Reset Alarm1 by reseting 1st bit of ds3231 status register
		data_tx[1] = data_rx[0] & 0xFD ;
		
		// Store updated value of ds3231 status register
		status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 2, data_rx, 0);
		
		// If I2C write is success return 1 else 0
		if(status == 0)
			return 1;
		else
			return 0;
	}
	
	// If I2C read is failure, return 0
	else
		return 0;
	
}

mos_uint8_t rtc3231_stopAlarm2( void )
{
	static mos_uint8_t data_tx[2] = {CONTROL_REG_ADDR,0x00}, data_rx[1], status;
	
	// Read current value of ds3231 control register
	status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 1, data_rx, 1);
	
	// If I2C read success
	if(status == 0 )
	{
		// Disable Alarm1 by reseting 1st bit of ds3231 control register
		data_tx[1] = data_rx[0] & 0xFD ;
		
		// Store updated value of ds3231 control register
		status = ds3231_i2c_port->transfer(SLAVE_ADDR, data_tx, 2, data_rx, 0);
		
		// If I2C write is success return 1 else 0
		if(status == 0)
			return 1;
		else
			return 0;
	}
	
	// If I2C read is failure, return 0
	else
		return 0;	
}


//#endif

#undef SLAVE_ADDR
#undef TIME_REG_ADDR
#undef DATE_REG_ADD
#undef CONTROL_REG_ADDR
#undef ALARM1_REG_ADDR
#undef ALARM2_REG_ADDR
#undef STATUS_REG_ADDR
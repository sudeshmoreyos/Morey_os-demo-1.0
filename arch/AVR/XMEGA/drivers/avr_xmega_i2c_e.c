#include "avr_xmega_i2c_e.h"
#include "Serial.h"
#include "util/delay_blocking.h"
#define I2C_SUCCESS (0x02 << 3)
#define delay 1

static void masterBegin (mos_uint32_t baudrate)
{
    mos_uint16_t temp=0;
	
	temp = (CONTROLLER_FREQ/2/baudrate) - 5;
	if (temp > 255)
		temp = 255;
	TWIE.MASTER.BAUD = (mos_uint8_t) temp;
	TWIE.MASTER.CTRLA = 1<<3;
	TWIE.MASTER.CTRLB = 0xD;
	TWIE.MASTER.STATUS = 0x01;	
}

static mos_uint8_t i2c_start(mos_uint8_t address, mos_uint8_t direction)
{	
	mos_uint8_t temp;
	if(direction > 0)
		temp = (address<<1)|0x01;
	else
		temp = (address<<1);
	
	TWIE.MASTER.ADDR = temp;
	while ( (TWIE.MASTER.STATUS & (1<<5)) == 0 );
	
	delay_ms_blocking(delay);
	return ((TWIE.MASTER.STATUS & 0x1F) << 3);
}

static mos_uint8_t i2c_write(unsigned char data)                           
{
    TWIE.MASTER.DATA = data;
	while ( (TWIE.MASTER.STATUS & (1<<6)) == 0) ;
	
	delay_ms_blocking(delay);
	return ((TWIE.MASTER.STATUS & 0x1F) << 3);
}

static mos_uint8_t i2c_read(mos_uint8_t * data, mos_uint8_t acknowledge)
{
    if (acknowledge == 1)
		TWIE.MASTER.CTRLC &= ~(1<<2);
	else
		TWIE.MASTER.CTRLC |= 1<<2;
	
	while ( (TWIE.MASTER.STATUS & (1<<5)) == 0 );
	
	*data = TWIE.MASTER.DATA;
	
	delay_ms_blocking(delay);
	return ((TWIE.MASTER.STATUS & 0xF) << 3);
}

static void i2c_stop(void)
{
    delay_ms_blocking(delay);
	TWIE.MASTER.CTRLC = 0x3;
	delay_ms_blocking(delay);
}

static mos_uint8_t masterTransfer(mos_uint8_t slave_address, mos_uint16_t data_tx_len, mos_uint8_t * data_tx, \
			mos_uint16_t data_rx_len, mos_uint8_t * data_rx)
{
	mos_uint8_t status;
	
	if( data_tx_len > 0 )
	{
		status = i2c_start(slave_address, 0);
		if( status != I2C_SUCCESS )
		{
			i2c_stop();
			return status;
		}
	}
	
	while(data_tx_len)
	{
		status = i2c_write( *data_tx );
		if( status != I2C_SUCCESS )
		{
			i2c_stop();
			return status;
		}

		data_tx++;
		data_tx_len--;
	}
	
	if( data_rx_len > 0 )
	{
		status = i2c_start(slave_address, 1);
		if( status != I2C_SUCCESS )
		{
			i2c_stop();
			return status;
		}
	}
	
	while(data_rx_len>1)
	{
		status = i2c_read( data_rx, 1 );
		if( status != I2C_SUCCESS )
		{
			i2c_stop();
			return status;
		}

		data_rx++;
		data_rx_len--;
	}
	
	if(data_rx_len==1)
	{
		status = i2c_read( data_rx, 0 );
		if( status != I2C_SUCCESS )
		{
			i2c_stop();
			return status;
		}

		data_rx++;
		data_rx_len--;
	}
	
	i2c_stop();	
	
	return 0;
}

const struct i2c_driver avr_xmega_i2c_e_driver = {
	masterBegin,
	masterTransfer
};

#undef I2C_SUCCESS
#undef delay
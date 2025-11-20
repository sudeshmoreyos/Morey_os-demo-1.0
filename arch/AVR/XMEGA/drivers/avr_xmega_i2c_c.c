#include "avr_xmega_i2c_c.h"
#define I2C_SUCCESS (0x02 << 3)

static void masterBegin (mos_uint32_t baudrate)
{
    mos_uint16_t temp=0;
	
	temp = (CONTROLLER_FREQ/2/baudrate) - 5;
	if (temp > 255)
		temp = 255;
	TWIC.MASTER.BAUD = (mos_uint8_t) temp;
	TWIC.MASTER.CTRLA = 1<<3;
	TWIC.MASTER.CTRLB = 0xD;
	TWIC.MASTER.STATUS = 0x01;	
}

static mos_uint8_t i2c_start(mos_uint8_t address, mos_uint8_t direction)
{
    TWIC.MASTER.ADDR = (address<<1)|direction;
	while ( TWIC.MASTER.STATUS & (1<<6) ) ;
	return ((TWIC.MASTER.STATUS & 0x1F) << 3);
}

static mos_uint8_t i2c_write(unsigned char data)                           
{
    TWIC.MASTER.DATA = data;
	while ( TWIC.MASTER.STATUS & (1<<6) ) ;
	return ((TWIC.MASTER.STATUS & 0x1F) << 3);
}

static mos_uint8_t i2c_read(mos_uint8_t * data, mos_uint8_t acknowledge)
{
    if (acknowledge == 1)
		TWIC.MASTER.CTRLC &= ~(1<<2);
	else
		TWIC.MASTER.CTRLC |= 1<<2;
	
	while ( (TWIC.MASTER.STATUS & (1<<7)) || (TWIC.MASTER.STATUS & (1<<6)) ) ;
	
	*data = TWIC.MASTER.DATA;
	
	return ((TWIC.MASTER.STATUS & 0x1F) << 3);
}

static void i2c_stop(void)
{
    TWIC.MASTER.CTRLC = 0x3;
}

static mos_uint8_t masterTransfer(mos_uint8_t slave_address, mos_uint16_t data_tx_len, mos_uint8_t * data_tx, \
			mos_uint16_t data_rx_len, mos_uint8_t * data_rx)
{
	mos_uint8_t status;
	
	if( data_tx_len > 0 )
	{
		status = i2c_start(slave_address, 0);
		if( status != I2C_SUCCESS )
		return status;
	}
	
	while(data_tx_len)
	{
		status = i2c_write( *data_tx );
		if( status != I2C_SUCCESS )
			return status;

		data_tx++;
		data_tx_len--;
	}
	
	if( data_rx_len > 0 )
	{
		status = i2c_start(slave_address, 0);
		if( status != I2C_SUCCESS )
		return status;
	}
	
	while(data_rx_len>1)
	{
		status = i2c_read( data_rx, 1 );
		if( status != I2C_SUCCESS )
			return status;

		data_rx++;
		data_rx_len--;
	}
	
	if(data_rx_len==1)
	{
		status = i2c_read( data_rx, 0 );
		if( status != I2C_SUCCESS )
			return status;

		data_rx++;
		data_rx_len--;
	}
	
	i2c_stop();	
	
	return 0;
}

const struct i2c_driver avr_xmega_i2c_c_driver = {
	masterBegin,
	masterTransfer
};

#undef I2C_SUCCESS
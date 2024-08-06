//-------------------Macros to change for I2Cx------------------//
// avr_mega_i2c_master
// I2C_AVAILABLE
// avr_mega_i2c_master_driver
// TWSR
// TWCR
// TWDR
// TWBR

#include "avr_mega_i2c_master.h"

#if defined(PLATFORM_SUPPORT_I2C) && defined(I2C_AVAILABLE)

#include "../../../../os/util/delay_blocking.h"

// define generic Macros for I2C registers
#define I2C_STATUS 			TWSR
#define I2C_CONTROL 		TWCR
#define I2C_DATA 			TWDR
#define I2C_BAUD			TWBR

#ifdef I2C_TIMEOUT_MS_TEMP
	#define I2C_TIMEOUT_MS 		I2C_TIMEOUT_MS_CONF
#else
	#define I2C_TIMEOUT_MS 		500
#endif

static mos_uint8_t baudrate_generator(mos_uint32_t baudrate, mos_uint8_t * twps)
{
	mos_uint8_t i = 0;
	float temp[4] = {1, 4, 16, 64}, twbr[4], error = 0, error_min = 0;
	
	for(i=0;i<4;i++)
	{
		twbr[i] = (( (float)CONTROLLER_FREQ / (float)baudrate) - 16.0 ) / temp[i] / 2.0 ; 
		error = 100 * (twbr[i] - (float)((mos_uint8_t)twbr[i]))/twbr[i];
		
		if(error < 0)
			error = -error;
		
		if(i==0)
		{
			error_min = error;
			*twps = i;
		}
		else
		{
			if(error < error_min)
			{
				error_min = error;
				*twps = i;
			}
		}
	}	
	return (mos_uint8_t)twbr[*twps];
}

static void begin (mos_uint32_t baudrate)
{
    mos_uint8_t temp=0;
	
	I2C_BAUD = baudrate_generator(baudrate, &temp);
	I2C_STATUS = temp;
}

static void end (void)
{	
	I2C_BAUD = 0;
	I2C_STATUS = 0;
	I2C_CONTROL = 0; 
}

static mos_uint8_t i2c_start(void)
{
    mos_uint16_t count = 0;
	mos_uint8_t status = 0;
	
	I2C_CONTROL = (1<<TWINT) | ( 1<< TWSTA ) | (1<< TWEN );
    while( ( I2C_CONTROL & (1<< TWINT)) == 0)
	{
		count++;
		delay_ms_blocking(1);
		if(count == I2C_TIMEOUT_MS)
		{
			return 0xFF;
		}
	}
	
    status = (I2C_STATUS & 0xF8);
	if(status == 0x00)
		return 0xFE;
	else
		return status;
}

static mos_uint8_t i2c_write(unsigned char data)                           
{
    mos_uint16_t count = 0;
	mos_uint8_t status = 0;
	
	I2C_DATA = data;
    I2C_CONTROL = (1<<TWINT)|(1<<TWEN);
    while( ( I2C_CONTROL & (1<< TWINT)) == 0)
	{
		count++;
		delay_ms_blocking(1);
		if(count == I2C_TIMEOUT_MS)
		{
			return 0xFF;
		}
	}

    status = (I2C_STATUS & 0xF8);
	if(status == 0x00)
		return 0xFE;
	else
		return status;
}

static mos_uint8_t i2c_read(mos_uint8_t * data, mos_uint8_t acknowledge)
{
    mos_uint16_t count = 0;
	mos_uint8_t status = 0;
	
	if(acknowledge == 0)
		I2C_CONTROL = ( 1 << TWINT ) | ( 1<< TWEN); 
    else
		I2C_CONTROL = ( 1 << TWINT ) | ( 1<< TWEN) | (1<<TWEA);
    
    while( ( I2C_CONTROL & (1 << TWINT) ) ==0)
	{
		count++;
		delay_ms_blocking(1);
		if(count == I2C_TIMEOUT_MS)
		{
			return 0xFF;
		}
	}
    *data = I2C_DATA;
	
    status = (I2C_STATUS & 0xF8);
	if(status == 0x00)
		return 0xFE;
	else
		return status;
}

static void i2c_stop(void)
{
    mos_uint16_t count = 0;
	
	I2C_CONTROL = (1<<TWINT) | (1<< TWEN ) | ( 1<< TWSTO ) ;
	while( ( I2C_CONTROL & (1 << TWSTO) ) ==0 )
	{
		count++;
		delay_ms_blocking(1);
		if(count == I2C_TIMEOUT_MS)
		{
			return;
		}
	}
}

static mos_uint8_t transfer(mos_uint8_t slave_address, void * data_tx_void, mos_uint16_t data_tx_len,  \
			void * data_rx_void, mos_uint16_t data_rx_len)
{
	mos_uint8_t status,restart=0;
	mos_uint8_t * data_tx, * data_rx;
	
	data_tx = (mos_uint8_t *)data_tx_void;
	data_rx = (mos_uint8_t *)data_rx_void;
	
	status = i2c_start();
	if( status != TW_START )
	{
		i2c_stop();	
		return status;
	}
	
	if( data_tx_len > 0 )
	{
		status = i2c_write( slave_address<<1 );
		if( status != TW_MT_SLA_ACK )
		{
			i2c_stop();	
			return status;
		}
	
		while(data_tx_len)
		{
			status = i2c_write( *data_tx );
			if( status != TW_MT_DATA_ACK )
			{
				i2c_stop();	
				return status;
			}

			data_tx++;
			data_tx_len--;
		}
		restart = 1;
	}
	
	if( data_rx_len > 0 )
	{
		if( restart == 1 )
		{
			status = i2c_start();
			if( status != TW_REP_START )
			{
				i2c_stop();	
				return status;
			}
		}
		
		status = i2c_write( (slave_address<<1) | 0x01 );
		if( status != TW_MR_SLA_ACK )
		{
			i2c_stop();
			return status;
		}
		
		while(data_rx_len>1)
		{
			status = i2c_read( data_rx, 1 );
			if( status != TW_MR_DATA_ACK )
			{
				i2c_stop();	
				return status;
			}
			
			data_rx++;
			data_rx_len--;
		}
			
		status = i2c_read( data_rx, 0 );
		if( status != TW_MR_DATA_NACK )
		{
			i2c_stop();
			return status;
		}
	}
	
	i2c_stop();	
	
	return 0;
}

const struct i2c_master_driver avr_mega_i2c_master_driver = {
	begin,
	end,
	transfer
};

// undefine generic Macros for I2C registers
#undef I2C_STATUS
#undef I2C_CONTROL
#undef I2C_DATA
#undef I2C_BAUD
#undef I2C_TIMEOUT_MS

#endif
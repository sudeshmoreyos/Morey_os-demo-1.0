#include "avr_mega_i2c.h"

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

static void masterBegin (mos_uint32_t baudrate)
{
    mos_uint8_t temp=0;
	
	TWBR = baudrate_generator(baudrate, &temp);
	TWSR = temp;
}

static mos_uint8_t i2c_start(void)
{
    TWCR = (1<<TWINT) | ( 1<< TWSTA ) | (1<< TWEN );
    while( ( TWCR & (1<< TWINT)) == 0);
    return (TWSR & 0xF8);
}

static mos_uint8_t i2c_write(unsigned char data)                           
{
    TWDR = data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while( ( TWCR & (1<< TWINT)) == 0);
	
    return (TWSR & 0xF8);
}

static mos_uint8_t i2c_read(mos_uint8_t * data, mos_uint8_t acknowledge)
{
    if(acknowledge == 0)
		TWCR = ( 1 << TWINT ) | ( 1<< TWEN); 
    else
		TWCR = ( 1 << TWINT ) | ( 1<< TWEN) | (1<<TWEA);
    
    while( ( TWCR & (1 << TWINT) ) ==0);
    *data = TWDR;
    return (TWSR & 0xF8);
}

static void i2c_stop(void)
{
    TWCR = (1<<TWINT) | (1<< TWEN ) | ( 1<< TWSTO ) ;
}

static mos_uint8_t masterTransfer(mos_uint8_t slave_address, mos_uint16_t data_tx_len, mos_uint8_t * data_tx, \
			mos_uint16_t data_rx_len, mos_uint8_t * data_rx)
{
	mos_uint8_t status, restart=0;
	
	status = i2c_start();
	if( status != MASTER_START )
		return status;
	
	if( data_tx_len > 0 )
	{
		status = i2c_write( slave_address<<1 );
		if( status != MASTER_ADDRESS_WRITE_ACKNOWLEDGE )
			return status;
		restart = 1;
	}
	
	while(data_tx_len)
	{
		status = i2c_write( *data_tx );
		if( status != MASTER_DATA_SEND_ACKNOWLEDGE )
			return status;

		data_tx++;
		data_tx_len--;
	}
	
	if( data_rx_len > 0 )
	{
		if( restart == 1)
		{
			status = i2c_start();
			if( status != MASTER_START_REPEAT )
				return status;
		}
	
		status = i2c_write( (slave_address<<1) | 0x01 );
		if( status != MASTER_ADDRESS_READ_ACKNOWLEDGE )
			return status;
	}
	
	while(data_rx_len>1)
	{
		status = i2c_read( data_rx, 1 );
		if( status != MASTER_DATA_READ_ACKNOWLEDGE )
			return status;

		data_rx++;
		data_rx_len--;
	}
	
	if(data_rx_len==1)
	{
		status = i2c_read( data_rx, 0 );
		if( status != MASTER_DATA_READ_NOACKNOWLEDGE )
			return status;

		data_rx++;
		data_rx_len--;
	}
	
	i2c_stop();	
	
	return 0;
}

const struct i2c_driver avr_mega_i2c_driver = {
	masterBegin,
	masterTransfer
};
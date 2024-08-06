#include "I2cSoft.h"

#ifdef I2C_SOFT_ENABLE

#ifdef I2C_SOFT0_SCL_PIN
	#ifdef I2C_SOFT_SCL_PIN
		#error Either I2C_SOFT_SCL_PIN Macro or I2C_SOFT0_SCL_PIN Macro must be defined
	#else
		#define I2C_SOFT_SCL_PIN I2C_SOFT0_SCL_PIN
	#endif
#endif

#ifdef I2C_SOFT0_SDA_PIN
	#ifdef I2C_SOFT_SDA_PIN
		#error I2C_SOFT_SDA_PIN Macro or I2C_SOFT0_SDA_PIN Macro must be defined
	#else
		#define I2C_SOFT_SDA_PIN I2C_SOFT0_SDA_PIN
	#endif
#endif

#ifndef I2C_SOFT_SCL_PIN
	#error I2C_SOFT_SCL_PIN or I2C_SOFT0_SCL_PIN Macro is not defined. Please assign digital pin to it.
#endif

#ifndef I2C_SOFT_SDA_PIN
	#error I2C_SOFT_SDA_PIN or I2C_SOFT0_SDA_PIN Macro is not defined. Please assign digital pin to it.
#endif

#include "Digital.h"
#include "util/delay_blocking.h"

mos_uint16_t soft_i2c_delayus=4;

static void scl_low(void) 
{
	Digital.write(I2C_SOFT_SCL_PIN, LOW);
	Digital.pinmode(I2C_SOFT_SCL_PIN, OUTPUT);
	Digital.write(I2C_SOFT_SCL_PIN, LOW);
	delay_us_blocking(1);
}

static void scl_high(void) 
{
	Digital.pinmode(I2C_SOFT_SCL_PIN, INPUT);
	delay_us_blocking(1);
}

static void sda_low(void) 
{
	Digital.write(I2C_SOFT_SDA_PIN, LOW);
	Digital.pinmode(I2C_SOFT_SDA_PIN, OUTPUT);
	Digital.write(I2C_SOFT_SDA_PIN, LOW);
	delay_us_blocking(1);
}

static void sda_high(void) 
{
	Digital.pinmode(I2C_SOFT_SDA_PIN, INPUT);
	delay_us_blocking(1);
}

static void begin (mos_uint32_t baudrate)
{	
	scl_high();
	sda_high();
	
	soft_i2c_delayus = 1000000/baudrate/2-1;
}

static void end (void)
{	
	scl_high();
	sda_high();	
	soft_i2c_delayus = 0;
}

static void i2c_delay(void)
{
	mos_uint8_t i;
	
	for(i=0;i<soft_i2c_delayus;i++)
	{
		delay_us_blocking(1);
	}	
}

static void i2c_start(void)
{
	GLOBAL_INTERRUPT_DISABLE();
	// make sure SCL is low before changing SDA output
	scl_low();
	// setting SDA line high
	sda_high();
	
	// Set SCL line HIGH
	scl_high();
	
	// Change SDA line from High to low to generate start condition
	sda_low();
	
	// wait for time delay according to baudrate
	i2c_delay();
	
	// Pull SCL low to complete cycle
	scl_low();
	GLOBAL_INTERRUPT_ENABLE();
}

static mos_uint8_t i2c_write(mos_uint8_t data)                           
{
	mos_uint8_t i;
	mos_uint16_t check;
	
	GLOBAL_INTERRUPT_DISABLE();
	for(i=0;i<8;i++)
	{
		// Pull SCL line low before changing SDA line
		scl_low();
		
		// Set SDA according to input bit, MSB first.
		if(data & 0x80)
			sda_high();
		else
			sda_low();
		
		// left shift data to get next bit
		data <<= 1;
		
		// wait for time delay according to baudrate
		i2c_delay();
		
		// Set SCL line HIGH
		scl_high();
		check=0;
		while(Digital.read(I2C_SOFT_SCL_PIN)==0)
		{
			delay_us_blocking(1);
			check++;
			if(check>50000)
				return 2;
		}
		
		// wait for time delay according to baudrate		
		i2c_delay();
	}
	
	// Pull SCL line low before reading acknowledgment bit in 9th cycle
	scl_low();
	
	// Pull SDA line high to bring it to read mode
	sda_high();
	
	// wait for time delay according to baudrate	
	i2c_delay();
	
	// Pull SCL line high to read acknowledgment bit
	scl_high();
	check=0;
	while(Digital.read(I2C_SOFT_SCL_PIN)==0)
	{
		delay_us_blocking(1);
		check++;
		if(check>50000)
			return 2;
	}
	
	i = Digital.read(I2C_SOFT_SDA_PIN);
	
	// wait for time delay according to baudrate
	i2c_delay();
	
	// Pull SCL low to complete cycle
	scl_low();
	
	GLOBAL_INTERRUPT_ENABLE();
	
	return i;
}

static void i2c_read(mos_uint8_t * data, mos_uint8_t acknowledge)
{
	mos_uint8_t i;
	mos_uint16_t check;
	
	*data = 0;
	GLOBAL_INTERRUPT_DISABLE();
	for(i=0;i<8;i++)
	{
		// Pull SCL line low before changing SDA line
		scl_low();
		
		// Pull SDA line high to bring it to read mode
		sda_high();
		
		// wait for time delay according to baudrate
		i2c_delay();
		
		// Set SCL line HIGH
		scl_high();
		check=0;
		while(Digital.read(I2C_SOFT_SCL_PIN)==0)
		{
			delay_us_blocking(1);
			check++;
			if(check>50000)
			{
				*data = 0;
				return;
			}
		}
		
		// left shift data to store next bit
		*data <<= 1;
		
		if(Digital.read(I2C_SOFT_SDA_PIN) == 1)
			*data |= 1;
		
		// wait for time delay according to baudrate		
		i2c_delay();
		
		
	}
	
	// Pull SCL line low before sending acknowledgment bit in 9th cycle
	scl_low();
	
	// Set SDA according to acknowledge bit, MSB first.
	if(acknowledge)
		sda_low();
	else
		sda_high();
	
	// wait for time delay according to baudrate
	i2c_delay();
	
	// Pull SCL line high to send acknowledge bit
	scl_high();
	
	// wait for time delay according to baudrate
	i2c_delay();
	
	// Pull SCL low to complete cycle
	scl_low();	
	
	GLOBAL_INTERRUPT_ENABLE();
}

static void i2c_stop(void)
{
	GLOBAL_INTERRUPT_DISABLE();
	// make SCL is low before changing SDA output
	scl_low();	
	
	// set SDA low
	sda_low();
	
	// Set SCL line high
	scl_high();
	
	// Change SDA line from Low to high to generate stop condition
	sda_high();
	
	// wait for time delay according to baudrate
	i2c_delay();
	
	// Pull SCL low to complete cycle
	scl_low();
	
	GLOBAL_INTERRUPT_ENABLE();
}

static mos_uint8_t transfer(mos_uint8_t slave_address, void * data_tx_void, mos_uint16_t data_tx_len, \
			 void * data_rx_void, mos_uint16_t data_rx_len  )
{
	mos_uint8_t status, restart=0;
	mos_uint8_t * data_tx, * data_rx;
	
	data_tx = (mos_uint8_t *)data_tx_void;
	data_rx = (mos_uint8_t *)data_rx_void;
	
	i2c_start();
	
	if( data_tx_len > 0 )
	{
		status = i2c_write( slave_address<<1 );
		if( status != 0 )
			return status;
		restart = 1;
	}
	
	while(data_tx_len)
	{
		status = i2c_write( *data_tx );
		if( status != 0 )
			return status;

		data_tx++;
		data_tx_len--;
	}
	
	if( data_rx_len > 0 )
	{
		if( restart == 1)
			i2c_start();
	
		status = i2c_write( (slave_address<<1) | 0x01 );
		if( status != 0 )
			return status;
	}
	
	while(data_rx_len>1)
	{
		i2c_read( data_rx, 1 );

		data_rx++;
		data_rx_len--;
	}
	
	if(data_rx_len==1)
		i2c_read( data_rx, 0 );
	
	i2c_stop();	
	
	return 0;
}

const struct i2c_master_driver I2cSoft = {
	begin,
	end,
	transfer
};

#endif
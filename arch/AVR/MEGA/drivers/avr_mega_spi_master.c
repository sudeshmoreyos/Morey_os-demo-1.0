//-------------------Macros to change for SPIx------------------//
// avr_mega_spi_master
// SPI_AVAILABLE
// avr_mega_spi_master_driver
// SPI_SS_PIN
// SPI_MOSI_PIN
// SPI_MISO_PIN
// SPI_SCK_PIN
// SPCR
// SPSR
// SPDR

#include "avr_mega_spi_master.h"

#if defined(PLATFORM_SUPPORT_SPI) && defined(SPI_AVAILABLE)
#include "../../../../os/drivers/Digital.h"
#include "../../../../os/util/delay_blocking.h"

#define SPI_CONTROL_REGISTER_TEMP 	SPCR
#define SPI_STATUS_REGISTER_TEMP 	SPSR
#define SPI_DATA_REGISTER_TEMP 		SPDR

static mos_uint8_t ss_pin_master_temp = 255;

static void begin(mos_uint8_t ss_pin)
{
	Digital.pinmode(SPI_MOSI_PIN,OUTPUT);
	Digital.pinmode(SPI_MISO_PIN,INPUT_PULLUP);
	Digital.pinmode(SPI_SCK_PIN,OUTPUT);
	Digital.pinmode(ss_pin,OUTPUT);
	Digital.write(ss_pin,HIGH);
	
	ss_pin_master_temp = ss_pin;
	
	SPI_CONTROL_REGISTER_TEMP = 0x50;
	SPI_STATUS_REGISTER_TEMP = 0x00;
}

static void end(void)
{
	SPI_CONTROL_REGISTER_TEMP = 0x00;
	SPI_STATUS_REGISTER_TEMP = 0x00;
	
	Digital.pinmode(SPI_MOSI_PIN,INPUT);
	Digital.pinmode(SPI_MISO_PIN,INPUT);
	Digital.pinmode(SPI_SCK_PIN,INPUT);
	Digital.pinmode(ss_pin_master_temp,INPUT);
}

static void config(mos_uint8_t speed, mos_uint8_t mode, mos_uint8_t data_order)
{
	mos_uint8_t x=0;
	
	x = SPI_CONTROL_REGISTER_TEMP;	
	x = x & 0xF0;
	
	if(speed == SPI_SPEED_BY4)
	{
		x |= 0x00;
		SPI_STATUS_REGISTER_TEMP &= ~(1<<0);
	}
	
	else if(speed == SPI_SPEED_BY16)
	{
		x |= 0x01;
		SPI_STATUS_REGISTER_TEMP &= ~(1<<0);
	}
	
	else if(speed == SPI_SPEED_BY64)
	{
		x |= 0x02;
		SPI_STATUS_REGISTER_TEMP &= ~(1<<0);
	}
	
	else if(speed == SPI_SPEED_BY128)
	{
		x |= 0x03;
		SPI_STATUS_REGISTER_TEMP &= ~(1<<0);
	}
	else if(speed == SPI_SPEED_BY2)
	{
		x |= 0x00;
		SPI_STATUS_REGISTER_TEMP |= (1<<0);
	}
	else if(speed == SPI_SPEED_BY8)
	{
		x |= 0x01;
		SPI_STATUS_REGISTER_TEMP |= (1<<0);
	}
	else if(speed == SPI_SPEED_BY32)
	{
		x |= 0x02;
		SPI_STATUS_REGISTER_TEMP |= (1<<0);
	}
	
	x |= (mode & 0x3) << 2;
	
	if(data_order == SPI_MSB_FIRST)
		 x &= ~(1<<5);
	else if(data_order == SPI_LSB_FIRST)
		 x |= 1<<5;
	 
	SPI_CONTROL_REGISTER_TEMP = x;
}

static void slaveSelect(void)
{
	Digital.write(ss_pin_master_temp,LOW);
}

static void slaveUnselect(void)
{
	Digital.write(ss_pin_master_temp,HIGH);
}

static mos_uint8_t transfer(mos_uint8_t data)
{	
	mos_uint8_t value;
	
	GLOBAL_INTERRUPT_DISABLE();
	SPI_DATA_REGISTER_TEMP = data;    
	asm volatile("nop");
	while(!(SPI_STATUS_REGISTER_TEMP & (1<<SPIF)));
	value = SPI_DATA_REGISTER_TEMP;
	GLOBAL_INTERRUPT_ENABLE();
	
	return value;
}

static void transferMulti(mos_uint8_t * data_tx, mos_uint8_t * data_rx, mos_uint16_t data_rx_tx_len)
{	
	while(data_rx_tx_len)
	{	
		*data_rx = transfer(*data_tx);		
		data_tx++;
		data_rx++;
		data_rx_tx_len--;
	}	
}

static void transmit(mos_uint8_t * data_tx, mos_uint16_t data_tx_len)
{	
	while(data_tx_len)
	{	
		transfer(*data_tx);		
		data_tx++;
		data_tx_len--;
	}	
}

static void receive(mos_uint8_t data_tx,  mos_uint8_t * data_rx, mos_uint16_t data_rx_len)
{	
	while(data_rx_len)
	{	
		*data_rx = transfer(data_tx);		
		data_rx++;
		data_rx_len--;
	}	
}


const struct spi_master_driver avr_mega_spi_master_driver = {
	begin,
	end,
	config,
	slaveSelect,
	slaveUnselect,
	transfer,
	transferMulti,
	transmit,
	receive
};

#undef SPI_CONTROL_REGISTER_TEMP
#undef SPI_STATUS_REGISTER_TEMP
#undef SPI_DATA_REGISTER_TEMP

#endif
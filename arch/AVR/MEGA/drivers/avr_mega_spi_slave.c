//-------------------Macros to change for SPIx------------------//
// avr_mega_spi_slave
// SPI_AVAILABLE
// avr_mega_spi_slave_driver
// SPI_MOSI_PIN
// SPI_MISO_PIN
// SPI_SCK_PIN
// SPI_SS_PIN
// SPCR
// SPSR
// SPDR
// SPI_SLAVE_ENABLE_TEMP
// SPI_SLAVE_RX_BUFFER_SIZE_TEMP
// SPI_SLAVE_TX_BUFFER_SIZE_TEMP

#include "avr_mega_spi_slave.h"

#if defined(PLATFORM_SUPPORT_SPI) && defined(SPI_AVAILABLE) && (SPI_SLAVE_ENABLE_TEMP == 1)
	#include "../../../../os/drivers/Digital.h"
	#include <avr/interrupt.h>
	
	#define SPI_CONTROL_REGISTER_TEMP 	SPCR
	#define SPI_STATUS_REGISTER_TEMP 	SPSR
	#define SPI_DATA_REGISTER_TEMP 		SPDR
	
	#ifdef SPI_SLAVE_BUFFER_ENABLE
		static mos_uint8_t data_read;
		static mos_uint8_t data_available=0;
	#else
		#include "../../../../os/util/ringbuf.h"
		static mos_uint8_t bufferSlaveRx[SPI_SLAVE_RX_BUFFER_SIZE_TEMP];
		static ringbuf_struct_t buffer_struct_slave_rx;
		static mos_uint8_t bufferSlaveTx[SPI_SLAVE_TX_BUFFER_SIZE_TEMP];
		static ringbuf_struct_t buffer_struct_slave_tx;
	#endif
	
	void (*spi_slave_callback_pointer)(mos_uint8_t data) = NULL;
	static mos_uint8_t spi_slave_callback_enable = 0;
	
	static void begin(void)
	{
		SPI_CONTROL_REGISTER_TEMP = 0xC0;
		SPI_STATUS_REGISTER_TEMP = 0x00;
		
		Digital.pinmode(SPI_MOSI_PIN,INPUT);
		Digital.pinmode(SPI_MISO_PIN,OUTPUT);
		Digital.pinmode(SPI_SCK_PIN,INPUT);
		Digital.pinmode(SPI_SS_PIN,INPUT);
		
		#ifdef SPI_SLAVE_BUFFER_ENABLE	
			ringbuf_init(&buffer_struct_slave_rx, bufferSlaveRx, SPI_SLAVE_RX_BUFFER_SIZE_TEMP);
			ringbuf_init(&buffer_struct_slave_tx, bufferSlaveTx, SPI_SLAVE_TX_BUFFER_SIZE_TEMP);
		#endif
		spi_slave_callback_enable = 0;
	}
	
	static void end(void)
	{
		SPI_CONTROL_REGISTER_TEMP = 0x00;
		SPI_STATUS_REGISTER_TEMP = 0x00;
		
		Digital.pinmode(SPI_MOSI_PIN,INPUT);
		Digital.pinmode(SPI_MISO_PIN,INPUT);
		Digital.pinmode(SPI_SCK_PIN,INPUT);
		Digital.pinmode(SPI_SS_PIN,INPUT);
		spi_slave_callback_enable = 0;
	}

	static void config(mos_uint8_t mode, mos_uint8_t data_order)
	{
			SPI_CONTROL_REGISTER_TEMP &= ~(0x3 << 2);
			SPI_CONTROL_REGISTER_TEMP |= (mode & 0x3) << 2;

			if(data_order == SPI_MSB_FIRST)
				 SPI_CONTROL_REGISTER_TEMP &= ~(1<<5);
			else if(data_order == SPI_LSB_FIRST)
				 SPI_CONTROL_REGISTER_TEMP |= 1<<5;
	}

	static void transfer(mos_uint8_t data)
	{
		SPI_DATA_REGISTER_TEMP = data;
	}
	
	static mos_uint8_t available(void)
	{
		#ifndef SPI_SLAVE_BUFFER_ENABLE
			return data_available;
		#else
			return ringbuf_count(&buffer_struct_slave_rx)
		#endif
	}
	
	void attachCallback(void(*spi_slave_callback)(mos_uint8_t data)))
	{
		spi_slave_callback_pointer = spi_slave_callback;
		spi_slave_callback_enable = 1;
	}
	
	static mos_uint8_t read(void)
	{
		#ifndef SPI_SLAVE_BUFFER_ENABLE			
			ATOMIC_ON();
			data_available = 0;
			ATOMIC_OFF();
			return data_read;
		#else
			return ringbuf_read(&buffer_struct_slave_rx);
		#endif
	}

	#ifdef SPI_SLAVE_BUFFER_ENABLE
		static void transferMulti(mos_uint8_t * data, mos_uint16_t data_len)
		{
			while(data_len)
			{
				ringbuf_write(&buffer_struct_slave_tx, *data);
				data++;
				data_len--;
			}
		}
		void txBufferFlush(void)
		{
			ringbuf_flush(&buffer_struct_slave_tx);
		}
		
		void rxBufferFlush(void)
		{
			ringbuf_flush(&buffer_struct_slave_rx);
		}
	#endif
	
	// SPI Transmission/reception complete ISR
	ISR(SPI_STC_vect)
	{
		mos_uint8_t data_temp;
		#ifndef SPI_SLAVE_BUFFER_ENABLE
			data_temp = SPI_DATA_REGISTER_TEMP;
			data_read = data_temp;
			data_available = 1;
		#else
			data_temp = SPI_DATA_REGISTER_TEMP;
			ringbuf_write(&buffer_struct_slave_rx, data_temp);		
			SPI_DATA_REGISTER_TEMP = ringbuf_read(&buffer_struct_slave_tx);
		#endif
		
		if(spi_slave_callback_enable == 1)
			spi_slave_callback_pointer(data_temp);
	}

	const struct spi_slave_driver avr_mega_spi_slave_driver = {
		begin,
		config,
		transfer,
		available,
		read,
		attachCallback,
		#ifdef SPI_SLAVE_BUFFER_ENABLE
			transferMulti,
			txBufferFlush,
			rxBufferFlush,
		#endif
	};
	
#undef SPI_CONTROL_REGISTER_TEMP
#undef SPI_STATUS_REGISTER_TEMP
#undef SPI_DATA_REGISTER_TEMP
	
#endif
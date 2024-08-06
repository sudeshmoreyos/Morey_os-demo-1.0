//-------------------Macros to change for SPIx------------------//
// AVR_MEGA_SPI_SLAVE_H
// SPI_AVAILABLE
// SPI_PORT
// avr_mega_spi_slave_driver
// SPI_SLAVE_ENABLE_TEMP

#ifndef AVR_MEGA_SPI_SLAVE_H
#define AVR_MEGA_SPI_SLAVE_H

#include "../../../../platform.h"

#if defined(PLATFORM_SUPPORT_SPI) && defined(SPI_AVAILABLE)

	#if SPI0_PORT_CONF == SPI_PORT
		#ifdef SPI0_SLAVE_ENABLE
			#define SPI_SLAVE_ENABLE_TEMP 1
		#else
			#define SPI_SLAVE_ENABLE_TEMP 0
		#endif
	#elif SPI1_PORT_CONF == SPI_PORT
		#ifdef SPI1_SLAVE_ENABLE
			#define SPI_SLAVE_ENABLE_TEMP 1
		#else
			#define SPI_SLAVE_ENABLE_TEMP 0
		#endif
	#endif
	
	#if SPI_SLAVE_ENABLE_TEMP == 1
		#include "../../../../os/drivers/SpiSlave.h"
		#include "avr_mega_spi_defines.h"
		
		#if SPI0_PORT_CONF == SPI_PORT
			
			#define SpiSlave avr_mega_spi_slave_driver
			#define SpiSlave0 SpiSlave
			
		#elif SPI1_PORT_CONF == SPI_PORT
		
			#define SpiSlave1 avr_mega_spi_slave_driver
			
		#endif
		
		extern const struct spi_slave_driver avr_mega_spi_slave_driver;
	#endif
	
#endif

#endif
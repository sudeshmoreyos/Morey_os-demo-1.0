//-------------------Macros to change for SPIx------------------//
// AVR_MEGA_SPI0_MASTER_H
// SPI0_AVAILABLE
// SPI0_PORT
// avr_mega_spi0_master_driver

#ifndef AVR_MEGA_SPI0_MASTER_H
#define AVR_MEGA_SPI0_MASTER_H

#include "../../../../platform.h"

#if defined(PLATFORM_SUPPORT_SPI) && defined(SPI0_AVAILABLE)
				
	#include "../../../../os/drivers/SpiMaster.h"
	#include "avr_mega_spi_defines.h"
	
	#if SPI0_PORT_CONF == SPI0_PORT
		
		#define SpiMaster avr_mega_spi0_master_driver
		#define SpiMaster0 SpiMaster
		
	#elif SPI1_PORT_CONF == SPI0_PORT
	
		#define SpiMaster1 avr_mega_spi0_master_driver
		
	#endif
	
	extern const struct spi_master_driver avr_mega_spi0_master_driver;		
#endif
#endif
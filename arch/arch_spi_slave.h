#ifndef ARCH_SPI_SLAVE_H
#define ARCH_SPI_SLAVE_H
	#ifdef PLATFORM_SUPPORT_SPI
		#if CPU == AVR
			#if CPU_SERIES == MEGA				
				#ifdef SPI_AVAILABLE
					#include "AVR/MEGA/drivers/avr_mega_spi_slave.h"
				#endif
				
				#ifdef SPI0_AVAILABLE
					#include "AVR/MEGA/drivers/avr_mega_spi0_slave.h"
				#endif
				
				#ifdef SPI1_AVAILABLE				
					#include "AVR/MEGA/drivers/avr_mega_spi1_slave.h"
				#endif				
			#elif CPU_SERIES == XMEGA
				#include "AVR/XMEGA/drivers/avr_xmega_spi_slave_c.h"
				#include "AVR/XMEGA/drivers/avr_xmega_spi_slave_d.h"
				#include "AVR/XMEGA/drivers/avr_xmega_spi_slave_e.h"
				#include "AVR/XMEGA/drivers/avr_xmega_spi_slave_f.h"
			#endif
		#endif
	#endif
#endif
		
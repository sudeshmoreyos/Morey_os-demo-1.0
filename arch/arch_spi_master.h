#ifndef ARCH_SPI_MASTER_H
#define ARCH_SPI_MASTER_H
	#ifdef PLATFORM_SUPPORT_SPI
		#if CPU == AVR
			#if CPU_SERIES == MEGA								
				#ifdef SPI_AVAILABLE
					#include "AVR/MEGA/drivers/avr_mega_spi_master.h"
				#endif
				
				#ifdef SPI0_AVAILABLE
					#include "AVR/MEGA/drivers/avr_mega_spi0_master.h"
				#endif
				
				#ifdef SPI1_AVAILABLE				
					#include "AVR/MEGA/drivers/avr_mega_spi1_master.h"
				#endif			
			#elif CPU_SERIES == XMEGA
				#include "AVR/XMEGA/drivers/avr_xmega_spi_master_c.h"
				#include "AVR/XMEGA/drivers/avr_xmega_spi_master_d.h"
				#include "AVR/XMEGA/drivers/avr_xmega_spi_master_e.h"
				#include "AVR/XMEGA/drivers/avr_xmega_spi_master_f.h"
			#endif
		#endif
	#endif
#endif
		
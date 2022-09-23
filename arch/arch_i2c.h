#ifndef ARCH_I2C_H
#define ARCH_I2C_H_H

	#ifdef PLATFORM_SUPPORT_I2C
		#if CPU == AVR
			#if CPU_SERIES == MEGA
				#include "AVR/MEGA/drivers/avr_mega_i2c.h"
			#elif CPU_SERIES == XMEGA
				#include "AVR/XMEGA/drivers/avr_xmega_i2c_c.h"
				#include "AVR/XMEGA/drivers/avr_xmega_i2c_e.h"
			#endif
		#endif
	#endif

#endif
				
		
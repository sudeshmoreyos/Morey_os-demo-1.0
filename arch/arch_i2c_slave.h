#ifndef ARCH_I2C_SLAVE_H
#define ARCH_I2C_SLAVE_H
	#ifdef PLATFORM_SUPPORT_I2C
		#if CPU == AVR
			#if CPU_SERIES == MEGA
				#include "AVR/MEGA/drivers/avr_mega_i2c_slave.h"
			#elif CPU_SERIES == XMEGA
				
			#endif
		#endif
	#endif
#endif
				
		
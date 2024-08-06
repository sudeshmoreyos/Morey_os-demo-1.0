#ifndef ARCH_I2C_MASTER_H
#define ARCH_I2C_MASTER_H
	#ifdef PLATFORM_SUPPORT_I2C
		#if CPU == AVR
			#if CPU_SERIES == MEGA							
				#ifdef I2C_AVAILABLE
					#include "AVR/MEGA/drivers/avr_mega_i2c_master.h"
				#endif
				
				#ifdef I2C0_AVAILABLE
					#include "AVR/MEGA/drivers/avr_mega_i2c0_master.h"
				#endif
				
				#ifdef I2C1_AVAILABLE				
					#include "AVR/MEGA/drivers/avr_mega_i2c1_master.h"
				#endif
			#elif CPU_SERIES == XMEGA
				#include "AVR/XMEGA/drivers/avr_xmega_i2c_master_c.h"
				#include "AVR/XMEGA/drivers/avr_xmega_i2c_master_e.h"
			#endif
		#endif
	#endif
#endif
				
		
#ifndef ARCH_DIGITAL_H
#define ARCH_DIGITAL_H

#if CPU == AVR
	#if CPU_SERIES == MEGA
		#include "AVR/MEGA/drivers/avr_mega_digital.h"
	#elif CPU_SERIES == XMEGA
		#include "AVR/XMEGA/drivers/avr_xmega_digital.h"
	#endif
#endif

#endif
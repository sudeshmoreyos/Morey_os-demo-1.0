#ifndef ARCH_DELAY_BLOCKING_H
#define ARCH_DELAY_BLOCKING_H

#if CPU == AVR
	#if CPU_SERIES == MEGA
		#include "AVR/MEGA/lib/avr_mega_delay_blocking.h"
	#elif CPU_SERIES == XMEGA
		#include "AVR/XMEGA/lib/avr_xmega_delay_blocking.h"
	#elif CPU_SERIES == MEGA0
		#include "AVR/MEGA0/lib/avr_mega0_delay_blocking.h"
	#endif
#endif

#endif
#ifndef ARCH_TIMER_DELAY_H
#define ARCH_TIMER_DELAY_H

#if CPU == AVR
	#if CPU_SERIES == MEGA
		#include "AVR/MEGA/drivers/avr_mega_timer_delay.h"
	#elif CPU_SERIES == XMEGA
		#include "AVR/XMEGA/drivers/avr_xmega_timer_delay.h"
	#endif
#endif

#endif
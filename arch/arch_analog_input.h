#ifndef ARCH_ANALOGINPUT_H
#define ARCH_ANALOGINPUT_H

#if CPU == AVR
	#if CPU_SERIES == MEGA
		#include "AVR/MEGA/drivers/avr_mega_analog_input.h"
	#elif CPU_SERIES == XMEGA
		#include "AVR/XMEGA/drivers/avr_xmega_analog_input.h"
	#endif
#endif

#endif
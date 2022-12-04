#ifndef AVR_MEGA_TIMER_DELAY_H
#define AVR_MEGA_TIMER_DELAY_H

#include "../../../../platform.h"

#ifdef PLATFORM_SUPPORT_TIMER_DELAY
	
	#if TIMER_TYPE != TIMER_0
		#define TIMER_DELAY0 TIMER_0
	#endif
	
	#if TIMER_TYPE != TIMER_1
		#define TIMER_DELAY1 TIMER_1
	#endif
	
	#if TIMER_TYPE != TIMER_2
		#define TIMER_DELAY2 TIMER_2
	#endif
	
	#include "../../../../os/drivers/TimerDelay.h"
	#define TimerDelay avr_mega_timer_delay_driver
	extern const struct timer_delay_driver avr_mega_timer_delay_driver;
#endif

#endif

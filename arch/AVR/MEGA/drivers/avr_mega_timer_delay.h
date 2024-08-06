#ifndef AVR_MEGA_TIMER_DELAY_H
#define AVR_MEGA_TIMER_DELAY_H

#include "../../../../platform.h"

#ifdef PLATFORM_SUPPORT_TIMER_DELAY
	#include "../../../../os/drivers/TimerDelay.h"
	#define TimerDelay avr_mega_timer_delay_driver
	extern const struct timer_delay_driver avr_mega_timer_delay_driver;
#endif

#endif
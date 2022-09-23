#ifndef TIMER_DELAY_H
#define TIMER_DELAY_H

#include "../../platform.h"

struct timer_delay_driver
{
	void (*begin) (mos_uint8_t timer_no , float timer_delay_ms, void (*timer_delay_callback)(void));	
};

#include "../../arch/arch_timer_delay.h"

#endif
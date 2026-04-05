#ifndef TIMER_DELAY_H
#define TIMER_DELAY_H

#include "../../platform.h"
typedef void (*voidIn_voidOut_fcn_t)(void);

struct timer_delay_driver
{
	//void (*begin) (mos_uint8_t timer_no , float timer_delay_ms, void (*timer_delay_callback)(void));
	void (*begin) (mos_uint8_t timer_no , float timer_delay_ms, voidIn_voidOut_fcn_t timer_delay_callback);
	void (*end) (mos_uint8_t timer_no);
};

#include "../../arch/arch_timer_delay.h"

#endif
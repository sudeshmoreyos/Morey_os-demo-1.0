#ifndef AVR_MEGA_DELAY_BLOCKING_H
#define AVR_MEGA_DELAY_BLOCKING_H

#include "../../../../platform.h"
#if ( (COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR) || (COMPILER == AVR_GCC ) )

#include <util/delay.h>

#define arch_delay_ms_blocking(ms) _delay_ms(ms)
#define arch_delay_us_blocking(us) _delay_us(us)

#endif

#endif
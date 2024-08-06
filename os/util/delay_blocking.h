#ifndef DELAY_BLOCKING_H
#define DELAY_BLOCKING_H

#include "../../arch/arch_delay_blocking.h"

#define delay_ms_blocking(ms) arch_delay_ms_blocking(ms)
#define delay_us_blocking(us) arch_delay_us_blocking(us)

#endif
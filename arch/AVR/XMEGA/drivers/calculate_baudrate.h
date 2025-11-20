#ifndef CALCULATE_BAUDRATE_H
#define CALCULATE_BAUDRATE_H

#include "../../../../platform.h"

float xmega_baudrate(mos_uint32_t freq, mos_uint32_t baudrate, mos_uint8_t speed, \
	mos_uint16_t * bsel_value, int * bscale_value);

#endif
#ifndef AVR_XMEGA_DIGITAL_H
#define AVR_XMEGA_DIGITAL_H

#include "../../../../platform.h"
#include "../../../../os/drivers/Digital.h"

#ifdef PLATFORM_SUPPORT_DIGITAL_IO

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define INPUT_PULLDOWN 3

#define LOW  0
#define HIGH 1
#define TOGGLE 2

#define Digital avr_xmega_digital_driver
extern const struct digital_driver avr_xmega_digital_driver;

#else
	#error Platform do not support DIGITAL IO

#endif

#endif

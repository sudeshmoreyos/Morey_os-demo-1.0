#ifndef AVR_MEGA_DIGITAL_H
#define AVR_MEGA_DIGITAL_H

#include "../../../../platform.h"

#ifdef PLATFORM_SUPPORT_DIGITAL_IO

#include "../../../../os/drivers/Digital.h"

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define LOW  0
#define HIGH 1
#define TOGGLE 2

#define Digital avr_mega_digital_driver
extern const struct digital_driver avr_mega_digital_driver;

#endif

#endif

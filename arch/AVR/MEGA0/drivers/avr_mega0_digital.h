#ifndef _AVR_MEGA0_DIGITAL_H_
#define _AVR_MEGA0_DIGITAL_H_

#include "../../../../platform.h"

#ifdef PLATFORM_SUPPORT_DIGITAL_IO

#include "../../../../os/drivers/Digital.h"

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define LOW  0
#define HIGH 1
#define TOGGLE 2

#define Digital avr_mega0_digital_driver
extern const struct digital_driver avr_mega0_digital_driver;

#endif
#endif

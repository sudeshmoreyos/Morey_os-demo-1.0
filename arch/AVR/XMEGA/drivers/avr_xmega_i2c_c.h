#ifndef AVR_XMEGA_I2C_C_H
#define AVR_XMEGA_I2C_C_H

#include "../../../../platform.h"

#ifdef PLATFORM_SUPPORT_I2C
	#include "../../../../os/drivers/I2C.h"
	#define I2C avr_xmega_i2c_c_driver
	#define I2C0 I2C
	extern const struct i2c_driver avr_xmega_i2c_c_driver;
#endif

#endif
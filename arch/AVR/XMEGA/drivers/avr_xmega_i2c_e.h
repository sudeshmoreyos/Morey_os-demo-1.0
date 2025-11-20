#ifndef AVR_XMEGA_I2C_E_H
#define AVR_XMEGA_I2C_E_H

#include "../../../../platform.h"

#ifdef PLATFORM_SUPPORT_I2C
	#include "../../../../os/drivers/I2C.h"
	#define I2C1 avr_xmega_i2c_e_driver
	extern const struct i2c_driver avr_xmega_i2c_e_driver;
#endif

#endif
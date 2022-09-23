#ifndef AVR_MEGA_I2C_H
#define AVR_MEGA_I2C_H

#include "../../../../platform.h"

#ifdef PLATFORM_SUPPORT_I2C
	#include "../../../../os/drivers/I2C.h"
	#include "i2c_status.h"
	#define I2C avr_mega_i2c_driver
	#define I2C0 I2C
	extern const struct i2c_driver avr_mega_i2c_driver;
#endif

#endif
#ifndef AVR_MEGA_I2C_H
#define AVR_MEGA_I2C_H

#include "../../../../platform.h"

#if defined(PLATFORM_SUPPORT_I2C) && defined(I2C_AVAILABLE)
	#include "../../../../os/drivers/I2cSlave.h"
	#define I2cSlave avr_mega_i2c_slave_driver
	#define I2cSlave0 I2cSlave
	extern const struct i2c_slave_driver avr_mega_i2c_slave_driver;
#endif

#endif
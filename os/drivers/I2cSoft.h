#ifndef I2C_SOFT_H
#define I2C_SOFT_H

#include "../../platform.h"

#ifdef I2C_SOFT0_ENABLE
	#ifdef I2C_SOFT_ENABLE
		#error Either I2C_SOFT_ENABLE Macro or I2C_SOFT0_ENABLE Macro must be defined
	#else
		#define I2C_SOFT_ENABLE
	#endif
#endif

#if defined(PLATFORM_SUPPORT_I2C) && defined(I2C_SOFT_ENABLE)
	#include "I2cMaster.h"
	#define I2cSoft0 I2cSoft
	extern const struct i2c_master_driver I2cSoft;
#endif

#endif
//-------------------Macros to change for I2Cx------------------//
// AVR_MEGA_I2C0_MASTER_H
// I2C0_AVAILABLE
// I2C0_PORT
// avr_mega_i2c0_master_driver

#ifndef AVR_MEGA_I2C0_MASTER_H
#define AVR_MEGA_I2C0_MASTER_H

#include "../../../../platform.h"

#if defined(PLATFORM_SUPPORT_I2C) && defined(I2C0_AVAILABLE)
				
	#include "../../../../os/drivers/I2cMaster.h"
	#include "i2c_status.h"
	
	#if I2C0_PORT_CONF == I2C0_PORT
		
		#ifdef I2C_TIMEOUT_MS_CONF
			#define I2C0_TIMEOUT_MS_CONF I2C0_TIMEOUT_MS_CONF
		#endif
		
		#ifdef I2C0_TIMEOUT_MS_CONF
			#define I2C_TIMEOUT_MS_TEMP I2C0_TIMEOUT_MS_CONF
		#endif
		
		#define I2cMaster avr_mega_i2c0_master_driver
		#define I2cMaster0 I2cMaster
		
	#elif I2C1_PORT_CONF == I2C0_PORT
		
		#ifdef I2C1_TIMEOUT_MS_CONF
			#define I2C_TIMEOUT_MS_TEMP I2C1_TIMEOUT_MS_CONF
		#endif
		
		#define I2cMaster1 avr_mega_i2c0_master_driver
		
	#endif
	
	extern const struct i2c_master_driver avr_mega_i2c0_master_driver;	
#endif

#endif
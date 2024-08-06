#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include "../../platform.h"

struct i2c_slave_driver
{	
	void (*begin)(mos_uint8_t slave_address);
	void (*end)(void);
	mos_uint8_t (*rxComplete) (void);
	void (*read) (void * data_rx_void, mos_uint16_t * data_rx_len);
	mos_uint8_t (*txComplete) (mos_uint16_t * data_tx_len);
	mos_uint8_t (*print) ( void * data_tx_void, mos_uint16_t data_tx_len);
	void (*attachRxCallback)(void (*rx_callback_function)(void));
	void (*attachTxCallback)(void (*tx_callback_function)(void));
	void (*detachRxCallback)(void);
	void (*detachTxCallback)(void);
};

#include "../../arch/arch_i2c_slave.h"

#endif
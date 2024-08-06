#ifndef I2C_H
#define I2C_H

#include "../../platform.h"

struct i2c_master_driver
{
	void (*begin)(mos_uint32_t baudrate);
	void (*end)(void);
	mos_uint8_t (*transfer)(mos_uint8_t slave_address, void * data_tx, mos_uint16_t data_tx_len, \
		void * data_rx, mos_uint16_t data_rx_len);
};

#include "../../arch/arch_i2c_master.h"

#endif
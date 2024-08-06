#ifndef I2C_H
#define I2C_H

#include "../../platform.h"

struct i2c_driver
{
	void (*masterBegin)(mos_uint32_t baudrate);
	mos_uint8_t (*masterTransfer)(mos_uint8_t slave_address, mos_uint16_t data_tx_len, mos_uint8_t * data_tx, \
		mos_uint16_t data_rx_len, mos_uint8_t * data_rx);
	
	/*
	void (*slaveBegin)(mos_uint8_t slave_address);
	mos_uint8_t (*slaveTransferComplete) (void);
	mos_uint8_t (*slaveRxData) (mos_uint16_t * data_rx_len, mos_uint8_t * data_rx);
	void (*slaveRxCompleteCallback) ( void (*callback_fcn) (mos_uint16_t * data_rx_lens, mos_uint8_t * data_rx) );	
	mos_uint8_t (*slaveTxData) (mos_uint16_t data_tx_len, mos_uint8_t * data_tx);
	*/
};

struct i2c_driver_interrupt
{
	void (*masterBegin)(mos_uint32_t baudrate);
	mos_uint8_t (*masterTransfer)(mos_uint8_t slave_address, mos_uint16_t data_tx_len, mos_uint8_t * data_tx, \
		mos_uint16_t data_rx_len, mos_uint8_t * data_rx);
	mos_uint8_t (*masterSend)(mos_uint8_t slave_address, mos_uint16_t data_tx_len, mos_uint8_t * data_tx, mos_uint16_t data_rx_len);
	mos_uint8_t (*masterTransferComplete) (void);
	mos_uint8_t (*masterRxData) (mos_uint16_t * data_rx_len_actual, mos_uint8_t * data_rx);

	/*
	void (*slaveBegin)(mos_uint8_t slave_address);
	mos_uint8_t (*slaveTransferComplete) (void);
	mos_uint8_t (*slaveRxData) (mos_uint16_t * data_rx_len, mos_uint8_t * data_rx);
	void (*slaveRxCompleteCallback) ( void (*callback_fcn) (mos_uint16_t * data_rx_lens, mos_uint8_t * data_rx) );	
	mos_uint8_t (*slaveTxData) (mos_uint16_t data_tx_len, mos_uint8_t * data_tx);
	*/
};

#include "../../arch/arch_i2c.h"

#endif
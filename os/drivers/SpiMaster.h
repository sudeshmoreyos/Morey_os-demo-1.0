#ifndef SPI_MASTER_H
#define SPI_MASTER_H

#include "../../platform.h"
struct spi_master_driver
{
	void (*begin)(mos_uint8_t ss_pin);
	void (*end)(void);
	void (*config)(mos_uint8_t speed, mos_uint8_t mode, mos_uint8_t data_order);
	void (*slaveSelect)(void);
	void (*slaveUnselect)(void);
	mos_uint8_t (*transfer)(mos_uint8_t data);
	void (*transferMulti)(mos_uint8_t * data_tx, mos_uint8_t * data_rx, mos_uint16_t data_rx_tx_len);
	void (*transmit)(mos_uint8_t * data_tx, mos_uint16_t data_tx_len);
	void (*receive)(mos_uint8_t data_tx,  mos_uint8_t * data_rx, mos_uint16_t data_rx_len);
};

#include "../../arch/arch_spi_master.h"

#endif
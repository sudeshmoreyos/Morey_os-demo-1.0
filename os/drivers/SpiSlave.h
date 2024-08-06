#ifndef SPI_SLAVE_H
#define SPI_SLAVE_H

#include "../../platform.h"

struct spi_slave_driver
{
	void (*begin)(void(*spi_slave_callback)(mos_uint8_t data_rx));
	void (*config)(mos_uint8_t mode, mos_uint8_t data_order);
	void (*transfer)(mos_uint8_t data);
	mos_uint8_t (*available)();
	mos_uint8_t (*read)();
	void (*attachCallback)(void(*spi_slave_callback)(mos_uint8_t data)));
	#ifdef SPI_SLAVE_BUFFER_ENABLE
		void (*transferMulti)(mos_uint8_t * data, mos_uint16_t data_len);
		void txBufferFlush();
		void rxBufferFlush();
	#endif
};

#include "../../arch/arch_spi_slave.h"

#endif
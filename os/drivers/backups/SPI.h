#ifndef SPI_H
#define SPI_H

#include "../../platform.h"

#ifndef SPI_SLAVE_CALLBACK_ENABLE
	#ifndef SPI_SLAVE_BUFFER_ENABLE
		#define SPI_SLAVE_CALLBACK_ENABLE
	#endif
#endif

struct spi_driver
{
	#ifdef SPI_MASTER_ENABLE
		void (*masterBegin)(mos_uint8_t ss_pin);
		void (*masterConfig)(mos_uint8_t speed, mos_uint8_t mode, mos_uint8_t data_order);
		mos_uint8_t (*masterTransfer)(mos_uint8_t data);
		void (*masterTransferMulti)(mos_uint8_t data_tx_len, mos_uint8_t * data_tx, mos_uint8_t * data_rx);
	#endif
				
	#ifdef SPI_SLAVE_ENABLE
		void (*slaveBegin)(void(*spi_slave_callback)(mos_uint8_t data_rx));
		void (*slaveConfig)(mos_uint8_t mode, mos_uint8_t data_order);
		void (*slaveTransfer)(mos_uint8_t data);
		#ifdef SPI_SLAVE_BUFFER_ENABLE
			void (*slaveTransferMulti)(mos_uint8_t data_len, mos_uint8_t * data);
			mos_uint8_t (*slaveAvailable)();
			mos_uint8_t (*slaveRead)();
			void slaveTxBufferFlush();
			void slaveRxBufferFlush();
		#endif
	#endif		
};

#include "../../arch/arch_spi.h"

#endif
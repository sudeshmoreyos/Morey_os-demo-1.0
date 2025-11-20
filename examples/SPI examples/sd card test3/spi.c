#include "spi.h"
#include "SpiMaster.h"

mos_uint8_t SPI_transfer(mos_uint8_t data)
{
	return SpiMaster.transfer(data);
}

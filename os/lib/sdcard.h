/*-----------------------------------------------------------------------
/  Low level disk interface modlue include file   (C)ChaN, 2016
/-----------------------------------------------------------------------*/

#ifndef SDCARD_H
#define SDCARD_H

#include "../../platform.h"
#include "SpiMaster.h"
#include "diskio.h"

mos_uint8_t sdcard_begin (const struct spi_master_driver * spi_pointer, mos_uint8_t ss_pin);
mos_uint8_t sdcard_status (void);
DRESULT sdcard_read (mos_uint8_t * buff, mos_uint32_t sector, mos_uint16_t count);
DRESULT sdcard_write (const mos_uint8_t * buff, mos_uint32_t sector, mos_uint16_t count);
DRESULT sdcard_ioctl ( mos_uint8_t ctrl, void *buff );

#endif

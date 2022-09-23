#ifndef SERIAL_H
#define SERIAL_H

#include "../../platform.h"

struct serial_driver
{
	void (*begin)(mos_uint32_t baudrate, mos_uint8_t mode);
	unsigned int (*available)(void);
	mos_uint8_t (*read)(void);
	void (*flush)(void);
	void (*write)(mos_uint8_t data);
	void (*print)(char *data);
	void (*println)(char *data);
	void (*printBytes)(mos_uint8_t * data, mos_uint16_t len);
	mos_uint8_t (*txComplete)(void);
};

#include "../../arch/arch_serial.h"

#endif
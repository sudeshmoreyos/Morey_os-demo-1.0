#ifndef SERIAL_H
#define SERIAL_H

#include "../../platform.h"

struct serial_driver
{
	void (*begin)(mos_uint32_t baudrate, mos_uint8_t mode);
	void (*end)(void);
	unsigned int (*available)(void);
	mos_uint8_t (*read)(void);
	void (*rxEmpty)(void);
	void (*write)(mos_uint8_t data);
	void (*print)(char *data);
	void (*println)(char *data);
	#ifdef PLATFORM_SUPPORT_CONST_PRINT
		void (*constPrintArch)(const char *data);
		void (*constPrintlnArch)(const char *data);
	#endif
	void (*printBytes)(mos_uint8_t * data, mos_uint16_t len);
	mos_uint8_t (*txComplete)(void);
	#ifdef SERIAL_LINE_ENABLE
		void (*terminator)(void * term, uint8_t len);
		unsigned int (*stringAvailable)(void);
		unsigned int (*readString)(char * data, int max_len);
	#endif
};

// Backward compatibility of flush with new function rxEmpty
// To be removed later
#define flush rxEmpty

#include "../../arch/arch_serial.h"

#endif
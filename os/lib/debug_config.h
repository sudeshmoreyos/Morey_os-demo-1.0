#ifndef DEBUG_CONFIG_H
#define DEBUG_CONFIG_H

#include "../../platform.h"

#ifdef PLATFORM_SUPPORT_UART
	#include "../drivers/Serial.h"

	#define DEBUG_INIT(baudrate) 	Serial.begin(baudrate,SERIAL_8N1)
	#define PRINT(data) 			Serial.print(data)
	#define PRINT_LN(data) 			Serial.println(data)
#else
	#define DEBUG_INIT(baudrate)
	#define PRINT(data)
	#define PRINT_LN(data)
#endif

#endif

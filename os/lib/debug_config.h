#ifndef DEBUG_CONFIG_H
#define DEBUG_CONFIG_H

#include "../../platform.h"

#ifdef PLATFORM_SUPPORT_UART
	#include "../drivers/Serial.h"
	
	#ifdef DEBUG_INIT_CONF
		#define DEBUG_INIT(baudrate) 		DEBUG_INIT_CONF(baudrate)
	#else
		#define DEBUG_INIT(baudrate)		Serial.begin(baudrate,SERIAL_8N1)
	#endif
	
	#ifdef DEBUG_PRINT_CONF
		#define PRINT(data) 				DEBUG_PRINT_CONF(data)
	#else
		#define PRINT(data) 				Serial.print(data)
	#endif
	
	#ifdef DEBUG_PRINT_LN_CONF
		#define PRINT_LN(data)				DEBUG_PRINT_LN_CONF(data)
	#else
		#define PRINT_LN(data) 				Serial.println(data)
	#endif
	
	#ifdef PLATFORM_SUPPORT_CONST_PRINT		
		#ifdef DEBUG_CONST_PRINT_CONF
			#define CONST_PRINT(data)			DEBUG_CONST_PRINT_CONF(data)
		#else
			#define CONST_PRINT(data)			Serial.constPrint(data)
		#endif
		
		#ifdef DEBUG_CONST_PRINT_LN_CONF
			#define CONST_PRINT_LN(data)		DEBUG_CONST_PRINT_LN_CONF(data)
		#else
			#define CONST_PRINT_LN(data)		Serial.constPrintln(data)
		#endif
	#else
		#define CONST_PRINT(data)			PRINT(data)
		#define CONST_PRINT_LN(data)		PRINT_LN(data)
	#endif
#else
	#define DEBUG_INIT(baudrate)
	#define PRINT(data)
	#define PRINT_LN(data)
	#define CONST_PRINT(data)			
	#define CONST_PRINT_LN(data)
#endif

#endif

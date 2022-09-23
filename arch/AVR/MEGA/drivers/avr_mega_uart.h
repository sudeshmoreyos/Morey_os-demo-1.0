#ifndef AVR_MEGA_UART_H
#define AVR_MEGA_UART_H

#include "../../../../platform.h"

#if defined(PLATFORM_SUPPORT_UART) && defined(UART_AVAILABLE)
// Defining Controller specific UART Macros from OS Serial Macros
	#if SERIAL0_PORT == UART_PORT
	
		#ifdef SERIAL0_ENABLE
			#define UART_ENABLE 1
		#else
			#define UART_ENABLE 0
		#endif
		
		#ifdef SERIAL0_TX_DISABLE
			#define UART_TX_DISABLE SERIAL0_TX_DISABLE
		#endif
		
		#ifdef SERIAL0_RX_DISABLE
			#define UART_RX_DISABLE SERIAL0_TX_DISABLE
		#endif
		
		#ifdef SERIAL0_RX_BUFFER_CONF
			#define UART_RX_BUFFER_CONF SERIAL0_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL0_TX_BUFFER_CONF
			#define UART_TX_BUFFER_CONF SERIAL0_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL0_TX_INTERRUPT_ENABLE
			#define UART_TX_INTERRUPT_ENABLE SERIAL0_TX_INTERRUPT_ENABLE
		#endif
	
	#elif SERIAL1_PORT == UART_PORT
	
		#ifdef SERIAL1_ENABLE
			#define UART_ENABLE 1
		#else
			#define UART_ENABLE 0
		#endif
		
		#ifdef SERIAL1_TX_DISABLE
			#define UART_TX_DISABLE SERIAL1_TX_DISABLE
		#endif
		
		#ifdef SERIAL1_RX_DISABLE
			#define UART_RX_DISABLE SERIAL1_TX_DISABLE
		#endif
		
		#ifdef SERIAL1_RX_BUFFER_CONF
			#define UART_RX_BUFFER_CONF SERIAL1_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL1_TX_BUFFER_CONF
			#define UART_TX_BUFFER_CONF SERIAL1_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL1_TX_INTERRUPT_ENABLE
			#define UART_TX_INTERRUPT_ENABLE SERIAL1_TX_INTERRUPT_ENABLE
		#endif
		
	#elif SERIAL2_PORT == UART_PORT
	
		#ifdef SERIAL2_ENABLE
			#define UART_ENABLE 1
		#else
			#define UART_ENABLE 0
		#endif
		
		#ifdef SERIAL2_TX_DISABLE
			#define UART_TX_DISABLE SERIAL2_TX_DISABLE
		#endif
		
		#ifdef SERIAL2_RX_DISABLE
			#define UART_RX_DISABLE SERIAL2_TX_DISABLE
		#endif
		
		#ifdef SERIAL2_RX_BUFFER_CONF
			#define UART_RX_BUFFER_CONF SERIAL2_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL2_TX_BUFFER_CONF
			#define UART_TX_BUFFER_CONF SERIAL2_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL2_TX_INTERRUPT_ENABLE
			#define UART_TX_INTERRUPT_ENABLE SERIAL2_TX_INTERRUPT_ENABLE
		#endif
		
	#elif SERIAL3_PORT == UART_PORT
	
		#ifdef SERIAL3_ENABLE
			#define UART_ENABLE 1
		#else
			#define UART_ENABLE 0
		#endif
		
		#ifdef SERIAL3_TX_DISABLE
			#define UART_TX_DISABLE SERIAL3_TX_DISABLE
		#endif
		
		#ifdef SERIAL3_RX_DISABLE
			#define UART_RX_DISABLE SERIAL3_TX_DISABLE
		#endif
		
		#ifdef SERIAL3_RX_BUFFER_CONF
			#define UART_RX_BUFFER_CONF SERIAL3_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL3_TX_BUFFER_CONF
			#define UART_TX_BUFFER_CONF SERIAL3_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL3_TX_INTERRUPT_ENABLE
			#define UART_TX_INTERRUPT_ENABLE SERIAL3_TX_INTERRUPT_ENABLE
		#endif
	#endif
		
	#if UART_ENABLE == 1
			
		#include "../../../../os/drivers/Serial.h"
		#include "avr_mega_uart_defines.h"
		
		#if SERIAL0_PORT == UART_PORT
		
			#define Serial avr_mega_uart_driver
			#define Serial0 Serial
			
		#elif SERIAL1_PORT == UART_PORT
			
			#define Serial1 avr_mega_uart_driver
			
		#elif SERIAL2_PORT == UART_PORT
			
			#define Serial2 avr_mega_uart_driver
			
		#elif SERIAL3_PORT == UART_PORT
		
			#define Serial3 avr_mega_uart_driver
		#endif
		
		extern const struct serial_driver avr_mega_uart_driver;		
	#endif
#endif

#endif
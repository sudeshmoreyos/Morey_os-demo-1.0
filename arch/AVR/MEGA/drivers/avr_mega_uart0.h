#ifndef AVR_MEGA_UART0_H
#define AVR_MEGA_UART0_H
#include "../../../../platform.h"

#if defined(PLATFORM_SUPPORT_UART) && defined(UART0_AVAILABLE)
// Defining Controller specific UART Macros from OS Serial Macros
	#if SERIAL0_PORT == UART0_PORT
	
		#ifdef SERIAL0_ENABLE
			#define UART0_ENABLE 1
			#define here2
		#else
			#define UART0_ENABLE 0
		#endif
		
		#ifdef SERIAL0_TX_DISABLE
			#define UART0_TX_DISABLE SERIAL0_TX_DISABLE
		#endif
		
		#ifdef SERIAL0_RX_DISABLE
			#define UART0_RX_DISABLE SERIAL0_TX_DISABLE
		#endif
		
		#ifdef SERIAL0_RX_BUFFER_CONF
			#define UART0_RX_BUFFER_CONF SERIAL0_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL0_TX_BUFFER_CONF
			#define UART0_TX_BUFFER_CONF SERIAL0_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL0_TX_INTERRUPT_ENABLE
			#define UART0_TX_INTERRUPT_ENABLE SERIAL0_TX_INTERRUPT_ENABLE
		#endif
	
	#elif SERIAL1_PORT == UART0_PORT
	
		#ifdef SERIAL1_ENABLE
			#define UART0_ENABLE 1
		#else
			#define UART0_ENABLE 0
		#endif
		
		#ifdef SERIAL1_TX_DISABLE
			#define UART0_TX_DISABLE SERIAL1_TX_DISABLE
		#endif
		
		#ifdef SERIAL1_RX_DISABLE
			#define UART0_RX_DISABLE SERIAL1_TX_DISABLE
		#endif
		
		#ifdef SERIAL1_RX_BUFFER_CONF
			#define UART0_RX_BUFFER_CONF SERIAL1_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL1_TX_BUFFER_CONF
			#define UART0_TX_BUFFER_CONF SERIAL1_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL1_TX_INTERRUPT_ENABLE
			#define UART0_TX_INTERRUPT_ENABLE SERIAL1_TX_INTERRUPT_ENABLE
		#endif
		
	#elif SERIAL2_PORT == UART0_PORT
	
		#ifdef SERIAL2_ENABLE
			#define UART0_ENABLE 1
		#else
			#define UART0_ENABLE 0
		#endif
		
		#ifdef SERIAL2_TX_DISABLE
			#define UART0_TX_DISABLE SERIAL2_TX_DISABLE
		#endif
		
		#ifdef SERIAL2_RX_DISABLE
			#define UART0_RX_DISABLE SERIAL2_TX_DISABLE
		#endif
		
		#ifdef SERIAL2_RX_BUFFER_CONF
			#define UART0_RX_BUFFER_CONF SERIAL2_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL2_TX_BUFFER_CONF
			#define UART0_TX_BUFFER_CONF SERIAL2_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL2_TX_INTERRUPT_ENABLE
			#define UART0_TX_INTERRUPT_ENABLE SERIAL2_TX_INTERRUPT_ENABLE
		#endif
		
	#elif SERIAL3_PORT == UART0_PORT
	
		#ifdef SERIAL3_ENABLE
			#define UART0_ENABLE 1
		#else
			#define UART0_ENABLE 0
		#endif
		
		#ifdef SERIAL3_TX_DISABLE
			#define UART0_TX_DISABLE SERIAL3_TX_DISABLE
		#endif
		
		#ifdef SERIAL3_RX_DISABLE
			#define UART0_RX_DISABLE SERIAL3_TX_DISABLE
		#endif
		
		#ifdef SERIAL3_RX_BUFFER_CONF
			#define UART0_RX_BUFFER_CONF SERIAL3_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL3_TX_BUFFER_CONF
			#define UART0_TX_BUFFER_CONF SERIAL3_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL3_TX_INTERRUPT_ENABLE
			#define UART0_TX_INTERRUPT_ENABLE SERIAL3_TX_INTERRUPT_ENABLE
		#endif
	#endif
		
	#if UART0_ENABLE == 1
			
		#include "../../../../os/drivers/Serial.h"
		#include "avr_mega_uart_defines.h"
		
		#if SERIAL0_PORT == UART0_PORT
		
			#define Serial avr_mega_uart0_driver
			#define Serial0 Serial
			
		#elif SERIAL1_PORT == UART0_PORT
			
			#define Serial1 avr_mega_uart0_driver
			
		#elif SERIAL2_PORT == UART0_PORT
			
			#define Serial2 avr_mega_uart0_driver
			
		#elif SERIAL3_PORT == UART0_PORT
		
			#define Serial3 avr_mega_uart0_driver
		#endif
		
		extern const struct serial_driver avr_mega_uart0_driver;		
	#endif
#endif
#endif
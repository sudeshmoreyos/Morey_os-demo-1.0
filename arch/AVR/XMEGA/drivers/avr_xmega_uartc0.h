#ifndef AVR_XMEGA_UARTC0_H
#define AVR_XMEGA_UARTC0_H

#include "../../../../platform.h"

#if defined(PLATFORM_SUPPORT_UART) && defined(UARTC0_AVAILABLE)
	
	// Defining Controller specific UART Macros from OS Serial Macros
	#if SERIAL0_PORT == UARTC0_PORT
	
		#ifdef SERIAL0_ENABLE
			#define UARTC0_ENABLE SERIAL0_ENABLE
		#else
			#define UARTC0_ENABLE 0
		#endif
		
		#ifdef SERIAL0_TX_DISABLE
			#define UARTC0_TX_DISABLE SERIAL0_TX_DISABLE
		#endif
		
		#ifdef SERIAL0_RX_DISABLE
			#define UARTC0_RX_DISABLE SERIAL0_TX_DISABLE
		#endif
		
		#ifdef SERIAL0_RX_BUFFER_CONF
			#define UARTC0_RX_BUFFER_CONF SERIAL0_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL0_TX_BUFFER_CONF
			#define UARTC0_TX_BUFFER_CONF SERIAL0_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL0_TX_INTERRUPT_ENABLE
			#define UARTC0_TX_INTERRUPT_ENABLE SERIAL0_TX_INTERRUPT_ENABLE
		#endif
	
	#elif SERIAL1_PORT == UARTC0_PORT
	
		#ifdef SERIAL1_ENABLE
			#define UARTC0_ENABLE SERIAL1_ENABLE
		#else
			#define UARTC0_ENABLE 0
		#endif
		
		#ifdef SERIAL1_TX_DISABLE
			#define UARTC0_TX_DISABLE SERIAL1_TX_DISABLE
		#endif
		
		#ifdef SERIAL1_RX_DISABLE
			#define UARTC0_RX_DISABLE SERIAL1_TX_DISABLE
		#endif
		
		#ifdef SERIAL1_RX_BUFFER_CONF
			#define UARTC0_RX_BUFFER_CONF SERIAL1_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL1_TX_BUFFER_CONF
			#define UARTC0_TX_BUFFER_CONF SERIAL1_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL1_TX_INTERRUPT_ENABLE
			#define UARTC0_TX_INTERRUPT_ENABLE SERIAL1_TX_INTERRUPT_ENABLE
		#endif
	
	#elif SERIAL2_PORT == UARTC0_PORT
	
		#ifdef SERIAL2_ENABLE
			#define UARTC0_ENABLE SERIAL2_ENABLE
		#else
			#define UARTC0_ENABLE 0
		#endif
		
		#ifdef SERIAL2_TX_DISABLE
			#define UARTC0_TX_DISABLE SERIAL2_TX_DISABLE
		#endif
		
		#ifdef SERIAL2_RX_DISABLE
			#define UARTC0_RX_DISABLE SERIAL2_TX_DISABLE
		#endif
		
		#ifdef SERIAL2_RX_BUFFER_CONF
			#define UARTC0_RX_BUFFER_CONF SERIAL2_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL2_TX_BUFFER_CONF
			#define UARTC0_TX_BUFFER_CONF SERIAL2_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL2_TX_INTERRUPT_ENABLE
			#define UARTC0_TX_INTERRUPT_ENABLE SERIAL2_TX_INTERRUPT_ENABLE
		#endif
	
	#elif SERIAL3_PORT == UARTC0_PORT
	
		#ifdef SERIAL3_ENABLE
			#define UARTC0_ENABLE SERIAL3_ENABLE
		#else
			#define UARTC0_ENABLE 0
		#endif
		
		#ifdef SERIAL3_TX_DISABLE
			#define UARTC0_TX_DISABLE SERIAL3_TX_DISABLE
		#endif
		
		#ifdef SERIAL3_RX_DISABLE
			#define UARTC0_RX_DISABLE SERIAL3_TX_DISABLE
		#endif
		
		#ifdef SERIAL3_RX_BUFFER_CONF
			#define UARTC0_RX_BUFFER_CONF SERIAL3_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL3_TX_BUFFER_CONF
			#define UARTC0_TX_BUFFER_CONF SERIAL3_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL3_TX_INTERRUPT_ENABLE
			#define UARTC0_TX_INTERRUPT_ENABLE SERIAL3_TX_INTERRUPT_ENABLE
		#endif
	
	#elif SERIAL4_PORT == UARTC0_PORT
	
		#ifdef SERIAL4_ENABLE
			#define UARTC0_ENABLE SERIAL4_ENABLE
		#else
			#define UARTC0_ENABLE 0
		#endif
		
		#ifdef SERIAL4_TX_DISABLE
			#define UARTC0_TX_DISABLE SERIAL4_TX_DISABLE
		#endif
		
		#ifdef SERIAL4_RX_DISABLE
			#define UARTC0_RX_DISABLE SERIAL4_TX_DISABLE
		#endif
		
		#ifdef SERIAL4_RX_BUFFER_CONF
			#define UARTC0_RX_BUFFER_CONF SERIAL4_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL4_TX_BUFFER_CONF
			#define UARTC0_TX_BUFFER_CONF SERIAL4_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL4_TX_INTERRUPT_ENABLE
			#define UARTC0_TX_INTERRUPT_ENABLE SERIAL4_TX_INTERRUPT_ENABLE
		#endif
	
	#elif SERIAL5_PORT == UARTC0_PORT
	
		#ifdef SERIAL5_ENABLE
			#define UARTC0_ENABLE SERIAL5_ENABLE
		#else
			#define UARTC0_ENABLE 0
		#endif
		
		#ifdef SERIAL5_TX_DISABLE
			#define UARTC0_TX_DISABLE SERIAL5_TX_DISABLE
		#endif
		
		#ifdef SERIAL5_RX_DISABLE
			#define UARTC0_RX_DISABLE SERIAL5_TX_DISABLE
		#endif
		
		#ifdef SERIAL5_RX_BUFFER_CONF
			#define UARTC0_RX_BUFFER_CONF SERIAL5_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL5_TX_BUFFER_CONF
			#define UARTC0_TX_BUFFER_CONF SERIAL5_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL5_TX_INTERRUPT_ENABLE
			#define UARTC0_TX_INTERRUPT_ENABLE SERIAL5_TX_INTERRUPT_ENABLE
		#endif
	
	#elif SERIAL6_PORT == UARTC0_PORT
	
		#ifdef SERIAL6_ENABLE
			#define UARTC0_ENABLE SERIAL6_ENABLE
		#else
			#define UARTC0_ENABLE 0
		#endif
		
		#ifdef SERIAL6_TX_DISABLE
			#define UARTC0_TX_DISABLE SERIAL6_TX_DISABLE
		#endif
		
		#ifdef SERIAL6_RX_DISABLE
			#define UARTC0_RX_DISABLE SERIAL6_TX_DISABLE
		#endif
		
		#ifdef SERIAL6_RX_BUFFER_CONF
			#define UARTC0_RX_BUFFER_CONF SERIAL6_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL6_TX_BUFFER_CONF
			#define UARTC0_TX_BUFFER_CONF SERIAL6_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL6_TX_INTERRUPT_ENABLE
			#define UARTC0_TX_INTERRUPT_ENABLE SERIAL6_TX_INTERRUPT_ENABLE
		#endif
		
	#elif SERIAL7_PORT == UARTC0_PORT
	
		#ifdef SERIAL7_ENABLE
			#define UARTC0_ENABLE SERIAL7_ENABLE
		#else
			#define UARTC0_ENABLE 0
		#endif
		
		#ifdef SERIAL7_TX_DISABLE
			#define UARTC0_TX_DISABLE SERIAL7_TX_DISABLE
		#endif
		
		#ifdef SERIAL7_RX_DISABLE
			#define UARTC0_RX_DISABLE SERIAL7_TX_DISABLE
		#endif
		
		#ifdef SERIAL7_RX_BUFFER_CONF
			#define UARTC0_RX_BUFFER_CONF SERIAL7_RX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL7_TX_BUFFER_CONF
			#define UARTC0_TX_BUFFER_CONF SERIAL7_TX_BUFFER_CONF
		#endif
		
		#ifdef SERIAL7_TX_INTERRUPT_ENABLE
			#define UARTC0_TX_INTERRUPT_ENABLE SERIAL7_TX_INTERRUPT_ENABLE
		#endif
		
	#endif
	
	#if UARTC0_ENABLE == 1
	
		#include "../../../../os/drivers/Serial.h"
		#include "avr_xmega_uart_defines.h"
		
		#if SERIAL0_PORT == UARTC0_PORT
		
			#define Serial avr_xmega_uartc0_driver
			#define Serial0 Serial
			
		#elif SERIAL1_PORT == UARTC0_PORT
			
			#define Serial1 avr_xmega_uartc0_driver
			
		#elif SERIAL2_PORT == UARTC0_PORT
			
			#define Serial2 avr_xmega_uartc0_driver
			
		#elif SERIAL3_PORT == UARTC0_PORT
		
			#define Serial3 avr_xmega_uartc0_driver
		
		#elif SERIAL4_PORT == UARTC0_PORT
		
			#define Serial4 avr_xmega_uartc0_driver
		
		#elif SERIAL5_PORT == UARTC0_PORT
		
			#define Serial5 avr_xmega_uartc0_driver
		
		#elif SERIAL6_PORT == UARTC0_PORT
		
			#define Serial6 avr_xmega_uartc0_driver
		
		#elif SERIAL7_PORT == UARTC0_PORT
		
			#define Serial7 avr_xmega_uartc0_driver
		
		#endif
		
		extern const struct serial_driver avr_xmega_uartc0_driver;
		
	#endif
#endif

#endif

// Macros to configure UARTC0 are : RXC0_BUFFER_CONF, TXC0_BUFFER_CONF, UARTC0_TX_DISABLE, UARTC0_RX_DISABLE & UARTC0_TX_INTERRUPT_ENABLE

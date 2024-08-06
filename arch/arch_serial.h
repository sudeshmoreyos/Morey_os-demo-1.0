#ifndef ARCH_SERIAL_H
#define ARCH_SERIAL_H

#ifdef PLATFORM_SUPPORT_UART
	#if CPU == AVR		
		#if CPU_SERIES == MEGA		
			#ifdef PLATFORM_SUPPORT_CONST_PRINT
				#include "avr/pgmspace.h"		
				#define constPrint(x) 	constPrintArch(PSTR(x))
				#define constPrintln(x) constPrintlnArch(PSTR(x))
			#else
				#define constPrint(x) 	Print(x)
				#define constPrintln(x) Println(x)
			#endif
			
			#ifdef UART_AVAILABLE
				#include "AVR/MEGA/drivers/avr_mega_uart.h"
			#endif
			
			#ifdef UART0_AVAILABLE
				#include "AVR/MEGA/drivers/avr_mega_uart0.h"
			#endif
			
			#ifdef UART1_AVAILABLE				
				#include "AVR/MEGA/drivers/avr_mega_uart1.h"
			#endif
			
			#ifdef UART2_AVAILABLE
				#include "AVR/MEGA/drivers/avr_mega_uart2.h"
			#endif
			
			#ifdef UART3_AVAILABLE				
				#include "AVR/MEGA/drivers/avr_mega_uart3.h"
			#endif
			
		#elif CPU_SERIES == XMEGA
			#ifdef PLATFORM_SUPPORT_CONST_PRINT
				#include "avr/pgmspace.h"		
				#define constPrint(x) 	constPrintArch(PSTR(x))
				#define constPrintln(x) constPrintlnArch(PSTR(x))
			#else
				#define constPrint(x) 	Print(x)
				#define constPrintln(x) Println(x)
			#endif
			
			#ifdef UARTC0_AVAILABLE				
				#include "AVR/XMEGA/drivers/avr_xmega_uartc0.h"				
			#endif
			
			#ifdef UARTC1_AVAILABLE
				#include "AVR/XMEGA/drivers/avr_xmega_uartc1.h"
			#endif
			
			#ifdef UARTD0_AVAILABLE
				#include "AVR/XMEGA/drivers/avr_xmega_uartd0.h"				
			#endif
			
			#ifdef UARTD1_AVAILABLE				
				#include "AVR/XMEGA/drivers/avr_xmega_uartd1.h"				
			#endif
			
			#ifdef UARTE0_AVAILABLE
				#include "AVR/XMEGA/drivers/avr_xmega_uarte0.h"				
			#endif
			
			#ifdef UARTE1_AVAILABLE				
				#include "AVR/XMEGA/drivers/avr_xmega_uarte1.h"				
			#endif
			
			#ifdef UARTF0_AVAILABLE				
				#include "AVR/XMEGA/drivers/avr_xmega_uartf0.h"
			#endif
			#ifdef UARTF1_AVAILABLE
				#include "AVR/XMEGA/drivers/avr_xmega_uartf1.h"				
			#endif					
		#endif
	#endif
#else
	#error Platform do not support Serial UART
#endif
#endif
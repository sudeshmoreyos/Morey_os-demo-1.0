#ifndef ARCH_LIQUID_CRYSTAL_H
#define ARCH_LIQUID_CRYSTAL_H

#if CPU == AVR		
	#if CPU_SERIES == MEGA
		#ifdef PLATFORM_SUPPORT_CONST_PRINT
			#include "avr/pgmspace.h"	
			#define lcd_constPrint(A,B) lcd_constPrintArch(A,PSTR(B))
		#else
			#define lcd_constPrint(A,B) lcd_Print(A,B)
		#endif
	#elif CPU_SERIES == XMEGA
		#ifdef PLATFORM_SUPPORT_CONST_PRINT
			#include "avr/pgmspace.h"	
			#define lcd_constPrint(A,B) lcd_constPrintArch(A,PSTR(B))
		#else
			#define lcd_constPrint(A,B) lcd_Print(A,B)
		#endif
	#elif CPU_SERIES == MEGA0
		#ifdef PLATFORM_SUPPORT_CONST_PRINT
			#include "avr/pgmspace.h"	
			#define lcd_constPrint(A,B) lcd_constPrintArch(A,PSTR(B))
		#else
			#define lcd_constPrint(A,B) lcd_Print(A,B)
		#endif
	#endif
#endif

#endif
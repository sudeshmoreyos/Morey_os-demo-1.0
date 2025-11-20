#ifndef INCLUDE_CONTROLLER_H
#define INCLUDE_CONTROLLER_H

#include "controller_list.h"

#if CPU == AVR
	#if CPU_SERIES == MEGA
	
		#if CONTROLLER == MEGA8
			#include "AVR/MEGA/boot/MEGA8/boot.h"
		#endif // CONTROLLER == MEGA8
		
		#if CONTROLLER == MEGA328P
			#include "AVR/MEGA/boot/MEGA328P/boot.h"
		#endif // CONTROLLER == MEGA328P
		
		#if CONTROLLER == MEGA328PB
			#include "AVR/MEGA/boot/MEGA328PB/boot.h"
		#endif // CONTROLLER == MEGA328PB
		
		#if CONTROLLER == MEGA2560
			#include "AVR/MEGA/boot/MEGA2560/boot.h"
		#endif // CONTROLLER == MEGA2560
		
		#if CONTROLLER == MEGA16
			#include "AVR/MEGA/boot/MEGA16/boot.h"
		#endif // CONTROLLER == MEGA16
		
		#if CONTROLLER == MEGA128
			#include "AVR/MEGA/boot/MEGA128/boot.h"
		#endif // CONTROLLER == MEGA128
		
	#elif CPU_SERIES == XMEGA
	
		#if CONTROLLER == XMEGA256
			#include "AVR/XMEGA/boot/XMEGA256/boot.h"
		#endif // CONTROLLER == XMEGA256
		
	#elif CPU_SERIES == MEGA0
	
		#if CONTROLLER == MEGA4809
			#include "AVR/MEGA0/boot/MEGA4809/boot.h"
		#endif // CONTROLLER == MEGA4809
	
	#endif // CPU_SERIES == MEGA
	
#endif	// CPU == AVR

#endif 	// INCLUDE_CONTROLLER_H

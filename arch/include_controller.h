#ifndef INCLUDE_CONTROLLER_H
#define INCLUDE_CONTROLLER_H

#include "controller_list.h"

#if CPU == AVR
	#if CPU_SERIES == MEGA
		#if CONTROLLER == MEGA8
			#include "AVR/MEGA/boot/MEGA8/boot.h"
		#endif
		
		#if CONTROLLER == MEGA328P
			#include "AVR/MEGA/boot/MEGA328P/boot.h"
		#endif
		
		#if CONTROLLER == MEGA328PB
			#include "AVR/MEGA/boot/MEGA328PB/boot.h"
		#endif
		
		#if CONTROLLER == MEGA2560
			#include "AVR/MEGA/boot/MEGA2560/boot.h"
		#endif
		#if CONTROLLER == MEGA16
			#include "AVR/MEGA/boot/MEGA16/boot.h"
		#endif
	#elif CPU_SERIES == XMEGA
		#if CONTROLLER == XMEGA256
			#include "AVR/XMEGA/boot/XMEGA256/boot.h"
		#endif
	#endif
#endif


#endif

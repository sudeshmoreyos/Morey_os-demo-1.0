#ifndef INCLUDE_CONTROLLER_H
#define INCLUDE_CONTROLLER_H

#include "controller_list.h"

#if CPU == AVR
	#if CPU_SERIES == MEGA
		#if CONTROLLER == MEGA8
			#include "AVR/MEGA/boot/MEGA8/boot.h"
		#endif
		
		#if CONTROLLER == MEGA328
			#include "AVR/MEGA/boot/MEGA328/boot.h"
		#endif
	#elif CPU_SERIES == XMEGA
		#if CONTROLLER == XMEGA256
			#include "AVR/XMEGA/boot/XMEGA256/boot.h"
		#endif
	#endif
#endif


#endif

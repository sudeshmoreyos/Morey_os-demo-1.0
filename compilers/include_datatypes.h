#ifndef INCLUDE_DATATYPE_H
#define INCLUDE_DATATYPE_H

#include "compiler_list.h"

#if ( (COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR) || (COMPILER == AVR_GCC ) || (COMPILER == CODEVISION_AVR ))
	#define NULL (void*)(0x00)
	typedef unsigned char mos_uint8_t;
	typedef unsigned short int mos_uint16_t;
	typedef unsigned long int mos_uint32_t ;
	typedef signed char mos_int8_t;
	typedef signed short int mos_int16_t;
	typedef signed long int mos_int32_t;
#endif

#endif //INCLUDE_DATATYPE_H
#ifndef INCLUDE_BOARD_H
#define INCLUDE_BOARD_H

#include "board_list.h"

#if BOARD == ARDUINO_UNO
	#include "ARDUINO/UNO/board_boot.h"
#elif BOARD == ARDUINO_MEGA
	#include "ARDUINO/MEGA/board_boot.h"
#endif

#endif

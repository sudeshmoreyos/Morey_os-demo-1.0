#ifndef AVR_MEGA_ANALOGINPUT_H
#define AVR_MEGA_ANALOGINPUT_H

#include "../../../../platform.h"
#include "../../../../os/drivers/AnalogInput.h"

#ifdef PLATFORM_SUPPORT_ANALOGINPUT
	
	#define AI_REF_AREF 0
	#define AI_REF_AVCC 1
	
	// internal ref 1.1 & 2.56 are platform dependent hence declared in controller_macros.h file
	
	#if CONTROLLER_FREQ == 20000000UL

		#define AI_SPEED_MODE0 5	// Divide by 32 = 625 KHz
		#define AI_SPEED_MODE1 6	// Divide by 64 = 312.5 KHz
		#define AI_SPEED_MODE2 7	// Divide by 128 = 156.75 KHz
		
		#ifdef ANALOGINPUT_HELP_ENABLE
			#warning Selected controller frequency = 16Mhz, AI conversion speed for different modes are : \
			AI_SPEED_MODE0 = 625 KHz, AI_SPEED_MODE1 = 312.5 KHz, AI_SPEED_MODE2 = 156.75 KHz
		#endif

	#elif CONTROLLER_FREQ == 16000000UL

		#define AI_SPEED_MODE0 4	// Divide by 16 = 1000 KHz
		#define AI_SPEED_MODE1 5	// Divide by 32 = 500 KHz
		#define AI_SPEED_MODE2 6	// Divide by 64 = 250 KHz
		#define AI_SPEED_MODE3 7	// Divide by 128 = 125 KHz
		
		#ifdef ANALOGINPUT_HELP_ENABLE
			#warning Selected controller frequency = 16Mhz, AI conversion speed for different modes are : \
			AI_SPEED_MODE0 = 1000 KHz, AI_SPEED_MODE1 = 500 KHz, AI_SPEED_MODE2 = 250 KHz, AI_SPEED_MODE3 = 125 KHz
		#endif
		
	#elif CONTROLLER_FREQ == 12000000UL

		#define AI_SPEED_MODE0 4	// Divide by 16 = 750 KHz
		#define AI_SPEED_MODE1 5	// Divide by 32 = 375 KHz
		#define AI_SPEED_MODE2 6	// Divide by 64 = 187.5 KHz
		#define AI_SPEED_MODE3 7	// Divide by 128 = 93.75 KHz
		
		#ifdef ANALOGINPUT_HELP_ENABLE
			#warning Selected controller frequency = 12Mhz, AI conversion speed for different modes are: \
			AI_SPEED_MODE0 = 750 KHz, AI_SPEED_MODE1 = 375 KHz, AI_SPEED_MODE2 = 187.5 KHz, AI_SPEED_MODE3 = 93.75 KHz
		#endif
		
	#elif CONTROLLER_FREQ == 8000000UL
		#define AI_SPEED_MODE0 3 	// Divide by 8 = 1000 KHz
		#define AI_SPEED_MODE1 4	// Divide by 16 = 500 KHz
		#define AI_SPEED_MODE2 5	// Divide by 32 = 250 KHz
		#define AI_SPEED_MODE3 6 	// Divide by 64 = 125 KHz
		#define AI_SPEED_MODE4 7	// Divide by 128 = 62.5 KHz
		
		#ifdef ANALOGINPUT_HELP_ENABLE
			#warning Selected controller frequency = 8Mhz, AI conversion speed for different modes are: \
			AI_SPEED_MODE0 = 1000 KHz, AI_SPEED_MODE1 = 500 KHz, AI_SPEED_MODE2 = 250 KHz \
			AI_SPEED_MODE3  = 125 KHz, AI_SPEED_MODE4  = 62.5 KHz
		#endif
		
	#elif CONTROLLER_FREQ == 1000000UL
		#define AI_SPEED_MODE0 1 	// Divide by 2 = 500 KHz
		#define AI_SPEED_MODE1 2	// Divide by 4 = 250 KHz
		#define AI_SPEED_MODE2 3 	// Divide by 8 = 125 KHz
		#define AI_SPEED_MODE3 4	// Divide by 16 = 62.5 KHz
		#define AI_SPEED_MODE4 5	// Divide by 32 = 31.25 KHz
		#define AI_SPEED_MODE5 6 	// Divide by 64 = 15.625 KHz
		#define AI_SPEED_MODE6 7	// Divide by 128 = 7.813 KHz
		
		#ifdef ANALOGINPUT_HELP_ENABLE
			#warning Selected controller frequency = 1Mhz, AI conversion speed for different modes are: \
			AI_SPEED_MODE0 = 500 KHz, AI_SPEED_MODE1 = 250 KHz, AI_SPEED_MODE2 = 125 KHz, AI_SPEED_MODE3 = 62.5 KHz \
			AI_SPEED_MODE4  = 31.25 KHz, AI_SPEED_MODE5  = 15.625 KHz, AI_SPEED_MODE6  = 7.813 KHz		
		#endif
	#endif	
	
	#define AI_CHANNEL0 0
	#define AI_CHANNEL1 1
	#define AI_CHANNEL2 2
	#define AI_CHANNEL3 3
	#define AI_CHANNEL4 4
	#define AI_CHANNEL5 5
	#define AI_CHANNEL6 6
	#define AI_CHANNEL7 7
	// AnalogInput channel more than 7 are platform dependent hence declared in controller_macros.h file
	
	#define AnalogInput avr_mega_analog_input_driver
	extern const struct analog_input_driver avr_mega_analog_input_driver;

#endif


#endif

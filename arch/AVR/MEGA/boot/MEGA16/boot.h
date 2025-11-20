#ifndef BOOT_H
#define BOOT_H

/** \addtogroup headers_MEGA16
* @{ \defgroup boot_MEGA16_h
* @{
* \brief Documentaion of boot.h header file for atmega16 controller    
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This header file implements atmega16 boot functions
*
* **USER DEFINED MACROS** that can be used in boot.h (These macros should be declared in config.h file) are :
*1. **MEGA16_DEBUG_LEVEL_CONF** \n
*=> Allowed values : {MEGA16_DEBUG_NONE, MEGA16_DEBUG_USER, MEGA16_DEBUG_LEVEL1, MEGA16_DEBUG_LEVEL2, MEGA16_DEBUG_LEVEL3} \n
*=> Eg. Declaration in config.h : #define MEGA16_DEBUG_LEVEL_CONF MEGA16_DEBUG_LEVEL1 \n
*=> Default value : MEGA16_DEBUG_NONE \n
*=> Usage Description : This macro can be defined to diagnose different execution steps of boot.h or boot.c for atmega16 controller,
*with increasing levels (i.e. level 1 2 3) deeper execution steps can be diagnosed. MEGA16_DEBUG_USER level enables MEGA16_DEBUG_MSG_USER Macro.
*To use this, user should manually write MEGA16_DEBUG_MSG_USER(msg) inside code sections of boot.h or boot.c to be diagnosed. \n
*\n
*2. **FREQ** \n
*=> Allowed values : {1000000, 2000000, 4000000, 8000000, 12000000, 16000000} \n
*=> Eg. Declaration in Makefile : FREQ = 1000000 \n
*=> Default value : 16000000 \n
*=> Usage Description : For AtMega16 Controller MOREY_OS supports only six frequencies i.e. 1000000 (1 Mhz), 2000000 (2 Mhz), 4000000 (4 Mhz),
* 8000000 (8 Mhz), 12000000 (12 Mhz) & 16000000 (16 Mhz). If user do not define FREQ macro in Makefile, by default 16000000
* frequency is selected. Please note except for OS_TIMER_TYPE = TIMER_1, for 12 MHz setting, timer module generates inaccurate timings. 
* Hence it is recommended either not to use 12 Mhz frequency, or use 12 MHz with OS_TIMER_TYPE = TIMER_1.\n
*\n
*3. **OS_TIMER_TYPE_CONF** \n
*=> Allowed values : {TIMER_0, TIMER_1, TIMER_2} \n
*=> Eg. Declaration in config.h : #define OS_TIMER_TYPE_CONF TIMER_1 \n
*=> Default value : TIMER_1 \n
*=> Usage Description : For mega16 Controller MOREY_OS supports only three values i.e. TIMER_0, TIMER_1, TIMER_2. 
* OS_TIMER_TYPE = TIMER_0 uses timer0 (8 bit), OS_TIMER_TYPE = TIMER_2 uses timer2 (8 bit) and OS_TIMER_TYPE = TIMER_1 uses timer1 (16 bit) of atMega16 controller. 
* If user do not define OS_TIMER_TYPE_CONF in config.h header, by default OS_TIMER_TYPE = TIMER_1 ( timer1 ) is selected. For Mega16 controller Morey_OS works most
* efficiently when OS_TIMER_TYPE_CONF is set to TIMER_1, which is also default setting.
* @}
*/

// include platform.h header file to include platform specific macros and data types
#include "../../../../../platform.h"

// If user has defined controller frequency using FREQ macro in Makefile
// Then set CONTROLLER_FREQ = FREQ, else take 16000000 ( 16 Mhz ) as default frequency
// Please note user defines FREQ macro in Makefile, while value of FREQ macro is
// automatically assigned to CONTROLLER_FREQ_CONF during compilation process
#ifdef CONTROLLER_FREQ_CONF 
	#define CONTROLLER_FREQ CONTROLLER_FREQ_CONF
#else    
	#define CONTROLLER_FREQ 16000000UL
#endif

// Include io.h header file based on COMPILER selected by user
// If user selected unsupported COMPILER error is generated
#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
	#include <avr/io.h>
#else
	#error Incorrect COMPILER, Supported compilers for atMega16 are AVR_STUDIO, WIN_AVR, AVR_GCC and CODEVISION_AVR
#endif

// Import controller specific macros by including controller_macros.h header file
#include "controller_macros.h"

// Declare Global Interrupt and atomic macros
#define GLOBAL_INTERRUPT_ENABLE() 	global_interrupt_enable()
#define GLOBAL_INTERRUPT_DISABLE()	global_interrupt_disable()

#define ATOMIC_ON() 			GLOBAL_INTERRUPT_DISABLE()
#define ATOMIC_OFF() 			GLOBAL_INTERRUPT_ENABLE()

#define IS_GLOBAL_INTERRUPT_ENABLE() 	is_global_interrupt_enable()
#define IS_GLOBAL_INTERRUPT_DISABLE() 	is_global_interrupt_disable()

#define IS_ATOMIC_ON()			IS_GLOBAL_INTERRUPT_DISABLE()
#define IS_ATOMIC_OFF()			IS_GLOBAL_INTERRUPT_ENABLE()

/** 
* \brief	Declare function to enable global interrupt. 
* This function can be used to do atomic operations 
*/
void global_interrupt_enable(void);

/** 
* \brief	Declare function to disable global interrupt. 
* This function must be called after completing atomic operations
*/
void global_interrupt_disable(void);

/** 
* \brief	Declare function to check if global interrupt is enabled
*/
mos_uint8_t is_global_interrupt_enable(void);

/** 
* \brief	Declare function to check if global interrupt is disabled
*/
mos_uint8_t is_global_interrupt_disable(void);


#ifndef DISABLE_OS

// These are important data types required by OS
typedef mos_uint16_t program_address_t;
typedef mos_uint32_t clock_second_t;
typedef mos_uint16_t clock_millisecond_t;

/**
* \name MACROS : Mega16 Boot Debug Levels
* \brief Define different levels for mega16 boot debugging
* @{
*/
#define MEGA16_DEBUG_NONE 0
#define MEGA16_DEBUG_USER 1
#define MEGA16_DEBUG_LEVEL1 2
#define MEGA16_DEBUG_LEVEL2 3
#define MEGA16_DEBUG_LEVEL3 4 
///@}

#if OS_TIMER_TYPE == TIMER_0

	// Please note 12 MHz crystal generates inaccurate timings
	// Hence it is recommended not to use 12 MHz frequency when OS_TIMER_TYPE = TIMER_0  
	
	// This macro informs different files that platform supports tickless timer with TIMER_0
	#define PLATFORM_SUPPORT_TICKLESS
	// This macros defines timer interrupt ticking sub-routine
	#define TIMER_INTERRUPT_TICKING	 	TIMER0_OVF_vect
	// This macros defines timer interrupt tickless sub-routine
	#define TIMER_INTERRUPT_TICKLESS 	TIMER0_COMP_vect
	// This macro defines timer counter register for TIMER_0
	#define TIMER_COUNTER_REGISTER 		TCNT0
	// This macro defines compare match register for TIMER_0
	#define TIMER_COMPARE_REGISTER		OCR0
	
	// Max value of TIMER_COMPARE_REGISTER
	#define TIMER_COMPARE_MAX_VALUE 	0xFF
	
	// If controller frequency is 16 MHZ then
	#if CONTROLLER_FREQ == 16000000UL
		
		// Start value of TIMER_COUNTER on reset
		#define TIMER_COUNTER_START_VALUE		0x06
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		16
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	16                 
                        
	// If controller frequency is 12 MHZ then
	#elif CONTROLLER_FREQ == 12000000UL
	
		// Start value of TIMER_COUNTER on reset
		#define TIMER_COUNTER_START_VALUE		0x16
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		20
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	12
    
	// If controller frequency is 8 MHZ then
	#elif CONTROLLER_FREQ == 8000000UL
	
		// Start value of TIMER_COUNTER on reset
		#define TIMER_COUNTER_START_VALUE		0x06
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		32
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	8  
	
	// If controller frequency is 4 MHZ then
	#elif CONTROLLER_FREQ == 4000000UL
	
		// Start value of TIMER_COUNTER on reset
		#define TIMER_COUNTER_START_VALUE		0x06
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		16
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	16
	
	// If controller frequency is 2 MHZ then
	#elif CONTROLLER_FREQ == 2000000UL
	
		// Start value of TIMER_COUNTER on reset
		#define TIMER_COUNTER_START_VALUE		0x06
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		32
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	8	

	// If controller frequency is 1 MHZ then
	#elif CONTROLLER_FREQ == 1000000UL
	
		// Start value of TIMER_COUNTER on reset
		#define TIMER_COUNTER_START_VALUE		0x06
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		20
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	12
	
	#else 
		#error Incorrect CONTROLLER_FREQ : For mega16 Controller MOREY_OS supports only Six frequencies i.e. 1000000, 2000000, 4000000, 8000000, 12000000 & 16000000 Hz
	#endif
	
#elif OS_TIMER_TYPE == TIMER_1 
	
	// This macro informs different files that platform supports tickless timer with TIMER_1
	#define PLATFORM_SUPPORT_TICKLESS
	// This macros defines timer interrupt ticking sub-routine
	#define TIMER_INTERRUPT_TICKING	 	TIMER1_COMPA_vect
	// This macros defines timer interrupt tickless sub-routine
	#define TIMER_INTERRUPT_TICKLESS 	TIMER1_COMPB_vect
	// This macro defines timer counter register for TIMER_1
	#define TIMER_COUNTER_REGISTER 		TCNT1
	// This macro defines compare match register for TIMER_1
	#define TIMER_COMPARE_REGISTER		OCR1B
	
	// Start value of TIMER_COUNTER on reset
	#define TIMER_COUNTER_START_VALUE 	0x0000
	
	// Max value of TIMER_COMPARE_REGISTER
	#define TIMER_COMPARE_MAX_VALUE 	0xFFFF
	
	// If controller frequency is 16 MHZ then
	#if CONTROLLER_FREQ == 16000000UL
	
		// Max value of compare match register can be
		#define TICKING_TIMER_MAX_TICK 			62500
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		250
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	250                 
                        
	// If controller frequency is 12 MHZ then
	#elif CONTROLLER_FREQ == 12000000UL
	
		// Max value of compare match register can be
		#define TICKING_TIMER_MAX_TICK 			46875
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		250
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	188
    
	// If controller frequency is 8 MHZ then
	#elif CONTROLLER_FREQ == 8000000UL
	
		// Max value of compare match register can be
		#define TICKING_TIMER_MAX_TICK 			62500
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		500
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	125   
	
	// If controller frequency is 4 MHZ then
	#elif CONTROLLER_FREQ == 4000000UL
	
		// Max value of compare match register can be
		#define TICKING_TIMER_MAX_TICK 			62500
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		125
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	500 
	
	// If controller frequency is 2 MHZ then
	#elif CONTROLLER_FREQ == 2000000UL
	
		// Max value of compare match register can be
		#define TICKING_TIMER_MAX_TICK 			62500
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		250
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	250 	

	// If controller frequency is 1 MHZ then
	#elif CONTROLLER_FREQ == 1000000UL
	
		// Max value of compare match register can be
		#define TICKING_TIMER_MAX_TICK 			62500
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		500
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	125
	
	#else 
		#error Incorrect CONTROLLER_FREQ : For mega16 Controller MOREY_OS supports only Six frequencies i.e. 1000000, 2000000, 4000000, 8000000, 12000000 & 16000000 Hz
	#endif
	 
    
#elif OS_TIMER_TYPE == TIMER_2

	// Please note 12 MHz crystal generates inaccurate timings
	// Hence it is recommended not to use 12 MHz frequency when OS_TIMER_TYPE = TIMER_2 
	
	// This macro informs different files that platform supports tickless timer with TIMER_2
	#define PLATFORM_SUPPORT_TICKLESS
	// This macros defines timer interrupt ticking sub-routine
	#define TIMER_INTERRUPT_TICKING	 	TIMER2_OVF_vect
	// This macros defines timer interrupt tickless sub-routine
	#define TIMER_INTERRUPT_TICKLESS 	TIMER2_COMP_vect
	// This macro defines timer counter register for TIMER_2
	#define TIMER_COUNTER_REGISTER 		TCNT2
	// This macro defines compare match register for TIMER_2
	#define TIMER_COMPARE_REGISTER		OCR2
	
	// Max value of TIMER_COMPARE_REGISTER
	#define TIMER_COMPARE_MAX_VALUE 	0xFF
	
	// If controller frequency is 16 MHZ then
	#if CONTROLLER_FREQ == 16000000UL
	
		// Start value of TIMER_COUNTER on reset
		#define TIMER_COUNTER_START_VALUE 	0x06
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		16
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	16                 
                        
	// If controller frequency is 12 MHZ then
	#elif CONTROLLER_FREQ == 12000000UL
	
		// Start value of TIMER_COUNTER on reset
		#define TIMER_COUNTER_START_VALUE 		0x16
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		20
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	12
    
	// If controller frequency is 8 MHZ then
	#elif CONTROLLER_FREQ == 8000000UL
	
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		8
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	32   
	
	// If controller frequency is 4 MHZ then
	#elif CONTROLLER_FREQ == 4000000UL
	
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		16
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	16 
	
	// If controller frequency is 2 MHZ then
	#elif CONTROLLER_FREQ == 2000000UL
	
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		16
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	16 	

	// If controller frequency is 1 MHZ then
	#elif CONTROLLER_FREQ == 1000000UL
	
		// Max milli-seconds passes until Timer counter 
		// resets after reaching peak value
		#define CLOCK_MILLISECOND_MAX_CONSTANT 		16
		// Number of Ticks for each milli seconds ( may be approximated )
		#define CLOCK_MILLISECOND_TO_TICK_CONSTANT 	16
	
	#else 
		#error Incorrect CONTROLLER_FREQ : For mega16 Controller MOREY_OS supports only Six frequencies i.e. 1000000, 2000000, 4000000, 8000000, 12000000 & 16000000 Hz
	#endif
    
#else 
	#error Incorrect OS_TIMER_TYPE : For mega16 Controller MOREY_OS supports only three values i.e. TIMER_0, TIMER_1, TIMER_2, or {1, 2, 3}

#endif

#define PLATFORM_INIT() 	platform_init()
#define WATCHDOG_PERIODIC() 	watchdog_periodic()
#define OS_SLEEP() 		sleep()

/** 
* \brief	Declare function to init essential peripherals required by OS
*
* This function initiates essential platform peripherals, which includes
* initiating timer required by OS clock, Watchdog timer to reset OS if
* it enters an infinite loop and sleep functions to put OS in sleep mode
* when no task is running to save power.
*/
void platform_init(void);

/** 
* \brief	Declare function to periodically reset watchdog timer 
*/
void watchdog_periodic(void);

/** 
* \brief	Declare function to go to sleep or idle mode to save power
*/
void sleep(void);


/** 
* \brief	Declare function to initiat ticking interrupt
*/
void init_ticking_interrupt(void);

#ifdef PLATFORM_SUPPORT_TICKLESS

/** 
* \brief	Declare function to enable tickless interrupt for required milliseconds settings
*/
void enable_tickless_call(clock_millisecond_t milliseconds_next);

/** 
* \brief	Declare function to disable tickless interrupt
*/
void disable_tickless_call(void);

/** 
* \brief	Declare function to read running milliseconds of timer
*/
clock_millisecond_t read_timer_millisecond(mos_uint8_t * half);

#endif

#endif

#endif

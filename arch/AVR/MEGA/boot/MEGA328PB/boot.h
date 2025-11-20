#ifndef BOOT_H
#define BOOT_H

/** \addtogroup headers_MEGA328PB
* @{ \defgroup boot_MEGA328PB_h
* @{
* \brief Documentaion of boot.h header file for atmega328pb controller    
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This header file implements atmega328pb boot functions
*
* **USER DEFINED MACROS** that can be used in boot.h (These macros should be declared in config.h file) are :
*1. **MEGA328PB_DEBUG_LEVEL_CONF** \n
*=> Allowed values : {MEGA328PB_DEBUG_NONE, MEGA328PB_DEBUG_USER, MEGA328PB_DEBUG_LEVEL1, MEGA328PB_DEBUG_LEVEL2, MEGA328PB_DEBUG_LEVEL3} \n
*=> Eg. Declaration : #define MEGA328PB_DEBUG_LEVEL_CONF MEGA328PB_DEBUG_LEVEL1 \n
*=> Default value : MEGA328PB_DEBUG_NONE \n
*=> Usage Description : This macro can be defined to diagnose different execution steps of boot.h or boot.c for atmega328pb controller,
*with increasing levels (i.e. level 1 2 3) deeper execution steps can be diagnosed. MEGA328PB_DEBUG_USER level enables MEGA328PB_DEBUG_MSG_USER Macro.
*To use this, user should manually write MEGA328PB_DEBUG_MSG_USER(msg) inside code sections of boot.h or boot.c to be diagnosed. \n
*\n
*2. **FREQ** \n
*=> Allowed values : {1000000, 8000000, 12000000, 16000000,20000000} \n
*=> Eg. Declaration in Makefile : FREQ = 1000000 \n
*=> Default value : 16000000 \n
*=> Usage Description : For Atmega328pb Controller MOREY_OS supports only five frequencies i.e. 1000000 (1 Mhz), 8000000 (8 Mhz),
* 12000000 (12 Mhz), 16000000 (16 Mhz) & 20000000 (20 Mhz). If user do not define FREQ macro in Makefile, by default 16000000
* frequency is selected. Please note except for OS_TIMER_TYPE = TIMER_1,TIMER_3 or TIMER_4 for 12 MHz setting, timer module generates inaccurate timings. 
* Hence it is recommended either not to use 12 Mhz frequency, or use 12 MHz with OS_TIMER_TYPE = TIMER_1,TIMER_3 or TIMER_4.\n
*\n
*3. **OS_TIMER_TYPE_CONF** \n
*=> Allowed values : {TIMER_0, TIMER_1, TIMER_2, TIMER_3, TIMER_4} \n
*=> Eg. Declaration in config.h : #define OS_TIMER_TYPE_CONF TIMER_4 \n
*=> Default value : TIMER_4 \n
*=> Usage Description : For mega328pb Controller MOREY_OS supports only five values i.e. TIMER_0, TIMER_1, TIMER_2, TIMER_3, TIMER_4. 
* OS_TIMER_TYPE = TIMER_0 uses timer0 (8 bit), OS_TIMER_TYPE = TIMER_2 uses timer2 (8 bit), OS_TIMER_TYPE = TIMER_1 uses timer1 (16 bit) 
* OS_TIMER_TYPE = TIMER_3 uses timer3 (16 bit) and OS_TIMER_TYPE = TIMER_4 uses timer4 (16 bit) of atmega328pb controller.
* If user do not define OS_TIMER_TYPE_CONF in config.h header, by default OS_TIMER_TYPE = TIMER_4 ( timer4 ) is selected.
* For Mega328PB controller Morey_OS works most efficiently when OS_TIMER_TYPE_CONF is set to TIMER_1, TIMER_3 and TIMER_4. 
* Timer_4 is the default setting.
* @}
*/

#include "../../../../../platform.h"

#ifdef CONTROLLER_FREQ_CONF 
    #define CONTROLLER_FREQ CONTROLLER_FREQ_CONF
#else    
    #define CONTROLLER_FREQ 16000000UL
#endif

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
	#include <avr/io.h>
#else
	#error Incorrect COMPILER, Supported compilers for atmega328pb are AVR_STUDIO, WIN_AVR and AVR_GCC
#endif

#include "controller_macros.h"

#define GLOBAL_INTERRUPT_ENABLE() 		global_interrupt_enable()
#define GLOBAL_INTERRUPT_DISABLE() 		global_interrupt_disable()

#define ATOMIC_ON() 					GLOBAL_INTERRUPT_DISABLE()
#define ATOMIC_OFF() 					GLOBAL_INTERRUPT_ENABLE()

#define IS_GLOBAL_INTERRUPT_ENABLE() 	is_global_interrupt_enable()
#define IS_GLOBAL_INTERRUPT_DISABLE() 	is_global_interrupt_disable()

#define IS_ATOMIC_ON()					IS_GLOBAL_INTERRUPT_DISABLE()
#define IS_ATOMIC_OFF()					IS_GLOBAL_INTERRUPT_ENABLE()

/** 
* \brief	Declare function to disable global interrupt. 
* This function can be used to do atomic operations 
*/
void global_interrupt_enable(void);

/** 
* \brief	Declare function to disable global interrupt. 
* This function must be called after completing atomic operations
*/
void global_interrupt_disable(void);


/** 
* \brief	Declare function to check if global interrupt is enabled.
* This function must be called to check if Global interrupt is already enabled or not.
*/
mos_uint8_t is_global_interrupt_enable(void);


#ifndef DISABLE_OS

typedef mos_uint16_t program_address_t;
typedef mos_uint32_t clock_second_t;
typedef mos_uint16_t clock_millisecond_t;

/**
* \name MACROS : Mega328pb Boot Debug Levels
* \brief Define different levels for Mega328pb boot debugging
* @{
*/
#define MEGA328PB_DEBUG_NONE 	0
#define MEGA328PB_DEBUG_USER 	1
#define MEGA328PB_DEBUG_LEVEL1 	2
#define MEGA328PB_DEBUG_LEVEL2 	3
#define MEGA328PB_DEBUG_LEVEL3 	4 
///@}

#if OS_TIMER_TYPE == TIMER_0

    // For OS_TIMER_TYPE == TIMER_0 timer0 is used, max accuracy achieved is 2 milli second
	// except for 8 MHz crystal which achieves max accuracy of 4 milli second
    // & except for 1 MHz crystal which achieves max accuracy of 8 milli second
    // Please note 12 MHz and 20 Mhz crystal generates inaccurate timings
    // Hence it is recommended not to use 12 MHz and 20 Mhz frequency when OS_TIMER_TYPE = TIMER_0 
    
	#define PLATFORM_SUPPORT_TICKLESS
	
	#define OS_TICKING_INTERRUPT			TIMER0_COMPA_vect
	#define OS_TICKLESS_INTERRUPT			TIMER0_COMPB_vect
	#define OS_TICKING_TIMER_COUNTER		TCNT0
	#define OS_TICKLESS_TIMER_REGISTER		OCR0B
	#define TIMER_COUNTER_START_VALUE		0x00
		 
    #if CONTROLLER_FREQ == 20000000UL
	
	#define COMPARE_MATCH_MAX_TICK 254
        #define CLOCK_MILLISECOND_MAX_CONSTANT 13
        #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 20
        // In some Timer settings, CLOCK_MILLISECOND_TO_TICK_CONSTANT has decimal values
        // TO keep integer calculations only, nearest higher value is used 
	
	#elif CONTROLLER_FREQ == 16000000UL

		#define COMPARE_MATCH_MAX_TICK 250
        #define CLOCK_MILLISECOND_MAX_CONSTANT 4
        #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 63
        // In some Timer settings, CLOCK_MILLISECOND_TO_TICK_CONSTANT has decimal values
        // TO keep integer calculations only, nearest higher value is used 
                        
    #elif CONTROLLER_FREQ == 12000000UL
    
		#define COMPARE_MATCH_MAX_TICK 188
        #define CLOCK_MILLISECOND_MAX_CONSTANT 4
        #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 47
        // In some Timer settings, CLOCK_MILLISECOND_TO_TICK_CONSTANT has decimal values
        // TO keep integer calculations only, nearest higher value is used
    
    #elif CONTROLLER_FREQ == 8000000UL
    
		#define COMPARE_MATCH_MAX_TICK 250
        #define CLOCK_MILLISECOND_MAX_CONSTANT 8
        #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 32
		// In some Timer settings, CLOCK_MILLISECOND_TO_TICK_CONSTANT has decimal values
        // TO keep integer calculations only, nearest higher value is used

    #elif CONTROLLER_FREQ == 1000000UL
    
		#define COMPARE_MATCH_MAX_TICK 250
        #define CLOCK_MILLISECOND_MAX_CONSTANT 16
        #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 16
		// In some Timer settings, CLOCK_MILLISECOND_TO_TICK_CONSTANT has decimal values
        // TO keep integer calculations only, nearest higher value is used
	
	#else 
		#error Incorrect CONTROLLER_FREQ : For mega328pb Controller MOREY_OS supports only five frequencies i.e. 1000000UL, 8000000UL, 12000000UL, 16000000UL & 20000000UL
    #endif
    
#elif OS_TIMER_TYPE == TIMER_2
    
     // For OS_TIMER_TYPE == TIMER_2 timer2 is used, max accuracy achieved is 2 milli second
     // except for 1 MHz crystal which achieves max accuracy of 4 milli second
     // Please note 12 MHz crystal generates inaccurate timings
     // Hence it is recommended not to use 12 MHz crystal
     
     #define PLATFORM_SUPPORT_TICKLESS
	 
	 #define OS_TICKING_INTERRUPT			TIMER2_COMPA_vect
	 #define OS_TICKLESS_INTERRUPT			TIMER2_COMPB_vect
	 #define OS_TICKING_TIMER_COUNTER		TCNT2
	 #define OS_TICKLESS_TIMER_REGISTER		OCR2B
	 #define TIMER_COUNTER_START_VALUE		0x00
	 
	 #if CONTROLLER_FREQ == 20000000UL
	
		#define COMPARE_MATCH_MAX_TICK 254
        #define CLOCK_MILLISECOND_MAX_CONSTANT 13
        #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 20
        // In some Timer settings, CLOCK_MILLISECOND_TO_TICK_CONSTANT has decimal values
        // TO keep integer calculations only, nearest higher value is used 
	 
     #elif CONTROLLER_FREQ == 16000000UL
		
		#define COMPARE_MATCH_MAX_TICK 250
        #define CLOCK_MILLISECOND_MAX_CONSTANT 4
        #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 63
        // In some Timer settings, CLOCK_MILLISECOND_TO_TICK_CONSTANT has decimal values
        // TO keep integer calculations only, nearest higher value is used 
                        
    #elif CONTROLLER_FREQ == 12000000UL
    
        #define COMPARE_MATCH_MAX_TICK 188
        #define CLOCK_MILLISECOND_MAX_CONSTANT 4
        #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 47
		// In some Timer settings, CLOCK_MILLISECOND_TO_TICK_CONSTANT has decimal values
        // TO keep integer calculations only, nearest higher value is used 
    
    #elif CONTROLLER_FREQ == 8000000UL
    
		#define COMPARE_MATCH_MAX_TICK 250
        #define CLOCK_MILLISECOND_MAX_CONSTANT 4
        #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 63
        // In some Timer settings, CLOCK_MILLISECOND_TO_TICK_CONSTANT has decimal values
        // TO keep integer calculations only, nearest higher value is used 
        
    #elif CONTROLLER_FREQ == 1000000UL
    
        #define COMPARE_MATCH_MAX_TICK 250
        #define CLOCK_MILLISECOND_MAX_CONSTANT 8
        #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 32
        // In some Timer settings, CLOCK_MILLISECOND_TO_TICK_CONSTANT has decimal values
        // TO keep integer calculations only, nearest higher value is used
	
	#else 
		#error Incorrect CONTROLLER_FREQ : For Mega328pb Controller MOREY_OS supports only five frequencies i.e. 1000000UL, 8000000UL, 12000000UL, 16000000UL & 20000000UL
    
    #endif
    
#elif (OS_TIMER_TYPE == TIMER_1) || (OS_TIMER_TYPE == TIMER_3) || (OS_TIMER_TYPE == TIMER_4)
    
     // For OS_TIMER_TYPE == timer1 or timer3 or timer4 is used, max accuracy achieved is 1 milli second for all frequencies
	 // Except for CONTROLLER_FREQ = 20000000UL, which has max accuracy of 2 milli second
	 
	#define PLATFORM_SUPPORT_TICKLESS
	
	#if OS_TIMER_TYPE == TIMER_1
		
		#define OS_TICKING_INTERRUPT				TIMER1_COMPA_vect
		#define OS_TICKLESS_INTERRUPT				TIMER1_COMPB_vect
		#define OS_TICKING_TIMER_COUNTER			TCNT1
		#define OS_TICKLESS_TIMER_REGISTER			OCR1B
		#define TIMER_COUNTER_START_VALUE			0x0000
		#define TIMER_REGISTER_DEFAULT_VALUE		0xFFFF
		
	#elif OS_TIMER_TYPE == TIMER_3
		
		#define OS_TICKING_INTERRUPT				TIMER3_COMPA_vect
		#define OS_TICKLESS_INTERRUPT				TIMER3_COMPB_vect
		#define OS_TICKING_TIMER_COUNTER			TCNT3
		#define OS_TICKLESS_TIMER_REGISTER			OCR3B
		#define TIMER_COUNTER_START_VALUE			0x0000
		#define TIMER_REGISTER_DEFAULT_VALUE		0xFFFF
		
	#elif OS_TIMER_TYPE == TIMER_4
		
		#define OS_TICKING_INTERRUPT				TIMER4_COMPA_vect
		#define OS_TICKLESS_INTERRUPT				TIMER4_COMPB_vect
		#define OS_TICKING_TIMER_COUNTER			TCNT4
		#define OS_TICKLESS_TIMER_REGISTER			OCR4B
		#define TIMER_COUNTER_START_VALUE			0x0000
		#define TIMER_REGISTER_DEFAULT_VALUE		0xFFFF
		
	#endif
	
	#if CONTROLLER_FREQ == 20000000UL
	
		#define COMPARE_MATCH_MAX_TICK 62500
        #define CLOCK_MILLISECOND_MAX_CONSTANT 200
        #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 313
		// In some Timer settings, CLOCK_MILLISECOND_TO_TICK_CONSTANT has decimal values
        // TO keep integer calculations only, nearest higher value is used
     
    #elif CONTROLLER_FREQ == 16000000UL
	
		#define COMPARE_MATCH_MAX_TICK 62500
        #define CLOCK_MILLISECOND_MAX_CONSTANT 250
        #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 250
                        
    #elif CONTROLLER_FREQ == 12000000UL
    
		#define COMPARE_MATCH_MAX_TICK 60000
        #define CLOCK_MILLISECOND_MAX_CONSTANT 40
        #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 1500
    
    #elif CONTROLLER_FREQ == 8000000UL
    
		#define COMPARE_MATCH_MAX_TICK 62500
        #define CLOCK_MILLISECOND_MAX_CONSTANT 500
        #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 125
        
    #elif CONTROLLER_FREQ == 1000000UL
     
		#define COMPARE_MATCH_MAX_TICK 62500
        #define CLOCK_MILLISECOND_MAX_CONSTANT 500
        #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 125       
	
	#else 
		#error Incorrect CONTROLLER_FREQ : For Mega328pb Controller MOREY_OS supports only five frequencies i.e. 1000000UL, 8000000UL, 12000000UL, 16000000UL & 20000000UL
	
    #endif
	 
#else 
	#error Incorrect OS_TIMER_TYPE : For Mega328pb Controller MOREY_OS supports only five values i.e. TIMER_0, TIMER_1 , TIMER_2, TIMER_3 & TIMER_4

#endif

#define PLATFORM_INIT() platform_init()
#define WATCHDOG_PERIODIC() watchdog_periodic()
#define OS_SLEEP() sleep()

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

#endif
#endif

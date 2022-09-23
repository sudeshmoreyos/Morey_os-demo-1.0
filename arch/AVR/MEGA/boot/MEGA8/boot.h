#ifndef BOOT_H
#define BOOT_H

/** \defgroup arch
* @{ \addtogroup arch
* @{ \defgroup AVR
* @{ \addtogroup AVR
* @{ \defgroup MEGA
* @{ \addtogroup MEGA
* @{ \defgroup boot_mega
* @{ \addtogroup boot_mega
* @{ \defgroup MEGA8
* @{ \addtogroup MEGA8
* @{ \defgroup boot_mega8_h
* @{ \addtogroup boot_mega8_h
* @{
* \brief Documentaion of boot.h header file for atmega8 controller    
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This header file implements atmega8 boot functions
*
* **USER DEFINED MACROS** that can be used in boot.h (These macros should be declared in config.h file) are :
*1. **MEGA8_DEBUG_LEVEL_CONF** \n
*=> Allowed values : {MEGA8_DEBUG_NONE, MEGA8_DEBUG_USER, MEGA8_DEBUG_LEVEL1, MEGA8_DEBUG_LEVEL2, MEGA8_DEBUG_LEVEL3} \n
*=> Eg. Declaration in config.h : #define MEGA8_DEBUG_LEVEL_CONF MEGA8_DEBUG_LEVEL1 \n
*=> Default value : MEGA8_DEBUG_NONE \n
*=> Usage Description : This macro can be defined to diagnose different execution steps of boot.h or boot.c for atmega8 controller,
*with increasing levels (i.e. level 1 2 3) deeper execution steps can be diagnosed. MEGA8_DEBUG_USER level enables MEGA8_DEBUG_MSG_USER Macro.
*To use this, user should manually write MEGA8_DEBUG_MSG_USER(msg) inside code sections of boot.h or boot.c to be diagnosed. \n
*\n
*2. **CONTROLLER_FREQ_CONF** \n
*=> Allowed values : {1000000, 8000000, 12000000, 16000000} \n
*=> Eg. Declaration in Makefile : FREQ = 1000000 \n
*=> Default value : 16000000 \n
*=> Usage Description : For Atmega8 Controller MOREY_OS supports only four frequencies i.e. 1000000 (1 Mhz), 8000000 (8 Mhz),
* 12000000 (12 Mhz) & 16000000 (16 Mhz). If user do not define FREQ macro in Makefile, by default 16000000
* frequency is selected. Please note except for TIMER_TYPE = TIMER_1, for 12 MHz setting, timer module generates inaccurate timings. 
* Hence it is recommended either not to use 12 Mhz frequency, or use 12 MHz with TIMER_TYPE = TIMER_1.\n
*\n
*3. **TIMER_TYPE_CONF** \n
*=> Allowed values : {TIMER_0, TIMER_1, TIMER_2} \n
*=> Eg. Declaration in config.h : #define TIMER_TYPE_CONF TIMER_1 \n
*=> Default value : TIMER_1 \n
*=> Usage Description : For mega8 Controller MOREY_OS supports only three values i.e. TIMER_0, TIMER_1, TIMER_2. 
* TIMER_TYPE = TIMER_0 uses timer0 (8 bit), TIMER_TYPE = TIMER_2 uses timer2 (8 bit) and TIMER_TYPE = TIMER_1 uses timer1 (16 bit) of atmega8 controller. 
* If user do not define TIMER_TYPE_CONF in config.h header, by default TIMER_TYPE = TIMER_1 ( timer1 ) is selected. For Mega8 controller Morey_OS works most
* efficiently when TIMER_TYPE_CONF is set to TIMER_1, which is also default setting.
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
#elif COMPILER == CODEVISION_AVR
	#include <mega8.h>
#else
	#error Incorrect COMPILER, Supported compilers for atmega8 are AVR_STUDIO, WIN_AVR, AVR_GCC and CODEVISION_AVR
#endif

#include "controller_macros.h"

#define GLOBAL_INTERRUPT_ENABLE() global_interrupt_enable()
#define GLOBAL_INTERRUPT_DISABLE() global_interrupt_disable()

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


#ifndef DISABLE_OS

typedef mos_uint16_t program_address_t;
typedef mos_uint32_t clock_second_t;
typedef mos_uint16_t clock_millisecond_t;

/**
* \name MACROS : Mega8 Boot Debug Levels
* \brief Define different levels for mega8 boot debugging
* @{
*/
#define MEGA8_DEBUG_NONE 0
#define MEGA8_DEBUG_USER 1
#define MEGA8_DEBUG_LEVEL1 2
#define MEGA8_DEBUG_LEVEL2 3
#define MEGA8_DEBUG_LEVEL3 4 
///@}

#ifdef TIMER_TYPE_CONF
    #define TIMER_TYPE TIMER_TYPE_CONF
#else
    #define TIMER_TYPE TIMER_1
#endif

#if TIMER_TYPE == TIMER_0

    // For TIMER_TYPE == 1 timer0 of mega0 is used, max accuracy achieved is 4 milli second
    // except for 1 MHz crystal which achieves max accuracy of 8 milli second
    // Please note 12 MHz crystal generates inaccurate timings
    // Hence it is recommended not to use 12 MHz frequency when TIMER_TYPE = 1  
    
    #if CONTROLLER_FREQ == 16000000UL
        #define TIMER_COUNTER_START_VALUE 0x06
        #define CLOCK_MILLISECOND_MAX_CONSTANT 4                   
                        
    #elif CONTROLLER_FREQ == 12000000UL
    
        #define TIMER_COUNTER_START_VALUE 0x44        
        #define CLOCK_MILLISECOND_MAX_CONSTANT 4
    
    #elif CONTROLLER_FREQ == 8000000UL
    
        #define TIMER_COUNTER_START_VALUE 0x83
        #define CLOCK_MILLISECOND_MAX_CONSTANT 4        

    #elif CONTROLLER_FREQ == 1000000UL
    
        #define TIMER_COUNTER_START_VALUE 0x83
        #define CLOCK_MILLISECOND_MAX_CONSTANT 8
	
	#else 
		#error Incorrect CONTROLLER_FREQ : For mega8 Controller MOREY_OS supports only four frequencies i.e. 1000000UL, 8000000UL, 12000000UL & 16000000UL
    #endif
    
#elif TIMER_TYPE == TIMER_2
    
     // For TIMER_TYPE == 2 timer2 is used, max accuracy achieved is 2 milli second
     // except for 1 MHz crystal which achieves max accuracy of 4 milli second
     // Please note 12 MHz crystal generates inaccurate timings
     // Hence it is recommended not to use 12 MHz crystal when TIMER_TYPE = 1
     
     #define PLATFORM_SUPPORT_TICKLESS
	 
     #if CONTROLLER_FREQ == 16000000UL
		
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
		#error Incorrect CONTROLLER_FREQ : For mega8 Controller MOREY_OS supports only four frequencies i.e. 1000000UL, 8000000UL, 12000000UL & 16000000UL
    
    #endif
    
#elif TIMER_TYPE == TIMER_1
    
     // For TIMER_TYPE == 3 timer1 is used, max accuracy achieved is 1 milli second for all frequencies
	 
	#define PLATFORM_SUPPORT_TICKLESS
     
    #if CONTROLLER_FREQ == 16000000UL
	
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
		#error Incorrect CONTROLLER_FREQ : For mega8 Controller MOREY_OS supports only four frequencies i.e. 1000000UL, 8000000UL, 12000000UL & 16000000UL
	
    #endif
	 
#else 
	#error Incorrect TIMER_TYPE : For mega8 Controller MOREY_OS supports only three values i.e. TIMER_0, TIMER_1, TIMER_2, or {1, 2, 3}

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

#ifndef BOOT_H
#define BOOT_H

/** \addtogroup arch
* @{ \addtogroup AVR
* @{ \defgroup XMEGA
* @{ \addtogroup XMEGA
* @{ \defgroup boot_xmega
* @{ \addtogroup boot_xmega
* @{ \defgroup XMEGA256
* @{ \addtogroup XMEGA256
* @{ \defgroup boot_xmega256_h
* @{ \addtogroup boot_xmega256_h
* @{
* \brief Documentaion of boot.h header file for atxmega256 controller    
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This header file implements atxmega256 boot functions
*
* **USER DEFINED MACROS** that can be used in boot.h (These macros should be declared in config.h file) are :
*1. **XMEGA256_DEBUG_LEVEL_CONF** \n
*=> Allowed values : {XMEGA256_DEBUG_NONE, XMEGA256_DEBUG_USER, XMEGA256_DEBUG_LEVEL1, XMEGA256_DEBUG_LEVEL2, XMEGA256_DEBUG_LEVEL3} \n
*=> Eg. Declaration : #define XMEGA256_DEBUG_LEVEL_CONF XMEGA256_DEBUG_LEVEL1 \n
*=> Default value : XMEGA256_DEBUG_NONE \n
*=> Usage Description : This macro can be defined to diagnose different execution steps of boot.h or boot.c for atmega8 controller,
*with increasing levels (i.e. level 1 2 3) deeper execution steps can be diagnosed. XMEGA256_DEBUG_USER level enables 
*XMEGA256_DEBUG_MSG_USER Macro. To use this, user should manually write XMEGA256_DEBUG_MSG_USER(msg) inside code sections
*of boot.h or boot.c to be diagnosed. \n
*\n
*2. **CONTROLLER_FREQ_CONF** \n
*=> Allowed values : {2000000UL, 8000000UL, 16000000UL, 32000000UL} \n
*=> Eg. Declaration : #define CONTROLLER_FREQ_CONF 20000000UL \n
*=> Default value : 32000000UL \n
*=> Usage Description : For Atmega8 Controller MOREY_OS supports only four frequencies i.e. 2000000UL (2 Mhz), 8000000UL (8 Mhz),
* 16000000UL (16 Mhz) & 32000000UL (32 Mhz). If user do not define CONTROLLER_FREQ_CONF in config.h header, by default 320000000UL
* frequency is selected.\n
*\n
*3. **TIMER_TYPE_CONF** \n
*=> Allowed values : {Recommended = 1, 2, 3, 4, 5, 6, 7 } or {TIMER_C0, TIMER_C1, TIMER_D0, TIMER_D1, TIMER_E0, TIMER_E1, TIMER_F0}\n
*=> Eg. Declaration : #define TIMER_TYPE_CONF 3 \n
*=> Eg. Alternate Declaration : #define TIMER_TYPE_CONF TIMER_D0 \n
*=> Default value : 3 \n
*=> Usage Description : For xmega256 Controller MOREY_OS supports seven values i.e. 1, 2, 3, 4, 5, 6, 7. 
*TIMER_TYPE_CONF = 1 or TIMER_TYPE_CONF = TIMER_C0 uses timerC0, TIMER_TYPE_CONF = 2 or TIMER_TYPE_CONF = TIMER_C1 uses timerC1,
*TIMER_TYPE_CONF = 3 or TIMER_TYPE_CONF = TIMER_D0 uses timerD0, TIMER_TYPE_CONF = 4 or TIMER_TYPE_CONF = TIMER_D1 uses timerD1,
*TIMER_TYPE_CONF = 5 or TIMER_TYPE_CONF = TIMER_E0 uses timerE0, TIMER_TYPE_CONF = 6 or TIMER_TYPE_CONF = TIMER_E1 uses timerE1,
*TIMER_TYPE_CONF = 7 or TIMER_TYPE_CONF = TIMER_F0 uses timerF0.\n
*It is recommended to use TIMER_TYPE_CONF options {1, 2, 3, 4, 5, 6, 7 } instead of 
*{TIMER_C0, TIMER_C1, TIMER_D0, TIMER_D1, TIMER_E0, TIMER_E1, TIMER_F0}. Because former options are portable when using
* same code for other platforms, while later options are specific to AVR XMEGA controller series.
* For XMega256 controller Morey_OS works most efficient in all timer options.
* @}
*/

#include "../../../../../platform.h"

#ifdef CONTROLLER_FREQ_CONF 
    #define CONTROLLER_FREQ CONTROLLER_FREQ_CONF
#else    
    #define CONTROLLER_FREQ 32000000UL
#endif

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
	#include <avr/io.h>
#elif COMPILER == CODEVISION_AVR
	#include <xmega256a3u.h>
#else
	#error Incorrect COMPILER, Supported compilers for atxmega256 are AVR_STUDIO, WIN_AVR, AVR_GCC and CODEVISION_AVR
#endif

#include "controller_macros.h"

#ifdef CONTROLLER_FREQ_CONF 
    #define CONTROLLER_FREQ CONTROLLER_FREQ_CONF
#else    
    #define CONTROLLER_FREQ 32000000UL
#endif

#ifndef DISABLE_OS

typedef mos_uint16_t program_address_t;
typedef mos_uint32_t clock_second_t;
typedef mos_uint16_t clock_millisecond_t;

/**
* \name MACROS : XMega256 Boot Debug Levels
* \brief Define different levels for Xmega256 boot debugging
* @{
*/
#define XMEGA256_DEBUG_NONE 0
#define XMEGA256_DEBUG_USER 1
#define XMEGA256_DEBUG_LEVEL1 2
#define XMEGA256_DEBUG_LEVEL2 3
#define XMEGA256_DEBUG_LEVEL3 4 
///@}

#define TIMER_C0 1
#define TIMER_C1 2
#define TIMER_D0 3
#define TIMER_D1 4
#define TIMER_E0 5
#define TIMER_E1 6
#define TIMER_F0 7

#ifdef TIMER_TYPE_CONF
    #define TIMER_TYPE TIMER_TYPE_CONF
#else
    #define TIMER_TYPE TIMER_C0
#endif

// For all timers max accuracy achieved is 1 milli second
#if ( TIMER_TYPE >= TIMER_C0 ) && ( TIMER_TYPE <= TIMER_F0 )

	#define PLATFORM_SUPPORT_TICKLESS
	
    #if TIMER_TYPE == TIMER_C0
        #define TIMER_PORT TCC0
    #elif TIMER_TYPE == TIMER_C1
        #define TIMER_PORT TCC1            
    #elif TIMER_TYPE == TIMER_D0
        #define TIMER_PORT TCD0        
    #elif TIMER_TYPE == TIMER_D1
        #define TIMER_PORT TCD1        
    #elif TIMER_TYPE == TIMER_E0
        #define TIMER_PORT TCE0
    #elif TIMER_TYPE == TIMER_E1
        #define TIMER_PORT TCE1
    #elif TIMER_TYPE == TIMER_F0
        #define TIMER_PORT TCF0
    #endif
#else
    #error Invalid TIMER_TYPE, supported values are TIMER_C0, TIMER_C1, TIMER_D0, TIMER_D1,TIMER_E0, TIMER_E1 and TIMER_F0 or {1, 2, 3, 4, 5, 6, 7}
#endif          

#if CONTROLLER_FREQ == 32000000UL        
    #define CLOCK_MILLISECOND_MAX_CONSTANT 500
    #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 125                   
                        
#elif CONTROLLER_FREQ == 16000000UL        
    #define CLOCK_MILLISECOND_MAX_CONSTANT 250
    #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 250
    
#elif CONTROLLER_FREQ == 8000000UL
    #define CLOCK_MILLISECOND_MAX_CONSTANT 500
    #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 125        

#elif CONTROLLER_FREQ == 2000000UL
    #define CLOCK_MILLISECOND_MAX_CONSTANT 250
    #define CLOCK_MILLISECOND_TO_TICK_CONSTANT 250
	
#else
	#error Incorrect CONTROLLER_FREQ : For xmega256 Controller MOREY_OS supports only four frequencies i.e. 2000000, 8000000, 16000000 & 32000000   
#endif

#define PLATFORM_INIT() platform_init()
#define WATCHDOG_PERIODIC() watchdog_periodic()
#define OS_SLEEP() sleep()
#define GLOBAL_INTERRUPT_ENABLE() global_interrupt_enable()
#define GLOBAL_INTERRUPT_DISABLE() global_interrupt_disable()

/** 
* \brief	Declare function to init essential peripherals required by OS
*
* This function initiates essential platform peripherals, which includes
* selecting controller frequency either internal or external Oscillator,
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
* \brief	Declare function to disable global interrupt. 
* This function can be used to do atomic operations 
*/
void global_interrupt_enable(void);

/** 
* \brief	Declare function to disable global interrupt. 
* This function must be called after completing atomic operations
*/
void global_interrupt_disable(void);


/*
ISR (TCE0_OVF_vect)
{
t++;
//code on overflow
}
ISR (TCE0_CCA_vect)
{
//code for CCA compare
}
ISR (TCE0_CCB_vect)
{
//code on CCB compare	
}
*/

/*

#if TIMER_TYPE == TIMER_C0
    #define TIMER_INTERRUPT1_FUNCTION_START()               \ 
    interrupt [TCC0_OVF_vect] void tcc0_overflow_isr(void)
    
#elif TIMER_TYPE == TIMER_C1
    #define TIMER_INTERRUPT1_FUNCTION_START()               \    
    interrupt [TCC1_OVF_vect] void tcc1_overflow_isr(void)
    
#elif TIMER_TYPE == TIMER_D0
    #define TIMER_INTERRUPT1_FUNCTION_START()               \    
    interrupt [TCD0_OVF_vect] void tcd0_overflow_isr(void)
    
#elif TIMER_TYPE == TIMER_D1
    #define TIMER_INTERRUPT1_FUNCTION_START()               \    
    interrupt [TCD1_OVF_vect] void tcd1_overflow_isr(void)
    
#elif TIMER_TYPE == TIMER_E0
    #define TIMER_INTERRUPT1_FUNCTION_START()               \    
    interrupt [TCE0_OVF_vect] void tce0_overflow_isr(void)
    
#elif TIMER_TYPE == TIMER_E1
    #define TIMER_INTERRUPT1_FUNCTION_START()               \    
    interrupt [TCE1_OVF_vect] void tce1_overflow_isr(void)
    
#elif TIMER_TYPE == TIMER_F0
    #define TIMER_INTERRUPT1_FUNCTION_START()               \    
    interrupt [TCF0_OVF_vect] void tcf0_overflow_isr(void)
    
#endif

#define TIMER_INTERRUPT1_FUNCTION_END() 

#if TIMER_TYPE == TIMER_C0
    #define TIMER_INTERRUPT2_AVAILABLE
    #define TIMER_INTERRUPT2_FUNCTION()                     \
    interrupt [TCC0_CCA_vect] void tcc0_compare_capture_a_isr(void)
    
#elif TIMER_TYPE == TIMER_C1
    #define TIMER_INTERRUPT2_AVAILABLE
    #define TIMER_INTERRUPT2_FUNCTION()                     \    
    interrupt [TCC1_CCA_vect] void tcc1_compare_capture_a_isr(void)
    
#elif TIMER_TYPE == TIMER_D0
    #define TIMER_INTERRUPT2_AVAILABLE
    #define TIMER_INTERRUPT2_FUNCTION()                     \    
    interrupt [TCD0_CCA_vect] void tcd0_compare_capture_a_isr(void)
    
#elif TIMER_TYPE == TIMER_D1
    #define TIMER_INTERRUPT2_AVAILABLE
    #define TIMER_INTERRUPT2_FUNCTION()                     \    
    interrupt [TCD1_CCA_vect] void tcd1_compare_capture_a_isr(void)
    
#elif TIMER_TYPE == TIMER_E0
    #define TIMER_INTERRUPT2_AVAILABLE
    #define TIMER_INTERRUPT2_FUNCTION()                     \    
    interrupt [TCE0_CCA_vect] void tce0_compare_capture_a_isr(void)
    
#elif TIMER_TYPE == TIMER_E1
    #define TIMER_INTERRUPT2_AVAILABLE
    #define TIMER_INTERRUPT2_FUNCTION()                     \    
    interrupt [TCE1_CCA_vect] void tce1_compare_capture_a_isr(void)
    
#elif TIMER_TYPE == TIMER_F0
    #define TIMER_INTERRUPT2_AVAILABLE
    #define TIMER_INTERRUPT2_FUNCTION()                     \    
    interrupt [TCF0_CCA_vect] void tcf0_compare_capture_a_isr(void)
        
#endif

#define PLATFORM_INIT() platform_init()
#define WATCHDOG_RESET() watchdog_reset()
#define PLATFORM_SLEEP() platform_sleep()

*/

#endif
#endif

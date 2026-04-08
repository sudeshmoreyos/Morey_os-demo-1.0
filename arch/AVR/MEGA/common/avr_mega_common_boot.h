#ifndef AVR_MEGA_COMMON_BOOT_H
#define AVR_MEGA_COMMON_BOOT_H

// #include "../../../../platform.h"

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
// #include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#else
#error Incorrect COMPILER, Supported compilers for Atmega8 are AVR_STUDIO, WIN_AVR, AVR_GCC and CODEVISION_AVR
#endif

#define GLOBAL_INTERRUPT_ENABLE()	global_interrupt_enable()
#define GLOBAL_INTERRUPT_DISABLE() 	global_interrupt_disable()

#define ATOMIC_ON()	GLOBAL_INTERRUPT_DISABLE()
#define ATOMIC_OFF()	GLOBAL_INTERRUPT_ENABLE()

#define IS_GLOBAL_INTERRUPT_ENABLE()	is_global_interrupt_enable()
#define IS_GLOBAL_INTERRUPT_DISABLE()	is_global_interrupt_disable()

#define IS_ATOMIC_ON()	IS_GLOBAL_INTERRUPT_DISABLE()
#define IS_ATOMIC_OFF()	IS_GLOBAL_INTERRUPT_ENABLE()

#define ATOMIC_ON_RESTORE()	critical_enter()
#define ATOMIC_OFF_RESTORE(x)	critical_exit(x)	

/**
* \name Global Interrupt Functions (Controller specific)
* \brief Define controller specific global interrupt enable and disable functions for atomic operations
* @{
*/

/** 
* \brief	Declare function to enable global interrupt. 
* This function can be used to do atomic operations 
*/
static inline void global_interrupt_enable(void)
{
#if (COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR) || (COMPILER == AVR_GCC)
	// Global enable interrupts
	sei();
#endif
}

/** 
* \brief	Declare function to disable global interrupt. 
* This function must be called after completing atomic operations
*/
static inline void global_interrupt_disable(void)
{
#if (COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR) || (COMPILER == AVR_GCC)
	// Global disable interrupts
	cli();
#endif
}

/** 
* \brief	Declare function to check if global interrupt is enabled.
* This function must be called to check if Global interrupt is already enabled or not.
*/
static inline mos_uint8_t is_global_interrupt_enable(void)
{
	if( SREG & (1<<7) )
		return 1;
	else
		return 0;
}

/** 
* \brief	Declare function to check if global interrupt is disabled.
* This function must be called to check if Global interrupt is already disabled or not.
*/
static inline mos_uint8_t is_global_interrupt_disable(void)
{
	if( SREG & (1<<7) )
		return 0;
	else
		return 1;
}

/** 
* \brief	Function to enter critical section
* This function must be called before entering a critical section/ atomic section of the code
*/
static inline uint8_t critical_enter(void)
{
    uint8_t sreg = SREG;                      /* save interrupt state */
    __asm__ volatile("cli" ::: "memory");     /* disable interrupts   */
    return sreg;
}

/** 
* \brief	Function to exir critical section
* This function must be called after exiting a critical/atomic section of the code
* It set current state back to state before entering critical/atomic section
*/
static inline void critical_exit(uint8_t sreg_saved)
{
    SREG = sreg_saved;                        /* restore old state    */
    __asm__ volatile("" ::: "memory");        /* compiler barrier     */
}

///@}

// Check if OS functionality is not disabled
#ifndef DISABLE_OS

// define typedefs required by OS and scheduler clock
typedef mos_uint16_t program_address_t;
typedef mos_uint32_t clock_second_t;
typedef mos_uint16_t clock_millisecond_t;

#define WATCHDOG_PERIODIC() watchdog_periodic()
#define OS_SLEEP() sleep()

static inline void watchdog_periodic(void)
{
#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC )) 
	wdt_reset();
#elif COMPILER == CODEVISION_AVR
	#asm("wdr")
#endif
}

static inline void sleep(void)
{
#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ) )
	sleep_mode();
#elif ( COMPILER == CODEVISION_AVR )
	idle();
#endif
}

#endif
#endif

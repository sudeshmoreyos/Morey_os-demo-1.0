/**\addtogroup sources_MEGA4809
* @{ \defgroup boot_MEGA4809_c
* @{
* \brief Documentaion of boot.h header file for atmega4809 controller
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements atmega4809 boot functions
* @}
*/

#include "boot.h"

#ifdef MEGA4809_DEBUG_LEVEL_CONF
	#define MEGA4809_DEBUG_LEVEL MEGA4809_DEBUG_LEVEL_CONF
#else
	#define MEGA4809_DEBUG_LEVEL MEGA4809_DEBUG_NONE
#endif

#if MEGA4809_DEBUG_LEVEL > MEGA4809_DEBUG_NONE    
	#include "../../../../../os/lib/debug.h"
#endif

#define MEGA4809_DEBUG_MSG(msg)				DEBUG_MSG_CONST3_LN("<DEBUG MEGA4809> ",NULL,msg)
#define MEGA4809_DEBUG_MSG_TEMP(msg) 		DEBUG_MSG_CONST3_LN("<DEBUG MEGA4809 USER> ",NULL,msg)
    
#if MEGA4809_DEBUG_LEVEL == MEGA4809_DEBUG_NONE    
    #define MEGA4809_DEBUG_MSG1(msg)
    #define MEGA4809_DEBUG_MSG2(msg)
    #define MEGA4809_DEBUG_MSG3(msg)
    #define MEGA4809_DEBUG_MSG_USER(msg)

#elif MEGA4809_DEBUG_LEVEL == MEGA4809_DEBUG_USER    
    #define MEGA4809_DEBUG_MSG1(msg)    
    #define MEGA4809_DEBUG_MSG2(msg)
    #define MEGA4809_DEBUG_MSG3(msg)
    #define MEGA4809_DEBUG_MSG_USER(msg)	MEGA4809_DEBUG_MSG_TEMP(msg)

#elif MEGA4809_DEBUG_LEVEL == MEGA4809_DEBUG_LEVEL1    
    #define MEGA4809_DEBUG_MSG1(msg) 		MEGA4809_DEBUG_MSG(msg)
    #define MEGA4809_DEBUG_MSG2(msg)
    #define MEGA4809_DEBUG_MSG3(msg)
    #define MEGA4809_DEBUG_MSG_USER(msg)

#elif MEGA4809_DEBUG_LEVEL == MEGA4809_DEBUG_LEVEL2    
    #define MEGA4809_DEBUG_MSG1(msg) 		MEGA4809_DEBUG_MSG(msg)
    #define MEGA4809_DEBUG_MSG2(msg) 		MEGA4809_DEBUG_MSG(msg)
    #define MEGA4809_DEBUG_MSG3(msg)
    #define MEGA4809_DEBUG_MSG_USER(msg)

#elif MEGA4809_DEBUG_LEVEL == MEGA4809_DEBUG_LEVEL3    
    #define MEGA4809_DEBUG_MSG1(msg) 		MEGA4809_DEBUG_MSG(msg)    
    #define MEGA4809_DEBUG_MSG2(msg) 		MEGA4809_DEBUG_MSG(msg)
    #define MEGA4809_DEBUG_MSG3(msg) 		MEGA4809_DEBUG_MSG(msg)
    #define MEGA4809_DEBUG_MSG_USER(msg)

#else
    #error Invalid MEGA4809 DEBUG LEVEL, supported levels : MEGA4809_DEBUG_LEVEL1, MEGA4809_DEBUG_LEVEL2, MEGA4809_DEBUG_LEVEL3, MEGA4809_DEBUG_USER
#endif

static void clock_init(void)
{
	// Enable the internal 20 MHz oscillator
    CLKCTRL_OSC20MCTRLA |= CLKCTRL_ENABLE_bm;   // Set ENABLE bit

    // Wait for the oscillator to stabilize
    for (volatile uint32_t i = 0; i < 10000; i++);

    // Select internal 20 MHz oscillator as the main clock source
    // Protected write sequence using CPU_CCP register
    CCP = CCP_IOREG_gc;     // Unlock clock control registers
    CLKCTRL_MCLKCTRLA = CLKCTRL_CLKSEL_OSC20M_gc;   // Select 20 MHz oscillator

    // Disable prescaler (set to 1:1)
    CCP = CCP_IOREG_gc;
    CLKCTRL_MCLKCTRLB = 0;  // Prescaler disabled, divide by 1

    // Optionally lock the clock configuration (optional, prevents accidental changes)
    CLKCTRL_MCLKLOCK = CLKCTRL_LOCKEN_bm;
	
	MEGA4809_DEBUG_MSG2("Clock Initiate Success : Internal 20 MHz Oscialltor");
}

void platform_no_os_init(void)
{	
	clock_init();
	global_interrupt_enable();
}

#ifndef DISABLE_OS

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
	#include <avr/wdt.h>
	#include <avr/sleep.h>
#endif

static void timer_init(void);
static void watchdog_init(void);
static void sleep_init(void);

void platform_init(void)
{	
	clock_init();
	timer_init();
	sleep_init();
	watchdog_init();
	global_interrupt_enable();
}

static void timer_init(void)
{
#if OS_TIMER_TYPE == TIMER_RTC
    // Configure the selected 32.768 kHz oscillator
#if 0 // TIMER_RTC_OSC == RTC_OSC_EXTERNAL // External crystal (XOSC32K)
	mos_uint8_t temp;

	// --- External 32.768 kHz crystal ---
    temp = (CLKCTRL_XOSC32KCTRLA & (~CLKCTRL_ENABLE_bm));
    CPU_CCP = CCP_IOREG_gc;
    CLKCTRL_XOSC32KCTRLA = temp;   // disable oscillator
	
    while (CLKCTRL_MCLKSTATUS & CLKCTRL_XOSC32KS_bm)
		; // wait until disabled

    temp = CLKCTRL_XOSC32KCTRLA & ~CLKCTRL_SEL_bm;  // SEL=0 → external crystal
    temp |= CLKCTRL_ENABLE_bm;                      // enable oscillator
    CPU_CCP = CCP_IOREG_gc;
    CLKCTRL_XOSC32KCTRLA = temp;

    // Wait until external oscillator is stable
    while (!(CLKCTRL_MCLKSTATUS & CLKCTRL_XOSC32KS_bm))
		;

#else // Internal crystal (XOSC32K)
	
	/*
	MEGA4809_DEBUG_MSG2("Here-3");
    // --- Internal 32.768 kHz oscillator (OSC32K) ---
    temp = CLKCTRL_OSC32KCTRLA;
    temp |= CLKCTRL_ENABLE_bm;     // enable oscillator
    CPU_CCP = CCP_IOREG_gc;        // unlock protected write
    CLKCTRL_OSC32KCTRLA = temp;
	
	MEGA4809_DEBUG_MSG2("Here-4");
	
    // Wait until oscillator is stable
    while (!(CLKCTRL_MCLKSTATUS & CLKCTRL_OSC32KS_bm))
		;
	MEGA4809_DEBUG_MSG2("Here-5");
	*/
	
#endif

    // Wait for RTC synchronization
    while (RTC.STATUS != 0)
		;
    
	// Configure RTC registers
#if 0
    RTC.CLKSEL = RTC_CLKSEL_TOSC32K_gc;  // external crystal
#else
	RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;   // internal oscillator
#endif
	
	// Wait until oscillator is stable
    // while (!(CLKCTRL_MCLKSTATUS & CLKCTRL_OSC32KS_bm))
	// ;	
	
    RTC.PER = TIMER_TICKING_MAX_TICK;
    RTC.CMP = TIMER_TICKLESS_DEFAULT_VALUE;

    RTC.DBGCTRL = RTC_DBGRUN_bm;  // run during debugging
    RTC.INTFLAGS = 0xFF;          // clear all interrupt flags
	
	RTC.INTCTRL = RTC_OVF_bm | RTC_CMP_bm;

    // Enable RTC with prescaler = 1 (full 32.768 kHz rate)
    RTC.CTRLA = RTC_PRESCALER_DIV1_gc  // no prescaling
              | RTC_RTCEN_bm           // enable RTC
              | RTC_RUNSTDBY_bm;       // run in standby
			  
     // Enable interrupts

    while (RTC.STATUS != 0)
		; // wait for sync agai

	
	MEGA4809_DEBUG_MSG2("OS TIMER : RTC Initiate Success, internal OSC32K");

#endif // OS_TIMER_TYPE == TIMER_RTC
}

static void watchdog_init(void)
{
    // Enable protected register write
    CCP = CCP_IOREG_gc;

    // Configure WDT:
    // - Timeout = 2s
    // - Enable WDT
    // - No interrupt (reset mode)
    WDT.CTRLA = 0x09;

    // Optional debug message
    MEGA4809_DEBUG_MSG2("Watchdog Timer initialized (2s timeout)");
}

void watchdog_periodic(void)
{
	#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC )) 
		wdt_reset();
	#endif
	MEGA4809_DEBUG_MSG3("Watchdog Reset");
}

static void sleep_init(void)
{
    #if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ) )
		// SLPCTRL.CTRLA = 0x00;
		set_sleep_mode(SLPCTRL_SMODE_IDLE_gc);
	#endif
	
    MEGA4809_DEBUG_MSG2("Sleep Mode Initiated");
}

void sleep(void)
{
	MEGA4809_DEBUG_MSG3("Sleeping");
	
	#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ) )
		// SLPCTRL.CTRLA = 0x01;
		// asm volatile ("sleep"); // Idle mode, only CPU Sleeps
		// SLPCTRL.CTRLA = 0x00;
		sleep_enable();
        //sei();
        sleep_cpu();
        sleep_disable();
	#elif ( COMPILER == CODEVISION_AVR )
		idle();
	#endif	
    MEGA4809_DEBUG_MSG3("Woke up");
}

#endif
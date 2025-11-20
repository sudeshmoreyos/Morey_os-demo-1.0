/** \addtogroup XMEGA256
* @{ \defgroup boot_xmega256_c
* @{ \addtogroup boot_xmega256_c
* @{
* \brief Documentaion of boot.h header file for atxmega256 controller
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements atxmega256 boot functions
* @}
*/

#ifndef DISABLE_OS

#include "boot.h"

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
	
	#include <avr/interrupt.h>
	#include <avr/wdt.h>
	#include <avr/sleep.h>
	
#elif COMPILER == CODEVISION_AVR
	#include <sleep.h>
#endif

#ifdef XMEGA256_DEBUG_LEVEL_CONF
    #define XMEGA256_DEBUG_LEVEL XMEGA256_DEBUG_LEVEL_CONF
#else
    #define XMEGA256_DEBUG_LEVEL XMEGA256_DEBUG_NONE
#endif

#if XMEGA256_DEBUG_LEVEL > XMEGA256_DEBUG_NONE    
    #include "../../../../../os/lib/debug.h"
#endif

#define XMEGA256_DEBUG_MSG(msg) 		DEBUG_MSG_LN("<DEBUG XMEGA256> ",NULL,msg)
#define XMEGA256_DEBUG_MSG_TEMP(msg) 	DEBUG_MSG_LN("<DEBUG XMEGA256 USER> ",NULL,msg)
    
#if XMEGA256_DEBUG_LEVEL == XMEGA256_DEBUG_NONE    
    #define XMEGA256_DEBUG_MSG1(msg)
    #define XMEGA256_DEBUG_MSG2(msg)
    #define XMEGA256_DEBUG_MSG3(msg)
    #define XMEGA256_DEBUG_MSG_USER(msg)

#elif XMEGA256_DEBUG_LEVEL == XMEGA256_DEBUG_USER    
    #define XMEGA256_DEBUG_MSG1(msg)    
    #define XMEGA256_DEBUG_MSG2(msg)
    #define XMEGA256_DEBUG_MSG3(msg)
    #define XMEGA256_DEBUG_MSG_USER(msg) XMEGA256_DEBUG_MSG_TEMP(msg)

#elif XMEGA256_DEBUG_LEVEL == XMEGA256_DEBUG_LEVEL1    
    #define XMEGA256_DEBUG_MSG1(msg) XMEGA256_DEBUG_MSG(msg)
    #define XMEGA256_DEBUG_MSG2(msg)
    #define XMEGA256_DEBUG_MSG3(msg)
    #define XMEGA256_DEBUG_MSG_USER(msg)

#elif XMEGA256_DEBUG_LEVEL == XMEGA256_DEBUG_LEVEL2    
    #define XMEGA256_DEBUG_MSG1(msg) XMEGA256_DEBUG_MSG(msg)
    #define XMEGA256_DEBUG_MSG2(msg) XMEGA256_DEBUG_MSG(msg)
    #define XMEGA256_DEBUG_MSG3(msg)
    #define XMEGA256_DEBUG_MSG_USER(msg)

#elif XMEGA256_DEBUG_LEVEL == XMEGA256_DEBUG_LEVEL3    
    #define XMEGA256_DEBUG_MSG1(msg) XMEGA256_DEBUG_MSG(msg)    
    #define XMEGA256_DEBUG_MSG2(msg) XMEGA256_DEBUG_MSG(msg)
    #define XMEGA256_DEBUG_MSG3(msg) XMEGA256_DEBUG_MSG(msg)
    #define XMEGA256_DEBUG_MSG_USER(msg)

#else
    #error Invalid XMEGA256 DEBUG LEVEL, supported levels : XMEGA256_DEBUG_LEVEL1, XMEGA256_DEBUG_LEVEL2, XMEGA256_DEBUG_LEVEL3, XMEGA256_DEBUG_USER
#endif

static void system_clock_init(void);
static void timer_init(void);
static void watchdog_init(void);
static void sleep_init(void);

void platform_init(void)
{
    unsigned char temp;
	
	// Enable Low (with round-robin), medium and high level interrupts
	// All Interrupt vectors are placed at the start of APPLICATION FLASH section
    temp=(PMIC.CTRL & (~(PMIC_RREN_bm | PMIC_IVSEL_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_LOLVLEN_bm))) |
        PMIC_LOLVLEN_bm | PMIC_RREN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
    CCP=CCP_IOREG_gc;
    PMIC.CTRL=temp;
	
    // Set the default priority for round-robin scheduling
    PMIC.INTPRI=0x00;
	
	system_clock_init();
	timer_init();
    sleep_init();
	watchdog_init();
	global_interrupt_enable();
}

static void system_clock_init(void)
{
    unsigned char n;
	
    // External 16000.000 kHz oscillator initialization
    // External Clock Source - Startup Time: 0.4-16 MHz Quartz Crystal - 16k CLK
    OSC.XOSCCTRL=OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
    // Enable the external oscillator/clock source
    OSC.CTRL|=OSC_XOSCEN_bm;

    // Wait for the external oscillator to stabilize
    while ((OSC.STATUS & OSC_XOSCRDY_bm)==0);

    // PLL initialization
    // PLL clock source: External Osc. or Clock
    // PLL multiplication factor: 2
    // PLL output/2: Off
    // PLL frequency: 32.000000 MHz
    // Set the PLL clock source and multiplication factor
    n=(OSC.PLLCTRL & (~(OSC_PLLSRC_gm | OSC_PLLDIV_bm | OSC_PLLFAC_gm))) |
        OSC_PLLSRC_XOSC_gc | (0<<OSC_PLLDIV_bp) | 2;
    CCP=CCP_IOREG_gc;
    OSC.PLLCTRL=n;
    // Enable the PLL
    OSC.CTRL|=OSC_PLLEN_bm;

    // System Clock prescaler A division factor: 1
    // System Clock prescalers B & C division factors: B:1, C:1
    // ClkPer4: 32000.000 kHz
    // ClkPer2: 32000.000 kHz
    // ClkPer:  32000.000 kHz
    // ClkCPU:  32000.000 kHz
    n=(CLK.PSCTRL & (~(CLK_PSADIV_gm | CLK_PSBCDIV1_bm | CLK_PSBCDIV0_bm))) |
        CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc;
    CCP=CCP_IOREG_gc;
    CLK.PSCTRL=n;

    // Wait for the PLL to stabilize
    while ((OSC.STATUS & OSC_PLLRDY_bm)==0);

    // Select the system clock source: Phase Locked Loop
    n=(CLK.CTRL & (~CLK_SCLKSEL_gm)) | CLK_SCLKSEL_PLL_gc;
    CCP=CCP_IOREG_gc;
    CLK.CTRL=n;

    // Disable the unused oscillators: 2 MHz, 32 MHz, internal 32 kHz
    OSC.CTRL&= ~(OSC_RC2MEN_bm | OSC_RC32MEN_bm | OSC_RC32KEN_bm);

    // ClkPer output disabled
    PORTCFG.CLKEVOUT&= ~(PORTCFG_CLKOUTSEL_gm | PORTCFG_CLKOUT_gm);
}

static void timer_init(void)
{
    //unsigned char s;
    unsigned char n;

    // Timer/Counter off
    TIMER_PORT.CTRLA=TC_CLKSEL_OFF_gc;
    // Issue a reset command
    TIMER_PORT.CTRLFSET=TC_CMD_RESET_gc;

	#if CONTROLLER_FREQ == 32000000UL
		// Clock source: ClkPer/256
		TIMER_PORT.CTRLA=TC_CLKSEL_DIV256_gc;
		
	#elif CONTROLLER_FREQ == 16000000UL	
		// Clock source: ClkPer/64
		TIMER_PORT.CTRLA=TC_CLKSEL_DIV64_gc;
		
	#elif CONTROLLER_FREQ == 8000000UL
		// Clock source: ClkPer/64
		TIMER_PORT.CTRLA=TC_CLKSEL_DIV64_gc;
	
	#elif CONTROLLER_FREQ == 2000000UL
		// Clock source: ClkPer/8
		TIMER_PORT.CTRLA=TC_CLKSEL_DIV8_gc;

	#endif
	
    // Mode: Normal Operation, Overflow Int./Event on TOP
    // Compare/Capture on channel A: Off
    // Compare/Capture on channel B: Off
    // Compare/Capture on channel C: Off
    // Compare/Capture on channel D: Off
    TIMER_PORT.CTRLB=(0<<TC0_CCDEN_bp) | (0<<TC0_CCCEN_bp) | (0<<TC0_CCBEN_bp) | (0<<TC0_CCAEN_bp) |
        TC_WGMODE_NORMAL_gc;
    // Capture event source: None
    // Capture event action: None
    TIMER_PORT.CTRLD=TC_EVACT_OFF_gc | TC_EVSEL_OFF_gc;

    // Set Timer/Counter in Normal mode
    TIMER_PORT.CTRLE=TC_BYTEM_NORMAL_gc;

    // Overflow interrupt: High Level
    // Error interrupt: Disabled
    TIMER_PORT.INTCTRLA=TC_ERRINTLVL_OFF_gc | TC_OVFINTLVL_HI_gc;

    // Compare/Capture channel A interrupt: High Level
    // Compare/Capture channel B interrupt: Disabled
    // Compare/Capture channel C interrupt: Disabled
    // Compare/Capture channel D interrupt: Disabled
    TIMER_PORT.INTCTRLB=TC_CCDINTLVL_OFF_gc | TC_CCCINTLVL_OFF_gc | TC_CCBINTLVL_OFF_gc | TC_CCAINTLVL_HI_gc;

    // High resolution extension: Off
    HIRESC.CTRLA&= ~HIRES_HREN0_bm;

    // Disable locking the AWEX configuration registers just to be sure
    n=MCU.AWEXLOCK & (~MCU_AWEXCLOCK_bm);
    CCP=CCP_IOREG_gc;
    MCU.AWEXLOCK=n;

    // Pattern generation: Off
    // Dead time insertion: Off
    AWEXC.CTRL=(0<<AWEX_PGM_bp) | (0<<AWEX_CWCM_bp) | (0<<AWEX_DTICCDEN_bp) | (0<<AWEX_DTICCCEN_bp) | 
        (0<<AWEX_DTICCBEN_bp) | (0<<AWEX_DTICCAEN_bp);

    // Fault protection initialization
    // Fault detection on OCD Break detection: On
    // Fault detection restart mode: Latched Mode
    // Fault detection action: None (Fault protection disabled)
    AWEXC.FDCTRL=(AWEXC.FDCTRL & (~(AWEX_FDDBD_bm | AWEX_FDMODE_bm | AWEX_FDACT_gm))) |
        (0<<AWEX_FDDBD_bp) | (0<<AWEX_FDMODE_bp) | AWEX_FDACT_NONE_gc;
    // Fault detect events: 
    // Event channel 0: Off
    // Event channel 1: Off
    // Event channel 2: Off
    // Event channel 3: Off
    // Event channel 4: Off
    // Event channel 5: Off
    // Event channel 6: Off
    // Event channel 7: Off
    AWEXC.FDEMASK=0b00000000;
    // Make sure the fault detect flag is cleared
    AWEXC.STATUS|=AWEXC.STATUS & AWEX_FDF_bm;

    // Clear the interrupt flags
    TIMER_PORT.INTFLAGS=TIMER_PORT.INTFLAGS;
    // Set Counter register
    TIMER_PORT.CNT=0x0000;
    // Set Period register
    TIMER_PORT.PER=0xF423;
    // Set channel A Compare/Capture register
    TIMER_PORT.CCA=0xFFFF;
    // Set channel B Compare/Capture register
    TIMER_PORT.CCB=0x0000;
    // Set channel C Compare/Capture register
    TIMER_PORT.CCC=0x0000;
    // Set channel D Compare/Capture register
    TIMER_PORT.CCD=0x0000;
}

// Watchdog Timer initialization
void watchdog_init(void)
{
    unsigned char n;

    // Watchdog Timer: On
    // Watchdog timeout period: 2 s
    n=(WDT.CTRL & (~WDT_PER_gm)) | WDT_PER_2KCLK_gc | WDT_ENABLE_bm | WDT_CEN_bm;
    CCP=CCP_IOREG_gc;
    WDT.CTRL=n;
    // Watchdog window mode: Off
    n=(WDT.WINCTRL & (~WDT_WEN_bm)) | WDT_WCEN_bm;
    CCP=CCP_IOREG_gc;
    WDT.WINCTRL=n;
    
    XMEGA256_DEBUG_MSG2(" Watchdog Timer initiated");
}

void watchdog_periodic(void)
{
	#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC )) 
		wdt_reset();
	#elif COMPILER == CODEVISION_AVR
		#asm("wdr")
	#endif
}

static void sleep_init(void)
{
    #if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ) || ( COMPILER == CODEVISION_AVR ))
		sleep_enable();
	#endif
	
    XMEGA256_DEBUG_MSG2("Sleep Mode Initiated");
}

void sleep(void)
{
    XMEGA256_DEBUG_MSG3("Sleeping");
	
	#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ) )
		sleep_mode();
	#elif ( COMPILER == CODEVISION_AVR )
		idle();
	#endif
	
    XMEGA256_DEBUG_MSG3("Woke up");
}

void global_interrupt_enable(void)
{
	// Global enable interrupts
	#if (COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR) || (COMPILER == AVR_GCC)
		sei();
	#elif COMPILER == CODEVISION_AVR
		#asm("sei")
	#endif
	XMEGA256_DEBUG_MSG3("Global Interrupt Enabled");
}

void global_interrupt_disable(void)
{
	// Global enable interrupts
	#if (COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR) || (COMPILER == AVR_GCC)
		cli();
	#elif COMPILER == CODEVISION_AVR
		#asm("cli")
	#endif
	
	XMEGA256_DEBUG_MSG3("Global Interrupt Disabled");
}

#endif

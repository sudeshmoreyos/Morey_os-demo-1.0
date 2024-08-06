/**\addtogroup sources_MEGA8
* @{ \defgroup boot_MEGA8_c
* @{
* \brief Documentaion of boot. source file for Atmega8 controller
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements atmega8 boot functions
* @}
*/

#include "boot.h"

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
	#include <avr/interrupt.h>
#endif

//static mos_uint8_t status_reg;

void global_interrupt_enable(void)
{
	// Global interrupts enable
	#if (COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR) || (COMPILER == AVR_GCC)
		sei();
	#endif
}

void global_interrupt_disable(void)
{
	// Global interrupts disable
	#if (COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR) || (COMPILER == AVR_GCC)
		cli();
	#endif
}

mos_uint8_t is_global_interrupt_enable(void)
{
	if( SREG & (1<<7) )
		return 1;
	else
		return 0;
}

mos_uint8_t is_global_interrupt_disable(void)
{
	if( SREG & (1<<7) )
		return 0;
	else
		return 1;
}

#ifndef DISABLE_OS

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))	
	#include <avr/wdt.h>
	#include <avr/sleep.h>
#endif

#ifdef MEGA8_DEBUG_LEVEL_CONF
	#define MEGA8_DEBUG_LEVEL 		MEGA8_DEBUG_LEVEL_CONF
#else
	#define MEGA8_DEBUG_LEVEL 		MEGA8_DEBUG_NONE
#endif

#if MEGA8_DEBUG_LEVEL > MEGA8_DEBUG_NONE    
    #include "../../../../../os/lib/debug.h"
#endif

#define MEGA8_DEBUG_MSG(msg)			DEBUG_MSG_CONST3_LN("<DEBUG MEGA8> ",NULL,msg)
#define MEGA8_DEBUG_MSG_TEMP(msg) 		DEBUG_MSG_CONST3_LN("<DEBUG MEGA8 USER> ",NULL,msg)
    
#if MEGA8_DEBUG_LEVEL == MEGA8_DEBUG_NONE    
	#define MEGA8_DEBUG_MSG1(msg)
	#define MEGA8_DEBUG_MSG2(msg)
	#define MEGA8_DEBUG_MSG3(msg)
	#define MEGA8_DEBUG_MSG_USER(msg)

#elif MEGA8_DEBUG_LEVEL == MEGA8_DEBUG_USER    
	#define MEGA8_DEBUG_MSG1(msg)    
	#define MEGA8_DEBUG_MSG2(msg)
	#define MEGA8_DEBUG_MSG3(msg)
	#define MEGA8_DEBUG_MSG_USER(msg) 	MEGA8_DEBUG_MSG_TEMP(msg)

#elif MEGA8_DEBUG_LEVEL == MEGA8_DEBUG_LEVEL1    
	#define MEGA8_DEBUG_MSG1(msg) 		MEGA8_DEBUG_MSG(msg)
	#define MEGA8_DEBUG_MSG2(msg)
	#define MEGA8_DEBUG_MSG3(msg)
	#define MEGA8_DEBUG_MSG_USER(msg)

#elif MEGA8_DEBUG_LEVEL == MEGA8_DEBUG_LEVEL2    
	#define MEGA8_DEBUG_MSG1(msg) 		MEGA8_DEBUG_MSG(msg)
	#define MEGA8_DEBUG_MSG2(msg) 		MEGA8_DEBUG_MSG(msg)
	#define MEGA8_DEBUG_MSG3(msg)
	#define MEGA8_DEBUG_MSG_USER(msg)

#elif MEGA8_DEBUG_LEVEL == MEGA8_DEBUG_LEVEL3    
	#define MEGA8_DEBUG_MSG1(msg) 		MEGA8_DEBUG_MSG(msg)    
	#define MEGA8_DEBUG_MSG2(msg) 		MEGA8_DEBUG_MSG(msg)
	#define MEGA8_DEBUG_MSG3(msg) 		MEGA8_DEBUG_MSG(msg)
	#define MEGA8_DEBUG_MSG_USER(msg)

#else
    #error Invalid MEGA8 DEBUG LEVEL, supported levels : MEGA8_DEBUG_LEVEL1, MEGA8_DEBUG_LEVEL2, MEGA8_DEBUG_LEVEL3, MEGA8_DEBUG_USER
#endif

static void timer_init(void);
static void watchdog_init(void);
static void sleep_init(void);

void platform_init(void)
{
	timer_init();
	sleep_init();
	watchdog_init();
	global_interrupt_enable();
}

static void timer_init(void)
{
	#if OS_TIMER_TYPE == TIMER_0 

		// For OS_TIMER_TYPE == TIMER_0, Timer0 is used
		
		#if CONTROLLER_FREQ == 16000000UL
		    
			MEGA8_DEBUG_MSG1("Selected Option : Timer type = TIMER_0  and Controller frequency = 16 Mhz");
			// for 16 MHz crystal frequency, 4 milli second is timer tick time.
			// with max error of 4 milli second
			// timer0 is used and overflow interrupt is enabled to generate desired tick time.
		    
			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 62.500 kHz
			TCCR0=(1<<CS02) | (0<<CS01) | (0<<CS00);
			TCNT0=TIMER_COUNTER_START_VALUE;

			// Timer(s)/Counter(s) Interrupt(s) initialization
			TIMSK |= (1<<TOIE0);

		    
		#elif CONTROLLER_FREQ == 12000000UL
	     
			MEGA8_DEBUG_MSG1("Selected Option : Timer type = TIMER_0  and Controller frequency = 12 Mhz");
			MEGA8_DEBUG_MSG1("For XTAL_FREQ = 12 MHz, 0.27% error in timer calculations, strongly recommended to use other frequencies");
			#warning For XTAL_FREQ = 12 MHz, 0.27% error in timer calculations, strongly recommended to use other frequencies
		    
			// for 12 MHz crystal frequency, 4 milli second is timer tick time.
			// with max error of 4 milli second
			// timer0 is used and overflow interrupt is enabled to generate desired tick time.
		    
			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 46.875 kHz
			TCCR0=(1<<CS02) | (0<<CS01) | (0<<CS00);
			TCNT0=TIMER_COUNTER_START_VALUE;

			// Timer(s)/Counter(s) Interrupt(s) initialization
			TIMSK |= (1<<TOIE0);
		
		#elif CONTROLLER_FREQ == 8000000UL
		    
			MEGA8_DEBUG_MSG1("Selected Option : Timer type = TIMER_0  and Controller frequency = 8 Mhz");
				
			// for 8 MHz crystal frequency, 4 milli second is timer tick time.
			// with max error of 4 milli second
			// timer0 is used and overflow interrupt is enabled to generate desired tick time.
			
			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 31.250 kHz
			TCCR0=(1<<CS02) | (0<<CS01) | (0<<CS00);
			TCNT0=0x83;

			// Timer(s)/Counter(s) Interrupt(s) initialization
			TIMSK |= (1<<TOIE0);
		
		#elif CONTROLLER_FREQ == 1000000UL
		    
			MEGA8_DEBUG_MSG1("Selected Option : Timer type = TIMER_0  and Controller frequency = 1 Mhz");
				
			// for 1 MHz crystal frequency, 8 milli second is timer tick time.
			// timer0 is used and overflow interrupt is enabled to generate desired tick time.
			
			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 15.625 kHz
			TCCR0=(0<<CS02) | (1<<CS01) | (1<<CS00);
			TCNT0=0x83;

			// Timer(s)/Counter(s) Interrupt(s) initialization
			TIMSK |= (1<<TOIE0);
		
		#else 
			#error Incorrect CONTROLLER_FREQ : For mega8 Controller MOREY_OS supports only four frequencies (Hz) i.e. 1000000, 8000000, 12000000 or 16000000
		#endif
		
	#elif OS_TIMER_TYPE == TIMER_2

		// For OS_TIMER_TYPE == TIMER_2, timer2 is used.
		// Clock Resolution is 1 milli second with errors for milli-second delays other than Timer Period/ Timer Tick Time
		// 12 Mhz Frequency generate error of 0.16% for all milli-second delays
        
		#if CONTROLLER_FREQ == 16000000UL
            
			MEGA8_DEBUG_MSG1("Selected Option : Timer type = TIMER_2  and Controller frequency = 16 Mhz");
			
			// for 16 MHz crystal frequency, 16 milli second is timer tick time, 
			// timer2 is used and compare match interrupt is enabled to generate desired tick time.
			
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 15.625 kHz
			// Mode: Normal top=0xFF
			// OC2 output: Disconnected
			// Timer Period: 16 ms
			ASSR=0<<AS2;
			TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (1<<CS21) | (1<<CS20);
			TCNT2=0x00;
			OCR2=COMPARE_MATCH_MAX_TICK;

			// Timer(s)/Counter(s) Interrupt(s) initialization
			TIMSK |= (1<<OCIE2);
            
		#elif CONTROLLER_FREQ == 12000000UL
            
			MEGA8_DEBUG_MSG1("Selected Option : Timer type = TIMER_2  and Controller frequency = 12 Mhz");
			MEGA8_DEBUG_MSG1("For XTAL_FREQ = 12MHz, 0.16% error in timer calculations, strongly recommended to use other frequencies");
			#warning For XTAL_FREQ = 12 MHz, 0.16% error in timer calculations, strongly recommended to use other frequencies
                
			// for 12 MHz crystal frequency, 20 milli second is timer tick time,
			// timer2 is used and compare match interrupt is enabled to generate desired tick time.
            
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 11.719 kHz
			// Mode: Normal top=0xFF
			// OC2 output: Disconnected
			// Timer Period: 20 ms
			ASSR=0<<AS2;
			TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (1<<CS21) | (1<<CS20);
			TCNT2=0x00;
			OCR2=COMPARE_MATCH_MAX_TICK;

			// Timer(s)/Counter(s) Interrupt(s) initialization
			TIMSK |= (1<<OCIE2);
            
        #elif CONTROLLER_FREQ == 8000000UL    
            
			MEGA8_DEBUG_MSG1("Selected Option : Timer type = TIMER_2  and Controller frequency = 8 Mhz");
			
			// for 8 MHz crystal frequency, 8 milli second is timer tick time,
			// timer2 is used and compare match interrupt is enabled to generate desired tick time.
            
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 31.250 kHz
			// Mode: Normal top=0xFF
			// OC2 output: Disconnected
			// Timer Period: 8 ms
			ASSR=0<<AS2;
			TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (1<<CS21) | (0<<CS20);
			TCNT2=0x00;            
			OCR2=COMPARE_MATCH_MAX_TICK;

			// Timer(s)/Counter(s) Interrupt(s) initialization
			TIMSK |= (1<<OCIE2);
            
		#elif CONTROLLER_FREQ == 1000000UL
            
			MEGA8_DEBUG_MSG1("Selected Option : Timer type = TIMER_2  and Controller frequency = 1 Mhz");
			
			// for 1 MHz crystal frequency, 16 milli second is timer tick time,
			// timer2 is used and compare match interrupt is enabled to generate desired tick time

			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 15.625 kHz
			// Mode: Normal top=0xFF
			// OC2 output: Disconnected
			// Timer Period: 16 ms
			ASSR=0<<AS2;
			TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (0<<CS21) | (0<<CS20);
			TCNT2=0x00;
			OCR2=COMPARE_MATCH_MAX_TICK;

			// Timer(s)/Counter(s) Interrupt(s) initialization
			TIMSK |= (1<<OCIE2);
            
		#else 
			#error Incorrect CONTROLLER_FREQ : For mega8 Controller MOREY_OS supports only four frequencies (Hz) i.e. 1000000, 8000000, 12000000 or 16000000
		#endif
        
	#elif OS_TIMER_TYPE == TIMER_1
	
		// For OS_TIMER_TYPE == TIMER_1, timer1 is used
		// Clock Resolution is 1 milli second without any error for any milli-second delays for all frequencies
        
		#if CONTROLLER_FREQ == 16000000UL
        
			MEGA8_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 16 Mhz");
		
			// for 16 MHz crystal frequency, 250 milli second is timer tick time, 
			// with resolution of 1 milli second
			// timer1 is used and compare match A interrupt is enabled to generate desired tick time.

			// Timer/Counter 1 initialization
			// Clock source: System Clock
			// Clock value: 250.000 kHz
			// Mode: Normal top=0xFFFF
			// OC1A output: Disconnected
			// OC1B output: Disconnected
			// Noise Canceler: Off
			// Input Capture on Falling Edge
			// Timer Period: 0.250 s
			// Timer1 Overflow Interrupt: Off
			// Input Capture Interrupt: Off
			// Compare A Match Interrupt: On
			// Compare B Match Interrupt: On
			TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
			TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
			TCNT1=0x0000;
			ICR1=0x0000;
			OCR1A=COMPARE_MATCH_MAX_TICK;
			OCR1B=0xFFFF;

			// Timer(s)/Counter(s) Interrupt(s) initialization
			TIMSK |= (1<<OCIE1A) | (1<<OCIE1B);

		#elif CONTROLLER_FREQ == 12000000UL
            
			MEGA8_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 12 Mhz");
				
			// for 12 MHz crystal frequency, 40 milli second is timer tick time, 
			// with max accuracy of 1 milli second
			// timer1 is used and compare match A interrupt is enabled to generate desired tick time.
		    
			// Timer/Counter 1 initialization
			// Clock source: System Clock
			// Clock value: 1500.000 kHz
			// Mode: Normal top=0xFFFF
			// OC1A output: Disconnected
			// OC1B output: Disconnected
			// Noise Canceler: Off
			// Input Capture on Falling Edge
			// Timer Period: 40 ms
			// Timer1 Overflow Interrupt: Off
			// Input Capture Interrupt: Off
			// Compare A Match Interrupt: On
			// Compare B Match Interrupt: On
			TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
			TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
			TCNT1=0x0000;
			ICR1=0x0000;
			OCR1A=COMPARE_MATCH_MAX_TICK;
			OCR1B=0xFFFF;

			// Timer(s)/Counter(s) Interrupt(s) initialization
			TIMSK |= (1<<OCIE1A) | (1<<OCIE1B);
            
		#elif CONTROLLER_FREQ == 8000000UL
            
			MEGA8_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 8 Mhz");
				
			// for 8 MHz crystal frequency, 500 milli second is timer tick time, 
			// with max accuracy of 1 milli second
			// timer1 is used and compare match A interrupt is enabled to generate desired tick time.
		
			// Timer/Counter 1 initialization
			// Clock source: System Clock
			// Clock value: 125.000 kHz
			// Mode: Normal top=0xFFFF
			// OC1A output: Disconnected
			// OC1B output: Disconnected
			// Noise Canceler: Off
			// Input Capture on Falling Edge
			// Timer Period: 0.500 s
			// Timer1 Overflow Interrupt: Off
			// Input Capture Interrupt: Off
			// Compare A Match Interrupt: On
			// Compare B Match Interrupt: On
			TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
			TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
			TCNT1=0x0000;
			ICR1=0x0000;
			OCR1A=COMPARE_MATCH_MAX_TICK;
			OCR1B=0xFFFF;

			// Timer(s)/Counter(s) Interrupt(s) initialization
			TIMSK |= (1<<OCIE1A) | (1<<OCIE1B);
            
		#elif CONTROLLER_FREQ == 1000000UL
            
			MEGA8_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 1 Mhz");
				
			// for 1 MHz crystal frequency, 500 milli second is timer tick time, 
			// with max accuracy of 1 milli second
			// timer1 is used and compare match A interrupt is enabled to generate desired tick time.
		       
			// Timer/Counter 1 initialization
			// Clock source: System Clock
			// Clock value: 125.000 kHz
			// Mode: Normal top=0xFFFF
			// OC1A output: Disconnected
			// OC1B output: Disconnected
			// Noise Canceler: Off
			// Input Capture on Falling Edge
			// Timer Period: 0.500 s
			// Timer1 Overflow Interrupt: Off
			// Input Capture Interrupt: Off
			// Compare A Match Interrupt: On
			// Compare B Match Interrupt: On
			TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
			TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
			TCNT1=0x0000;
			ICR1=0x0000;
			OCR1A=COMPARE_MATCH_MAX_TICK;
			OCR1B=0xFFFF;

			// Timer(s)/Counter(s) Interrupt(s) initialization
			TIMSK |= (1<<OCIE1A) | (1<<OCIE1B);
            
		#else 
			#error Incorrect CONTROLLER_FREQ : For mega8 Controller MOREY_OS supports only four frequencies (Hz) i.e. 1000000, 8000000, 12000000 or 16000000
		#endif        
    #endif                
}

static void watchdog_init(void)
{  
    // Watchdog Timer initialization
    // Watchdog Timer Prescaler: OSC/2048k
    WDTCR=(1<<WDCE) | (1<<WDE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
    WDTCR=(0<<WDCE) | (1<<WDE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
    
    MEGA8_DEBUG_MSG2("Watchdog Timer initiated");
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
	
    MEGA8_DEBUG_MSG2("Sleep Mode Initiated");
}

void sleep(void)
{
    MEGA8_DEBUG_MSG3("Sleeping");
	
	#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ) )
		sleep_mode();
	#elif ( COMPILER == CODEVISION_AVR )
		idle();
	#endif
	
    MEGA8_DEBUG_MSG3("Woke up");
}

#endif
/**\addtogroup sources_MEGA328P
* @{ \defgroup boot_MEGA328P_c
* @{
* \brief Documentaion of boot.c source file for Atmega328P controller
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements atmega328P boot functions
* @}
*/

#include "boot.h"

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
	#include <avr/interrupt.h>
#endif

void global_interrupt_enable(void)
{
	// Global enable interrupts
	#if (COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR) || (COMPILER == AVR_GCC)
		sei();
	#endif
}

void global_interrupt_disable(void)
{
	// Global enable interrupts
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

#ifdef MEGA328P_DEBUG_LEVEL_CONF
	#define MEGA328P_DEBUG_LEVEL		MEGA328P_DEBUG_LEVEL_CONF
#else
	#define MEGA328P_DEBUG_LEVEL 		MEGA328P_DEBUG_NONE
#endif

#if MEGA328P_DEBUG_LEVEL > MEGA328P_DEBUG_NONE    
	#include "../../../../../os/lib/debug.h"
#endif

#define MEGA328P_DEBUG_MSG(msg)			DEBUG_MSG_CONST3_LN("<DEBUG MEGA328P> ",NULL,msg)
#define MEGA328P_DEBUG_MSG_TEMP(msg)		DEBUG_MSG_CONST3_LN("<DEBUG MEGA328P USER> ",NULL,msg)
    
#if MEGA328P_DEBUG_LEVEL == MEGA328P_DEBUG_NONE    
    #define MEGA328P_DEBUG_MSG1(msg)
    #define MEGA328P_DEBUG_MSG2(msg)
    #define MEGA328P_DEBUG_MSG3(msg)
    #define MEGA328P_DEBUG_MSG_USER(msg)

#elif MEGA328P_DEBUG_LEVEL == MEGA328P_DEBUG_USER    
    #define MEGA328P_DEBUG_MSG1(msg)    
    #define MEGA328P_DEBUG_MSG2(msg)
    #define MEGA328P_DEBUG_MSG3(msg)
    #define MEGA328P_DEBUG_MSG_USER(msg)	MEGA328P_DEBUG_MSG_TEMP(msg)

#elif MEGA328P_DEBUG_LEVEL == MEGA328P_DEBUG_LEVEL1    
    #define MEGA328P_DEBUG_MSG1(msg) 		MEGA328P_DEBUG_MSG(msg)
    #define MEGA328P_DEBUG_MSG2(msg)
    #define MEGA328P_DEBUG_MSG3(msg)
    #define MEGA328P_DEBUG_MSG_USER(msg)

#elif MEGA328P_DEBUG_LEVEL == MEGA328P_DEBUG_LEVEL2    
    #define MEGA328P_DEBUG_MSG1(msg) 		MEGA328P_DEBUG_MSG(msg)
    #define MEGA328P_DEBUG_MSG2(msg) 		MEGA328P_DEBUG_MSG(msg)
    #define MEGA328P_DEBUG_MSG3(msg)
    #define MEGA328P_DEBUG_MSG_USER(msg)

#elif MEGA328P_DEBUG_LEVEL == MEGA328P_DEBUG_LEVEL3    
    #define MEGA328P_DEBUG_MSG1(msg) 		MEGA328P_DEBUG_MSG(msg)    
    #define MEGA328P_DEBUG_MSG2(msg) 		MEGA328P_DEBUG_MSG(msg)
    #define MEGA328P_DEBUG_MSG3(msg) 		MEGA328P_DEBUG_MSG(msg)
    #define MEGA328P_DEBUG_MSG_USER(msg)

#else
	#error Invalid MEGA328P DEBUG LEVEL, supported levels : MEGA328P_DEBUG_LEVEL1, MEGA328P_DEBUG_LEVEL2, MEGA328P_DEBUG_LEVEL3, MEGA328P_DEBUG_USER
#endif

static void timer_init(void);
static void watchdog_init(void);
static void sleep_init(void);

void platform_init(void)
{
	// Oscillator division factor: 1
	CLKPR=(1<<CLKPCE);
	CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
	
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
            
			MEGA328P_DEBUG_MSG1("Selected Option : Timer type = TIMER_0  and Controller frequency = 16 Mhz");
			// for 16 MHz crystal frequency, 16 milli second is timer tick time, 
			// with max error of 0.36 milli second
			// timer0 is used and compare match interrupt is enabled to generate desired tick time.
			
			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 15.625 kHz
			// Mode: Normal top=0xFF
			// OC0A output: Disconnected
			// OC0B output: Disconnected
			// Timer Period: 16 ms
			TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
			TCCR0B=(0<<WGM02) | (1<<CS02) | (0<<CS01) | (1<<CS00);
			TCNT0=0x00;
			OCR0A=COMPARE_MATCH_MAX_TICK;
			OCR0B=0xFF;

			// Timer0 Interrupt(s) initialization
			TIMSK0=(1<<OCIE0B) | (1<<OCIE0A) | (0<<TOIE0);

            
		#elif CONTROLLER_FREQ == 12000000UL
      
			MEGA328P_DEBUG_MSG1("Selected Option : Timer type = TIMER_0 and Controller frequency = 12 Mhz");
			MEGA328P_DEBUG_MSG1("For XTAL_FREQ = 12 MHz, 0.16% error in timer calculations, strongly recommended to use other frequencies");
			#warning For XTAL_FREQ = 12 MHz, 0.16% error in timer calculations, strongly recommended to use other frequencies
            
			// for 12 MHz crystal frequency, 20 milli second is timer tick time, 
			// timer0 is used and compare match interrupt is enabled to generate desired tick time
				
			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 11.719 kHz
			// Mode: Normal top=0xFF
			// OC0A output: Disconnected
			// OC0B output: Disconnected
			// Timer Period: 20 ms
			TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
			TCCR0B=(0<<WGM02) | (1<<CS02) | (0<<CS01) | (1<<CS00);
			TCNT0=0x00;
			OCR0A=COMPARE_MATCH_MAX_TICK;
			OCR0B=0xFF;
				
			// Timer0 Interrupt(s) initialization
			TIMSK0=(1<<OCIE0B) | (1<<OCIE0A) | (0<<TOIE0);
        
		#elif CONTROLLER_FREQ == 8000000UL
            
			MEGA328P_DEBUG_MSG1("Selected Option : Timer type = TIMER_0 and Controller frequency = 8 Mhz");
			
			// for 8 MHz crystal frequency, 8 milli second is timer tick time, 
			// with max error of 0.168 ms
			// timer0 is used and compare match interrupt is enabled to generate desired tick time.
			
			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 31.250 kHz
			// Mode: Normal top=0xFF
			// OC0A output: Disconnected
			// OC0B output: Disconnected
			// Timer Period: 8 ms
			TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
			TCCR0B=(0<<WGM02) | (1<<CS02) | (0<<CS01) | (0<<CS00);
			TCNT0=0x00;
			OCR0A = COMPARE_MATCH_MAX_TICK;
			OCR0B = 0xFF;
			
			// Timer0 Interrupt(s) initialization
			TIMSK0=(1<<OCIE0B) | (1<<OCIE0A) | (0<<TOIE0);
        
		#elif CONTROLLER_FREQ == 1000000UL
		
			MEGA328P_DEBUG_MSG1("Selected Option : Timer type = TIMER_0   and Controller frequency = 1 Mhz");
			
			// for 1 MHz crystal frequency, 16 milli second is timer tick time, 
			// with max error of 0.36 ms
			// timer0 is used and compare match interrupt is enabled to generate desired tick time.
			
			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 15.625 kHz
			// Mode: Normal top=0xFF
			// OC0A output: Disconnected
			// OC0B output: Disconnected
			// Timer Period: 16 ms
			TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
			TCCR0B=(0<<WGM02) | (0<<CS02) | (1<<CS01) | (1<<CS00);
			TCNT0=0x00;
			OCR0A = COMPARE_MATCH_MAX_TICK;
			OCR0B = 0xFF;

			// Timer0 Interrupt(s) initialization
			TIMSK0=(1<<OCIE0B) | (1<<OCIE0A) | (0<<TOIE0);
        
		#else 
			#error Incorrect CONTROLLER_FREQ : For mega328P Controller MOREY_OS supports only four frequencies i.e. 1000000, 8000000, 12000000 or 16000000
		#endif
        
    #elif OS_TIMER_TYPE == TIMER_2

		// For OS_TIMER_TYPE == 2 timer2 is used
        
		#if CONTROLLER_FREQ == 16000000UL
            
			MEGA328P_DEBUG_MSG1("Selected Option : Timer type = TIMER_2 and Controller frequency = 16 Mhz");
			
			// for 16 MHz crystal frequency, 16 milli second is timer tick time, 
			// with max error of 0.36 ms
			// timer2 is used and compare match interrupt is enabled to generate desired tick time.
			
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 15.625 kHz
			// Mode: Normal top=0xFF
			// OC2A output: Disconnected
			// OC2B output: Disconnected
			// Timer Period: 16 ms
			ASSR=(0<<EXCLK) | (0<<AS2);
			TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (0<<WGM20);
			TCCR2B=(0<<WGM22) | (1<<CS22) | (1<<CS21) | (1<<CS20);
			TCNT2=0x00;
			OCR2A = COMPARE_MATCH_MAX_TICK;
			OCR2B = 0xFF;

			// Timer/Counter 2 Interrupt(s) initialization
			TIMSK2=(1<<OCIE2B) | (1<<OCIE2A) | (0<<TOIE2);
						
		#elif CONTROLLER_FREQ == 12000000UL
            
			MEGA328P_DEBUG_MSG1("Selected Option : Timer type = TIMER_2 and Controller frequency = 12 Mhz");
			MEGA328P_DEBUG_MSG1("For XTAL_FREQ = 12MHz, 0.16% error in timer calculations, strongly recommended to use other frequencies");
			#warning For XTAL_FREQ = 12 MHz, 0.16% error in timer calculations, strongly recommended to use other frequencies
                
			// for 12 MHz crystal frequency, 20 milli second is timer tick time, 
			// timer2 is used and compare match interrupt is enabled to generate desired tick time
			
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 11.719 kHz
			// Mode: Normal top=0xFF
			// OC2A output: Disconnected
			// OC2B output: Disconnected
			// Timer Period: 20 ms
			ASSR=(0<<EXCLK) | (0<<AS2);
			TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (0<<WGM20);
			TCCR2B=(0<<WGM22) | (1<<CS22) | (1<<CS21) | (1<<CS20);
			TCNT2=0x00;
			OCR2A = COMPARE_MATCH_MAX_TICK;
			OCR2B = 0xFF;

			// Timer/Counter 2 Interrupt(s) initialization
			TIMSK2=(1<<OCIE2B) | (1<<OCIE2A) | (0<<TOIE2);
            
		#elif CONTROLLER_FREQ == 8000000UL    
            
			MEGA328P_DEBUG_MSG1("Selected Option : Timer type = TIMER_2  and Controller frequency = 8 Mhz");
			
			// for 8 MHz crystal frequency, 8 milli second is timer tick time, 
			// with max error of 0.168 ms
			// timer2 is used and compare match interrupt is enabled to generate desired tick time.
			
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 31.250 kHz
			// Mode: Normal top=0xFF
			// OC2A output: Disconnected
			// OC2B output: Disconnected
			// Timer Period: 8 ms
			ASSR=(0<<EXCLK) | (0<<AS2);
			TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (0<<WGM20);
			TCCR2B=(0<<WGM22) | (1<<CS22) | (1<<CS21) | (0<<CS20);
			TCNT2=0x00;
			OCR2A = COMPARE_MATCH_MAX_TICK;
			OCR2B = 0xFF;

			// Timer/Counter 2 Interrupt(s) initialization
			TIMSK2=(1<<OCIE2B) | (1<<OCIE2A) | (0<<TOIE2);
            
		#elif CONTROLLER_FREQ == 1000000UL
            
			MEGA328P_DEBUG_MSG1("Selected Option : Timer type = TIMER_2  and Controller frequency = 1 Mhz");
			
			// for 1 MHz crystal frequency, 16 milli second is timer tick time, 
			// with max error of 0.36 ms
			// timer2 is used and compare match interrupt is enabled to generate desired tick time
			
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 15.625 kHz
			// Mode: Normal top=0xFF
			// OC2A output: Disconnected
			// OC2B output: Disconnected
			// Timer Period: 16 ms
			ASSR=(0<<EXCLK) | (0<<AS2);
			TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (0<<WGM20);
			TCCR2B=(0<<WGM22) | (1<<CS22) | (0<<CS21) | (0<<CS20);
			TCNT2=0x00;
			OCR2A = COMPARE_MATCH_MAX_TICK;
			OCR2B = 0xFF;

			// Timer/Counter 2 Interrupt(s) initialization
			TIMSK2=(1<<OCIE2B) | (1<<OCIE2A) | (0<<TOIE2);
            
		#else 
			#error Incorrect CONTROLLER_FREQ : For mega328P Controller MOREY_OS supports only four frequencies i.e. 1000000, 8000000, 12000000 or 16000000
		#endif
        
	#elif OS_TIMER_TYPE == TIMER_1
		
		// For OS_TIMER_TYPE == 3 timer1 is used
        
		#if CONTROLLER_FREQ == 16000000UL
        
			MEGA328P_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 16 Mhz");
		
			// for 16 MHz crystal frequency, 250 milli second is timer tick time,
			// timer1 is used and compare match A interrupt is enabled to generate desired tick time.
				
			// Timer/Counter 1 initialization
			// Clock source: System Clock
			// Clock value: 250.000 kHz
			// Mode: Normal top=0xFFFF
			// OC1A output: Disconnected
			// OC1B output: Disconnected
			// Noise Canceler: Off
			// Input Capture on Falling Edge
			// Timer Period: 250 ms
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
			
			// Timer/Counter 1 Interrupt(s) initialization
			TIMSK1=(0<<ICIE1) | (1<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);

		#elif CONTROLLER_FREQ == 12000000UL
            
			MEGA328P_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 12 Mhz");
			
			// for 12 MHz crystal frequency, 40 milli second is timer tick time,
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
			// Compare A Match Interrupt: ON
			// Compare B Match Interrupt: ON
			TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
			TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
			TCNT1=0x0000;
			ICR1=0x0000;
			OCR1A=COMPARE_MATCH_MAX_TICK;
			OCR1B=0xFFFF;

			// Timer/Counter 1 Interrupt(s) initialization
			TIMSK1=(0<<ICIE1) | (1<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);
            
		#elif CONTROLLER_FREQ == 8000000UL
            
			MEGA328P_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 8 Mhz");
			
			// for 8 MHz crystal frequency, 500 milli second is timer tick time, 
			// timer1 is used and compare match A interrupt is enabled to generate desired tick time.

			// Timer/Counter 1 initialization
			// Clock source: System Clock
			// Clock value: 125.000 kHz
			// Mode: Normal top=0xFFFF
			// OC1A output: Disconnected
			// OC1B output: Disconnected
			// Noise Canceler: Off
			// Input Capture on Falling Edge
			// Timer Period: 0.5 s
			// Timer1 Overflow Interrupt: On
			// Input Capture Interrupt: Off
			// Compare A Match Interrupt: ON
			// Compare B Match Interrupt: ON
			TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
			TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
			TCNT1=0x0000;
			ICR1=0x0000;
			OCR1A=COMPARE_MATCH_MAX_TICK;
			OCR1B=0xFFFF;

			// Timer/Counter 1 Interrupt(s) initialization
			TIMSK1=(0<<ICIE1) | (1<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);
            
		#elif CONTROLLER_FREQ == 1000000UL
            
			MEGA328P_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 1 Mhz");
			
			// for 1 MHz crystal frequency, 500 milli second is timer tick time, 
			// timer1 is used and compare match A interrupt is enabled to generate desired tick time.

			// Timer/Counter 1 initialization
			// Clock source: System Clock
			// Clock value: 125.000 kHz
			// Mode: Normal top=0xFFFF
			// OC1A output: Disconnected
			// OC1B output: Disconnected
			// Noise Canceler: Off
			// Input Capture on Falling Edge
			// Timer Period: 0.5 s
			// Timer1 Overflow Interrupt: On
			// Input Capture Interrupt: Off
			// Compare A Match Interrupt: ON
			// Compare B Match Interrupt: ON
			TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
			TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
			TCNT1=0x0000;
			ICR1=0x0000;
			OCR1A=COMPARE_MATCH_MAX_TICK;
			OCR1B=0xFFFF;

			// Timer/Counter 1 Interrupt(s) initialization
			TIMSK1=(0<<ICIE1) | (1<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);
		#else 
			#error Incorrect CONTROLLER_FREQ : For mega328P Controller MOREY_OS supports only four frequencies i.e. 1000000, 8000000, 12000000 or 16000000
		#endif
        
    #endif                
}

static void watchdog_init(void)
{  	
	wdt_reset();
	wdt_enable(WDTO_2S);
	MEGA328P_DEBUG_MSG2("Watchdog Timer initiated");
}

void watchdog_periodic(void)
{
	#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC )) 
		wdt_reset();
	#endif
}

static void sleep_init(void)
{
    #if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
		sleep_enable();
	#endif
	
    MEGA328P_DEBUG_MSG2("Sleep Mode Initiated");
}

void sleep(void)
{
    MEGA328P_DEBUG_MSG3("Sleeping");
	
	#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ) )
		sleep_mode();
	#endif
	
    MEGA328P_DEBUG_MSG3("Woke up");
}

#endif
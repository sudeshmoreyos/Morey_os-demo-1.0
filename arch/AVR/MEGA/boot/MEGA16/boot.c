/**\addtogroup sources_MEGA16
* @{ \defgroup boot_MEGA16_c
* @{
* \brief Documentaion of boot.h header file for atmega16 controller
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements atmega16 boot functions
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
	#elif COMPILER == CODEVISION_AVR
		#asm("sei")
	#endif
	// MEGA16_DEBUG_MSG3("Global Interrupt Enabled");
}

void global_interrupt_disable(void)
{
	// Global enable interrupts
	#if (COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR) || (COMPILER == AVR_GCC)
		cli();
	#elif COMPILER == CODEVISION_AVR
		#asm("cli")
	#endif
	//MEGA16_DEBUG_MSG3("Global Interrupt Disabled");
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
#elif COMPILER == CODEVISION_AVR
	#include <sleep.h>
#endif

#ifdef MEGA16_DEBUG_LEVEL_CONF
	#define MEGA16_DEBUG_LEVEL MEGA16_DEBUG_LEVEL_CONF
#else
	#define MEGA16_DEBUG_LEVEL MEGA16_DEBUG_NONE
#endif

#if MEGA16_DEBUG_LEVEL > MEGA16_DEBUG_NONE    
	#include "../../../../../os/lib/debug.h"
#endif

#define MEGA16_DEBUG_MSG(msg)		DEBUG_MSG_CONST3_LN("<DEBUG MEGA16> ",NULL,msg)
#define MEGA16_DEBUG_MSG_TEMP(msg) 	DEBUG_MSG_CONST3_LN("<DEBUG MEGA16 USER> ",NULL,msg)
    
#if MEGA16_DEBUG_LEVEL == MEGA16_DEBUG_NONE    
    #define MEGA16_DEBUG_MSG1(msg)
    #define MEGA16_DEBUG_MSG2(msg)
    #define MEGA16_DEBUG_MSG3(msg)
    #define MEGA16_DEBUG_MSG_USER(msg)

#elif MEGA16_DEBUG_LEVEL == MEGA16_DEBUG_USER    
    #define MEGA16_DEBUG_MSG1(msg)    
    #define MEGA16_DEBUG_MSG2(msg)
    #define MEGA16_DEBUG_MSG3(msg)
    #define MEGA16_DEBUG_MSG_USER(msg) 	MEGA16_DEBUG_MSG_TEMP(msg)

#elif MEGA16_DEBUG_LEVEL == MEGA16_DEBUG_LEVEL1    
    #define MEGA16_DEBUG_MSG1(msg) 	MEGA16_DEBUG_MSG(msg)
    #define MEGA16_DEBUG_MSG2(msg)
    #define MEGA16_DEBUG_MSG3(msg)
    #define MEGA16_DEBUG_MSG_USER(msg)

#elif MEGA16_DEBUG_LEVEL == MEGA16_DEBUG_LEVEL2    
    #define MEGA16_DEBUG_MSG1(msg) 	MEGA16_DEBUG_MSG(msg)
    #define MEGA16_DEBUG_MSG2(msg) 	MEGA16_DEBUG_MSG(msg)
    #define MEGA16_DEBUG_MSG3(msg)
    #define MEGA16_DEBUG_MSG_USER(msg)

#elif MEGA16_DEBUG_LEVEL == MEGA16_DEBUG_LEVEL3    
    #define MEGA16_DEBUG_MSG1(msg) 	MEGA16_DEBUG_MSG(msg)    
    #define MEGA16_DEBUG_MSG2(msg) 	MEGA16_DEBUG_MSG(msg)
    #define MEGA16_DEBUG_MSG3(msg) 	MEGA16_DEBUG_MSG(msg)
    #define MEGA16_DEBUG_MSG_USER(msg)

#else
    #error Invalid MEGA16 DEBUG LEVEL, supported levels : MEGA16_DEBUG_LEVEL1, MEGA16_DEBUG_LEVEL2, MEGA16_DEBUG_LEVEL3, MEGA16_DEBUG_USER
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
    
		// Please note 12 MHz crystal generates inaccurate timings
		// Hence it is recommended not to use 12 MHz frequency when OS_TIMER_TYPE = TIMER_0 
		
		// Timer0 is used. 
		// Overflow interrupt is used for ticking timer.
		// While Compare match interrupt is used for tickless timer.
		
		#if CONTROLLER_FREQ == 16000000UL
		
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_0 and Controller frequency = 16 Mhz");
			// for 16 MHz crystal frequency, 16 milli second is timer tick time.
			// with max accuracy of 16 milli second

			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 15.625 kHz
			// Mode: Normal top=0xFF
			// OC0 output: Disconnected
			// Timer Period: 16 ms
			TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (1<<CS00);
		    
		#elif CONTROLLER_FREQ == 12000000UL
	      
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_0 and Controller frequency = 12s Mhz");
			MEGA16_DEBUG_MSG1("For FREQ = 12 MHz, 0.16% error in timer calculations, strongly recommended to use other frequencies");
			#warning For FREQ = 12 MHz, 0.16% error in timer calculations, strongly recommended to use other frequencies
		    
			// for 12 MHz crystal frequency, 20 milli second is timer tick time.
			// with inaccurate values for any milli seconds
		    
			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 11.719 kHz
			// Mode: Normal top=0xFF
			// OC0 output: Disconnected
			// Timer Period: 19.968 ms
			TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (1<<CS00);

		
		#elif CONTROLLER_FREQ == 8000000UL
		    
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_0 and Controller frequency = 8 Mhz");
				
			// for 8 MHz crystal frequency, 8 milli second is timer tick time.
			// with max accuracy of 8 milli second
			
			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 31.250 kHz
			// Mode: Normal top=0xFF
			// OC0 output: Disconnected
			// Timer Period: 8 ms
			TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (0<<CS00);
			
		#elif CONTROLLER_FREQ == 4000000UL
		    
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_0 and Controller frequency = 4 Mhz");
				
			// for 4 MHz crystal frequency, 16 milli second is timer tick time.
			// with max accuracy of 16 milli second
			// timer0 is used and compare match interrupt is enabled to generate desired tick time.
			
			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 15.625 kHz
			// Mode: Normal top=0xFF
			// OC0 output: Disconnected
			// Timer Period: 16 ms
			TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (0<<CS00);
			
		#elif CONTROLLER_FREQ == 2000000UL
		    
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_0 and Controller frequency = 2 Mhz");
				
			// for 2 MHz crystal frequency, 8 milli second is timer tick time.
			// with max accuracy of 8 milli second
			// timer0 is used and compare match interrupt is enabled to generate desired tick time.
			
			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 31.250 kHz
			// Mode: Normal top=0xFF
			// OC0 output: Disconnected
			// Timer Period: 8 ms
			TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (1<<CS01) | (1<<CS00);
		
		#elif CONTROLLER_FREQ == 1000000UL
		    
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_0 and Controller frequency = 1 Mhz");
				
			// for 2 MHz crystal frequency, 16 milli second is timer tick time.
			// with max accuracy of 16 milli second
			// timer0 is used and compare match interrupt is enabled to generate desired tick time.
			
			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 15.625 kHz
			// Mode: Normal top=0xFF
			// OC0 output: Disconnected
			// Timer Period: 16 ms
			TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (1<<CS01) | (1<<CS00);
		
		#else 
		    #error Incorrect CONTROLLER_FREQ : For mega16 Controller MOREY_OS supports only Six frequencies i.e. 1000000, 2000000, 4000000, 8000000, 12000000 & 16000000 Hz
		#endif
		
		// Timer0 counter is initialized to a start value so that timer overflow interrupt occurs at desired timer period
		TCNT0=TIMER_COUNTER_START_VALUE;
		// Timero compare match register is initialized to max counter value so that by default it do not generate interrupt
		OCR0=TIMER_COMPARE_MAX_VALUE;

		// Timer(s)/Counter(s) Interrupt(s) initialization
		// Ticking interrupt i.e. Timer0 overflow interrupt is initialized
		// Tickless interrupt i.e. Timer0 compare interrupt is initialized only when tickless timer is scheduled by scheduler ( ptimer.c file )
		TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);
	
	#elif OS_TIMER_TYPE == TIMER_1
    
		// For OS_TIMER_TYPE == TIMER_1 is used, max accuracy achieved is 1 milli second for all frequencies
		
		// Timer1 is used. 
		// Compare match A interrupt is used for tickless timer.
		// While Compare match B interrupt is used for tickless timer.
		
		#if CONTROLLER_FREQ == 16000000UL
		
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_1 and Controller frequency = 16 Mhz");
			
			// for 16 MHz crystal frequency, 250 milli second is timer tick time, 
			// with max accuracy of 1 milli second
			// timer1 is used and compare match A interrupt is enabled to generate desired tick time.
			
			// Timer/Counter 1 initialization
			// Clock source: System Clock
			// Clock value: 250.000 kHz
			// Mode: Normal top=0xFFFF
			// OC1A output: Disconnected
			// OC1B output: Disconnected
			// Noise Canceler: Off
			// Input Capture on Falling Edge
			// Timer Period: 0.26214 s
			// Timer1 Overflow Interrupt: Off
			// Input Capture Interrupt: Off
			// Compare A Match Interrupt: On
			// Compare B Match Interrupt: On
			TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
			TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);

		#elif CONTROLLER_FREQ == 12000000UL
		    
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_1 and Controller frequency = 12 Mhz");
				
			// for 12 MHz crystal frequency, 250 milli second is timer tick time, 
			// with max accuracy of 1 milli second
			// timer1 is used and compare match A interrupt is enabled to generate desired tick time.
		    
			// Timer/Counter 1 initialization
			// Clock source: System Clock
			// Clock value: 187.500 kHz
			// Mode: Normal top=0xFFFF
			// OC1A output: Disconnected
			// OC1B output: Disconnected
			// Noise Canceler: Off
			// Input Capture on Falling Edge
			// Timer Period: 0.34953 s
			// Timer1 Overflow Interrupt: Off
			// Input Capture Interrupt: Off
			// Compare A Match Interrupt: On
			// Compare B Match Interrupt: Off
			TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
			TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);

		#elif CONTROLLER_FREQ == 8000000UL
		    
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_1 and Controller frequency = 8 Mhz");
				
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
			// Timer Period: 0.52429 s
			// Timer1 Overflow Interrupt: Off
			// Input Capture Interrupt: Off
			// Compare A Match Interrupt: On
			// Compare B Match Interrupt: Off
			TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
			TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
		    
		#elif CONTROLLER_FREQ == 4000000UL
		    
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_1 and Controller frequency = 4 Mhz");
				
			// for 4 MHz crystal frequency, 125 milli second is timer tick time, 
			// with max accuracy of 1 milli second
			// timer1 is used and compare match A interrupt is enabled to generate desired tick time.
		
			// Timer/Counter 1 initialization
			// Clock source: System Clock
			// Clock value: 500.000 kHz
			// Mode: Normal top=0xFFFF
			// OC1A output: Disconnected
			// OC1B output: Disconnected
			// Noise Canceler: Off
			// Input Capture on Falling Edge
			// Timer Period: 0.13107 s
			// Timer1 Overflow Interrupt: Off
			// Input Capture Interrupt: Off
			// Compare A Match Interrupt: On
			// Compare B Match Interrupt: Off
			TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
			TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
			
		#elif CONTROLLER_FREQ == 2000000UL
		    
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_1 and Controller frequency = 2 Mhz");
				
			// for 2 MHz crystal frequency, 125 milli second is timer tick time, 
			// with max accuracy of 1 milli second
			// timer1 is used and compare match A interrupt is enabled to generate desired tick time.
		
			// Timer/Counter 1 initialization
			// Clock source: System Clock
			// Clock value: 250.000 kHz
			// Mode: Normal top=0xFFFF
			// OC1A output: Disconnected
			// OC1B output: Disconnected
			// Noise Canceler: Off
			// Input Capture on Falling Edge
			// Timer Period: 0.26214 s
			// Timer1 Overflow Interrupt: Off
			// Input Capture Interrupt: Off
			// Compare A Match Interrupt: On
			// Compare B Match Interrupt: Off
			TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
			TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
			
		#elif CONTROLLER_FREQ == 1000000UL
		    
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_1 and Controller frequency = 1 Mhz");
				
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
			// Timer Period: 0.52429 s
			// Timer1 Overflow Interrupt: Off
			// Input Capture Interrupt: Off
			// Compare A Match Interrupt: On
			// Compare B Match Interrupt: Off
			TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
			TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
		    
		#else 
			#error Incorrect CONTROLLER_FREQ : For mega16 Controller MOREY_OS supports only Six frequencies i.e. 1000000, 2000000, 4000000, 8000000, 12000000 & 16000000 Hz
		#endif
		
		// Initialize Timer counter1 with 0 value when ticking interrupt occurs i.e. Compare match A interrupts
		TCNT1=0x0000;
		// Not used hence dont need to worry about this register
		ICR1=0x0000;
		// Set compare match register A to max ticking value to generate desired max milliseconds per ticking timer cycle
		OCR1A=TICKING_TIMER_MAX_TICK;
		
		// By default compare match B interrupt is always enabled, since timer counter1 do not attain any 
		// value greater than TICKING_TIMER_MAX_TICK. Hence compare match B interrupt or tickless interrupt 
		// only occur if Compare match B register value is less than or equal to TICKING_TIMER_MAX_TICK.
		// So we set compare match register B to max counter value to make sure it do not generate any interrupt by default.
		OCR1B=0xFFFF;

		// Timer(s)/Counter(s) Interrupt(s) initialization
		// Ticking interrupt i.e. Timer0 compare match A interrupt is initialized
		// Tickless interrupt i.e. Timer0 compare match B interrupt is initialized
		TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (1<<OCIE1A) | (1<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
        
	#elif OS_TIMER_TYPE == TIMER_2

		// Please note 12 MHz crystal generates inaccurate timings
		// Hence it is recommended not to use 12 MHz frequency when OS_TIMER_TYPE = TIMER_2 
		
		#if CONTROLLER_FREQ == 16000000UL
		    
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_2 and Controller frequency = 16 Mhz");
			
			// for 16 MHz crystal frequency, 16 milli second is timer tick time
			// with max accuracy of 16 milli second
			// timer2 is used and compare match interrupt is enabled to generate desired tick time.
				
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 15.625 kHz
			// Mode: Normal top=0xFF
			// OC2 output: Disconnected
			// Timer Period: 16 ms
			ASSR=0<<AS2;
			TCCR2=(0<<FOC2) | (0<<WGM20) | (0<<COM21) | (0<<COM20) | (0<<WGM21) | (1<<CS22) | (1<<CS21) | (1<<CS20);
		    
		#elif CONTROLLER_FREQ == 12000000UL
		    
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_2 and Controller frequency = 12 Mhz");
			MEGA16_DEBUG_MSG1("For XTAL_FREQ = 12MHz, 0.16% error in timer calculations, strongly recommended to use other frequencies");
			#warning For XTAL_FREQ = 12 MHz, 0.16% error in timer calculations, strongly recommended to use other frequencies
			
			// for 12 MHz crystal frequency, 20 milli second is timer tick time
			// timer2 is used and compare match interrupt is enabled to generate desired tick time.
		    
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 11.719 kHz
			// Mode: Normal top=0xFF
			// OC2 output: Disconnected
			// Timer Period: 19.968 ms
			ASSR=0<<AS2;
			TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (1<<CS21) | (1<<CS20);

		    
		#elif CONTROLLER_FREQ == 8000000UL    
		    
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_2 and Controller frequency = 8 Mhz");
				
			// for 8 MHz crystal frequency, 8 milli second is timer tick time
			// with max accuracy of 8 milli second
			// timer2 is used and compare match interrupt is enabled to generate desired tick time.
		    
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 31.250 kHz
			// Mode: Normal top=0xFF
			// OC2 output: Disconnected
			// Timer Period: 8 ms
			ASSR=0<<AS2;
			TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (1<<CS21) | (0<<CS20);
			
		#elif CONTROLLER_FREQ == 4000000UL    
		    
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_2 and Controller frequency = 4 Mhz");
				
			// for 4 MHz crystal frequency, 16 milli second is timer tick time
			// with max accuracy of 16 milli seconds
			// timer2 is used and compare match interrupt is enabled to generate desired tick time.
		    
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 15.625 kHz
			// Mode: Normal top=0xFF
			// OC2 output: Disconnected
			// Timer Period: 16 ms
			ASSR=0<<AS2;
			TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (1<<CS21) | (0<<CS20);

		#elif CONTROLLER_FREQ == 2000000UL    
		    
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_2 and Controller frequency = 2 Mhz");
				
			// for 2 MHz crystal frequency, 16 milli second is timer tick time
			// with max accuracy of 16 milli second
			// timer2 is used and compare match interrupt is enabled to generate desired tick time.
		    
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 15.625 kHz
			// Mode: Normal top=0xFF
			// OC2 output: Disconnected
			// Timer Period: 16 ms
			ASSR=0<<AS2;
			TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (0<<CS21) | (1<<CS20);
			
		#elif CONTROLLER_FREQ == 1000000UL
		    
			MEGA16_DEBUG_MSG1("Selected Option : OS_TIMER_TYPE = TIMER_2 and Controller frequency = 1 Mhz");
				
			// for 1 MHz crystal frequency, 16 milli second is timer tick time
			// with max accuracy of 16 milli second
			// timer2 is used and compare match interrupt is enabled to generate desired tick time.
		    
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 15.625 kHz
			// Mode: Normal top=0xFF
			// OC2 output: Disconnected
			// Timer Period: 16 ms
			ASSR=0<<AS2;
			TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (0<<CS21) | (0<<CS20);
		    
		#else 
			#error Incorrect CONTROLLER_FREQ : For mega16 Controller MOREY_OS supports only Six frequencies i.e. 1000000, 2000000, 4000000, 8000000, 12000000 & 16000000 Hz
		#endif
		
		// Timer2 counter is initialized to a start value so that timer overflow interrupt occurs at desired timer period
		TCNT2=TIMER_COUNTER_START_VALUE;
		// Timer2 compare match register is initialized to max counter value so that by default it do not generate interrupt
		OCR2=TIMER_COMPARE_MAX_VALUE;

		// Timer(s)/Counter(s) Interrupt(s) initialization
		// Ticking interrupt i.e. Timer2 overflow interrupt is initialized
		// Tickless interrupt i.e. Timer2 compare interrupt is initialized only when tickless timer is scheduled by scheduler ( ptimer.c file )
		TIMSK=(0<<OCIE2) | (1<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
    #endif                
}

static void watchdog_init(void)
{  
	// Watchdog Timer initialization
	// Watchdog Timer Prescaler: OSC/2048k
	WDTCR=(0<<WDTOE) | (1<<WDE) | (1<<WDP2) | (1<<WDP1) | (1<<WDP0);
    
	MEGA16_DEBUG_MSG2("Watchdog Timer initiated");
}

void watchdog_periodic(void)
{
	#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC )) 
		wdt_reset();
	#endif
}

static void sleep_init(void)
{
	#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ) || ( COMPILER == CODEVISION_AVR ))
		sleep_enable();
	#endif
	
	MEGA16_DEBUG_MSG2("Sleep Mode Initiated");
}

void sleep(void)
{
	MEGA16_DEBUG_MSG3("Sleeping");
	
	#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ) )
		sleep_mode();
	#endif
	
	MEGA16_DEBUG_MSG3("Woke up");
}

void init_ticking_interrupt(void)
{
	TIMER_COUNTER_REGISTER =  TIMER_COUNTER_START_VALUE;	
}

#ifdef PLATFORM_SUPPORT_TICKLESS
void enable_tickless_call(clock_millisecond_t milliseconds_next)
{
	TIMER_COMPARE_REGISTER = milliseconds_next * CLOCK_MILLISECOND_TO_TICK_CONSTANT + TIMER_COUNTER_START_VALUE;
	
	#if OS_TIMER_TYPE == TIMER_0
		TIFR |= (1<<OCF0);
		TIMSK |= (1<<OCIE0);
	
	#elif OS_TIMER_TYPE == TIMER_2
		TIFR |= (1<<OCF2);
		TIMSK |= (1<<OCIE2);
	#endif	
}

void disable_tickless_call(void)
{
	TIMER_COMPARE_REGISTER = TIMER_COMPARE_MAX_VALUE;
	
	#if OS_TIMER_TYPE == TIMER_0
	
		TIMSK &= ~(1<<OCIE0);
		
	#elif OS_TIMER_TYPE == TIMER_2
	
		TIMSK &= ~(1<<OCIE2);
	#endif	
}

clock_millisecond_t read_timer_millisecond(mos_uint8_t * half)
{	
	mos_uint16_t x;
	
	x = TIMER_COUNTER_REGISTER;
	*half = (((x-TIMER_COUNTER_START_VALUE)%CLOCK_MILLISECOND_TO_TICK_CONSTANT)*2)/CLOCK_MILLISECOND_TO_TICK_CONSTANT;
	
	return (x-TIMER_COUNTER_START_VALUE)/CLOCK_MILLISECOND_TO_TICK_CONSTANT;
}
#endif

#endif
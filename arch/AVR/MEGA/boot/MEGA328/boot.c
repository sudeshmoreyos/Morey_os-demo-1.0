/**\addtogroup MEGA328
* @{ \defgroup boot_mega328_c
* @{ \addtogroup boot_mega328_c
* @{
* \brief Documentaion of boot.h header file for atmega328 controller
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements atmega328 boot functions
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
	// MEGA328_DEBUG_MSG3("Global Interrupt Enabled");
}

void global_interrupt_disable(void)
{
	// Global enable interrupts
	#if (COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR) || (COMPILER == AVR_GCC)
		cli();
	#elif COMPILER == CODEVISION_AVR
		#asm("cli")
	#endif
	//MEGA328_DEBUG_MSG3("Global Interrupt Disabled");
}

#ifndef DISABLE_OS

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
	#include <avr/wdt.h>
	#include <avr/sleep.h>
	
#elif COMPILER == CODEVISION_AVR
	#include <sleep.h>
#endif

#ifdef MEGA328_DEBUG_LEVEL_CONF
    #define MEGA328_DEBUG_LEVEL MEGA328_DEBUG_LEVEL_CONF
#else
    #define MEGA328_DEBUG_LEVEL MEGA328_DEBUG_NONE
#endif

#if MEGA328_DEBUG_LEVEL > MEGA328_DEBUG_NONE    
    #include "../../../../../os/lib/debug.h"
#endif

#define MEGA328_DEBUG_MSG(msg)		DEBUG_MSG_LN("<DEBUG MEGA328> ",NULL,msg)
#define MEGA328_DEBUG_MSG_TEMP(msg) DEBUG_MSG_LN("<DEBUG MEGA328 USER> ",NULL,msg)
    
#if MEGA328_DEBUG_LEVEL == MEGA328_DEBUG_NONE    
    #define MEGA328_DEBUG_MSG1(msg)
    #define MEGA328_DEBUG_MSG2(msg)
    #define MEGA328_DEBUG_MSG3(msg)
    #define MEGA328_DEBUG_MSG_USER(msg)

#elif MEGA328_DEBUG_LEVEL == MEGA328_DEBUG_USER    
    #define MEGA328_DEBUG_MSG1(msg)    
    #define MEGA328_DEBUG_MSG2(msg)
    #define MEGA328_DEBUG_MSG3(msg)
    #define MEGA328_DEBUG_MSG_USER(msg) MEGA328_DEBUG_MSG_TEMP(msg)

#elif MEGA328_DEBUG_LEVEL == MEGA328_DEBUG_LEVEL1    
    #define MEGA328_DEBUG_MSG1(msg) MEGA328_DEBUG_MSG(msg)
    #define MEGA328_DEBUG_MSG2(msg)
    #define MEGA328_DEBUG_MSG3(msg)
    #define MEGA328_DEBUG_MSG_USER(msg)

#elif MEGA328_DEBUG_LEVEL == MEGA328_DEBUG_LEVEL2    
    #define MEGA328_DEBUG_MSG1(msg) MEGA328_DEBUG_MSG(msg)
    #define MEGA328_DEBUG_MSG2(msg) MEGA328_DEBUG_MSG(msg)
    #define MEGA328_DEBUG_MSG3(msg)
    #define MEGA328_DEBUG_MSG_USER(msg)

#elif MEGA328_DEBUG_LEVEL == MEGA328_DEBUG_LEVEL3    
    #define MEGA328_DEBUG_MSG1(msg) MEGA328_DEBUG_MSG(msg)    
    #define MEGA328_DEBUG_MSG2(msg) MEGA328_DEBUG_MSG(msg)
    #define MEGA328_DEBUG_MSG3(msg) MEGA328_DEBUG_MSG(msg)
    #define MEGA328_DEBUG_MSG_USER(msg)

#else
    #error Invalid MEGA328 DEBUG LEVEL, supported levels : MEGA328_DEBUG_LEVEL1, MEGA328_DEBUG_LEVEL2, MEGA328_DEBUG_LEVEL3, MEGA328_DEBUG_USER
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
    #if TIMER_TYPE == TIMER_0

        // For TIMER_TYPE == 1 timer0 is used, max accuracy achieved is 4 milli second
        // except for 1 MHz crystal which achieves max accuracy of 8 milli second
        
        #if CONTROLLER_FREQ == 16000000UL
            
            MEGA328_DEBUG_MSG1("Selected Option : Timer type = TIMER_0  and Controller frequency = 16 Mhz");
            // for 16 MHz crystal frequency, 4 milli second is timer tick time, 
            // with max accuracy of 2 milli second
            // timer0 is used and compare match interrupt is enabled to generate desired tick time.
            
			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 62.500 kHz
			// Mode: Normal top=0xFF
			// OC0A output: Disconnected
			// OC0B output: Disconnected
			// Timer Period: 4.096 ms
			TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
			TCCR0B=(0<<WGM02) | (1<<CS02) | (0<<CS01) | (0<<CS00);
			TCNT0=0x00;
			OCR0A = COMPARE_MATCH_MAX_TICK;
			OCR0B = 0xFF;

            // Timer0 Interrupt(s) initialization
			TIMSK0=(1<<OCIE0B) | (1<<OCIE0A) | (0<<TOIE0);

            
        #elif CONTROLLER_FREQ == 12000000UL
      
            MEGA328_DEBUG_MSG1("Selected Option : Timer type = TIMER_0   and Controller frequency = 12 Mhz");
            MEGA328_DEBUG_MSG1("For XTAL_FREQ = 12 MHz, 0.27% error in timer calculations, strongly recommended to use other frequencies");
            #warning For XTAL_FREQ = 12 MHz, 0.27% error in timer calculations, strongly recommended to use other frequencies
            
            // for 12 MHz crystal frequency, 4 milli second is timer tick time, 
            // timer0 is used and compare match interrupt is enabled to generate desired tick time.
            
			// Timer/Counter 0 initialization
			// Clock source: System Clock
			// Clock value: 46.875 kHz
			// Mode: Normal top=0xFF
			// OC0A output: Disconnected
			// OC0B output: Disconnected
			// Timer Period: 4 ms
			TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
			TCCR0B=(0<<WGM02) | (1<<CS02) | (0<<CS01) | (0<<CS00);
			TCNT0=0x00;
			OCR0A = COMPARE_MATCH_MAX_TICK;
			OCR0B = 0xFF;
			
            // Timer0 Interrupt(s) initialization
			TIMSK0=(1<<OCIE0B) | (1<<OCIE0A) | (0<<TOIE0);
        
        #elif CONTROLLER_FREQ == 8000000UL
            
            MEGA328_DEBUG_MSG1("Selected Option : Timer type = TIMER_0   and Controller frequency = 8 Mhz");
			
            // for 8 MHz crystal frequency, 8 milli second is timer tick time, 
            // with max accuracy of 4 milli second
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
            
            MEGA328_DEBUG_MSG1("Selected Option : Timer type = TIMER_0   and Controller frequency = 1 Mhz");
			
            // for 1 MHz crystal frequency, 16 milli second is timer tick time, 
            // with max accuracy of 8 milli second
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
            #error Incorrect CONTROLLER_FREQ : For mega328 Controller MOREY_OS supports only four frequencies i.e. 1000000UL, 8000000UL, 12000000UL & 16000000UL
        #endif
        
    #elif TIMER_TYPE == TIMER_2

        // For TIMER_TYPE == 2 timer2 is used, max accuracy achieved is 2 milli second
        // except for 1 MHz crystal which achieves accuracy of 4 milli second
        
        #if CONTROLLER_FREQ == 16000000UL
            
            MEGA328_DEBUG_MSG1("Selected Option : Timer type = TIMER_2  and Controller frequency = 16 Mhz");
			
            // for 16 MHz crystal frequency, 4 milli second is timer tick time, 
            // with max accuracy of 2 milli second
            // timer2 is used and compare match interrupt is enabled to generate desired tick time.
            
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 62.500 kHz
			// Mode: Normal top=0xFF
			// OC2A output: Disconnected
			// OC2B output: Disconnected
			// Timer Period: 4 ms
			ASSR=(0<<EXCLK) | (0<<AS2);
			TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (0<<WGM20);
			TCCR2B=(0<<WGM22) | (1<<CS22) | (1<<CS21) | (0<<CS20);
			TCNT2=0x00;
			OCR2A = COMPARE_MATCH_MAX_TICK;
			OCR2B = 0xFF;

			// Timer/Counter 2 Interrupt(s) initialization
			TIMSK2=(1<<OCIE2B) | (1<<OCIE2A) | (0<<TOIE2);
						
        #elif CONTROLLER_FREQ == 12000000UL
            
            MEGA328_DEBUG_MSG1("Selected Option : Timer type = TIMER_2 and Controller frequency = 12 Mhz");
            MEGA328_DEBUG_MSG1("For XTAL_FREQ = 12MHz, 0.27% error in timer calculations, strongly recommended to use other frequencies");
            #warning For XTAL_FREQ = 12 MHz, 0.27% error in timer calculations, strongly recommended to use other frequencies
                
            // for 12 MHz crystal frequency, 4 milli second is timer tick time, 
            // with max accuracy of 2 milli second
            // timer2 is used and compare match interrupt is enabled to generate desired tick time.
            
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 46.875 kHz
			// Mode: Normal top=0xFF
			// OC2A output: Disconnected
			// OC2B output: Disconnected
			// Timer Period: 4 ms
			ASSR=(0<<EXCLK) | (0<<AS2);
			TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (0<<WGM20);
			TCCR2B=(0<<WGM22) | (1<<CS22) | (1<<CS21) | (0<<CS20);
			TCNT2=0x00;
			OCR2A = COMPARE_MATCH_MAX_TICK;
			OCR2B = 0xFF;

			// Timer/Counter 2 Interrupt(s) initialization
			TIMSK2=(1<<OCIE2B) | (1<<OCIE2A) | (0<<TOIE2);
            
        #elif CONTROLLER_FREQ == 8000000UL    
            
            MEGA328_DEBUG_MSG1("Selected Option : Timer type = TIMER_2  and Controller frequency = 8 Mhz");
			
            // for 8 MHz crystal frequency, 4 milli second is timer tick time, 
            // with max accuracy of 2 milli second
            // timer2 is used and compare match interrupt is enabled to generate desired tick time.
            
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 62.500 kHz
			// Mode: Normal top=0xFF
			// OC2A output: Disconnected
			// OC2B output: Disconnected
			// Timer Period: 4 ms
			ASSR=(0<<EXCLK) | (0<<AS2);
			TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (0<<WGM20);
			TCCR2B=(0<<WGM22) | (1<<CS22) | (0<<CS21) | (1<<CS20);
			TCNT2=0x00;
			OCR2A = COMPARE_MATCH_MAX_TICK;
			OCR2B = 0xFF;

			// Timer/Counter 2 Interrupt(s) initialization
			TIMSK2=(1<<OCIE2B) | (1<<OCIE2A) | (0<<TOIE2);
            
        #elif CONTROLLER_FREQ == 1000000UL
            
            MEGA328_DEBUG_MSG1("Selected Option : Timer type = TIMER_2  and Controller frequency = 1 Mhz");
			
            // for 1 MHz crystal frequency, 8 milli second is timer tick time, 
            // with max accuracy of 4 milli second
            // timer2 is used and compare match interrupt is enabled to generate desired tick time
            
			// Timer/Counter 2 initialization
			// Clock source: System Clock
			// Clock value: 31.250 kHz
			// Mode: Normal top=0xFF
			// OC2A output: Disconnected
			// OC2B output: Disconnected
			// Timer Period: 8 ms
			ASSR=(0<<EXCLK) | (0<<AS2);
			TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (0<<WGM20);
			TCCR2B=(0<<WGM22) | (0<<CS22) | (1<<CS21) | (1<<CS20);
			TCNT2=0x00;
			OCR2A = COMPARE_MATCH_MAX_TICK;
			OCR2B = 0xFF;

			// Timer/Counter 2 Interrupt(s) initialization
			TIMSK2=(1<<OCIE2B) | (1<<OCIE2A) | (0<<TOIE2);
            
        #else 
            #error Incorrect CONTROLLER_FREQ : For mega328 Controller MOREY_OS supports only four frequencies i.e. 1000000UL, 8000000UL, 12000000UL & 16000000UL
        #endif
        
    #elif TIMER_TYPE == TIMER_1 //3
        // For TIMER_TYPE == 3 timer1 is used, max accuracy achieved is 1 milli second for all frequencies
        
        #if CONTROLLER_FREQ == 16000000UL
        
		MEGA328_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 16 Mhz");
		
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
            // Timer Period: 0.25 s
            // Timer1 Overflow Interrupt: On
            // Input Capture Interrupt: Off
            // Compare A Match Interrupt: ON
            // Compare B Match Interrupt: ON
            TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
            TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
            TCNT1H=0x00;
            TCNT1L=0x00;
            ICR1H=0x00;
            ICR1L=0x00;
            OCR1AH = (COMPARE_MATCH_MAX_TICK & 0xFF00)>>8;
            OCR1AL = COMPARE_MATCH_MAX_TICK & 0xFF;
            OCR1BH=0xFF;
            OCR1BL=0xFF;

			// Timer/Counter 1 Interrupt(s) initialization
			TIMSK1=(0<<ICIE1) | (1<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);

        #elif CONTROLLER_FREQ == 12000000UL
            
            MEGA328_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 12 Mhz");
			
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
            // Timer1 Overflow Interrupt: On
            // Input Capture Interrupt: Off
            // Compare A Match Interrupt: ON
            // Compare B Match Interrupt: ON
            TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
            TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
            TCNT1H=0x00;
            TCNT1L=0x00;
            ICR1H=0x00;
            ICR1L=0x00;
            OCR1AH = (COMPARE_MATCH_MAX_TICK & 0xFF00)>>8;
            OCR1AL = COMPARE_MATCH_MAX_TICK & 0xFF;
            OCR1BH=0xFF;
            OCR1BL=0xFF;

			// Timer/Counter 1 Interrupt(s) initialization
			TIMSK1=(0<<ICIE1) | (1<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);
            
        #elif CONTROLLER_FREQ == 8000000UL
            
            MEGA328_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 8 Mhz");
			
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
            // Timer Period: 0.5 s
            // Timer1 Overflow Interrupt: On
            // Input Capture Interrupt: Off
            // Compare A Match Interrupt: ON
            // Compare B Match Interrupt: ON
            TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
            TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
            TCNT1H=0x00;
            TCNT1L=0x00;
            ICR1H=0x00;
            ICR1L=0x00;
            OCR1AH = (COMPARE_MATCH_MAX_TICK & 0xFF00)>>8;
            OCR1AL = COMPARE_MATCH_MAX_TICK & 0xFF;
            OCR1BH=0xFF;
            OCR1BL=0xFF;

			// Timer/Counter 1 Interrupt(s) initialization
			TIMSK1=(0<<ICIE1) | (1<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);
            
         #elif CONTROLLER_FREQ == 1000000UL
            
            MEGA328_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 1 Mhz");
			
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
            // Timer Period: 0.5 s
            // Timer1 Overflow Interrupt: On
            // Input Capture Interrupt: Off
            // Compare A Match Interrupt: ON
            // Compare B Match Interrupt: ON
            TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
            TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
            TCNT1H=0x00;
            TCNT1L=0x00;
            ICR1H=0x00;
            ICR1L=0x00;
            OCR1AH = (COMPARE_MATCH_MAX_TICK & 0xFF00)>>8;
            OCR1AL = COMPARE_MATCH_MAX_TICK & 0xFF;
            OCR1BH=0xFF;
            OCR1BL=0xFF;

			// Timer/Counter 1 Interrupt(s) initialization
			TIMSK1=(0<<ICIE1) | (1<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);
        #else 
            #error Incorrect CONTROLLER_FREQ : For mega328 Controller MOREY_OS supports only four frequencies i.e. 1000000UL, 8000000UL, 12000000UL & 16000000UL
        #endif
        
    #endif                
}

static void watchdog_init(void)
{  
    // Watchdog Timer initialization
    // Watchdog Timer Prescaler: OSC/1024k
    WDTCSR|=(1<<WDCE) | (1<<WDE);
	WDTCSR=(0<<WDIF) | (0<<WDIE) | (1<<WDP3) | (0<<WDCE) | (1<<WDE) | (0<<WDP2) | (0<<WDP1) | (1<<WDP0);
    
    MEGA328_DEBUG_MSG2("Watchdog Timer initiated");
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
	
    MEGA328_DEBUG_MSG2("Sleep Mode Initiated");
}

void sleep(void)
{
    MEGA328_DEBUG_MSG3("Sleeping");
	
	#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ) )
		sleep_mode();
	#elif ( COMPILER == CODEVISION_AVR )
		idle();
	#endif
	
    MEGA328_DEBUG_MSG3("Woke up");
}

#endif
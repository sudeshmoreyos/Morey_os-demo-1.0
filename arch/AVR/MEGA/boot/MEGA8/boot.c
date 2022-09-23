/**\addtogroup MEGA8
* @{ \defgroup boot_mega8_c
* @{ \addtogroup boot_mega8_c
* @{
* \brief Documentaion of boot.h header file for atmega8 controller
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements atmega8 boot functions
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
	// MEGA8_DEBUG_MSG3("Global Interrupt Enabled");
}

void global_interrupt_disable(void)
{
	// Global enable interrupts
	#if (COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR) || (COMPILER == AVR_GCC)
		cli();
	#elif COMPILER == CODEVISION_AVR
		#asm("cli")
	#endif
	//MEGA8_DEBUG_MSG3("Global Interrupt Disabled");
}

#ifndef DISABLE_OS

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
	

	#include <avr/wdt.h>
	#include <avr/sleep.h>
	
#elif COMPILER == CODEVISION_AVR
	#include <sleep.h>
#endif

#ifdef MEGA8_DEBUG_LEVEL_CONF
    #define MEGA8_DEBUG_LEVEL MEGA8_DEBUG_LEVEL_CONF
#else
    #define MEGA8_DEBUG_LEVEL MEGA8_DEBUG_NONE
#endif

#if MEGA8_DEBUG_LEVEL > MEGA8_DEBUG_NONE    
    #include "../../../../../os/lib/debug.h"
#endif

#define MEGA8_DEBUG_MSG(msg)		DEBUG_MSG_LN("<DEBUG MEGA8> ",NULL,msg)
#define MEGA8_DEBUG_MSG_TEMP(msg) 	DEBUG_MSG_LN("<DEBUG MEGA8 USER> ",NULL,msg)
    
#if MEGA8_DEBUG_LEVEL == MEGA8_DEBUG_NONE    
    #define MEGA8_DEBUG_MSG1(msg)
    #define MEGA8_DEBUG_MSG2(msg)
    #define MEGA8_DEBUG_MSG3(msg)
    #define MEGA8_DEBUG_MSG_USER(msg)

#elif MEGA8_DEBUG_LEVEL == MEGA8_DEBUG_USER    
    #define MEGA8_DEBUG_MSG1(msg)    
    #define MEGA8_DEBUG_MSG2(msg)
    #define MEGA8_DEBUG_MSG3(msg)
    #define MEGA8_DEBUG_MSG_USER(msg) MEGA8_DEBUG_MSG_TEMP(msg)

#elif MEGA8_DEBUG_LEVEL == MEGA8_DEBUG_LEVEL1    
    #define MEGA8_DEBUG_MSG1(msg) MEGA8_DEBUG_MSG(msg)
    #define MEGA8_DEBUG_MSG2(msg)
    #define MEGA8_DEBUG_MSG3(msg)
    #define MEGA8_DEBUG_MSG_USER(msg)

#elif MEGA8_DEBUG_LEVEL == MEGA8_DEBUG_LEVEL2    
    #define MEGA8_DEBUG_MSG1(msg) MEGA8_DEBUG_MSG(msg)
    #define MEGA8_DEBUG_MSG2(msg) MEGA8_DEBUG_MSG(msg)
    #define MEGA8_DEBUG_MSG3(msg)
    #define MEGA8_DEBUG_MSG_USER(msg)

#elif MEGA8_DEBUG_LEVEL == MEGA8_DEBUG_LEVEL3    
    #define MEGA8_DEBUG_MSG1(msg) MEGA8_DEBUG_MSG(msg)    
    #define MEGA8_DEBUG_MSG2(msg) MEGA8_DEBUG_MSG(msg)
    #define MEGA8_DEBUG_MSG3(msg) MEGA8_DEBUG_MSG(msg)
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
    #if TIMER_TYPE == TIMER_0 //1

        // For TIMER_TYPE == 1 timer0 is used, max accuracy achieved is 4 milli second
        // except for 1 MHz crystal which achieves max accuracy of 8 milli second
        
        #if CONTROLLER_FREQ == 16000000UL
            
            MEGA8_DEBUG_MSG1("Selected Option : Timer type = 1  and Controller frequency = 16 Mhz");
            // for 16 MHz crystal frequency, 4 milli second is timer tick time.
            // timer0 is used and overflow interrupt is enabled to generate desired tick time.
            
            // Timer/Counter 0 initialization
            // Clock source: System Clock
            // Clock value: 62.500 kHz
            TCCR0=(1<<CS02) | (0<<CS01) | (0<<CS00);
            TCNT0=0x06;

            // Timer(s)/Counter(s) Interrupt(s) initialization
            TIMSK |= (1<<TOIE0);

            
        #elif CONTROLLER_FREQ == 12000000UL
      
            MEGA8_DEBUG_MSG1("Selected Option : Timer type = 1  and Controller frequency = 12 Mhz");
            MEGA8_DEBUG_MSG1("For XTAL_FREQ = 12 MHz, 0.27% error in timer calculations, strongly recommended to use other frequencies");
            #warning For XTAL_FREQ = 12 MHz, 0.27% error in timer calculations, strongly recommended to use other frequencies
            
            // for 12 MHz crystal frequency, 4 milli second is timer tick time.
            // timer0 is used and overflow interrupt is enabled to generate desired tick time.
            
            // Timer/Counter 0 initialization
            // Clock source: System Clock
            // Clock value: 46.875 kHz
            TCCR0=(1<<CS02) | (0<<CS01) | (0<<CS00);
            TCNT0=0x44;

            // Timer(s)/Counter(s) Interrupt(s) initialization
            TIMSK |= (1<<TOIE0);
        
        #elif CONTROLLER_FREQ == 8000000UL
            
            MEGA8_DEBUG_MSG1("Selected Option : Timer type = 1  and Controller frequency = 8 Mhz");
			
            // for 8 MHz crystal frequency, 4 milli second is timer tick time.
            // timer0 is used and overflow interrupt is enabled to generate desired tick time.
                
            // Timer/Counter 0 initialization
            // Clock source: System Clock
            // Clock value: 31.250 kHz
            TCCR0=(1<<CS02) | (0<<CS01) | (0<<CS00);
            TCNT0=0x83;

            // Timer(s)/Counter(s) Interrupt(s) initialization
            TIMSK |= (1<<TOIE0);
        
        #elif CONTROLLER_FREQ == 1000000UL
            
            MEGA8_DEBUG_MSG1("Selected Option : Timer type = 1  and Controller frequency = 1 Mhz");
			
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
            #error For mega8 Controller MOREY_OS supports only four frequencies i.e. 1, 8, 12 & 16 MHz
        #endif
        
    #elif TIMER_TYPE == TIMER_2 //2

        // For TIMER_TYPE == 2 timer2 is used, max accuracy achieved is 2 milli second
        // except for 1 MHz crystal which achieves accuracy of 4 milli second
        
        #if CONTROLLER_FREQ == 16000000UL
            
            MEGA8_DEBUG_MSG1("Selected Option : Timer type = 2  and Controller frequency = 16 Mhz");
			
            // for 16 MHz crystal frequency, 4 milli second is timer tick time, 
            // with max accuracy of 2 milli second
            // timer2 is used and compare match interrupt is enabled to generate desired tick time.
            
            // Timer/Counter 2 initialization
            // Clock source: System Clock
            // Clock value: 62.500 kHz
            // Mode: Normal top=0xFF
            // OC2 output: Disconnected
            // Timer Period: 4 ms
            ASSR=0<<AS2;
			TCCR2=(0<<FOC2) | (0<<WGM20) | (0<<COM21) | (0<<COM20) | (0<<WGM21) | (1<<CS22) | (1<<CS21) | (0<<CS20);
            TCNT2=0x00;
            OCR2=0xFA;

            // Timer(s)/Counter(s) Interrupt(s) initialization
            TIMSK |= (1<<OCIE2);
            
        #elif CONTROLLER_FREQ == 12000000UL
            
            MEGA8_DEBUG_MSG1("Selected Option : Timer type = 2  and Controller frequency = 12 Mhz");
            MEGA8_DEBUG_MSG1("For XTAL_FREQ = 12MHz, 0.27% error in timer calculations, strongly recommended to use other frequencies");
            #warning For XTAL_FREQ = 12 MHz, 0.27% error in timer calculations, strongly recommended to use other frequencies
                
            // for 12 MHz crystal frequency, 4 milli second is timer tick time, 
            // with max accuracy of 2 milli second
            // timer2 is used and compare match interrupt is enabled to generate desired tick time.
            
            // Timer/Counter 2 initialization
            // Clock source: System Clock
            // Clock value: 46.875 kHz
            // Mode: Normal top=0xFF
            // OC2 output: Disconnected
            // Timer Period: 4.0107 ms
            ASSR=0<<AS2;
			TCCR2=(0<<FOC2) | (0<<WGM20) | (0<<COM21) | (0<<COM20) | (0<<WGM21) | (1<<CS22) | (1<<CS21) | (0<<CS20);
            TCNT2=0x00;
            OCR2=0xBC;

            // Timer(s)/Counter(s) Interrupt(s) initialization
            TIMSK |= (1<<OCIE2);
            
        #elif CONTROLLER_FREQ == 8000000UL    
            
            MEGA8_DEBUG_MSG1("Selected Option : Timer type = 2  and Controller frequency = 8 Mhz");
			
            // for 8 MHz crystal frequency, 4 milli second is timer tick time, 
            // with max accuracy of 2 milli second
            // timer2 is used and compare match interrupt is enabled to generate desired tick time.
            
            // Timer/Counter 2 initialization
            // Clock source: System Clock
            // Clock value: 62.500 kHz
            // Mode: Normal top=0xFF
            // OC2 output: Disconnected
            // Timer Period: 4 ms
            ASSR=0<<AS2;
            TCCR2=(0<<FOC2) | (0<<WGM20) | (0<<COM21) | (0<<COM20) | (0<<WGM21) | (1<<CS22) | (0<<CS21) | (1<<CS20);
            TCNT2=0x00;
            OCR2=0xFA;

            // Timer(s)/Counter(s) Interrupt(s) initialization
            TIMSK |= (1<<OCIE2);
            
        #elif CONTROLLER_FREQ == 1000000UL
            
            MEGA8_DEBUG_MSG1("Selected Option : Timer type = 2  and Controller frequency = 1 Mhz");
			
            // for 1 MHz crystal frequency, 8 milli second is timer tick time, 
            // with max accuracy of 4 milli second
            // timer2 is used and compare match interrupt is enabled to generate desired tick time
            
            // Timer/Counter 2 initialization
            // Clock source: System Clock
            // Clock value: 31.250 kHz
            // Mode: Normal top=0xFF
            // OC2 output: Disconnected
            // Timer Period: 8 ms
            ASSR=0<<AS2;
			TCCR2=(0<<FOC2) | (0<<WGM20) | (0<<COM21) | (0<<COM20) | (0<<WGM21) | (0<<CS22) | (1<<CS21) | (1<<CS20);
            TCNT2=0x00;
            OCR2=0xFA;

            // Timer(s)/Counter(s) Interrupt(s) initialization
            TIMSK |= (1<<OCIE2);
            
        #else 
            #error For mega8 Controller MOREY_OS supports only four frequencies i.e. 1, 8, 12 & 16 MHz
        #endif
        
    #elif TIMER_TYPE == TIMER_1 //3
        // For TIMER_TYPE == 3 timer1 is used, max accuracy achieved is 1 milli second for all frequencies
        
        #if CONTROLLER_FREQ == 16000000UL
        
        //MEGA8_DEBUG_MSG1_NO_TIME("Selected Option : Timer type = 3  and Controller frequency = 16 Mhz");
		MEGA8_DEBUG_MSG1("Selected Option : Timer type = 3  and Controller frequency = 16 Mhz");
		
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
            OCR1AH=0xF4;
            OCR1AL=0x24;
            OCR1BH=0xFF;
            OCR1BL=0xFF;

            // Timer(s)/Counter(s) Interrupt(s) initialization
            TIMSK |= (1<<OCIE1A) | (1<<OCIE1B);

        #elif CONTROLLER_FREQ == 12000000UL
            
            MEGA8_DEBUG_MSG1("Selected Option : Timer type = 3  and Controller frequency = 12 Mhz");
			
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
            OCR1AH=0xEA;
            OCR1AL=0x60;
            OCR1BH=0xFF;
            OCR1BL=0xFF;

            // Timer(s)/Counter(s) Interrupt(s) initialization
            TIMSK |= (1<<OCIE1A) | (1<<OCIE1B);
            
        #elif CONTROLLER_FREQ == 8000000UL
            
            MEGA8_DEBUG_MSG1("Selected Option : Timer type = 3  and Controller frequency = 8 Mhz");
			
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
            OCR1AH=0xF4;
            OCR1AL=0x24;
            OCR1BH=0xFF;
            OCR1BL=0xFF;

            // Timer(s)/Counter(s) Interrupt(s) initialization
            TIMSK |= (1<<OCIE1A) | (1<<OCIE1B);
            
         #elif CONTROLLER_FREQ == 1000000UL
            
            MEGA8_DEBUG_MSG1("Selected Option : Timer type = 3  and Controller frequency = 1 Mhz");
			
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
            OCR1AH=0xF4;
            OCR1AL=0x24;
            OCR1BH=0xFF;
            OCR1BL=0xFF;

            // Timer(s)/Counter(s) Interrupt(s) initialization
            TIMSK |= (1<<OCIE1A) | (1<<OCIE1B);
            
        #else 
            #error For mega8 Controller MOREY_OS supports only four frequencies i.e. 1, 8, 12 & 16 MHz
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
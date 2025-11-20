/**\addtogroup sources_MEGA2560
* @{ \defgroup boot_MEGA2560_c
* @{
* \brief Documentaion of boot.h header file for atmega2560 controller
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements atmega2560 boot functions
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
	// MEGA2560_DEBUG_MSG3("Global Interrupt Enabled");
}

void global_interrupt_disable(void)
{
	// Global enable interrupts
	#if (COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR) || (COMPILER == AVR_GCC)
		cli();
	#endif
	//MEGA2560_DEBUG_MSG3("Global Interrupt Disabled");
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

#ifdef MEGA2560_DEBUG_LEVEL_CONF
    #define MEGA2560_DEBUG_LEVEL MEGA2560_DEBUG_LEVEL_CONF
#else
    #define MEGA2560_DEBUG_LEVEL MEGA2560_DEBUG_NONE
#endif

#if MEGA2560_DEBUG_LEVEL > MEGA2560_DEBUG_NONE    
    #include "../../../../../os/lib/debug.h"
#endif

#define MEGA2560_DEBUG_MSG(msg)			DEBUG_MSG_CONST3_LN("<DEBUG MEGA328PB> ",NULL,msg)
#define MEGA2560_DEBUG_MSG_TEMP(msg) 	DEBUG_MSG_CONST3_LN("<DEBUG MEGA328PB USER> ",NULL,msg)
    
#if MEGA2560_DEBUG_LEVEL == MEGA2560_DEBUG_NONE    
    #define MEGA2560_DEBUG_MSG1(msg)
    #define MEGA2560_DEBUG_MSG2(msg)
    #define MEGA2560_DEBUG_MSG3(msg)
    #define MEGA2560_DEBUG_MSG_USER(msg)

#elif MEGA2560_DEBUG_LEVEL == MEGA2560_DEBUG_USER    
    #define MEGA2560_DEBUG_MSG1(msg)    
    #define MEGA2560_DEBUG_MSG2(msg)
    #define MEGA2560_DEBUG_MSG3(msg)
    #define MEGA2560_DEBUG_MSG_USER(msg) MEGA2560_DEBUG_MSG_TEMP(msg)

#elif MEGA2560_DEBUG_LEVEL == MEGA2560_DEBUG_LEVEL1    
    #define MEGA2560_DEBUG_MSG1(msg) MEGA2560_DEBUG_MSG(msg)
    #define MEGA2560_DEBUG_MSG2(msg)
    #define MEGA2560_DEBUG_MSG3(msg)
    #define MEGA2560_DEBUG_MSG_USER(msg)

#elif MEGA2560_DEBUG_LEVEL == MEGA2560_DEBUG_LEVEL2    
    #define MEGA2560_DEBUG_MSG1(msg) MEGA2560_DEBUG_MSG(msg)
    #define MEGA2560_DEBUG_MSG2(msg) MEGA2560_DEBUG_MSG(msg)
    #define MEGA2560_DEBUG_MSG3(msg)
    #define MEGA2560_DEBUG_MSG_USER(msg)

#elif MEGA2560_DEBUG_LEVEL == MEGA2560_DEBUG_LEVEL3    
    #define MEGA2560_DEBUG_MSG1(msg) MEGA2560_DEBUG_MSG(msg)    
    #define MEGA2560_DEBUG_MSG2(msg) MEGA2560_DEBUG_MSG(msg)
    #define MEGA2560_DEBUG_MSG3(msg) MEGA2560_DEBUG_MSG(msg)
    #define MEGA2560_DEBUG_MSG_USER(msg)

#else
    #error Invalid MEGA2560 DEBUG LEVEL, supported levels : MEGA2560_DEBUG_LEVEL1, MEGA2560_DEBUG_LEVEL2, MEGA2560_DEBUG_LEVEL3, MEGA2560_DEBUG_USER
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

        // For OS_TIMER_TYPE == TIMER_0 timer0 is used, max accuracy achieved is 4 milli second
        // except for 1 MHz crystal which achieves max accuracy of 8 milli second
        
        #if CONTROLLER_FREQ == 16000000UL
            
            MEGA2560_DEBUG_MSG1("Selected Option : Timer type = TIMER_0  and Controller frequency = 16 Mhz");
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
      
            MEGA2560_DEBUG_MSG1("Selected Option : Timer type = TIMER_0   and Controller frequency = 12 Mhz");
            MEGA2560_DEBUG_MSG1("For XTAL_FREQ = 12 MHz, 0.27% error in timer calculations, strongly recommended to use other frequencies");
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
            
            MEGA2560_DEBUG_MSG1("Selected Option : Timer type = TIMER_0   and Controller frequency = 8 Mhz");
			
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
            
            MEGA2560_DEBUG_MSG1("Selected Option : Timer type = TIMER_0   and Controller frequency = 1 Mhz");
			
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
            #error Incorrect CONTROLLER_FREQ : For mega2560 Controller MOREY_OS supports only four frequencies i.e. 1000000UL, 8000000UL, 12000000UL & 16000000UL
        #endif
        
    #elif OS_TIMER_TYPE == TIMER_2

        // For OS_TIMER_TYPE == 2 timer2 is used, max accuracy achieved is 2 milli second
        // except for 1 MHz crystal which achieves accuracy of 4 milli second
        
        #if CONTROLLER_FREQ == 16000000UL
            
            MEGA2560_DEBUG_MSG1("Selected Option : Timer type = TIMER_2  and Controller frequency = 16 Mhz");
			
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
            
            MEGA2560_DEBUG_MSG1("Selected Option : Timer type = TIMER_2 and Controller frequency = 12 Mhz");
            MEGA2560_DEBUG_MSG1("For XTAL_FREQ = 12MHz, 0.27% error in timer calculations, strongly recommended to use other frequencies");
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
            
            MEGA2560_DEBUG_MSG1("Selected Option : Timer type = TIMER_2  and Controller frequency = 8 Mhz");
			
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
            
            MEGA2560_DEBUG_MSG1("Selected Option : Timer type = TIMER_2  and Controller frequency = 1 Mhz");
			
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
            #error Incorrect CONTROLLER_FREQ : For mega2560 Controller MOREY_OS supports only four frequencies i.e. 1000000UL, 8000000UL, 12000000UL & 16000000UL
        #endif
        
    #elif (OS_TIMER_TYPE == TIMER_1) || (OS_TIMER_TYPE == TIMER_3) || (OS_TIMER_TYPE == TIMER_4) || (OS_TIMER_TYPE == TIMER_5)
        // For OS_TIMER_TYPE == TIMER_1/3/4/5 timer1/3/4/5 is used, max accuracy achieved is 1 milli second for all frequencies
	
		#if OS_TIMER_TYPE == TIMER_1
		
			#define TCCRA_TEMP 	TCCR1A
			#define TCCRB_TEMP 	TCCR1B
			#define TCNTH_TEMP 	TCNT1H
            #define TCNTL_TEMP	TCNT1L
            #define ICRH_TEMP 	ICR1H
			#define ICRL_TEMP 	ICR1L
			#define OCRAH_TEMP 	OCR1AH
			#define OCRAL_TEMP 	OCR1AL
			#define OCRBH_TEMP 	OCR1BH
			#define OCRBL_TEMP 	OCR1BL
			#define TIMSK_TEMP	TIMSK1
		
		#elif OS_TIMER_TYPE == TIMER_3
			
			#define TCCRA_TEMP 	TCCR3A
			#define TCCRB_TEMP 	TCCR3B
			#define TCNTH_TEMP 	TCNT3H
            #define TCNTL_TEMP	TCNT3L
            #define ICRH_TEMP 	ICR3H
			#define ICRL_TEMP 	ICR3L
			#define OCRAH_TEMP 	OCR3AH
			#define OCRAH_TEMP 	OCR3AH
			#define OCRAL_TEMP 	OCR3AL
			#define OCRBH_TEMP 	OCR3BH
			#define OCRBL_TEMP 	OCR3BL
			#define TIMSK_TEMP	TIMSK3
		
		#elif OS_TIMER_TYPE == TIMER_4
			
			#define TCCRA_TEMP 	TCCR4A
			#define TCCRB_TEMP 	TCCR4B
			#define TCNTH_TEMP 	TCNT4H
            #define TCNTL_TEMP	TCNT4L
            #define ICRH_TEMP 	ICR4H
			#define ICRL_TEMP 	ICR4L
			#define OCRAH_TEMP 	OCR4AH
			#define OCRAH_TEMP 	OCR4AH
			#define OCRAL_TEMP 	OCR4AL
			#define OCRBH_TEMP 	OCR4BH
			#define OCRBL_TEMP 	OCR4BL
			#define TIMSK_TEMP	TIMSK4
		
		#elif OS_TIMER_TYPE == TIMER_5
			
			#define TCCRA_TEMP 	TCCR5A
			#define TCCRB_TEMP 	TCCR5B
			#define TCNTH_TEMP 	TCNT5H
            #define TCNTL_TEMP	TCNT5L
            #define ICRH_TEMP 	ICR5H
			#define ICRL_TEMP 	ICR5L
			#define OCRAH_TEMP 	OCR5AH
			#define OCRAH_TEMP 	OCR5AH
			#define OCRAL_TEMP 	OCR5AL
			#define OCRBH_TEMP 	OCR5BH
			#define OCRBL_TEMP 	OCR5BL
			#define TIMSK_TEMP	TIMSK5
		
		#endif
        
        #if CONTROLLER_FREQ == 16000000UL
        
		MEGA2560_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 16 Mhz");
		
            // for 16 MHz crystal frequency, 250 milli second is timer tick time, 
            // with max accuracy of 1 milli second
            // timer1 is used and compare match A interrupt is enabled to generate desired tick time.
            
            // Timer/Counter 1 initialization
            // Clock source: System Clock
            // Clock value: 250.000 kHz
            // Mode: CTC top = OCR1A
            // OC1A output: Disconnected
            // OC1B output: Disconnected
            // Noise Canceler: Off
            // Input Capture on Falling Edge
            // Timer Period: 0.25 s
            // Timer1 Overflow Interrupt: Off
            // Input Capture Interrupt: Off
            // Compare A Match Interrupt: ON
            // Compare B Match Interrupt: ON
            TCCRA_TEMP=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);
            TCCRB_TEMP=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
            TCNTH_TEMP=0x00;
            TCNTL_TEMP=0x00;
            ICRH_TEMP=0x00;
            ICRL_TEMP=0x00;
            OCRAH_TEMP = (COMPARE_MATCH_MAX_TICK & 0xFF00)>>8;
            OCRAL_TEMP = COMPARE_MATCH_MAX_TICK & 0xFF;
            OCR1BH=0xFF;
            OCRBL_TEMP=0xFF;

			// Timer/Counter 1 Interrupt(s) initialization
			TIMSK_TEMP=(0<<ICIE1) | (1<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);

        #elif CONTROLLER_FREQ == 12000000UL
            
            MEGA2560_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 12 Mhz");
			
            // for 12 MHz crystal frequency, 40 milli second is timer tick time, 
            // with max accuracy of 1 milli second
            // timer1 is used and compare match A interrupt is enabled to generate desired tick time.
            
            // Timer/Counter 1 initialization
            // Clock source: System Clock
            // Clock value: 1500.000 kHz
            // Mode: CTC top = OCR1A
            // OC1A output: Disconnected
            // OC1B output: Disconnected
            // Noise Canceler: Off
            // Input Capture on Falling Edge
            // Timer Period: 40 ms
            // Timer1 Overflow Interrupt: On
            // Input Capture Interrupt: Off
            // Compare A Match Interrupt: ON
            // Compare B Match Interrupt: ON
            TCCRA_TEMP=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);
            TCCRB_TEMP=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
            TCNTH_TEMP=0x00;
            TCNTL_TEMP=0x00;
            ICRH_TEMP=0x00;
            ICRL_TEMP=0x00;
            OCRAH_TEMP = (COMPARE_MATCH_MAX_TICK & 0xFF00)>>8;
            OCRAL_TEMP = COMPARE_MATCH_MAX_TICK & 0xFF;
            OCR1BH=0xFF;
            OCRBL_TEMP=0xFF;

			// Timer/Counter 1 Interrupt(s) initialization
			TIMSK_TEMP=(0<<ICIE1) | (1<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);
            
        #elif CONTROLLER_FREQ == 8000000UL
            
            MEGA2560_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 8 Mhz");
			
            // for 8 MHz crystal frequency, 500 milli second is timer tick time, 
            // with max accuracy of 1 milli second
            // timer1 is used and compare match A interrupt is enabled to generate desired tick time.
        
            // Timer/Counter 1 initialization
            // Clock source: System Clock
            // Clock value: 125.000 kHz
            // Mode: CTC top = OCR1A
            // OC1A output: Disconnected
            // OC1B output: Disconnected
            // Noise Canceler: Off
            // Input Capture on Falling Edge
            // Timer Period: 0.5 s
            // Timer1 Overflow Interrupt: On
            // Input Capture Interrupt: Off
            // Compare A Match Interrupt: ON
            // Compare B Match Interrupt: ON
            TCCRA_TEMP=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);
            TCCRB_TEMP=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
            TCNTH_TEMP=0x00;
            TCNTL_TEMP=0x00;
            ICRH_TEMP=0x00;
            ICRL_TEMP=0x00;
            OCRAH_TEMP = (COMPARE_MATCH_MAX_TICK & 0xFF00)>>8;
            OCRAL_TEMP = COMPARE_MATCH_MAX_TICK & 0xFF;
            OCR1BH=0xFF;
            OCRBL_TEMP=0xFF;

			// Timer/Counter 1 Interrupt(s) initialization
			TIMSK_TEMP=(0<<ICIE1) | (1<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);
            
         #elif CONTROLLER_FREQ == 1000000UL
            
            MEGA2560_DEBUG_MSG1("Selected Option : Timer type = TIMER_1  and Controller frequency = 1 Mhz");
			
            // for 1 MHz crystal frequency, 500 milli second is timer tick time, 
            // with max accuracy of 1 milli second
            // timer1 is used and compare match A interrupt is enabled to generate desired tick time.
               
            // Timer/Counter 1 initialization
            // Clock source: System Clock
            // Clock value: 125.000 kHz
            // Mode: CTC top = OCR1A
            // OC1A output: Disconnected
            // OC1B output: Disconnected
            // Noise Canceler: Off
            // Input Capture on Falling Edge
            // Timer Period: 0.5 s
            // Timer1 Overflow Interrupt: On
            // Input Capture Interrupt: Off
            // Compare A Match Interrupt: ON
            // Compare B Match Interrupt: ON
            TCCRA_TEMP=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);
            TCCRB_TEMP=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
            TCNTH_TEMP=0x00;
            TCNTL_TEMP=0x00;
            ICRH_TEMP=0x00;
            ICRL_TEMP=0x00;
            OCRAH_TEMP = (COMPARE_MATCH_MAX_TICK & 0xFF00)>>8;
            OCRAL_TEMP = COMPARE_MATCH_MAX_TICK & 0xFF;
            OCR1BH=0xFF;
            OCRBL_TEMP=0xFF;

			// Timer/Counter 1 Interrupt(s) initialization
			TIMSK_TEMP=(0<<ICIE1) | (1<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);
        #else 
            #error Incorrect CONTROLLER_FREQ : For mega2560 Controller MOREY_OS supports only four frequencies i.e. 1000000UL, 8000000UL, 12000000UL & 16000000UL
        #endif
        
    #endif                
}

static void watchdog_init(void)
{  
    // Watchdog Timer initialization
    // Watchdog Timer Prescaler: OSC/1024k
    WDTCSR|=(1<<WDCE) | (1<<WDE);
	WDTCSR=(0<<WDIF) | (0<<WDIE) | (1<<WDP3) | (0<<WDCE) | (1<<WDE) | (0<<WDP2) | (0<<WDP1) | (1<<WDP0);
    
    MEGA2560_DEBUG_MSG2("Watchdog Timer initiated");
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
	
    MEGA2560_DEBUG_MSG2("Sleep Mode Initiated");
}

void sleep(void)
{
    MEGA2560_DEBUG_MSG3("Sleeping");
	
	#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ) )
		sleep_mode();
	#elif ( COMPILER == CODEVISION_AVR )
		idle();
	#endif
	
    MEGA2560_DEBUG_MSG3("Woke up");
}

#endif
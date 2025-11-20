/**\addtogroup sources_MEGA16
* @{ \defgroup clock_MEGA16_c
* @{
* \brief Documentaion of clock.c source file for atmega16 controller.   
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements OS clock functions for atmega16 controller.
* @}
*/

#ifndef DISABLE_OS

#include "../../../../../os/sys/clock.h"
#include "../../../../../os/sys/ptimer.h"
#include "boot.h"

#ifdef CLOCK_DEBUG_LEVEL_CONF
    #define CLOCK_DEBUG_LEVEL CLOCK_DEBUG_LEVEL_CONF
#else
    #define CLOCK_DEBUG_LEVEL CLOCK_DEBUG_NONE
#endif

#if CLOCK_DEBUG_LEVEL > CLOCK_DEBUG_NONE
    #include "../../../../../os/lib/debug.h"
#endif

#define CLOCK_DEBUG_MSG(msg) 	DEBUG_MSG_LN("<DEBUG CLOCK> ",NULL,msg)
    
#if CLOCK_DEBUG_LEVEL == CLOCK_DEBUG_NONE
    #define CLOCK_DEBUG_MSG_USER(msg)
    #define CLOCK_DEBUG_MSG1(msg)
    #define CLOCK_DEBUG_MSG2(msg)

#elif CLOCK_DEBUG_LEVEL == CLOCK_DEBUG_USER
    #define CLOCK_DEBUG_MSG_USER(msg) 	CLOCK_DEBUG_MSG(msg)
    #define CLOCK_DEBUG_MSG1(msg)
    #define CLOCK_DEBUG_MSG2(msg)
    
#elif CLOCK_DEBUG_LEVEL == CLOCK_DEBUG_LEVEL1
    #define CLOCK_DEBUG_MSG_USER(msg)        
    #define CLOCK_DEBUG_MSG1(msg) 	CLOCK_DEBUG_MSG(msg)
    #define CLOCK_DEBUG_MSG2(msg)
     
#elif CLOCK_DEBUG_LEVEL == CLOCK_DEBUG_LEVEL2
    #define CLOCK_DEBUG_MSG_USER(msg)
    #define CLOCK_DEBUG_MSG1(msg) 	CLOCK_DEBUG_MSG(msg)
    #define CLOCK_DEBUG_MSG2(msg) 	CLOCK_DEBUG_MSG(msg)     
#endif

#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ) )
	#include <avr/interrupt.h>
#endif

static clock_second_t seconds=0;
static clock_millisecond_t milliseconds=0;

// Timer interrupt service routine

#if OS_TIMER_TYPE == TIMER_0
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER0_OVF_vect)
	#elif COMPILER == CODEVISION_AVR
		interrupt [TIM0_OVF] void timer0_ovf_isr(void)
	#endif
	{
		TCNT0 = TIMER_COUNTER_START_VALUE;
		milliseconds = milliseconds + CLOCK_MILLISECOND_MAX_CONSTANT;
		seconds = seconds + milliseconds/1000;
		milliseconds = milliseconds%1000;       	
		CLOCK_DEBUG_MSG1("Timer Interrupt-1 called");
		ptimer_request_poll();
	}
	
#elif OS_TIMER_TYPE == TIMER_2
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER2_COMP_vect)
	#elif COMPILER == CODEVISION_AVR
		interrupt [TIM2_COMP] void timer2_comp_isr(void)
	#endif
	{
		if(TCNT2 >= COMPARE_MATCH_MAX_TICK)
		{
			TCNT2 = 0x00;
			milliseconds = milliseconds + CLOCK_MILLISECOND_MAX_CONSTANT;
			seconds = seconds + milliseconds/1000;
			milliseconds = milliseconds%1000;
		}
		OCR2 = COMPARE_MATCH_MAX_TICK;
		CLOCK_DEBUG_MSG1("Timer Interrupt-1 called");
		ptimer_request_poll();
	}
#elif OS_TIMER_TYPE == TIMER_1
	
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER1_COMPA_vect)
	#elif COMPILER == CODEVISION_AVR
		interrupt [TIM1_COMPA] void timer1_compa_isr(void)
	#endif
	{
		TCNT1H = 0x00;
		TCNT1L = 0x00;
		milliseconds = milliseconds + CLOCK_MILLISECOND_MAX_CONSTANT;
		seconds = seconds + milliseconds/1000;
		milliseconds = milliseconds%1000;
		CLOCK_DEBUG_MSG1("Timer Interrupt-1 called");
		ptimer_request_poll();
	}
	
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER1_COMPB_vect)
	#elif COMPILER == CODEVISION_AVR
		interrupt [TIM1_COMPB] void timer1_compb_isr(void)
	#endif
	{
		OCR1BH = 0xFF;
		OCR1BL = 0xFF;
		CLOCK_DEBUG_MSG1("Timer Interrupt-2 called");
		ptimer_request_poll();
	}
#endif

void get_clock_time(clock_second_t * seconds_value, clock_millisecond_t * milliseconds_value )
{
	int i=0;
	clock_second_t seconds_temp=0;
	clock_millisecond_t milliseconds_temp=0, milliseconds_temp_old=0;
	
	#ifdef PLATFORM_SUPPORT_TICKLESS
		#if OS_TIMER_TYPE == TIMER_1
			mos_uint16_t x, y;
		#endif
	#endif
    
    for(i=0;i<10;i++)
    {
        seconds_temp = seconds;
        
        #ifdef PLATFORM_SUPPORT_TICKLESS
		#if OS_TIMER_TYPE == TIMER_2
			milliseconds_temp = milliseconds + TCNT2/CLOCK_MILLISECOND_TO_TICK_CONSTANT;
		#elif OS_TIMER_TYPE == TIMER_1
			x = TCNT1L;
			y = TCNT1H;
			x = (y<<8) | x;
			milliseconds_temp = milliseconds + x/CLOCK_MILLISECOND_TO_TICK_CONSTANT;
		#endif
        #endif
        
        if(i>0)
        {
            if ( seconds == seconds_temp &&  milliseconds_temp == milliseconds_temp_old  )
                break;
        }
        
        milliseconds_temp_old = milliseconds_temp; 
    }
    
    *seconds_value = seconds_temp;
    *milliseconds_value = milliseconds_temp;       
}

#ifdef PLATFORM_SUPPORT_TICKLESS
void schedule_tickless_timer_platform(clock_millisecond_t milliseconds_next)
{
	if(milliseconds < milliseconds_next)
	{
		milliseconds_next = milliseconds_next - milliseconds;			
		if( (milliseconds_next/CLOCK_MILLISECOND_MAX_CONSTANT) == 0)
		{
		#if OS_TIMER_TYPE == TIMER_2
			mos_uint8_t x, a;
			a = milliseconds_next * CLOCK_MILLISECOND_TO_TICK_CONSTANT;
			// read TCNT2 as late as possible since its value is updating
			x = TCNT2;
			if ( x >= a )
			{
				OCR2 = COMPARE_MATCH_MAX_TICK;
				CLOCK_DEBUG_MSG2("Tickless expired : ticks");
				ptimer_request_poll();
			}
			else
			{
				OCR2 = a;
				#if CLOCK_DEBUG_LEVEL >= CLOCK_DEBUG_LEVEL2
					DEBUG_MSG_CONST3("<DEBUG CLOCK> ",NULL,"Condition satisfied, tickless milliSeconds = ");
					DEBUG_PRINT_INTEGER_LONG_LN(milliseconds_next);
				#endif
			}
		#elif OS_TIMER_TYPE == TIMER_1
			mos_uint16_t a, x, y;
			a = milliseconds_next * CLOCK_MILLISECOND_TO_TICK_CONSTANT;
			// read TCNT1 as late as possible since its value is updating
			x = TCNT1L;
			y = TCNT1H;
			x = (y << 8)|x;
			if ( x >= a )
			{
				OCR1BH = 0xFF;
				OCR1BL = 0xFF;
				CLOCK_DEBUG_MSG2("Tickless expired : ticks");
				ptimer_request_poll();
			}
			else
			{
				OCR1BH = a >> 8;
				OCR1BL = a & 0xFF;
				#if CLOCK_DEBUG_LEVEL >= CLOCK_DEBUG_LEVEL2
					DEBUG_MSG_CONST3("<DEBUG CLOCK> ",NULL,"Condition satisfied, tickless milliSeconds = ");
					DEBUG_PRINT_INTEGER_LONG_LN(milliseconds_next);
				#endif
			}			
		#endif
		}
		
		else
		{
			CLOCK_DEBUG_MSG2("Condition not satisfied");
		}
	}		
	
	else
	{
		CLOCK_DEBUG_MSG2("Tickless expired : milliseconds");
		ptimer_request_poll();
	}			
}	
#endif
#endif



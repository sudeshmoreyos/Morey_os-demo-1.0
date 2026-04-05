/**\addtogroup sources_MEGA8
* @{ \defgroup clock_MEGA8_c
* @{
* \brief Documentaion of clock.c source file for atmega8 controller.   
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements OS clock functions for atmega8 controller.
* @}
*/

// Keywords to update in order to migrate to new avr controller : MEGA8, atmega8

#ifndef DISABLE_OS

#include "../../../../../os/sys/clock.h"
#include "../../../../../os/sys/ptimer.h"
#include "boot.h"

#ifdef CLOCK_DEBUG_LEVEL_CONF
#define CLOCK_DEBUG_LEVEL	CLOCK_DEBUG_LEVEL_CONF
#else
#define CLOCK_DEBUG_LEVEL	CLOCK_DEBUG_NONE
#endif // CLOCK_DEBUG_LEVEL_CONF

#if CLOCK_DEBUG_LEVEL > CLOCK_DEBUG_NONE
#include "../../../../../os/lib/debug.h"
#endif

#define CLOCK_DEBUG_MSG(msg)	DEBUG_MSG_LN("<DEBUG CLOCK> ",NULL,msg)
    
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
#define CLOCK_DEBUG_MSG1(msg) 		CLOCK_DEBUG_MSG(msg)
#define CLOCK_DEBUG_MSG2(msg)
 
#elif CLOCK_DEBUG_LEVEL == CLOCK_DEBUG_LEVEL2

#define CLOCK_DEBUG_MSG_USER(msg)
#define CLOCK_DEBUG_MSG1(msg) 		CLOCK_DEBUG_MSG(msg)
#define CLOCK_DEBUG_MSG2(msg) 		CLOCK_DEBUG_MSG(msg)     

#endif // CLOCK_DEBUG_LEVEL == CLOCK_DEBUG_NONE

#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ) )
#include <avr/interrupt.h>
#endif

static volatile clock_second_t seconds=0;
static volatile clock_millisecond_t milliseconds=0;

#if OS_TIMER_TYPE == TIMER_2

// Ticking + Tickless Timer interrupt service routine, especially for TIMER2
#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
ISR(OS_TICKING_INTERRUPT)
#endif
{
	if(OS_TICKING_TIMER_COUNTER >= COMPARE_MATCH_MAX_TICK)
	{
		OS_TICKING_TIMER_COUNTER = 0x00;
		milliseconds = milliseconds + CLOCK_MILLISECOND_MAX_CONSTANT;
		seconds = seconds + milliseconds/1000;
		milliseconds = milliseconds%1000;
		CLOCK_DEBUG_MSG1("Ticking Timer Interrupt called");
	}
#if CLOCK_DEBUG_LEVEL >= CLOCK_DEBUG_LEVEL1
	else
		CLOCK_DEBUG_MSG1("Tickless Timer Interrupt called");
#endif
	OS_TICKLESS_TIMER_REGISTER = COMPARE_MATCH_MAX_TICK;
	ptimer_request_poll();
}

#else
	
// Ticking Timer interrupt service routine
#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
ISR(OS_TICKING_INTERRUPT)
#endif
{
	OS_TICKING_TIMER_COUNTER = TIMER_COUNTER_START_VALUE;
	milliseconds = milliseconds + CLOCK_MILLISECOND_MAX_CONSTANT;
	seconds = seconds + milliseconds/1000;
	milliseconds = milliseconds%1000;
	CLOCK_DEBUG_MSG1("Ticking Timer Interrupt called");
	ptimer_request_poll();
}

#ifdef PLATFORM_SUPPORT_TICKLESS
// Tickless Timer interrupt service routine
#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
ISR(OS_TICKLESS_INTERRUPT)
#endif
{
	OS_TICKLESS_TIMER_REGISTER = TIMER_REGISTER_DEFAULT_VALUE;
	CLOCK_DEBUG_MSG1("Tickless Timer Interrupt called");
	ptimer_request_poll();
}

#endif // PLATFORM_SUPPORT_TICKLESS
#endif // OS_TIMER_TYPE == TIMER_2

void get_clock_time(clock_second_t * seconds_value, clock_millisecond_t * milliseconds_value )
{
    mos_uint8_t i=0;
    clock_second_t seconds_temp=0;
    clock_millisecond_t milliseconds_temp=0, milliseconds_temp_old=0;
    
    for(i=0;i<10;i++)
    {
        seconds_temp = seconds;
        milliseconds_temp = milliseconds + (OS_TICKING_TIMER_COUNTER-TIMER_COUNTER_START_VALUE)/CLOCK_MILLISECOND_TO_TICK_CONSTANT;
        
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
		if(milliseconds_next < CLOCK_MILLISECOND_MAX_CONSTANT)
		{				
			mos_uint16_t a, x;
			a = milliseconds_next * CLOCK_MILLISECOND_TO_TICK_CONSTANT;
			// read OS_TICKING_TIMER_COUNTER as late as possible since its value is updating
			x = OS_TICKING_TIMER_COUNTER;
			if ( x >= a )
			{
				OS_TICKLESS_TIMER_REGISTER = TIMER_REGISTER_DEFAULT_VALUE;
				CLOCK_DEBUG_MSG2("Tickless expired : ticks");
				ptimer_request_poll();
			}
			else
			{
				OS_TICKLESS_TIMER_REGISTER = a;
#if CLOCK_DEBUG_LEVEL >= CLOCK_DEBUG_LEVEL2
				DEBUG_MSG_CONST3("<DEBUG CLOCK> ",NULL,"Condition satisfied, tickless milliSeconds = ");
				DEBUG_PRINT_INTEGER_LONG_LN(milliseconds_next);
#endif
			}
		}
#if CLOCK_DEBUG_LEVEL >= CLOCK_DEBUG_LEVEL2	
		else
		{
			CLOCK_DEBUG_MSG2("Condition not satisfied");
		}
#endif
	}		
	
	else
	{
		CLOCK_DEBUG_MSG2("Tickless expired : milliseconds");
		ptimer_request_poll();
	}			
}	

#endif // PLATFORM_SUPPORT_TICKLESS
#endif // DISABLE_OS



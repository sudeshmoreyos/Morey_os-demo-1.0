/**\addtogroup sources_MEGA328PB
* @{ \defgroup clock_MEGA328PB_c
* @{
* \brief Documentaion of clock.c source file for atmega328pb controller.   
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements OS clock functions for atmega328pb controller.
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

#define CLOCK_DEBUG_MSG(msg) DEBUG_MSG_CONST3_LN("<DEBUG CLOCK> ",NULL,msg)
    
#if CLOCK_DEBUG_LEVEL == CLOCK_DEBUG_NONE
    #define CLOCK_DEBUG_MSG_USER(msg)
    #define CLOCK_DEBUG_MSG1(msg)
    #define CLOCK_DEBUG_MSG2(msg)

#elif CLOCK_DEBUG_LEVEL == CLOCK_DEBUG_USER
    #define CLOCK_DEBUG_MSG_USER(msg) CLOCK_DEBUG_MSG(msg)
    #define CLOCK_DEBUG_MSG1(msg)
    #define CLOCK_DEBUG_MSG2(msg)
    
#elif CLOCK_DEBUG_LEVEL == CLOCK_DEBUG_LEVEL1
    #define CLOCK_DEBUG_MSG_USER(msg)        
    #define CLOCK_DEBUG_MSG1(msg) CLOCK_DEBUG_MSG(msg)
    #define CLOCK_DEBUG_MSG2(msg)
     
#elif CLOCK_DEBUG_LEVEL == CLOCK_DEBUG_LEVEL2
    #define CLOCK_DEBUG_MSG_USER(msg)
    #define CLOCK_DEBUG_MSG1(msg) CLOCK_DEBUG_MSG(msg)
    #define CLOCK_DEBUG_MSG2(msg) CLOCK_DEBUG_MSG(msg)     
#endif

#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ) )
	#include <avr/interrupt.h>
#endif

static clock_second_t seconds=0;
static clock_millisecond_t milliseconds=0;

// Ticking Timer interrupt service routine
#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
	ISR(OS_TICKING_INTERRUPT)
#endif
{
	OS_TICKING_TIMER_COUNTER = TIMER_COUNTER_START_VALUE;
	milliseconds += CLOCK_MILLISECOND_MAX_CONSTANT;
	seconds += milliseconds/1000;
	milliseconds %= 1000;
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
#endif

void get_clock_time(clock_second_t * seconds_value, clock_millisecond_t * milliseconds_value )
{
    mos_uint8_t i=0;
    clock_second_t seconds_temp=0;
    clock_millisecond_t milliseconds_temp=0, milliseconds_temp_old=0;
    
    for(i=0;i<10;i++)
    {
        seconds_temp = seconds;
        milliseconds_temp = milliseconds + OS_TICKING_TIMER_COUNTER/CLOCK_MILLISECOND_TO_TICK_CONSTANT;
        
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



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
	#define CLOCK_DEBUG_LEVEL 		CLOCK_DEBUG_LEVEL_CONF
#else
	#define CLOCK_DEBUG_LEVEL 		CLOCK_DEBUG_NONE
#endif

#if CLOCK_DEBUG_LEVEL > CLOCK_DEBUG_NONE
	#include "../../../../../os/lib/debug.h"
#endif

#define CLOCK_DEBUG_MSG(msg) 			DEBUG_MSG_LN("<DEBUG CLOCK> ",NULL,msg)
    
#if CLOCK_DEBUG_LEVEL == CLOCK_DEBUG_NONE
	#define CLOCK_DEBUG_MSG_USER(msg)
	#define CLOCK_DEBUG_MSG1(msg)
	#define CLOCK_DEBUG_MSG2(msg)

#elif CLOCK_DEBUG_LEVEL == CLOCK_DEBUG_USER
	#define CLOCK_DEBUG_MSG_USER(msg)	CLOCK_DEBUG_MSG(msg)
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
#endif

#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ) )
	#include <avr/interrupt.h>
#endif

static clock_second_t seconds=0;
static clock_millisecond_t milliseconds=0;

// Timer interrupt ticking service routine
ISR(TIMER_INTERRUPT_TICKING)
{
	init_ticking_interrupt();
	milliseconds = milliseconds + CLOCK_MILLISECOND_MAX_CONSTANT;
	seconds = seconds + milliseconds/1000;
	milliseconds = milliseconds%1000;
	disable_tickless_call();
	CLOCK_DEBUG_MSG1("Timer Interrupt Ticking called");
	ptimer_request_poll();
}

#ifdef PLATFORM_SUPPORT_TICKLESS
// Timer interrupt ticking service routine
ISR(TIMER_INTERRUPT_TICKLESS)
{
	disable_tickless_call();
	CLOCK_DEBUG_MSG1("Timer Interrupt Tickless called");
	ptimer_request_poll();
}
#endif

void get_clock_time(clock_second_t * seconds_value, clock_millisecond_t * milliseconds_value )
{
	mos_uint8_t i=0,temp;
	clock_second_t seconds_temp=0;
	clock_millisecond_t milliseconds_temp=0, milliseconds_temp_old=0;
    
	for(i=0;i<10;i++)
	{
		seconds_temp = seconds;
		#ifdef PLATFORM_SUPPORT_TICKLESS
			milliseconds_temp = milliseconds + read_timer_millisecond(&temp);
		#else
			milliseconds_temp = milliseconds;
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
	mos_uint8_t x,y;
	clock_millisecond_t milliseconds_temp;
	
	milliseconds_temp = milliseconds;
	if(milliseconds_temp < milliseconds_next)
	{
		milliseconds_next = milliseconds_next - milliseconds_temp;
		if( milliseconds_next/CLOCK_MILLISECOND_MAX_CONSTANT == 0)
		{			
			x = IS_ATOMIC_OFF();
			if(x)
			{
				ATOMIC_ON();
			}			
			milliseconds_temp = read_timer_millisecond(&y);
			if ( milliseconds_temp < milliseconds_next-1 )
			{
				enable_tickless_call(milliseconds_next);
				#if CLOCK_DEBUG_LEVEL >= CLOCK_DEBUG_LEVEL2
					if(x)
					{
						ATOMIC_OFF();
					}
					x=0;
					DEBUG_MSG_CONST3("<DEBUG CLOCK> ",NULL,"Condition satisfied, tickless milliSeconds - 1 = ");
					DEBUG_PRINT_INTEGER_LONG_LN(milliseconds_next);
				#endif
			}
			else if ( milliseconds_temp < milliseconds_next )
			{
				if(y)
				{
					enable_tickless_call(milliseconds_next + 1);
					#if CLOCK_DEBUG_LEVEL >= CLOCK_DEBUG_LEVEL2
						if(x)
						{
							ATOMIC_OFF();
						}
						x=0;
						DEBUG_MSG_CONST3("<DEBUG CLOCK> ",NULL,"Condition satisfied, tickless milliSeconds + 1 = ");
						DEBUG_PRINT_INTEGER_LONG_LN(milliseconds_next + 1);
					#endif
				}
				else
				{
					enable_tickless_call(milliseconds_next);
					#if CLOCK_DEBUG_LEVEL >= CLOCK_DEBUG_LEVEL2
						if(x)
						{
							ATOMIC_OFF();
						}
						x=0;
						DEBUG_MSG_CONST3("<DEBUG CLOCK> ",NULL,"Condition satisfied, tickless milliSeconds = ");
						DEBUG_PRINT_INTEGER_LONG_LN(milliseconds_next);
					#endif					
				}
			}
			else
			{
				disable_tickless_call();
				CLOCK_DEBUG_MSG2("Tickless expired : Ticks");
				ptimer_request_poll();
			}
			
			if(x)
			{
				ATOMIC_OFF();
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



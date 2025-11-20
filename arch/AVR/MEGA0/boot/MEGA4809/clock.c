/**\addtogroup sources_MEGA4809
* @{ \defgroup clock_MEGA4809_c
* @{
* \brief Documentaion of clock.c source file for atmega4809 controller.   
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements OS clock functions for atmega4809 controller.
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

// #pragma GCC diagnostic ignored "-Wmisspelled-isr"

static volatile clock_second_t seconds=0;
static volatile clock_millisecond_t milliseconds=0;

#if OS_TIMER_TYPE == TIMER_RTC
	ISR(OS_COMMON_INTERRUPT)
	{
		if (RTC.INTFLAGS & RTC_OVF_bm) 
		{
			// Clear the OVF flag by writing a '1'
			RTC.INTFLAGS = RTC_OVF_bm;
			
			milliseconds += CLOCK_MILLISECOND_MAX_CONSTANT;
			if (milliseconds >= 1000) 
			{
				milliseconds -= 1000;
				seconds++;
			}
			CLOCK_DEBUG_MSG1("Ticking Timer Interrupt called");
			ptimer_request_poll();
		}
		
		#ifdef PLATFORM_SUPPORT_TICKLESS		
			if (RTC.INTFLAGS & RTC_CMP_bm) 
			{
				// Clear the CMP flag by writing a '1'
				RTC.INTFLAGS = RTC_CMP_bm;
										
				// Wait for RTC synchronization
				while (RTC.STATUS != 0)
					;
				
				OS_TICKLESS_TIMER_REGISTER = TIMER_TICKLESS_DEFAULT_VALUE;
				CLOCK_DEBUG_MSG1("Tickless Timer Interrupt called");
				ptimer_request_poll();
			}
		#endif
	}
	
#else
	// Ticking Timer interrupt service routine
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(OS_TICKING_INTERRUPT)
		{
			OS_TICKING_TIMER_COUNTER = TIMER_TICKING_START_VALUE;
			
			milliseconds += CLOCK_MILLISECOND_MAX_CONSTANT;
			if (milliseconds >= 1000) 
			{
				milliseconds -= 1000;
				seconds++;
			}
			CLOCK_DEBUG_MSG1("Ticking Timer Interrupt called");
			ptimer_request_poll();
		}
	#else
	#error "Unsupported compiler"
	#endif

	#ifdef PLATFORM_SUPPORT_TICKLESS
		// Tickless Timer interrupt service routine
		#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
			ISR(OS_TICKLESS_INTERRUPT)
			{
				OS_TICKLESS_TIMER_REGISTER = TIMER_TICKLESS_DEFAULT_VALUE;
				CLOCK_DEBUG_MSG1("Tickless Timer Interrupt called");
				ptimer_request_poll();
			}
		#else
		#error "Unsupported compiler"
		#endif
	#endif
#endif

void get_clock_time(clock_second_t *seconds_value, clock_millisecond_t *milliseconds_value)
{
    clock_second_t sec1, sec2;
    clock_millisecond_t ms1, ms2;
    mos_uint16_t counter1, counter2;
	mos_uint8_t iterations =0;

    // repeat until consistent snapshot
    do {
        sec1 = seconds;
        ms1 = milliseconds;
        counter1 = OS_TICKING_TIMER_COUNTER;

        // small delay to ensure counter can roll, if about to overflow
        sec2 = seconds;
        ms2 = milliseconds;
        counter2 = OS_TICKING_TIMER_COUNTER;
		
		iterations++;
		if(iterations >= 10)
		{
			CLOCK_DEBUG_MSG2("get_clock_time : 10 iteration loop break");
			break;
		}
    } while ((sec1 != sec2) || (ms1 != ms2) || (counter2 < counter1));  

    // Compute consistent millisecond value
    clock_millisecond_t ms_temp = ms1 + (counter1 / CLOCK_MILLISECOND_TO_TICK_CONSTANT);

    *seconds_value = sec1;
    *milliseconds_value = ms_temp;
}


#ifdef PLATFORM_SUPPORT_TICKLESS
	void schedule_tickless_timer_platform(clock_millisecond_t milliseconds_next)
	{
		clock_millisecond_t ms1, ms2;
		mos_uint16_t a;

		// Take a stable snapshot of 'milliseconds'
		do {
			ms1 = milliseconds;
			ms2 = milliseconds;
		} while (ms1 != ms2);

		// Use the stable snapshot
		if (ms1 < milliseconds_next)
		{
			milliseconds_next = milliseconds_next - ms1;
			if (milliseconds_next > 0 && milliseconds_next < CLOCK_MILLISECOND_MAX_CONSTANT)
			{
				// Conversion guaranteed 16-bit safe
				a = milliseconds_next * CLOCK_MILLISECOND_TO_TICK_CONSTANT;

				// Read OS_TICKING_TIMER_COUNTER as late as possible since it’s continuously updating
				if (OS_TICKING_TIMER_COUNTER >= a)
				{
					#if OS_TIMER_TYPE == TIMER_RTC
						// Wait for RTC synchronization
						while (RTC.STATUS != 0)
							;
					#endif
					OS_TICKLESS_TIMER_REGISTER = TIMER_TICKLESS_DEFAULT_VALUE;
					CLOCK_DEBUG_MSG2("Tickless expired : ticks");
					ptimer_request_poll();
				}
				
				else 
				{
					#if OS_TIMER_TYPE == TIMER_RTC
						// Wait for RTC synchronization
						while (RTC.STATUS != 0)
							;
					#endif
					// Set compare value
					OS_TICKLESS_TIMER_REGISTER = a;
					
					#if OS_TIMER_TYPE == TIMER_RTC
						// Wait for RTC synchronization
						while (RTC.STATUS != 0)
							;
					#endif

					// Re-read counter immediately after writing
					if (OS_TICKING_TIMER_COUNTER >= a) 
					{
						// Timer passed compare point before hardware could latch
						OS_TICKLESS_TIMER_REGISTER = TIMER_TICKLESS_DEFAULT_VALUE;
						CLOCK_DEBUG_MSG2("Tickless expired : ticks");
						ptimer_request_poll();
					}
					#if CLOCK_DEBUG_LEVEL >= CLOCK_DEBUG_LEVEL2
						else
						{
							DEBUG_MSG_CONST3("<DEBUG CLOCK> ", NULL, "Condition satisfied, tickless milliSeconds = ");
							DEBUG_PRINT_INTEGER_LONG_LN(milliseconds_next);
						}
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
		#if CLOCK_DEBUG_LEVEL >= CLOCK_DEBUG_LEVEL2
			else
			{
				CLOCK_DEBUG_MSG2("Tickless expired : milliseconds");
				ptimer_request_poll();
			}
		#endif
	}
#endif
#endif

/*
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

void schedule_tickless_timer_platform(clock_millisecond_t milliseconds_next)
{
	if(milliseconds < milliseconds_next)
	{
		milliseconds_next = milliseconds_next - milliseconds;			
		if(milliseconds_next < CLOCK_MILLISECOND_MAX_CONSTANT)
		{				
			mos_uint16_t a, x;
			
			// Conversion: milliseconds_next * CLOCK_MILLISECOND_TO_TICK_CONSTANT
			// Guaranteed 16-bit safe by design — no overflow possible
			a = milliseconds_next * CLOCK_MILLISECOND_TO_TICK_CONSTANT;
			
			// read OS_TICKING_TIMER_COUNTER as late as possible since its value is updating
			x = OS_TICKING_TIMER_COUNTER;
			if ( x >= a )
			{
				OS_TICKLESS_TIMER_REGISTER = TIMER_TICKLESS_DEFAULT_VALUE;
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
*/



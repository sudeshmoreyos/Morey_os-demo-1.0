/** \addtogroup XMEGA256
* @{ \defgroup clock_xmega256_c
* @{ \addtogroup clock_xmega256_c
* @{
* \brief Documentaion of clock.c source file for atxmega256 controller.   
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements OS clock functions for atxmega256 controller.
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

#define CLOCK_DEBUG_MSG(msg) DEBUG_MSG_LN("<DEBUG CLOCK> ",NULL,msg)
    
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

clock_second_t seconds=0;
clock_millisecond_t milliseconds=0;

// Timer interrupt service routine
#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
	#if TIMER_TYPE == TIMER_C0
		ISR (TCC0_OVF_vect)
	#elif TIMER_TYPE == TIMER_C1
		ISR (TCC1_OVF_vect)
	#elif TIMER_TYPE == TIMER_D0
		ISR (TCD0_OVF_vect)
	#elif TIMER_TYPE == TIMER_D1
		ISR (TCD1_OVF_vect)
	#elif TIMER_TYPE == TIMER_E0
		ISR (TCE0_OVF_vect)
	#elif TIMER_TYPE == TIMER_E1
		ISR (TCE1_OVF_vect)
	#elif TIMER_TYPE == TIMER_F0
		ISR (TCF0_OVF_vect)
	#endif
#elif COMPILER == CODEVISION_AVR
	#if TIMER_TYPE == TIMER_C0
		interrupt [TCC0_OVF_vect] void tcc0_overflow_isr(void)
	#elif TIMER_TYPE == TIMER_C1
		interrupt [TCC1_OVF_vect] void tcc1_overflow_isr(void)
	#elif TIMER_TYPE == TIMER_D0
		interrupt [TCD0_OVF_vect] void tcd0_overflow_isr(void)
	#elif TIMER_TYPE == TIMER_D1
		interrupt [TCD1_OVF_vect] void tcd1_overflow_isr(void)
	#elif TIMER_TYPE == TIMER_E0
		interrupt [TCE0_OVF_vect] void tce0_overflow_isr(void)
	#elif TIMER_TYPE == TIMER_E1
		interrupt [TCE1_OVF_vect] void tce1_overflow_isr(void)
	#elif TIMER_TYPE == TIMER_F0
		interrupt [TCF0_OVF_vect] void tcf0_overflow_isr(void)
	#endif
#endif
{
	milliseconds = milliseconds + CLOCK_MILLISECOND_MAX_CONSTANT;
	seconds = seconds + milliseconds/1000;
	milliseconds = milliseconds%1000;
	CLOCK_DEBUG_MSG1("Timer Interrupt-1 called");
	ptimer_request_poll();
}

#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
	#if TIMER_TYPE == TIMER_C0
		ISR (TCC0_CCA_vect)
	#elif TIMER_TYPE == TIMER_C1
		ISR (TCC1_CCA_vect)
	#elif TIMER_TYPE == TIMER_D0
		ISR (TCD0_CCA_vect)
	#elif TIMER_TYPE == TIMER_D1
		ISR (TCD1_CCA_vect)
	#elif TIMER_TYPE == TIMER_E0
		ISR (TCE0_CCA_vect)
	#elif TIMER_TYPE == TIMER_E1
		ISR (TCE1_CCA_vect)
	#elif TIMER_TYPE == TIMER_F0
		ISR (TCF0_CCA_vect)
	#endif
#elif COMPILER == CODEVISION_AVR
	#if TIMER_TYPE == TIMER_C0
		interrupt [TCC0_CCA_vect] void tcc0_compare_capture_a_isr(void)
	#elif TIMER_TYPE == TIMER_C1
		interrupt [TCC1_CCA_vect] void tcc1_compare_capture_a_isr(void)
	#elif TIMER_TYPE == TIMER_D0
		interrupt [TCD0_CCA_vect] void tcd0_compare_capture_a_isr(void)
	#elif TIMER_TYPE == TIMER_D1
		interrupt [TCD1_CCA_vect] void tcd1_compare_capture_a_isr(void)
	#elif TIMER_TYPE == TIMER_E0
		interrupt [TCE0_CCA_vect] void tce0_compare_capture_a_isr(void)
	#elif TIMER_TYPE == TIMER_E1
		interrupt [TCE1_CCA_vect] void tce1_compare_capture_a_isr(void)
	#elif TIMER_TYPE == TIMER_F0
		interrupt [TCF0_CCA_vect] void tcf0_compare_capture_a_isr(void)
	#endif
#endif
{
	TIMER_PORT.CCA=0xFFFF;
	CLOCK_DEBUG_MSG1("Timer Interrupt-2 called");
	ptimer_request_poll();
}

void get_clock_time(clock_second_t * seconds_value, clock_millisecond_t * milliseconds_value )
{
    int i=0;
    clock_second_t seconds_temp=0;
    clock_millisecond_t milliseconds_temp=0, milliseconds_temp_old=0;
    
    for(i=0;i<10;i++)
    {
        seconds_temp = seconds;
        milliseconds_temp = milliseconds + TIMER_PORT.CNT/CLOCK_MILLISECOND_TO_TICK_CONSTANT;
        
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
				mos_uint16_t a;
				a = milliseconds_next * CLOCK_MILLISECOND_TO_TICK_CONSTANT;
				if ( TIMER_PORT.CNT >= a )
				{
					TIMER_PORT.CCA=0xFFFF;
					CLOCK_DEBUG_MSG2("Tickless expired : ticks");
					ptimer_request_poll();
				}
				else
				{
					TIMER_PORT.CCA=a;
					#if (CLOCK_DEBUG_LEVEL >= CLOCK_DEBUG_LEVEL2) && (DEBUG_LEVEL > DEBUG_LEVEL_NONE)
						DEBUG_MSG("<DEBUG CLOCK> ",NULL,"Condition satisfied, tickless milliSeconds = ");
						debug_print_integer_long_ln(milliseconds_next);
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



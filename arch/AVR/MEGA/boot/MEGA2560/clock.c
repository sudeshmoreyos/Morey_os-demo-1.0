/**\addtogroup sources_MEGA2560
* @{ \defgroup clock_MEGA2560_c
* @{
* \brief Documentaion of clock.c source file for atmega2560 controller.   
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements OS clock functions for atmega2560 controller.
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

#define CLOCK_DEBUG_MSG(msg) 			DEBUG_MSG_CONST3_LN("<DEBUG CLOCK> ",NULL,msg)
    
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
#endif

#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ) )
	#include <avr/interrupt.h>
#endif

static clock_second_t seconds=0;
static clock_millisecond_t milliseconds=0;

// Timer interrupt service routine

#if OS_TIMER_TYPE == TIMER_0
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER0_COMPA_vect)
	#endif
	{
		TCNT0 = 0x00;
		milliseconds = milliseconds + CLOCK_MILLISECOND_MAX_CONSTANT;
		seconds = seconds + milliseconds/1000;
		milliseconds = milliseconds%1000;
		CLOCK_DEBUG_MSG1("Timer Interrupt-1 called");
		ptimer_request_poll();
	}
	
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER0_COMPB_vect)
	#endif
	{
		OCR0B = 0xFF;
		CLOCK_DEBUG_MSG1("Timer Interrupt-2 called");
		ptimer_request_poll();
	}
	
#elif OS_TIMER_TYPE == TIMER_2
	
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER2_COMPA_vect)
	#endif
	{
		TCNT2 = 0x00;
		milliseconds = milliseconds + CLOCK_MILLISECOND_MAX_CONSTANT;
		seconds = seconds + milliseconds/1000;
		milliseconds = milliseconds%1000;
		CLOCK_DEBUG_MSG1("Timer Interrupt-1 called");
		ptimer_request_poll();
	}
	
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER2_COMPB_vect)
	#endif
	{
		OCR2B = 0xFF;
		CLOCK_DEBUG_MSG1("Timer Interrupt-2 called");
		ptimer_request_poll();
	}
	
#elif OS_TIMER_TYPE == TIMER_1
	
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER1_COMPA_vect)
	#endif
	{
		// In CTC mode TCNT is automatically cleared
		//TCNT1H = 0x00;
		//TCNT1L = 0x00;
		milliseconds = milliseconds + CLOCK_MILLISECOND_MAX_CONSTANT;
		seconds = seconds + milliseconds/1000;
		milliseconds = milliseconds%1000;
		CLOCK_DEBUG_MSG1("Timer Interrupt-1 called");
		ptimer_request_poll();
	}
	
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER1_COMPB_vect)
	#endif
	{
		OCR1BH = 0xFF;
		OCR1BL = 0xFF;
		CLOCK_DEBUG_MSG1("Timer Interrupt-2 called");
		ptimer_request_poll();
	}

#elif OS_TIMER_TYPE == TIMER_3
	
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER3_COMPA_vect)
	#endif
	{
		// In CTC mode TCNT is automatically cleared
		//TCNT3H = 0x00;
		//TCNT3L = 0x00;
		milliseconds = milliseconds + CLOCK_MILLISECOND_MAX_CONSTANT;
		seconds = seconds + milliseconds/1000;
		milliseconds = milliseconds%1000;
		CLOCK_DEBUG_MSG1("Timer Interrupt-1 called");
		ptimer_request_poll();
	}
	
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER3_COMPB_vect)
	#endif
	{
		OCR3BH = 0xFF;
		OCR3BL = 0xFF;
		CLOCK_DEBUG_MSG1("Timer Interrupt-2 called");
		ptimer_request_poll();
	}
	
#elif OS_TIMER_TYPE == TIMER_4
	
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER4_COMPA_vect)
	#endif
	{
		// In CTC mode TCNT is automatically cleared
		//TCNT4H = 0x00;
		//TCNT4L = 0x00;
		milliseconds = milliseconds + CLOCK_MILLISECOND_MAX_CONSTANT;
		seconds = seconds + milliseconds/1000;
		milliseconds = milliseconds%1000;
		CLOCK_DEBUG_MSG1("Timer Interrupt-1 called");
		ptimer_request_poll();
	}
	
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER4_COMPB_vect)
	#endif
	{
		OCR4BH = 0xFF;
		OCR4BL = 0xFF;
		CLOCK_DEBUG_MSG1("Timer Interrupt-2 called");
		ptimer_request_poll();
	}
	
#elif OS_TIMER_TYPE == TIMER_5
	
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER5_COMPA_vect)
	#endif
	{
		// In CTC mode TCNT is automatically cleared
		//TCNT5H = 0x00;
		//TCNT5L = 0x00;
		milliseconds = milliseconds + CLOCK_MILLISECOND_MAX_CONSTANT;
		seconds = seconds + milliseconds/1000;
		milliseconds = milliseconds%1000;
		CLOCK_DEBUG_MSG1("Timer Interrupt-1 called");
		ptimer_request_poll();
	}
	
	#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ))
		ISR(TIMER5_COMPB_vect)
	#endif
	{
		OCR5BH = 0xFF;
		OCR5BL = 0xFF;
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
		#if (OS_TIMER_TYPE == TIMER_1) || (OS_TIMER_TYPE == TIMER_3) || (OS_TIMER_TYPE == TIMER_4) || (OS_TIMER_TYPE == TIMER_5)
			mos_uint16_t x, y;
		#endif
	#endif
    
    for(i=0;i<10;i++)
    {
        seconds_temp = seconds;
        
        #ifdef PLATFORM_SUPPORT_TICKLESS
			#if OS_TIMER_TYPE == TIMER_0
				milliseconds_temp = milliseconds + TCNT0/CLOCK_MILLISECOND_TO_TICK_CONSTANT;
			#elif OS_TIMER_TYPE == TIMER_2
				milliseconds_temp = milliseconds + TCNT2/CLOCK_MILLISECOND_TO_TICK_CONSTANT;
			#elif OS_TIMER_TYPE == TIMER_1
				x = TCNT1L;
				y = TCNT1H;
				x = (y<<8) | x;
				milliseconds_temp = milliseconds + x/CLOCK_MILLISECOND_TO_TICK_CONSTANT;
			#elif OS_TIMER_TYPE == TIMER_3
				x = TCNT3L;
				y = TCNT3H;
				x = (y<<8) | x;
				milliseconds_temp = milliseconds + x/CLOCK_MILLISECOND_TO_TICK_CONSTANT;
			#elif OS_TIMER_TYPE == TIMER_4
				x = TCNT4L;
				y = TCNT4H;
				x = (y<<8) | x;
				milliseconds_temp = milliseconds + x/CLOCK_MILLISECOND_TO_TICK_CONSTANT;
			#elif OS_TIMER_TYPE == TIMER_5
				x = TCNT5L;
				y = TCNT5H;
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
			#if OS_TIMER_TYPE == TIMER_0
				mos_uint16_t a, x;
				a = milliseconds_next * CLOCK_MILLISECOND_TO_TICK_CONSTANT;
				// read TCNT0 as late as possible since its value is updating
				x = TCNT0;
				if ( x >= a )
				{
					OCR0B = 0xFF;
					CLOCK_DEBUG_MSG2("Tickless expired : ticks");
					ptimer_request_poll();
				}
				else
				{
					OCR0B = a;					
					#if CLOCK_DEBUG_LEVEL >= CLOCK_DEBUG_LEVEL2	
						DEBUG_CONST_PRINT("Condition satisfied, tickless milliSeconds = ");
						DEBUG_PRINT_INTEGER_LN(milliseconds_next);
					#endif
				}			
			#elif OS_TIMER_TYPE == TIMER_2
				mos_uint16_t a, x;
				a = milliseconds_next * CLOCK_MILLISECOND_TO_TICK_CONSTANT;
				// read TCNT1 as late as possible since its value is updating
				x = TCNT2;
				if ( x >= a )
				{
					OCR2B = 0xFF;
					CLOCK_DEBUG_MSG2("Tickless expired : ticks");
					ptimer_request_poll();
				}
				else
				{
					OCR2B = a;
					#if CLOCK_DEBUG_LEVEL >= CLOCK_DEBUG_LEVEL2	
						DEBUG_CONST_PRINT("Condition satisfied, tickless milliSeconds = ");
						DEBUG_PRINT_INTEGER_LN(milliseconds_next);
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
						DEBUG_CONST_PRINT("Condition satisfied, tickless milliSeconds = ");
						DEBUG_PRINT_INTEGER_LN(milliseconds_next);
					#endif
				}

			#elif OS_TIMER_TYPE == TIMER_3
				mos_uint16_t a, x, y;
				a = milliseconds_next * CLOCK_MILLISECOND_TO_TICK_CONSTANT;
				// read TCNT1 as late as possible since its value is updating
				x = TCNT3L;
				y = TCNT3H;
				x = (y << 8)|x;
				if ( x >= a )
				{
					OCR3BH = 0xFF;
					OCR3BL = 0xFF;
					CLOCK_DEBUG_MSG2("Tickless expired : ticks");
					ptimer_request_poll();
				}
				else
				{
					OCR3BH = a >> 8;
					OCR3BL = a & 0xFF;
					#if CLOCK_DEBUG_LEVEL >= CLOCK_DEBUG_LEVEL2
						DEBUG_MSG_CONST3("<DEBUG CLOCK> ",NULL,"Condition satisfied, tickless milliSeconds = ");
						DEBUG_PRINT_INTEGER_LONG_LN(milliseconds_next);
					#endif
				}
				
			#elif OS_TIMER_TYPE == TIMER_4
				mos_uint16_t a, x, y;
				a = milliseconds_next * CLOCK_MILLISECOND_TO_TICK_CONSTANT;
				// read TCNT1 as late as possible since its value is updating
				x = TCNT4L;
				y = TCNT4H;
				x = (y << 8)|x;
				if ( x >= a )
				{
					OCR4BH = 0xFF;
					OCR4BL = 0xFF;
					CLOCK_DEBUG_MSG2("Tickless expired : ticks");
					ptimer_request_poll();
				}
				else
				{
					OCR4BH = a >> 8;
					OCR4BL = a & 0xFF;
					#if CLOCK_DEBUG_LEVEL >= CLOCK_DEBUG_LEVEL2
						DEBUG_MSG_CONST3("<DEBUG CLOCK> ",NULL,"Condition satisfied, tickless milliSeconds = ");
						DEBUG_PRINT_INTEGER_LONG_LN(milliseconds_next);
					#endif
				}

			#elif OS_TIMER_TYPE == TIMER_5
				mos_uint16_t a, x, y;
				a = milliseconds_next * CLOCK_MILLISECOND_TO_TICK_CONSTANT;
				// read TCNT1 as late as possible since its value is updating
				x = TCNT5L;
				y = TCNT5H;
				x = (y << 8)|x;
				if ( x >= a )
				{
					OCR5BH = 0xFF;
					OCR5BL = 0xFF;
					CLOCK_DEBUG_MSG2("Tickless expired : ticks");
					ptimer_request_poll();
				}
				else
				{
					OCR5BH = a >> 8;
					OCR5BL = a & 0xFF;
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

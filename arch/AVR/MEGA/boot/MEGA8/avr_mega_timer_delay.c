/**\addtogroup sources_MEGA8
* @{ \defgroup avr_mega_timer_delay_MEGA8_c
* @{
* \brief Documentation of avr_mega_timer_delay.c source file for atmega8 controller.   
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements TIMER Delay functions for atmega8 controller.
* @}
*/

#include "../../drivers/avr_mega_timer_delay.h"

#ifdef TIMER_DELAY_ENABLE

#ifndef OS_TIMER_TYPE
#define OS_TIMER_TYPE -1
#endif

#ifdef TIMER_DELAY_HELP_ENABLE	
#if OS_TIMER_TYPE == TIMER_0
#warning Available Timer numbers are : TIMER_DELAY1, TIMER_DELAY2. OS scheduler uses TIMER0 hence TIMER_DELAY0 is not available
#elif OS_TIMER_TYPE == TIMER_1
#warning Available Timer numbers are : TIMER_DELAY0, TIMER_DELAY2. OS scheduler uses TIMER1 hence TIMER_DELAY1 is not available
#elif OS_TIMER_TYPE == TIMER_2
#warning Available Timer numbers are : TIMER_DELAY0, TIMER_DELAY1. OS scheduler uses TIMER2 hence TIMER_DELAY2 is not available
#endif //OS_TIMER_TYPE == TIMER_0
#endif //TIMER_DELAY_HELP_ENABLE

#define TIMER_DELAY_RESOLUTION_US 100
#define TIMER_DELAY_SCALE (1000 / TIMER_DELAY_RESOLUTION_US)  // =10

#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ) )
#include <avr/interrupt.h>
#endif

#if OS_TIMER_TYPE != TIMER_0
static void (* volatile callback0)(void);
static volatile mos_uint16_t count0=0, count0_max=0;
#endif

#if OS_TIMER_TYPE != TIMER_1
static void (* volatile callback1)(void);
static volatile mos_uint16_t count1=0, count1_max=0;
#endif

#if OS_TIMER_TYPE != TIMER_2
static void (* volatile callback2)(void);
static volatile mos_uint16_t count2=0, count2_max=0;
#endif

static void begin (mos_uint8_t timer_no , float timer_delay_ms, void (*timer_delay_callback)(void))
{
	if(timer_delay_ms < 0.1f)
	{
		end(timer_no);  // optional but safer
		return;
	}
		
#if OS_TIMER_TYPE != TIMER_0
	if( timer_no == TIMER_0 )
	{
		mos_uint8_t state;
		
		TIMSK &= ~(1<<TOIE0);
		TCCR0 = 0;
		
		state = ATOMIC_ON_RESTORE();
		count0 = 0;
		
		if( timer_delay_ms > (65500/TIMER_DELAY_SCALE) )
		{
			count0_max = 65500;
		}
		else
			count0_max = (mos_uint16_t)(timer_delay_ms*TIMER_DELAY_SCALE + 0.5f);
		
		callback0 = timer_delay_callback;
#if CONTROLLER_FREQ == 16000000UL
		TCNT0=0x38;
#elif CONTROLLER_FREQ == 12000000UL
		TCNT0=0x6A;
#elif CONTROLLER_FREQ == 8000000UL
		TCNT0=0x9C;
#elif CONTROLLER_FREQ == 1000000UL
		TCNT0=0x9C;
#endif
		ATOMIC_OFF_RESTORE(state);
		
#if CONTROLLER_FREQ == 16000000UL
		TCCR0=(0<<CS02) | (1<<CS01) | (0<<CS00);
		TIMSK |= (1<<TOIE0);
#elif CONTROLLER_FREQ == 12000000UL
		TCCR0=(0<<CS02) | (1<<CS01) | (0<<CS00);
		TIMSK |= (1<<TOIE0);
#elif CONTROLLER_FREQ == 8000000UL
		TCCR0=(0<<CS02) | (1<<CS01) | (0<<CS00);
		TIMSK |= (1<<TOIE0);
#elif CONTROLLER_FREQ == 1000000UL
		TCCR0=(0<<CS02) | (0<<CS01) | (1<<CS00);
		TIMSK |= (1<<TOIE0);
#endif
	}
#endif

#if OS_TIMER_TYPE != TIMER_1	
	if( timer_no == TIMER_1 )
	{	
		mos_uint8_t state;
		TIMSK &= ~(1<<OCIE1A);
		TCCR1A = 0;
		TCCR1B = 0;
		
		state = ATOMIC_ON_RESTORE();
		count1 = 0;
		
		if( timer_delay_ms > (65500/TIMER_DELAY_SCALE) )
		{
			count1_max = 65500;
		}
		else
			count1_max = (mos_uint16_t)(timer_delay_ms*TIMER_DELAY_SCALE + 0.5f);
		
		callback1 = timer_delay_callback;	
		TCNT1=0x0000;
		ATOMIC_OFF_RESTORE(state);
		
#if CONTROLLER_FREQ == 16000000UL
		OCR1A=0x00C7;
		TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
		TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
		ICR1=0x0000;
		OCR1B=0x0000;
		TIMSK |= (1<<OCIE1A);
#elif CONTROLLER_FREQ == 12000000UL
		OCR1A=0x0095;
		TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
		TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
		ICR1=0x0000;
		OCR1B=0x0000;
		TIMSK |= (1<<OCIE1A);
#elif CONTROLLER_FREQ == 8000000UL
		OCR1A=0x0063;
		TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
		TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
		ICR1=0x0000;
		OCR1B=0x0000;
		TIMSK |= (1<<OCIE1A);
#elif CONTROLLER_FREQ == 1000000UL
		OCR1A=0x0063;
		TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
		TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
		ICR1=0x0000;
		OCR1B=0x0000;
		TIMSK |= (1<<OCIE1A);
#endif
	}
#endif

#if OS_TIMER_TYPE != TIMER_2
	if( timer_no == TIMER_2 )
	{	
		mos_uint8_t state;
		TIMSK &= ~(1<<OCIE2);
		TCCR2 = 0;
		
		state = ATOMIC_ON_RESTORE();
		count2 = 0;
		
		if( timer_delay_ms > (65500/TIMER_DELAY_SCALE) )
		{
			count2_max = 65500;
		}
		else
			count2_max = (mos_uint16_t)(timer_delay_ms*TIMER_DELAY_SCALE + 0.5f);
		
		callback2 = timer_delay_callback;
		TCNT2=0x00;
		ATOMIC_OFF_RESTORE(state);
		
#if CONTROLLER_FREQ == 16000000UL
		OCR2=0xC7;
		TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (1<<CS21) | (0<<CS20);
		TIMSK |= (1<<OCIE2);
#elif CONTROLLER_FREQ == 12000000UL
		OCR2=0x95;
		TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (1<<CS21) | (0<<CS20);
		TIMSK |= (1<<OCIE2);
#elif CONTROLLER_FREQ == 8000000UL
		OCR2=0x63;
		TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (1<<CS21) | (0<<CS20);
		TIMSK |= (1<<OCIE2);
#elif CONTROLLER_FREQ == 1000000UL
		OCR2=0x63;
		TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (0<<CS21) | (1<<CS20);
		TIMSK |= (1<<OCIE2);
#endif
	}
#endif

	if(IS_GLOBAL_INTERRUPT_DISABLE())
		GLOBAL_INTERRUPT_ENABLE();
}

static void end (mos_uint8_t timer_no)
{
	
#if OS_TIMER_TYPE != TIMER_0
	if( timer_no == TIMER_0 )
	{
		TCCR0=(0<<CS02) | (0<<CS01) | (0<<CS00);
		TCNT0=0x00;
		
		TIMSK &= ~(1<<TOIE0);
	}
#endif

#if OS_TIMER_TYPE != TIMER_1	
	if( timer_no == TIMER_1 )
	{	
		TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
		TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
		TCNT1=0x00;
		ICR1=0x00;
		OCR1A=0x00;
		OCR1B=0x00;
		
		TIMSK &= ~(1<<OCIE1A);
	}
#endif

#if OS_TIMER_TYPE != TIMER_2
	if( timer_no == TIMER_2 )
	{	
		TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (0<<WGM21) | (0<<CS22) | (0<<CS21) | (0<<CS20);
		TCNT2=0x00;
		OCR2=0x00;
		
		TIMSK &= ~(1<<OCIE2);
	}
#endif
}

#if OS_TIMER_TYPE != TIMER_0
ISR(TIMER0_OVF_vect)
{
#if CONTROLLER_FREQ == 16000000UL
	TCNT0=0x38;
#elif CONTROLLER_FREQ == 12000000UL
	TCNT0=0x6A;
#elif CONTROLLER_FREQ == 8000000UL
	TCNT0=0x9C;
#elif CONTROLLER_FREQ == 1000000UL
	TCNT0=0x9C;
#endif
	count0++;
	if(count0 >= count0_max)
	{
		count0 = 0;
		if(callback0)
			callback0();
	}
}
#endif

#if OS_TIMER_TYPE != TIMER_1
ISR(TIMER1_COMPA_vect)
{
	count1++;
	if(count1 >= count1_max)
	{
		count1 = 0;
		if(callback1)
			callback1();
	}
}
#endif

#if OS_TIMER_TYPE != TIMER_2
ISR(TIMER2_COMP_vect)
{
	count2++;
	if(count2 >= count2_max)
	{
		count2 = 0;
		if(callback2)
			callback2();
	}
}
#endif

const struct timer_delay_driver avr_mega_timer_delay_driver = {
	begin,
	end
};

#if OS_TIMER_TYPE == -1
#undef OS_TIMER_TYPE
#endif

#undef TIMER_DELAY_RESOLUTION_US
#undef TIMER_DELAY_SCALE

#endif


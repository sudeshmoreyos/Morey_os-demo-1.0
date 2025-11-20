/**\addtogroup sources_MEGA128
* @{ \defgroup avr_mega_timer_delay_MEGA128_c
* @{
* \brief Documentaion of avr_mega_timer_delay.c source file for atmega128 controller.   
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements TIMER Delay functions for atmega128 controller.
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
	#endif
#endif

#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ) )
	#include <avr/interrupt.h>
#endif


#if OS_TIMER_TYPE != TIMER_0
	static void (*callback0)(void);
	static mos_uint16_t count0=0, count0_max=0;
#endif

#if OS_TIMER_TYPE != TIMER_1
	static void (*callback1)(void);
	static mos_uint16_t count1=0, count1_max=0;
#endif

#if OS_TIMER_TYPE != TIMER_2
	static void (*callback2)(void);
	static mos_uint16_t count2=0, count2_max=0;
#endif

static void begin (mos_uint8_t timer_no , float timer_delay_ms, void (*timer_delay_callback)(void))
{	
	#if OS_TIMER_TYPE != TIMER_0
		if( timer_no == TIMER_0 )
		{
			#if CONTROLLER_FREQ == 16000000UL
				TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (1<<WGM01) | (0<<WGM00);
				TCCR0B=(0<<WGM02) | (0<<CS02) | (1<<CS01) | (0<<CS00);
				TCNT0=0x00;
				OCR0A=0xC7;
				OCR0B=0x00;
			#elif CONTROLLER_FREQ == 12000000UL
				TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (1<<WGM01) | (0<<WGM00);
				TCCR0B=(0<<WGM02) | (0<<CS02) | (1<<CS01) | (0<<CS00);
				TCNT0=0x00;
				OCR0A=0x95;
				OCR0B=0x00;
			#elif CONTROLLER_FREQ == 8000000UL
				TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (1<<WGM01) | (0<<WGM00);
				TCCR0B=(0<<WGM02) | (0<<CS02) | (1<<CS01) | (0<<CS00);
				TCNT0=0x00;
				OCR0A=0x63;
				OCR0B=0x00;
			#elif CONTROLLER_FREQ == 1000000UL
				TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (1<<WGM01) | (0<<WGM00);
				TCCR0B=(0<<WGM02) | (0<<CS02) | (0<<CS01) | (1<<CS00);
				TCNT0=0x00;
				OCR0A=0x63;
				OCR0B=0x00;
			#endif
			
			if( timer_delay_ms*10 > 65500 )
			{
				count0_max = 65500;
			}
			else
				count0_max = (mos_uint16_t)(timer_delay_ms*10);
			
			callback0 = timer_delay_callback;
			TIMSK0=(0<<OCIE0B) | (1<<OCIE0A) | (0<<TOIE0);
		}
	#endif

	#if OS_TIMER_TYPE != TIMER_1	
		if( timer_no == TIMER_1 )
		{	
			#if CONTROLLER_FREQ == 16000000UL
				TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
				TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
				TCNT1H=0x00;
				TCNT1L=0x00;
				ICR1H=0x00;
				ICR1L=0x00;
				OCR1AH=0x00;
				OCR1AL=0xC7;
				OCR1BH=0x00;
				OCR1BL=0x00;
			#elif CONTROLLER_FREQ == 12000000UL
				TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
				TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
				TCNT1H=0x00;
				TCNT1L=0x00;
				ICR1H=0x00;
				ICR1L=0x00;
				OCR1AH=0x00;
				OCR1AL=0x95;
				OCR1BH=0x00;
				OCR1BL=0x00;
			#elif CONTROLLER_FREQ == 8000000UL
				TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
				TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
				TCNT1H=0x00;
				TCNT1L=0x00;
				ICR1H=0x00;
				ICR1L=0x00;
				OCR1AH=0x00;
				OCR1AL=0x63;
				OCR1BH=0x00;
				OCR1BL=0x00;
			#elif CONTROLLER_FREQ == 1000000UL
				TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
				TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
				TCNT1H=0x00;
				TCNT1L=0x00;
				ICR1H=0x00;
				ICR1L=0x00;
				OCR1AH=0x00;
				OCR1AL=0x63;
				OCR1BH=0x00;
				OCR1BL=0x00;
			#endif
			
			if( timer_delay_ms*10 > 65500 )
			{
				count1_max = 65500;
			}
			else
				count1_max = (mos_uint16_t)(timer_delay_ms*10);
			
			callback1 = timer_delay_callback;
			
			TIMSK1=(0<<ICIE1) | (0<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1);
		}
	#endif
	
	#if OS_TIMER_TYPE != TIMER_2
		if( timer_no == TIMER_2 )
		{	
			#if CONTROLLER_FREQ == 16000000UL
				ASSR=(0<<EXCLK) | (0<<AS2);
				TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (1<<WGM21) | (0<<WGM20);
				TCCR2B=(0<<WGM22) | (0<<CS22) | (1<<CS21) | (0<<CS20);
				TCNT2=0x00;
				OCR2A=0xC7;
				OCR2B=0x00;
			#elif CONTROLLER_FREQ == 12000000UL
				ASSR=(0<<EXCLK) | (0<<AS2);
				TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (1<<WGM21) | (0<<WGM20);
				TCCR2B=(0<<WGM22) | (0<<CS22) | (1<<CS21) | (0<<CS20);
				TCNT2=0x00;
				OCR2A=0x95;
				OCR2B=0x00;
			#elif CONTROLLER_FREQ == 8000000UL
				ASSR=(0<<EXCLK) | (0<<AS2);
				TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (1<<WGM21) | (0<<WGM20);
				TCCR2B=(0<<WGM22) | (0<<CS22) | (1<<CS21) | (0<<CS20);
				TCNT2=0x00;
				OCR2A=0x63;
				OCR2B=0x00;
			#elif CONTROLLER_FREQ == 1000000UL
				ASSR=(0<<EXCLK) | (0<<AS2);
				TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (1<<WGM21) | (0<<WGM20);
				TCCR2B=(0<<WGM22) | (0<<CS22) | (0<<CS21) | (1<<CS20);
				TCNT2=0x00;
				OCR2A=0x63;
				OCR2B=0x00;
			#endif
			
			if( timer_delay_ms*10 > 65500 )
			{
				count2_max = 65500;
			}
			else
				count2_max = (mos_uint16_t)(timer_delay_ms*10);
			
			callback2 = timer_delay_callback;
			TIMSK2=(0<<OCIE2B) | (1<<OCIE2A) | (0<<TOIE2);			
		}
	#endif
}

static void end (mos_uint8_t timer_no)
{	
	#if OS_TIMER_TYPE != TIMER_0
		if( timer_no == TIMER_0 )
		{
			TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
			TCCR0B=(0<<WGM02) | (0<<CS02) | (0<<CS01) | (0<<CS00);
			TCNT0=0x00;
			OCR0A=0x00;
			OCR0B=0x00;
		
			TIMSK0=(0<<OCIE0B) | (0<<OCIE0A) | (0<<TOIE0);
		}
	#endif

	#if OS_TIMER_TYPE != TIMER_1	
		if( timer_no == TIMER_1 )
		{	
			TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
			TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
			TCNT1H=0x00;
			TCNT1L=0x00;
			ICR1H=0x00;
			ICR1L=0x00;
			OCR1AH=0x00;
			OCR1AL=0x00;
			OCR1BH=0x00;
			OCR1BL=0x00;
			
			TIMSK1=(0<<ICIE1) | (0<<OCIE1B) | (0<<OCIE1A) | (0<<TOIE1);
		}
	#endif
	
	#if OS_TIMER_TYPE != TIMER_2
		if( timer_no == TIMER_2 )
		{	
			ASSR=(0<<EXCLK) | (0<<AS2);
			TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (0<<WGM20);
			TCCR2B=(0<<WGM22) | (0<<CS22) | (0<<CS21) | (0<<CS20);
			TCNT2=0x00;
			OCR2A=0x00;
			OCR2B=0x00;
			
			TIMSK2=(0<<OCIE2B) | (0<<OCIE2A) | (0<<TOIE2);			
		}
	#endif
}

#if OS_TIMER_TYPE != TIMER_0
	ISR(TIMER0_COMPA_vect)
	{
		count0++;
		if(count0 >= count0_max)
		{
			count0 = 0;
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
			callback1();
		}
	}
#endif

#if OS_TIMER_TYPE != TIMER_2
	ISR(TIMER2_COMPA_vect)
	{
		count2++;
		if(count2 >= count2_max)
		{
			count2 = 0;
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

#endif



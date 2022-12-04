#include "../../drivers/avr_mega_timer_delay.h"

#ifdef TIMER_DELAY_HELP_ENABLE	
	#if TIMER_TYPE == TIMER_0
		#warning Available Timer numbers are : TIMER_DELAY1, TIMER_DELAY2. OS scheduler uses TIMER0 hence TIMER_DELAY0 is not available
	#elif TIMER_TYPE == TIMER_1
		#warning Available Timer numbers are : TIMER_DELAY0, TIMER_DELAY2. OS scheduler uses TIMER1 hence TIMER_DELAY1 is not available
	#elif TIMER_TYPE == TIMER_2
		#warning Available Timer numbers are : TIMER_DELAY0, TIMER_DELAY1. OS scheduler uses TIMER2 hence TIMER_DELAY2 is not available
	#endif
#endif

#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ) )
	#include <avr/interrupt.h>
#endif

#if TIMER_TYPE != TIMER_0
	static void (*callback0)(void);
	mos_uint16_t count0=0, count0_max=0;
#endif

#if TIMER_TYPE != TIMER_1
	static void (*callback1)(void);
	mos_uint16_t count1=0, count1_max=0;
#endif

#if TIMER_TYPE != TIMER_2
	static void (*callback2)(void);
	mos_uint16_t count2=0, count2_max=0;
#endif

static void begin (mos_uint8_t timer_no , float timer_delay_ms, void (*timer_delay_callback)(void))
{	
	#if TIMER_TYPE != TIMER_0
		if( timer_no == TIMER_0 )
		{
			#if CONTROLLER_FREQ == 16000000UL
				TCCR0=(0<<CS02) | (1<<CS01) | (0<<CS00);
				TCNT0=0x38;
			#elif CONTROLLER_FREQ == 12000000UL
				TCCR0=(0<<CS02) | (1<<CS01) | (0<<CS00);
				TCNT0=0x6A;
			#elif CONTROLLER_FREQ == 8000000UL
				TCCR0=(0<<CS02) | (1<<CS01) | (0<<CS00);
				TCNT0=0x9C;
			#elif CONTROLLER_FREQ == 1000000UL
				TCCR0=(0<<CS02) | (0<<CS01) | (1<<CS00);
				TCNT0=0x9C;
			#endif
			
			if( timer_delay_ms*10 > 65500 )
			{
				count0_max = 65500;
			}
			else
				count0_max = (mos_uint16_t)(timer_delay_ms*10);
			
			callback0 = timer_delay_callback;
			TIMSK |= (1<<TOIE0);
		}
	#endif

	#if TIMER_TYPE != TIMER_1	
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
			
			TIMSK |= (1<<OCIE1A);
		}
	#endif
	
	#if TIMER_TYPE != TIMER_2
		if( timer_no == TIMER_2 )
		{	
			#if CONTROLLER_FREQ == 16000000UL
				ASSR=0<<AS2;
				TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (1<<CS21) | (0<<CS20);
				TCNT2=0x00;
				OCR2=0xC7;
			#elif CONTROLLER_FREQ == 12000000UL
				ASSR=0<<AS2;
				TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (1<<CS21) | (0<<CS20);
				TCNT2=0x00;
				OCR2=0x95;
			#elif CONTROLLER_FREQ == 8000000UL
				ASSR=0<<AS2;
				TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (1<<CS21) | (0<<CS20);
				TCNT2=0x00;
				OCR2=0x63;
			#elif CONTROLLER_FREQ == 1000000UL
				ASSR=0<<AS2;
				TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (0<<CS21) | (1<<CS20);
				TCNT2=0x00;
				OCR2=0x63;
			#endif
			
			if( timer_delay_ms*10 > 65500 )
			{
				count2_max = 65500;
			}
			else
				count2_max = (mos_uint16_t)(timer_delay_ms*10);
			
			callback2 = timer_delay_callback;
			TIMSK |= (1<<OCIE2);
		}
	#endif
}

static void end (mos_uint8_t timer_no)
{	
	#if TIMER_TYPE != TIMER_0
		if( timer_no == TIMER_0 )
		{
			TCCR0=(0<<CS02) | (0<<CS01) | (0<<CS00);
			TCNT0=0x00;
			
			TIMSK &= ~(1<<TOIE0);
		}
	#endif

	#if TIMER_TYPE != TIMER_1	
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
			
			TIMSK &= ~(1<<OCIE1A);
		}
	#endif
	
	#if TIMER_TYPE != TIMER_2
		if( timer_no == TIMER_2 )
		{	
			ASSR=0<<AS2;
			TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (0<<WGM21) | (0<<CS22) | (0<<CS21) | (0<<CS20);
			TCNT2=0x00;
			OCR2=0x00;
			
			TIMSK &= ~(1<<OCIE2);
		}
	#endif
}

#if TIMER_TYPE != TIMER_0
	ISR(TIMER0_OVF_vect)
	{
		TCNT0=0x38;
		count0++;
		if(count0 >= count0_max)
		{
			count0 = 0;
			callback0();
		}
	}
#endif

#if TIMER_TYPE != TIMER_1
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

#if TIMER_TYPE != TIMER_2
	ISR(TIMER2_COMP_vect)
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

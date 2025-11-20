/**\addtogroup sources_MEGA328PB
* @{ \defgroup avr_mega_timer_delay_MEGA328PB_c
* @{
* \brief Documentaion of avr_mega_timer_delay.c source file for atmega328pb controller.   
* \author Sudesh Morey <sudesh.moreyos@gmail.com>
*
* This source file implements TIMER Delay functions for atmega328pb controller.
* @}
*/

#include "../../drivers/avr_mega_timer_delay.h"

#ifdef TIMER_DELAY_WARNING_ENABLE
	
	#if OS_TIMER_TYPE == TIMER_0
	#warning "Morey OS scheduler clock is depedent on TIMER_0. Hence either use TIMER_1 or TIMER_2 or TIMER_3 or TIMER_4 \
	in TimerDelay.begin function or change OS timer using OS_TIMER_TYPE_CONF Macro."
	#endif
	
	#if OS_TIMER_TYPE == TIMER_1
	#warning "Morey OS scheduler clock is depedent on TIMER_1. Hence either use TIMER_0 or TIMER_2 or TIMER_3 or TIMER_4 \
	in TimerDelay.begin function or change OS timer using OS_TIMER_TYPE_CONF Macro."
	#endif
	
	#if OS_TIMER_TYPE == TIMER_2
	#warning "Morey OS scheduler clock is depedent on TIMER_2. Hence either use TIMER_0 or TIMER_1 or TIMER_3 or TIMER_4 \
	in TimerDelay.begin function or change OS timer using OS_TIMER_TYPE_CONF Macro."
	#endif
	
	#if OS_TIMER_TYPE == TIMER_3
	#warning "Morey OS scheduler clock is depedent on TIMER_3. Hence either use TIMER_0 or TIMER_1 or TIMER_2 or TIMER_4 \
	in TimerDelay.begin function or change OS timer using OS_TIMER_TYPE_CONF Macro."
	#endif
	
	#if OS_TIMER_TYPE == TIMER_4
	#warning "Morey OS scheduler clock is depedent on TIMER_4. Hence either use TIMER_0 or TIMER_1 or TIMER_2 or TIMER_3 \
	in TimerDelay.begin function or change OS timer using OS_TIMER_TYPE_CONF Macro."
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

#if OS_TIMER_TYPE != TIMER_3
	static void (*callback3)(void);
	static mos_uint16_t count3=0, count3_max=0;
#endif

#if OS_TIMER_TYPE != TIMER_4
	static void (*callback4)(void);
	static mos_uint16_t count4=0, count4_max=0;
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
	
	#if OS_TIMER_TYPE != TIMER_3	
		if( timer_no == TIMER_3 )
		{	
			#if CONTROLLER_FREQ == 16000000UL
				TCCR3A=(0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<WGM31) | (0<<WGM30);
				TCCR3B=(0<<ICNC3) | (0<<ICES3) | (0<<WGM33) | (1<<WGM32) | (0<<CS32) | (1<<CS31) | (0<<CS30);
				TCNT3H=0x00;
				TCNT3L=0x00;
				ICR3H=0x00;
				ICR3L=0x00;
				OCR3AH=0x00;
				OCR3AL=0xC7;
				OCR3BH=0x00;
				OCR3BL=0x00;
			#elif CONTROLLER_FREQ == 12000000UL
				TCCR3A=(0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<WGM31) | (0<<WGM30);
				TCCR3B=(0<<ICNC3) | (0<<ICES3) | (0<<WGM33) | (1<<WGM32) | (0<<CS32) | (1<<CS31) | (0<<CS30);
				TCNT3H=0x00;
				TCNT3L=0x00;
				ICR3H=0x00;
				ICR3L=0x00;
				OCR3AH=0x00;
				OCR3AL=0x95;
				OCR3BH=0x00;
				OCR3BL=0x00;
			#elif CONTROLLER_FREQ == 8000000UL				
				TCCR3A=(0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<WGM31) | (0<<WGM30);
				TCCR3B=(0<<ICNC3) | (0<<ICES3) | (0<<WGM33) | (1<<WGM32) | (0<<CS32) | (1<<CS31) | (0<<CS30);
				TCNT3H=0x00;
				TCNT3L=0x00;
				ICR3H=0x00;
				ICR3L=0x00;
				OCR3AH=0x00;
				OCR3AL=0x63;
				OCR3BH=0x00;
				OCR3BL=0x00;
			#elif CONTROLLER_FREQ == 1000000UL				
				TCCR3A=(0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<WGM31) | (0<<WGM30);
				TCCR3B=(0<<ICNC3) | (0<<ICES3) | (0<<WGM33) | (1<<WGM32) | (0<<CS32) | (0<<CS31) | (1<<CS30);
				TCNT3H=0x00;
				TCNT3L=0x00;
				ICR3H=0x00;
				ICR3L=0x00;
				OCR3AH=0x00;
				OCR3AL=0x63;
				OCR3BH=0x00;
				OCR3BL=0x00;
			#endif
			
			if( timer_delay_ms*10 > 65500 )
			{
				count3_max = 65500;
			}
			else
				count3_max = (mos_uint16_t)(timer_delay_ms*10);
			
			callback3 = timer_delay_callback;
			
			TIMSK3=(0<<ICIE3) | (0<<OCIE3B) | (1<<OCIE3A) | (0<<TOIE3);
		}
	#endif
	
	#if OS_TIMER_TYPE != TIMER_4	
		if( timer_no == TIMER_4 )
		{	
			#if CONTROLLER_FREQ == 16000000UL
				TCCR4A=(0<<COM4A1) | (0<<COM4A0) | (0<<COM4B1) | (0<<COM4B0) | (0<<WGM41) | (0<<WGM40);
				TCCR4B=(0<<ICNC4) | (0<<ICES4) | (0<<WGM43) | (1<<WGM42) | (0<<CS42) | (1<<CS41) | (0<<CS40);
				TCNT4H=0x00;
				TCNT4L=0x00;
				ICR4H=0x00;
				ICR4L=0x00;
				OCR4AH=0x00;
				OCR4AL=0xC7;
				OCR4BH=0x00;
				OCR4BL=0x00;
			#elif CONTROLLER_FREQ == 12000000UL
				TCCR4A=(0<<COM4A1) | (0<<COM4A0) | (0<<COM4B1) | (0<<COM4B0) | (0<<WGM41) | (0<<WGM40);
				TCCR4B=(0<<ICNC4) | (0<<ICES4) | (0<<WGM43) | (1<<WGM42) | (0<<CS42) | (1<<CS41) | (0<<CS40);
				TCNT4H=0x00;
				TCNT4L=0x00;
				ICR4H=0x00;
				ICR4L=0x00;
				OCR4AH=0x00;
				OCR4AL=0x95;
				OCR4BH=0x00;
				OCR4BL=0x00;
			#elif CONTROLLER_FREQ == 8000000UL				
				TCCR4A=(0<<COM4A1) | (0<<COM4A0) | (0<<COM4B1) | (0<<COM4B0) | (0<<WGM41) | (0<<WGM40);
				TCCR4B=(0<<ICNC4) | (0<<ICES4) | (0<<WGM43) | (1<<WGM42) | (0<<CS42) | (1<<CS41) | (0<<CS40);
				TCNT4H=0x00;
				TCNT4L=0x00;
				ICR4H=0x00;
				ICR4L=0x00;
				OCR4AH=0x00;
				OCR4AL=0x63;
				OCR4BH=0x00;
				OCR4BL=0x00;
			#elif CONTROLLER_FREQ == 1000000UL				
				TCCR4A=(0<<COM4A1) | (0<<COM4A0) | (0<<COM4B1) | (0<<COM4B0) | (0<<WGM41) | (0<<WGM40);
				TCCR4B=(0<<ICNC4) | (0<<ICES4) | (0<<WGM43) | (1<<WGM42) | (0<<CS42) | (0<<CS41) | (1<<CS40);
				TCNT4H=0x00;
				TCNT4L=0x00;
				ICR4H=0x00;
				ICR4L=0x00;
				OCR4AH=0x00;
				OCR4AL=0x63;
				OCR4BH=0x00;
				OCR4BL=0x00;
			#endif
			
			if( timer_delay_ms*10 > 65500 )
			{
				count4_max = 65500;
			}
			else
				count4_max = (mos_uint16_t)(timer_delay_ms*10);
			
			callback4 = timer_delay_callback;
			
			TIMSK4=(0<<ICIE4) | (0<<OCIE4B) | (1<<OCIE4A) | (0<<TOIE4);
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
	
	#if OS_TIMER_TYPE != TIMER_3	
		if( timer_no == TIMER_3 )
		{	
			TCCR3A=(0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<WGM31) | (0<<WGM30);
			TCCR3B=(0<<ICNC3) | (0<<ICES3) | (0<<WGM33) | (0<<WGM32) | (0<<CS32) | (0<<CS31) | (0<<CS30);
			TCNT3H=0x00;
			TCNT3L=0x00;
			ICR3H=0x00;
			ICR3L=0x00;
			OCR3AH=0x00;
			OCR3AL=0x00;
			OCR3BH=0x00;
			OCR3BL=0x00;
			
			TIMSK3=(0<<ICIE3) | (0<<OCIE3B) | (0<<OCIE3A) | (0<<TOIE3);
		}
	#endif
	
	#if OS_TIMER_TYPE != TIMER_4
		if( timer_no == TIMER_4 )
		{	
			TCCR4A=(0<<COM4A1) | (0<<COM4A0) | (0<<COM4B1) | (0<<COM4B0) | (0<<WGM41) | (0<<WGM40);
			TCCR4B=(0<<ICNC4) | (0<<ICES4) | (0<<WGM43) | (0<<WGM42) | (0<<CS42) | (0<<CS41) | (0<<CS40);
			TCNT4H=0x00;
			TCNT4L=0x00;
			ICR4H=0x00;
			ICR4L=0x00;
			OCR4AH=0x00;
			OCR4AL=0x00;
			OCR4BH=0x00;
			OCR4BL=0x00;
			
			TIMSK4=(0<<ICIE4) | (0<<OCIE4B) | (0<<OCIE4A) | (0<<TOIE4);
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

#if OS_TIMER_TYPE != TIMER_3
	ISR(TIMER3_COMPA_vect)
	{
		count3++;
		if(count3 >= count3_max)
		{
			count3 = 0;
			callback3();
		}
	}
#endif

#if OS_TIMER_TYPE != TIMER_4
	ISR(TIMER4_COMPA_vect)
	{
		count4++;
		if(count4 >= count4_max)
		{
			count4 = 0;
			callback4();
		}
	}
#endif

const struct timer_delay_driver avr_mega_timer_delay_driver = {
	begin,
	end
};


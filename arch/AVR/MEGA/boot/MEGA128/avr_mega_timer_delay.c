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
#warning Available Delay Timer numbers are : TIMER_DELAY1, TIMER_DELAY2, TIMER_DELAY3. OS scheduler uses TIMER0 hence TIMER_DELAY0 is not available
#elif OS_TIMER_TYPE == TIMER_1
#warning Available Delay Timer numbers are : TIMER_DELAY0, TIMER_DELAY2, TIMER_DELAY3. OS scheduler uses TIMER1 hence TIMER_DELAY1 is not available
#elif OS_TIMER_TYPE == TIMER_2
#warning Available Delay Timer numbers are : TIMER_DELAY0, TIMER_DELAY1, TIMER_DELAY3. OS scheduler uses TIMER2 hence TIMER_DELAY2 is not available
#elif OS_TIMER_TYPE == TIMER_3
#warning Available Delay Timer numbers are : TIMER_DELAY0, TIMER_DELAY1, TIMER_DELAY2. OS scheduler uses TIMER3 hence TIMER_DELAY3 is not available
#endif // OS_TIMER_TYPE == TIMER_0
#endif // TIMER_DELAY_HELP_ENABLE

#if ((COMPILER == AVR_STUDIO) || (COMPILER == WIN_AVR ) || (COMPILER == AVR_GCC ) )
#include <avr/interrupt.h>
#endif


#if OS_TIMER_TYPE != TIMER_0
static void (*callback0)(void);
volatile static mos_uint16_t count0=0, count0_max=0;
#endif

#if OS_TIMER_TYPE != TIMER_1
static void (*callback1)(void);
volatile static mos_uint16_t count1=0, count1_max=0;
#endif

#if OS_TIMER_TYPE != TIMER_2
static void (*callback2)(void);
volatile static mos_uint16_t count2=0, count2_max=0;
#endif

#if OS_TIMER_TYPE != TIMER_3
static void (*callback3)(void);
volatile static mos_uint16_t count3=0, count3_max=0;
#endif


static void begin (mos_uint8_t timer_no , float timer_delay_ms, void (*timer_delay_callback)(void))
{	
	if (timer_delay_ms < 0.1)
		return;
	
#if OS_TIMER_TYPE != TIMER_0
	if( timer_no == TIMER_0 )
	{
		TIMSK &= ~((1<<TOIE0) | (1<<OCIE0));
		
#if CONTROLLER_FREQ == 16000000UL
		ASSR=0<<AS0;
		TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (1<<WGM01) | (0<<CS02) | (1<<CS01) | (0<<CS00);
		TCNT0=0x00;
		OCR0=0xC7;
#elif CONTROLLER_FREQ == 12000000UL
		ASSR=0<<AS0;
		TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (1<<WGM01) | (0<<CS02) | (1<<CS01) | (0<<CS00);
		TCNT0=0x00;
		OCR0=0x95;
#elif CONTROLLER_FREQ == 8000000UL
		ASSR=0<<AS0;
		TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (1<<WGM01) | (0<<CS02) | (1<<CS01) | (0<<CS00);
		TCNT0=0x00;
		OCR0=0x63;
#elif CONTROLLER_FREQ == 1000000UL
		ASSR=0<<AS0;
		TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (1<<WGM01) | (0<<CS02) | (0<<CS01) | (1<<CS00);
		TCNT0=0x00;
		OCR0=0x63;
#endif // CONTROLLER_FREQ == 16000000UL

		count0 = 0;
		
		if( timer_delay_ms*10 > 65500 )
		{
			count0_max = 65500;
		}
		else
			count0_max = (mos_uint16_t)(timer_delay_ms*10);
		
		callback0 = timer_delay_callback;
		TIMSK |= (1<<OCIE0);
	}
#endif // OS_TIMER_TYPE != TIMER_0

#if OS_TIMER_TYPE != TIMER_1	
	if( timer_no == TIMER_1 )
	{	
		TIMSK &= ~((1<<TICIE1) | (1<<OCIE1A) | (1<<OCIE1B) | (1<<TOIE1));
#if CONTROLLER_FREQ == 16000000UL
		TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);
		TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
		TCNT1H=0x00;
		TCNT1L=0x00;
		ICR1H=0x00;
		ICR1L=0x00;
		OCR1AH=0x00;
		OCR1AL=0xC7;
		OCR1BH=0x00;
		OCR1BL=0x00;
		OCR1CH=0x00;
		OCR1CL=0x00;
#elif CONTROLLER_FREQ == 12000000UL
		TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);
		TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
		TCNT1H=0x00;
		TCNT1L=0x00;
		ICR1H=0x00;
		ICR1L=0x00;
		OCR1AH=0x00;
		OCR1AL=0x95;
		OCR1BH=0x00;
		OCR1BL=0x00;
		OCR1CH=0x00;
		OCR1CL=0x00;
#elif CONTROLLER_FREQ == 8000000UL
		TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);
		TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
		TCNT1H=0x00;
		TCNT1L=0x00;
		ICR1H=0x00;
		ICR1L=0x00;
		OCR1AH=0x00;
		OCR1AL=0x63;
		OCR1BH=0x00;
		OCR1BL=0x00;
		OCR1CH=0x00;
		OCR1CL=0x00;
#elif CONTROLLER_FREQ == 1000000UL
		TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);
		TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10);
		TCNT1H=0x00;
		TCNT1L=0x00;
		ICR1H=0x00;
		ICR1L=0x00;
		OCR1AH=0x00;
		OCR1AL=0x63;
		OCR1BH=0x00;
		OCR1BL=0x00;
		OCR1CH=0x00;
		OCR1CL=0x00;
#endif
		count1 = 0;
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
	
#if OS_TIMER_TYPE != TIMER_2
	if( timer_no == TIMER_2 )
	{	
		TIMSK &= ~((1<<OCIE2)|(1<<TOIE2));
#if CONTROLLER_FREQ == 16000000UL
		TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (1<<CS21) | (0<<CS20);
		TCNT2=0x00;
		OCR2=0xC7;
#elif CONTROLLER_FREQ == 12000000UL
		TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (1<<CS21) | (0<<CS20);
		TCNT2=0x00;
		OCR2=0x95;
#elif CONTROLLER_FREQ == 8000000UL
		TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (1<<CS21) | (0<<CS20);
		TCNT2=0x00;
		OCR2=0x63;
#elif CONTROLLER_FREQ == 1000000UL
		TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (1<<WGM21) | (0<<CS22) | (0<<CS21) | (1<<CS20);
		TCNT2=0x00;
		OCR2=0x63;
#endif
		count2 =0;
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

#if OS_TIMER_TYPE != TIMER_3	
	if( timer_no == TIMER_3 )
	{	
		ETIMSK &= ~((1<<TICIE3) | (1<<OCIE3A) | (1<<OCIE3B) | (1<<TOIE3) | (1<<OCIE3C));
#if CONTROLLER_FREQ == 16000000UL
		TCCR3A=(0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<COM3C1) | (0<<COM3C0) | (0<<WGM31) | (0<<WGM30);
		TCCR3B=(0<<ICNC3) | (0<<ICES3) | (0<<WGM33) | (1<<WGM32) | (0<<CS32) | (1<<CS31) | (0<<CS30);
		TCNT3H=0x00;
		TCNT3L=0x00;
		ICR3H=0x00;
		ICR3L=0x00;
		OCR3AH=0x00;
		OCR3AL=0xC7;
		OCR3BH=0x00;
		OCR3BL=0x00;
		OCR3CH=0x00;
		OCR3CL=0x00;
#elif CONTROLLER_FREQ == 12000000UL
		TCCR3A=(0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<COM3C1) | (0<<COM3C0) | (0<<WGM31) | (0<<WGM30);
		TCCR3B=(0<<ICNC3) | (0<<ICES3) | (0<<WGM33) | (1<<WGM32) | (0<<CS32) | (1<<CS31) | (0<<CS30);
		TCNT3H=0x00;
		TCNT3L=0x00;
		ICR3H=0x00;
		ICR3L=0x00;
		OCR3AH=0x00;
		OCR3AL=0x95;
		OCR3BH=0x00;
		OCR3BL=0x00;
		OCR3CH=0x00;
		OCR3CL=0x00;
#elif CONTROLLER_FREQ == 8000000UL
		TCCR3A=(0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<COM3C1) | (0<<COM3C0) | (0<<WGM31) | (0<<WGM30);
		TCCR3B=(0<<ICNC3) | (0<<ICES3) | (0<<WGM33) | (1<<WGM32) | (0<<CS32) | (1<<CS31) | (0<<CS30);
		TCNT3H=0x00;
		TCNT3L=0x00;
		ICR3H=0x00;
		ICR3L=0x00;
		OCR3AH=0x00;
		OCR3AL=0x63;
		OCR3BH=0x00;
		OCR3BL=0x00;
		OCR3CH=0x00;
		OCR3CL=0x00;
#elif CONTROLLER_FREQ == 1000000UL
		TCCR3A=(0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<COM3C1) | (0<<COM3C0) | (0<<WGM31) | (0<<WGM30);
		TCCR3B=(0<<ICNC3) | (0<<ICES3) | (0<<WGM33) | (1<<WGM32) | (0<<CS32) | (0<<CS31) | (1<<CS30);
		TCNT3H=0x00;
		TCNT3L=0x00;
		ICR3H=0x00;
		ICR3L=0x00;
		OCR3AH=0x00;
		OCR3AL=0x63;
		OCR3BH=0x00;
		OCR3BL=0x00;
		OCR3CH=0x00;
		OCR3CL=0x00;
#endif
		count3=0;
		if( timer_delay_ms*10 > 65500 )
		{
			count3_max = 65500;
		}
		else
			count3_max = (mos_uint16_t)(timer_delay_ms*10);
		
		callback3 = timer_delay_callback;
		ETIMSK |= (1<<OCIE3A);
	}
#endif
}

static void end (mos_uint8_t timer_no)
{	
#if OS_TIMER_TYPE != TIMER_0
	if( timer_no == TIMER_0 )
	{
		ASSR=0<<AS0;
		TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (0<<CS01) | (0<<CS00);
		TCNT0=0x00;
		OCR0=0x00;
		
		TIMSK &= ~((1<<OCIE0)|(1<<TOIE0));
	}
#endif

#if OS_TIMER_TYPE != TIMER_1	
	if( timer_no == TIMER_1 )
	{	
		TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10);
		TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
		TCNT1H=0x00;
		TCNT1L=0x00;
		ICR1H=0x00;
		ICR1L=0x00;
		OCR1AH=0x00;
		OCR1AL=0x00;
		OCR1BH=0x00;
		OCR1BL=0x00;
		OCR1CH=0x00;
		OCR1CL=0x00;
			
		TIMSK &= ~((1<<TICIE1) | (1<<OCIE1A) | (1<<OCIE1B) | (1<<TOIE1));
	}
#endif

#if OS_TIMER_TYPE != TIMER_2
	if( timer_no == TIMER_2 )
	{	
		TCCR2=(0<<WGM20) | (0<<COM21) | (0<<COM20) | (0<<WGM21) | (0<<CS22) | (0<<CS21) | (0<<CS20);
		TCNT2=0x00;
		OCR2=0x00;
		
		TIMSK &= ~((1<<OCIE2)|(1<<TOIE2));
	}
#endif

#if OS_TIMER_TYPE != TIMER_3
	if( timer_no == TIMER_3 )
	{	
		TCCR3A=(0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<COM3C1) | (0<<COM3C0) | (0<<WGM31) | (0<<WGM30);
		TCCR3B=(0<<ICNC3) | (0<<ICES3) | (0<<WGM33) | (0<<WGM32) | (0<<CS32) | (0<<CS31) | (0<<CS30);
		TCNT3H=0x00;
		TCNT3L=0x00;
		ICR3H=0x00;
		ICR3L=0x00;
		OCR3AH=0x00;
		OCR3AL=0x00;
		OCR3BH=0x00;
		OCR3BL=0x00;
		OCR3CH=0x00;
		OCR3CL=0x00;
		
		ETIMSK &= ~((1<<TICIE3) | (1<<OCIE3A) | (1<<OCIE3B) | (1<<TOIE3) | (1<<OCIE3C));
	}
#endif
}

#if OS_TIMER_TYPE != TIMER_0
ISR(TIMER0_COMP_vect)
{
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

#if OS_TIMER_TYPE != TIMER_3
ISR(TIMER3_COMPA_vect)
{
	count3++;
	if(count3 >= count3_max)
	{
		count3 = 0;
		if(callback3)
			callback3();
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



#include "avr_mega_digital.h"

#ifdef PLATFORM_SUPPORT_DIGITAL_IO

#define PORTA_INDEX 0
#define PORTB_INDEX 1
#define PORTC_INDEX 2
#define PORTD_INDEX 3
#define PORTE_INDEX 4
#define PORTF_INDEX 5
#define PORTG_INDEX 6
#define PORTH_INDEX 7
#define PORTJ_INDEX 9
#define PORTK_INDEX 10
#define PORTL_INDEX 11

static void pinmode(mos_uint8_t os_pin, mos_uint8_t value)
{
	mos_uint8_t port, pin;
	
	port = os_pin >> 3;
	pin = os_pin & 0x07;
	
	if (value <= INPUT_PULLUP)
	{ 
		if( value == OUTPUT )
		{
			mos_uint8_t or_value = 1 << pin;
			
			switch(port) 
			{
				#ifdef PORTA_AVAILABLE
				case PORTA_INDEX :
					DDRA |= or_value;
					break;
				#endif
				
				#ifdef PORTB_AVAILABLE
				case PORTB_INDEX :
					DDRB |= or_value;
					break;
				#endif
				
				#ifdef PORTC_AVAILABLE
				case PORTC_INDEX :
					DDRC |= or_value;
					break;
				#endif
				
				#ifdef PORTD_AVAILABLE
				case PORTD_INDEX :
					DDRD |= or_value;
					break;
				#endif
				
				#ifdef PORTE_AVAILABLE
				case PORTE_INDEX :
					DDRE |= or_value;
					break;
				#endif
				
				#ifdef PORTF_AVAILABLE
				case PORTF_INDEX :
					DDRF |= or_value;
					break;
				#endif
				
				#ifdef PORTG_AVAILABLE
				case PORTG_INDEX :
					DDRG |= or_value;
					break;
				#endif
				
				#ifdef PORTH_AVAILABLE
				case PORTH_INDEX :
					DDRH |= or_value;
					break;
				#endif
				
				#ifdef PORTJ_AVAILABLE
				case PORTJ_INDEX :
					DDRJ |= or_value;
					break;
				#endif
				
				#ifdef PORTK_AVAILABLE
				case PORTK_INDEX :
					DDRK |= or_value;
					break;
				#endif
				
				#ifdef PORTL_AVAILABLE
				case PORTL_INDEX :
					DDRL |= or_value;
					break;
				#endif
			}
		}
	
		else
		{
			mos_uint8_t and_value = ~(1 << pin);
			
			switch(port) 
			{
				#ifdef PORTA_AVAILABLE
				case PORTA_INDEX :
					DDRA &= and_value;
					break;
				#endif
				
				#ifdef PORTB_AVAILABLE
				case PORTB_INDEX :
					DDRB &= and_value;
					break;
				#endif
				
				#ifdef PORTC_AVAILABLE
				case PORTC_INDEX :
					DDRC &= and_value;
					break;
				#endif
				
				#ifdef PORTD_AVAILABLE
				case PORTD_INDEX :
					DDRD &= and_value;
					break;
				#endif
				
				#ifdef PORTE_AVAILABLE
				case PORTE_INDEX :
					DDRE &= and_value;
					break;
				#endif
				
				#ifdef PORTF_AVAILABLE
				case PORTF_INDEX :
					DDRF &= and_value;
					break;
				#endif
				
				#ifdef PORTG_AVAILABLE
				case PORTG_INDEX :
					DDRG &= and_value;
					break;
				#endif
				
				#ifdef PORTH_AVAILABLE
				case PORTH_INDEX :
					DDRH &= and_value;
					break;
				#endif
				
				#ifdef PORTJ_AVAILABLE
				case PORTJ_INDEX :
					DDRJ &= and_value;
					break;
				#endif
				
				#ifdef PORTK_AVAILABLE
				case PORTK_INDEX :
					DDRK &= and_value;
					break;
				#endif
				
				#ifdef PORTL_AVAILABLE
				case PORTL_INDEX :
					DDRL &= and_value;
					break;
				#endif
			}			
	}
		
		if( value == INPUT_PULLUP )
		{
			mos_uint8_t or_value = 1 << pin;
			
			switch(port) 
			{
				#ifdef PORTA_AVAILABLE
				case PORTA_INDEX :
					PORTA |= or_value;
					break;
				#endif
				
				#ifdef PORTB_AVAILABLE
				case PORTB_INDEX :
					PORTB |= or_value;
					break;
				#endif
				
				#ifdef PORTC_AVAILABLE
				case PORTC_INDEX :
					PORTC |= or_value;
					break;
				#endif
				
				#ifdef PORTD_AVAILABLE
				case PORTD_INDEX :
					PORTD |= or_value;
					break;
				#endif
				
				#ifdef PORTE_AVAILABLE
				case PORTE_INDEX :
					PORTE |= or_value;
					break;
				#endif
				
				#ifdef PORTF_AVAILABLE
				case PORTF_INDEX :
					PORTF |= or_value;
					break;
				#endif
				
				#ifdef PORTG_AVAILABLE
				case PORTG_INDEX :
					PORTG |= or_value;
					break;
				#endif
				
				#ifdef PORTH_AVAILABLE
				case PORTH_INDEX :
					PORTH |= or_value;
					break;
				#endif
				
				#ifdef PORTJ_AVAILABLE
				case PORTJ_INDEX :
					PORTJ |= or_value;
					break;
				#endif
				
				#ifdef PORTK_AVAILABLE
				case PORTK_INDEX :
					PORTK |= or_value;
					break;
				#endif
				
				#ifdef PORTL_AVAILABLE
				case PORTL_INDEX :
					PORTL |= or_value;
					break;
				#endif
			}
		}
		
		else
		{
			mos_uint8_t and_value = ~(1 << pin);
			
			switch(port) 
			{
				#ifdef PORTA_AVAILABLE
				case PORTA_INDEX :
					PORTA &= and_value;
					break;
				#endif
				
				#ifdef PORTB_AVAILABLE
				case PORTB_INDEX :
					PORTB &= and_value;
					break;
				#endif
				
				#ifdef PORTC_AVAILABLE
				case PORTC_INDEX :
					PORTC &= and_value;
					break;
				#endif
				
				#ifdef PORTD_AVAILABLE
				case PORTD_INDEX :
					PORTD &= and_value;
					break;
				#endif
				
				#ifdef PORTE_AVAILABLE
				case PORTE_INDEX :
					PORTE &= and_value;
					break;
				#endif
				
				#ifdef PORTF_AVAILABLE
				case PORTF_INDEX :
					PORTF &= and_value;
					break;
				#endif
				
				#ifdef PORTG_AVAILABLE
				case PORTG_INDEX :
					PORTG &= and_value;
					break;
				#endif
				
				#ifdef PORTH_AVAILABLE
				case PORTH_INDEX :
					PORTH &= and_value;
					break;
				#endif
				
				#ifdef PORTJ_AVAILABLE
				case PORTJ_INDEX :
					PORTJ &= and_value;
					break;
				#endif
				
				#ifdef PORTK_AVAILABLE
				case PORTK_INDEX :
					PORTK &= and_value;
					break;
				#endif
				
				#ifdef PORTL_AVAILABLE
				case PORTL_INDEX :
					PORTL &= and_value;
					break;
				#endif
			}			
		}
	}
}

static void write(mos_uint8_t os_pin, mos_uint8_t value)
{
	mos_uint8_t port, pin;
	
	port = os_pin >> 3;
	pin = os_pin & 0x07;
	
	if ( value <= TOGGLE)
	{
		if( value == HIGH )
		{
			mos_uint8_t or_value = 1 << pin;
			
			switch(port) 
			{
				#ifdef PORTA_AVAILABLE
				case PORTA_INDEX :
					PORTA |= or_value;
					break;
				#endif
				
				#ifdef PORTB_AVAILABLE
				case PORTB_INDEX :
					PORTB |= or_value;
					break;
				#endif
				
				#ifdef PORTC_AVAILABLE
				case PORTC_INDEX :
					PORTC |= or_value;
					break;
				#endif
				
				#ifdef PORTD_AVAILABLE
				case PORTD_INDEX :
					PORTD |= or_value;
					break;
				#endif
				
				#ifdef PORTE_AVAILABLE
				case PORTE_INDEX :
					PORTE |= or_value;
					break;
				#endif
				
				#ifdef PORTF_AVAILABLE
				case PORTF_INDEX :
					PORTF |= or_value;
					break;
				#endif
				
				#ifdef PORTG_AVAILABLE
				case PORTG_INDEX :
					PORTG |= or_value;
					break;
				#endif
				
				#ifdef PORTH_AVAILABLE
				case PORTH_INDEX :
					PORTH |= or_value;
					break;
				#endif
				
				#ifdef PORTJ_AVAILABLE
				case PORTJ_INDEX :
					PORTJ |= or_value;
					break;
				#endif
				
				#ifdef PORTK_AVAILABLE
				case PORTK_INDEX :
					PORTK |= or_value;
					break;
				#endif
				
				#ifdef PORTL_AVAILABLE
				case PORTL_INDEX :
					PORTL |= or_value;
					break;
				#endif
			}	
		}
		
		else if( value == LOW )
		{
			mos_uint8_t and_value = ~(1 << pin);
			
			switch(port) 
			{
				#ifdef PORTA_AVAILABLE
				case PORTA_INDEX :
					PORTA &= and_value;
					break;
				#endif
				
				#ifdef PORTB_AVAILABLE
				case PORTB_INDEX :
					PORTB &= and_value;
					break;
				#endif
				
				#ifdef PORTC_AVAILABLE
				case PORTC_INDEX :
					PORTC &= and_value;
					break;
				#endif
				
				#ifdef PORTD_AVAILABLE
				case PORTD_INDEX :
					PORTD &= and_value;
					break;
				#endif
				
				#ifdef PORTE_AVAILABLE
				case PORTE_INDEX :
					PORTE &= and_value;
					break;
				#endif
				
				#ifdef PORTF_AVAILABLE
				case PORTF_INDEX :
					PORTF &= and_value;
					break;
				#endif
				
				#ifdef PORTG_AVAILABLE
				case PORTG_INDEX :
					PORTG &= and_value;
					break;
				#endif
				
				#ifdef PORTH_AVAILABLE
				case PORTH_INDEX :
					PORTH &= and_value;
					break;
				#endif
				
				#ifdef PORTJ_AVAILABLE
				case PORTJ_INDEX :
					PORTJ &= and_value;
					break;
				#endif
				
				#ifdef PORTK_AVAILABLE
				case PORTK_INDEX :
					PORTK &= and_value;
					break;
				#endif
				
				#ifdef PORTL_AVAILABLE
				case PORTL_INDEX :
					PORTL &= and_value;
					break;
				#endif
			}	
		}
	
		else if( value == TOGGLE )
		{
			mos_uint8_t or_value = 1 << pin;
			
			switch(port) 
			{
				#ifdef PORTA_AVAILABLE
				case PORTA_INDEX :
					PORTA ^= or_value;
					break;
				#endif
				
				#ifdef PORTB_AVAILABLE
				case PORTB_INDEX :
					PORTB ^= or_value;
					break;
				#endif
				
				#ifdef PORTC_AVAILABLE
				case PORTC_INDEX :
					PORTC ^= or_value;
					break;
				#endif
				
				#ifdef PORTD_AVAILABLE
				case PORTD_INDEX :
					PORTD ^= or_value;
					break;
				#endif
				
				#ifdef PORTE_AVAILABLE
				case PORTE_INDEX :
					PORTE ^= or_value;
					break;
				#endif
				
				#ifdef PORTF_AVAILABLE
				case PORTF_INDEX :
					PORTF ^= or_value;
					break;
				#endif
				
				#ifdef PORTG_AVAILABLE
				case PORTG_INDEX :
					PORTG ^= or_value;
					break;
				#endif
				
				#ifdef PORTH_AVAILABLE
				case PORTH_INDEX :
					PORTH ^= or_value;
					break;
				#endif
				
				#ifdef PORTJ_AVAILABLE
				case PORTJ_INDEX :
					PORTJ ^= or_value;
					break;
				#endif
				
				#ifdef PORTK_AVAILABLE
				case PORTK_INDEX :
					PORTK ^= or_value;
					break;
				#endif
				
				#ifdef PORTL_AVAILABLE
				case PORTL_INDEX :
					PORTL ^= or_value;
					break;
				#endif
			}	
		}
	}
}

static mos_uint8_t read(mos_uint8_t os_pin)
{
	mos_uint8_t port, pin;
	mos_uint8_t and_value; 
	
	port = os_pin >> 3;
	pin = os_pin & 0x07;
	and_value = 1 << pin;
	
	switch(port) 
	{
		#ifdef PORTA_AVAILABLE
		case PORTA_INDEX :
			return (PINA & and_value) >> pin;
			break;
		#endif
		
		#ifdef PORTB_AVAILABLE
		case PORTB_INDEX :
			return (PINB & and_value) >> pin;
			break;
		#endif
		
		#ifdef PORTC_AVAILABLE
		case PORTC_INDEX :
			return (PINC & and_value) >> pin;
			break;
		#endif
		
		#ifdef PORTD_AVAILABLE
		case PORTD_INDEX :
			return (PIND & and_value) >> pin;
			break;
		#endif
		
		#ifdef PORTE_AVAILABLE
		case PORTE_INDEX :
			return (PINE & and_value) >> pin;
			break;
		#endif
		
		#ifdef PORTF_AVAILABLE
		case PORTF_INDEX :
			return (PINF & and_value) >> pin;
			break;
		#endif
		
		#ifdef PORTG_AVAILABLE
		case PORTG_INDEX :
			return (PING & and_value) >> pin;
			break;
		#endif
		
		#ifdef PORTH_AVAILABLE
		case PORTH_INDEX :
			return (PINH & and_value) >> pin;
			break;
		#endif
		
		#ifdef PORTJ_AVAILABLE
		case PORTJ_INDEX :
			return (PINJ & and_value) >> pin;
			break;
		#endif
		
		#ifdef PORTK_AVAILABLE
		case PORTK_INDEX :
			return (PINK & and_value) >> pin;
			break;
		#endif
		
		#ifdef PORTL_AVAILABLE
		case PORTL_INDEX :
			return (PINL & and_value) >> pin;
			break;
		#endif
	}
	
	return 0;
}

const struct digital_driver avr_mega_digital_driver = {
	pinmode,
	write,
	read
};

#endif

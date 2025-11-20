#include "avr_mega0_digital.h"

#ifdef PLATFORM_SUPPORT_DIGITAL_IO

#define PORTA_INDEX 0
#define PORTB_INDEX 1
#define PORTC_INDEX 2
#define PORTD_INDEX 3
#define PORTE_INDEX 4
#define PORTF_INDEX 5

static void pinmode(mos_uint8_t os_pin, mos_uint8_t value)
{
	mos_uint8_t port, pin;
	
	port = os_pin >> 3;
	pin = os_pin & 0x07;
	
	if (value <= INPUT_PULLUP)
	{ 
		mos_uint8_t or_value = 1 << pin;
		
		if( value == OUTPUT )
		{				
			switch(port) 
			{
				#ifdef PORTA_AVAILABLE
				case PORTA_INDEX :
					PORTA.DIRSET = or_value;
					break;
				#endif
				
				#ifdef PORTB_AVAILABLE
				case PORTB_INDEX :
					PORTB.DIRSET = or_value;
					break;
				#endif
				
				#ifdef PORTC_AVAILABLE
				case PORTC_INDEX :
					PORTC.DIRSET = or_value;
					break;
				#endif
				
				#ifdef PORTD_AVAILABLE
				case PORTD_INDEX :
					PORTD.DIRSET = or_value;
					break;
				#endif
				
				#ifdef PORTE_AVAILABLE
				case PORTE_INDEX :
					PORTE.DIRSET = or_value;
					break;
				#endif
				
				#ifdef PORTF_AVAILABLE
				case PORTF_INDEX :
					PORTF.DIRSET = or_value;
					break;
				#endif
			}
		}
	
		else
		{	
			switch(port) 
			{
				#ifdef PORTA_AVAILABLE
				case PORTA_INDEX :
					PORTA.DIRCLR = or_value;
					break;
				#endif
				
				#ifdef PORTB_AVAILABLE
				case PORTB_INDEX :
					PORTB.DIRCLR = or_value;
					break;
				#endif
				
				#ifdef PORTC_AVAILABLE
				case PORTC_INDEX :
					PORTC.DIRCLR = or_value;
					break;
				#endif
				
				#ifdef PORTD_AVAILABLE
				case PORTD_INDEX :
					PORTD.DIRCLR = or_value;
					break;
				#endif
				
				#ifdef PORTE_AVAILABLE
				case PORTE_INDEX :
					PORTE.DIRCLR = or_value;
					break;
				#endif
				
				#ifdef PORTF_AVAILABLE
				case PORTF_INDEX :
					PORTF.DIRCLR = or_value;
					break;
				#endif
			}			
		}
		
		if( value == INPUT_PULLUP )
		{	
			switch(port) 
			{
				#ifdef PORTA_AVAILABLE
				case PORTA_INDEX :
					switch (pin)
					{
						case 0:
							PORTA.PIN0CTRL |= 0x08;
							break;
						case 1:
							PORTA.PIN1CTRL |= 0x08;
							break;
						case 2:
							PORTA.PIN2CTRL |= 0x08;
							break;
						case 3:
							PORTA.PIN3CTRL |= 0x08;
							break;
						case 4:
							PORTA.PIN4CTRL |= 0x08;
							break;
						case 5:
							PORTA.PIN5CTRL |= 0x08;
							break;
						case 6:
							PORTA.PIN6CTRL |= 0x08;
							break;
						case 7:
							PORTA.PIN7CTRL |= 0x08;
							break;
					}
				#endif
				
				#ifdef PORTB_AVAILABLE
				case PORTB_INDEX :
					switch (pin)
					{
						case 0:
							PORTB.PIN0CTRL |= 0x08;
							break;
						case 1:
							PORTB.PIN1CTRL |= 0x08;
							break;
						case 2:
							PORTB.PIN2CTRL |= 0x08;
							break;
						case 3:
							PORTB.PIN3CTRL |= 0x08;
							break;
						case 4:
							PORTB.PIN4CTRL |= 0x08;
							break;
						case 5:
							PORTB.PIN5CTRL |= 0x08;
							break;
						case 6:
							PORTB.PIN6CTRL |= 0x08;
							break;
						case 7:
							PORTB.PIN7CTRL |= 0x08;
							break;
					}
				#endif
				
				#ifdef PORTC_AVAILABLE
				case PORTC_INDEX :
					switch (pin)
					{
						case 0:
							PORTC.PIN0CTRL |= 0x08;
							break;
						case 1:
							PORTC.PIN1CTRL |= 0x08;
							break;
						case 2:
							PORTC.PIN2CTRL |= 0x08;
							break;
						case 3:
							PORTC.PIN3CTRL |= 0x08;
							break;
						case 4:
							PORTC.PIN4CTRL |= 0x08;
							break;
						case 5:
							PORTC.PIN5CTRL |= 0x08;
							break;
						case 6:
							PORTC.PIN6CTRL |= 0x08;
							break;
						case 7:
							PORTC.PIN7CTRL |= 0x08;
							break;
					}
				#endif
				
				#ifdef PORTD_AVAILABLE
				case PORTD_INDEX :
					switch (pin)
					{
						case 0:
							PORTD.PIN0CTRL |= 0x08;
							break;
						case 1:
							PORTD.PIN1CTRL |= 0x08;
							break;
						case 2:
							PORTD.PIN2CTRL |= 0x08;
							break;
						case 3:
							PORTD.PIN3CTRL |= 0x08;
							break;
						case 4:
							PORTD.PIN4CTRL |= 0x08;
							break;
						case 5:
							PORTD.PIN5CTRL |= 0x08;
							break;
						case 6:
							PORTD.PIN6CTRL |= 0x08;
							break;
						case 7:
							PORTD.PIN7CTRL |= 0x08;
							break;
					}
				#endif
				
				#ifdef PORTE_AVAILABLE
				case PORTE_INDEX :
					switch (pin)
					{
						case 0:
							PORTE.PIN0CTRL |= 0x08;
							break;
						case 1:
							PORTE.PIN1CTRL |= 0x08;
							break;
						case 2:
							PORTE.PIN2CTRL |= 0x08;
							break;
						case 3:
							PORTE.PIN3CTRL |= 0x08;
							break;
						case 4:
							PORTE.PIN4CTRL |= 0x08;
							break;
						case 5:
							PORTE.PIN5CTRL |= 0x08;
							break;
						case 6:
							PORTE.PIN6CTRL |= 0x08;
							break;
						case 7:
							PORTE.PIN7CTRL |= 0x08;
							break;
					}
				#endif
				
				#ifdef PORTF_AVAILABLE
				case PORTF_INDEX :
					switch (pin)
					{
						case 0:
							PORTF.PIN0CTRL |= 0x08;
							break;
						case 1:
							PORTF.PIN1CTRL |= 0x08;
							break;
						case 2:
							PORTF.PIN2CTRL |= 0x08;
							break;
						case 3:
							PORTF.PIN3CTRL |= 0x08;
							break;
						case 4:
							PORTF.PIN4CTRL |= 0x08;
							break;
						case 5:
							PORTF.PIN5CTRL |= 0x08;
							break;
						case 6:
							PORTF.PIN6CTRL |= 0x08;
							break;
						case 7:
							PORTF.PIN7CTRL |= 0x08;
							break;
					}
				#endif
			}
		}
		
		else
		{	
			switch(port) 
			{
				#ifdef PORTA_AVAILABLE
				case PORTA_INDEX :
					switch (pin)
					{
						case 0:
							PORTA.PIN0CTRL &= 0xF7;
							break;
						case 1:
							PORTA.PIN1CTRL &= 0xF7;
							break;
						case 2:
							PORTA.PIN2CTRL &= 0xF7;
							break;
						case 3:
							PORTA.PIN3CTRL &= 0xF7;
							break;
						case 4:
							PORTA.PIN4CTRL &= 0xF7;
							break;
						case 5:
							PORTA.PIN5CTRL &= 0xF7;
							break;
						case 6:
							PORTA.PIN6CTRL &= 0xF7;
							break;
						case 7:
							PORTA.PIN7CTRL &= 0xF7;
							break;
					}
				#endif
				
				#ifdef PORTB_AVAILABLE
				case PORTB_INDEX :
					switch (pin)
					{
						case 0:
							PORTB.PIN0CTRL &= 0xF7;
							break;
						case 1:
							PORTB.PIN1CTRL &= 0xF7;
							break;
						case 2:
							PORTB.PIN2CTRL &= 0xF7;
							break;
						case 3:
							PORTB.PIN3CTRL &= 0xF7;
							break;
						case 4:
							PORTB.PIN4CTRL &= 0xF7;
							break;
						case 5:
							PORTB.PIN5CTRL &= 0xF7;
							break;
						case 6:
							PORTB.PIN6CTRL &= 0xF7;
							break;
						case 7:
							PORTB.PIN7CTRL &= 0xF7;
							break;
					}
				#endif
				
				#ifdef PORTC_AVAILABLE
				case PORTC_INDEX :
					switch (pin)
					{
						case 0:
							PORTC.PIN0CTRL &= 0xF7;
							break;
						case 1:
							PORTC.PIN1CTRL &= 0xF7;
							break;
						case 2:
							PORTC.PIN2CTRL &= 0xF7;
							break;
						case 3:
							PORTC.PIN3CTRL &= 0xF7;
							break;
						case 4:
							PORTC.PIN4CTRL &= 0xF7;
							break;
						case 5:
							PORTC.PIN5CTRL &= 0xF7;
							break;
						case 6:
							PORTC.PIN6CTRL &= 0xF7;
							break;
						case 7:
							PORTC.PIN7CTRL &= 0xF7;
							break;
					}
				#endif
				
				#ifdef PORTD_AVAILABLE
				case PORTD_INDEX :
					switch (pin)
					{
						case 0:
							PORTD.PIN0CTRL &= 0xF7;
							break;
						case 1:
							PORTD.PIN1CTRL &= 0xF7;
							break;
						case 2:
							PORTD.PIN2CTRL &= 0xF7;
							break;
						case 3:
							PORTD.PIN3CTRL &= 0xF7;
							break;
						case 4:
							PORTD.PIN4CTRL &= 0xF7;
							break;
						case 5:
							PORTD.PIN5CTRL &= 0xF7;
							break;
						case 6:
							PORTD.PIN6CTRL &= 0xF7;
							break;
						case 7:
							PORTD.PIN7CTRL &= 0xF7;
							break;
					}
				#endif
				
				#ifdef PORTE_AVAILABLE
				case PORTE_INDEX :
					switch (pin)
					{
						case 0:
							PORTE.PIN0CTRL &= 0xF7;
							break;
						case 1:
							PORTE.PIN1CTRL &= 0xF7;
							break;
						case 2:
							PORTE.PIN2CTRL &= 0xF7;
							break;
						case 3:
							PORTE.PIN3CTRL &= 0xF7;
							break;
						case 4:
							PORTE.PIN4CTRL &= 0xF7;
							break;
						case 5:
							PORTE.PIN5CTRL &= 0xF7;
							break;
						case 6:
							PORTE.PIN6CTRL &= 0xF7;
							break;
						case 7:
							PORTE.PIN7CTRL &= 0xF7;
							break;
					}
				#endif
				
				#ifdef PORTF_AVAILABLE
				case PORTF_INDEX :
					switch (pin)
					{
						case 0:
							PORTF.PIN0CTRL &= 0xF7;
							break;
						case 1:
							PORTF.PIN1CTRL &= 0xF7;
							break;
						case 2:
							PORTF.PIN2CTRL &= 0xF7;
							break;
						case 3:
							PORTF.PIN3CTRL &= 0xF7;
							break;
						case 4:
							PORTF.PIN4CTRL &= 0xF7;
							break;
						case 5:
							PORTF.PIN5CTRL &= 0xF7;
							break;
						case 6:
							PORTF.PIN6CTRL &= 0xF7;
							break;
						case 7:
							PORTF.PIN7CTRL &= 0xF7;
							break;
					}
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
		mos_uint8_t or_value = 1 << pin;
		
		if( value == HIGH )
		{	
			switch(port) 
			{
				#ifdef PORTA_AVAILABLE
				case PORTA_INDEX :
					PORTA.OUTSET = or_value;
					break;
				#endif
				
				#ifdef PORTB_AVAILABLE
				case PORTB_INDEX :
					PORTB.OUTSET = or_value;
					break;
				#endif
				
				#ifdef PORTC_AVAILABLE
				case PORTC_INDEX :
					PORTC.OUTSET = or_value;
					break;
				#endif
				
				#ifdef PORTD_AVAILABLE
				case PORTD_INDEX :
					PORTD.OUTSET = or_value;
					break;
				#endif
				
				#ifdef PORTE_AVAILABLE
				case PORTE_INDEX :
					PORTE.OUTSET = or_value;
					break;
				#endif
				
				#ifdef PORTF_AVAILABLE
				case PORTF_INDEX :
					PORTF.OUTSET = or_value;
					break;
				#endif
			}	
		}
		
		else if( value == LOW )
		{	
			switch(port) 
			{
				#ifdef PORTA_AVAILABLE
				case PORTA_INDEX :
					PORTA.OUTCLR = or_value;
					break;
				#endif
				
				#ifdef PORTB_AVAILABLE
				case PORTB_INDEX :
					PORTB.OUTCLR = or_value;
					break;
				#endif
				
				#ifdef PORTC_AVAILABLE
				case PORTC_INDEX :
					PORTC.OUTCLR = or_value;
					break;
				#endif
				
				#ifdef PORTD_AVAILABLE
				case PORTD_INDEX :
					PORTD.OUTCLR = or_value;
					break;
				#endif
				
				#ifdef PORTE_AVAILABLE
				case PORTE_INDEX :
					PORTE.OUTCLR = or_value;
					break;
				#endif
				
				#ifdef PORTF_AVAILABLE
				case PORTF_INDEX :
					PORTF.OUTCLR = or_value;
					break;
				#endif
			}
		}
	
		else if( value == TOGGLE )
		{			
			switch(port) 
			{
				#ifdef PORTA_AVAILABLE
				case PORTA_INDEX :
					PORTA.OUTTGL = or_value;
					break;
				#endif
				
				#ifdef PORTB_AVAILABLE
				case PORTB_INDEX :
					PORTB.OUTTGL = or_value;
					break;
				#endif
				
				#ifdef PORTC_AVAILABLE
				case PORTC_INDEX :
					PORTC.OUTTGL = or_value;
					break;
				#endif
				
				#ifdef PORTD_AVAILABLE
				case PORTD_INDEX :
					PORTD.OUTTGL = or_value;
					break;
				#endif
				
				#ifdef PORTE_AVAILABLE
				case PORTE_INDEX :
					PORTE.OUTTGL = or_value;
					break;
				#endif
				
				#ifdef PORTF_AVAILABLE
				case PORTF_INDEX :
					PORTF.OUTTGL = or_value;
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
			return (PORTA.IN & and_value) >> pin;
			break;
		#endif
		
		#ifdef PORTB_AVAILABLE
		case PORTB_INDEX :
			return (PORTB.IN & and_value) >> pin;
			break;
		#endif
		
		#ifdef PORTC_AVAILABLE
		case PORTC_INDEX :
			return (PORTC.IN & and_value) >> pin;
			break;
		#endif
		
		#ifdef PORTD_AVAILABLE
		case PORTD_INDEX :
			return (PORTD.IN & and_value) >> pin;
			break;
		#endif
		
		#ifdef PORTE_AVAILABLE
		case PORTE_INDEX :
			return (PORTE.IN & and_value) >> pin;
			break;
		#endif
		
		#ifdef PORTF_AVAILABLE
		case PORTF_INDEX :
			return (PORTF.IN & and_value) >> pin;
			break;
		#endif		
	}
	return 0;
}

const struct digital_driver avr_mega0_digital_driver = {
	pinmode,
	write,
	read
};

#endif

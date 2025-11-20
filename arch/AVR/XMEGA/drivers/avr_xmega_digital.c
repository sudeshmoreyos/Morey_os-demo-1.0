#include "avr_xmega_digital.h"

#ifdef PLATFORM_SUPPORT_DIGITAL_IO

static void setpin(char port,mos_uint8_t pin, mos_uint8_t value)
{
    if (pin < 8 && value <= INPUT_PULLDOWN)
    {                        
        if(value == OUTPUT)
        {
            #if defined(PORTA_AVAILABLE)
                if( port == 'A')
                    PORTA.DIRSET = 1 << pin;
            #endif
                        
            #if defined(PORTB_AVAILABLE)
                if( port == 'B')
                    PORTB.DIRSET = 1 << pin;
            #endif
                        
            #if defined(PORTC_AVAILABLE)
                if( port == 'C')
                    PORTC.DIRSET = 1 << pin;
            #endif
                        
            #if defined(PORTD_AVAILABLE)
                if( port == 'D')
                    PORTD.DIRSET = 1 << pin;
            #endif
                
            #if defined(PORTE_AVAILABLE)
                if( port == 'E')
                    PORTE.DIRSET = 1 << pin;                        
            #endif
			
			#if defined(PORTF_AVAILABLE)
                if( port == 'F')
                    PORTF.DIRSET = 1 << pin;                        
            #endif
			
			#if defined(PORTR_AVAILABLE)
                if( port == 'R')
                    PORTR.DIRSET = 1 << pin;                        
            #endif
			
        }
            
        else
        {
            #if defined(PORTA_AVAILABLE)
                if( port == 'A')
                    PORTA.DIRCLR = (1 << pin);
            #endif
                        
            #if defined(PORTB_AVAILABLE)
                if( port == 'B')
                    PORTB.DIRCLR = (1 << pin);
            #endif
                        
            #if defined(PORTC_AVAILABLE)
                if( port == 'C')
                    PORTC.DIRCLR = (1 << pin);
            #endif
                        
            #if defined(PORTD_AVAILABLE)
                if( port == 'D')
                    PORTD.DIRCLR = (1 << pin);
            #endif
                        
            #if defined(PORTE_AVAILABLE)
                if( port == 'E')
                    PORTE.DIRCLR = (1 << pin);                        
            #endif
			
			#if defined(PORTF_AVAILABLE)
                if( port == 'F')
                    PORTF.DIRCLR = (1 << pin);
            #endif
                        
            #if defined(PORTR_AVAILABLE)
                if( port == 'R')
                    PORTR.DIRCLR = (1 << pin);                        
            #endif
            
        }
            
        if((value == INPUT_PULLDOWN) || (value == INPUT_PULLUP))
        {
            mos_uint8_t temp = 0;
			if(value == INPUT_PULLDOWN)
				temp = 0x10;
			else
				temp = 0x18;
			
			#if defined(PORTA_AVAILABLE)
                if( port == 'A')
				{
                    if( pin == 0)
						PORTA.PIN0CTRL = temp;
					else if( pin == 1)
						PORTA.PIN1CTRL = temp;
					else if( pin == 2)
						PORTA.PIN2CTRL = temp;
					else if( pin == 3)
						PORTA.PIN3CTRL = temp;
					else if( pin == 4)
						PORTA.PIN4CTRL = temp;
					else if( pin == 5)
						PORTA.PIN5CTRL = temp;
					else if( pin == 6)
						PORTA.PIN6CTRL = temp;
					else if( pin == 7)
						PORTA.PIN7CTRL = temp;
				}
            #endif
			
			#if defined(PORTB_AVAILABLE)
                if( port == 'B')
				{
                    if( pin == 0)
						PORTB.PIN0CTRL = temp;
					else if( pin == 1)
						PORTB.PIN1CTRL = temp;
					else if( pin == 2)
						PORTB.PIN2CTRL = temp;
					else if( pin == 3)
						PORTB.PIN3CTRL = temp;
					else if( pin == 4)
						PORTB.PIN4CTRL = temp;
					else if( pin == 5)
						PORTB.PIN5CTRL = temp;
					else if( pin == 6)
						PORTB.PIN6CTRL = temp;
					else if( pin == 7)
						PORTB.PIN7CTRL = temp;
				}
			#endif
			
			#if defined(PORTC_AVAILABLE)
                if( port == 'C')
				{
                    if( pin == 0)
						PORTC.PIN0CTRL = temp;
					else if( pin == 1)
						PORTC.PIN1CTRL = temp;
					else if( pin == 2)
						PORTC.PIN2CTRL = temp;
					else if( pin == 3)
						PORTC.PIN3CTRL = temp;
					else if( pin == 4)
						PORTC.PIN4CTRL = temp;
					else if( pin == 5)
						PORTC.PIN5CTRL = temp;
					else if( pin == 6)
						PORTC.PIN6CTRL = temp;
					else if( pin == 7)
						PORTC.PIN7CTRL = temp;
				}
            #endif
			
			#if defined(PORTD_AVAILABLE)
                if( port == 'D')
				{
                    if( pin == 0)
						PORTD.PIN0CTRL = temp;
					else if( pin == 1)
						PORTD.PIN1CTRL = temp;
					else if( pin == 2)
						PORTD.PIN2CTRL = temp;
					else if( pin == 3)
						PORTD.PIN3CTRL = temp;
					else if( pin == 4)
						PORTD.PIN4CTRL = temp;
					else if( pin == 5)
						PORTD.PIN5CTRL = temp;
					else if( pin == 6)
						PORTD.PIN6CTRL = temp;
					else if( pin == 7)
						PORTD.PIN7CTRL = temp;
				}
            #endif

			#if defined(PORTE_AVAILABLE)
                if( port == 'E')
				{
                    if( pin == 0)
						PORTE.PIN0CTRL = temp;
					else if( pin == 1)
						PORTE.PIN1CTRL = temp;
					else if( pin == 2)
						PORTE.PIN2CTRL = temp;
					else if( pin == 3)
						PORTE.PIN3CTRL = temp;
					else if( pin == 4)
						PORTE.PIN4CTRL = temp;
					else if( pin == 5)
						PORTE.PIN5CTRL = temp;
					else if( pin == 6)
						PORTE.PIN6CTRL = temp;
					else if( pin == 7)
						PORTE.PIN7CTRL = temp;
				}
            #endif

			#if defined(PORTF_AVAILABLE)
                if( port == 'F')
				{
                    if( pin == 0)
						PORTF.PIN0CTRL = temp;
					else if( pin == 1)
						PORTF.PIN1CTRL = temp;
					else if( pin == 2)
						PORTF.PIN2CTRL = temp;
					else if( pin == 3)
						PORTF.PIN3CTRL = temp;
					else if( pin == 4)
						PORTF.PIN4CTRL = temp;
					else if( pin == 5)
						PORTF.PIN5CTRL = temp;
					else if( pin == 6)
						PORTF.PIN6CTRL = temp;
					else if( pin == 7)
						PORTF.PIN7CTRL = temp;
				}
            #endif

			#if defined(PORTR_AVAILABLE)
                if( port == 'R')
				{
                    if( pin == 0)
						PORTR.PIN0CTRL = temp;
					else if( pin == 1)
						PORTR.PIN1CTRL = temp;
				}
            #endif			
        }
    }
}

static void digital_write(char port, mos_uint8_t pin, mos_uint8_t value)
{
    if(value == HIGH)
    {
        #if defined(PORTA_AVAILABLE)
            if( port == 'A')
                PORTA.OUTSET = 1 << pin;
        #endif
                        
        #if defined(PORTB_AVAILABLE)
            if( port == 'B')
                PORTB.OUTSET = 1 << pin;
        #endif
                        
        #if defined(PORTC_AVAILABLE)
            if( port == 'C')
                PORTC.OUTSET = 1 << pin;
        #endif
                        
        #if defined(PORTD_AVAILABLE)
            if( port == 'D')
                PORTD.OUTSET = 1 << pin;
        #endif
                
        #if defined(PORTE_AVAILABLE)
            if( port == 'E')
                PORTE.OUTSET = 1 << pin;                        
        #endif
		
		#if defined(PORTF_AVAILABLE)
            if( port == 'F')
                PORTF.OUTSET = 1 << pin;                        
        #endif
		
		#if defined(PORTR_AVAILABLE)
            if( port == 'R')
                PORTR.OUTSET = 1 << pin;                        
        #endif
    }            
    
	else if (value == LOW)
    {
        #if defined(PORTA_AVAILABLE)
            if( port == 'A')
                PORTA.OUTCLR = 1 << pin;
        #endif
                        
        #if defined(PORTB_AVAILABLE)
            if( port == 'B')
                PORTB.OUTCLR = 1 << pin;
        #endif
                        
        #if defined(PORTC_AVAILABLE)
            if( port == 'C')
                PORTC.OUTCLR = 1 << pin;
        #endif
                        
        #if defined(PORTD_AVAILABLE)
            if( port == 'D')
                PORTD.OUTCLR = 1 << pin;
        #endif
                
        #if defined(PORTE_AVAILABLE)
            if( port == 'E')
                PORTE.OUTCLR = 1 << pin;                        
        #endif
		
		#if defined(PORTF_AVAILABLE)
            if( port == 'F')
                PORTF.OUTCLR = 1 << pin;                        
        #endif
		
		#if defined(PORTR_AVAILABLE)
            if( port == 'R')
                PORTR.OUTCLR = 1 << pin;                        
        #endif
    }                
	
	else if (value == TOGGLE)
    {
        #if defined(PORTA_AVAILABLE)
            if( port == 'A')
                PORTA.OUTTGL = 1 << pin;
        #endif
                        
        #if defined(PORTB_AVAILABLE)
            if( port == 'B')
                PORTB.OUTTGL = 1 << pin;
        #endif
                        
        #if defined(PORTC_AVAILABLE)
            if( port == 'C')
                PORTC.OUTTGL = 1 << pin;
        #endif
                        
        #if defined(PORTD_AVAILABLE)
            if( port == 'D')
                PORTD.OUTTGL = 1 << pin;
        #endif
                
        #if defined(PORTE_AVAILABLE)
            if( port == 'E')
                PORTE.OUTTGL = 1 << pin;                        
        #endif
		
		#if defined(PORTF_AVAILABLE)
            if( port == 'F')
                PORTF.OUTTGL = 1 << pin;                        
        #endif
		
		#if defined(PORTR_AVAILABLE)
            if( port == 'R')
                PORTR.OUTTGL = 1 << pin;                        
        #endif           
    }                

}

static mos_uint8_t digital_read(char port, char pin)
{
    #if defined(PORTA_AVAILABLE)
        if( port == 'A')
            return (PORTA.IN & (1 << pin)) >> pin ;
    #endif
                            
    #if defined(PORTB_AVAILABLE)
        if( port == 'B')
            return (PORTB.IN & (1 << pin)) >> pin ;
    #endif
                            
    #if defined(PORTC_AVAILABLE)
        if( port == 'C')
            return (PORTC.IN & (1 << pin)) >> pin ;
    #endif
                            
    #if defined(PORTD_AVAILABLE)
        if( port == 'D')
            return (PORTD.IN & (1 << pin)) >> pin ;
    #endif
                    
    #if defined(PORTE_AVAILABLE)
        if( port == 'E')
            return (PORTE.IN & (1 << pin)) >> pin ;                        
    #endif
	
	#if defined(PORTF_AVAILABLE)
        if( port == 'F')
            return (PORTF.IN & (1 << pin)) >> pin ;                        
    #endif
	
	#if defined(PORTR_AVAILABLE)
        if( port == 'R')
            return (PORTR.IN & (1 << pin)) >> pin ;                        
    #endif
	
	return 0;
} 

static mos_uint8_t ospin2controllerpin(mos_uint8_t os_pin, char * controller_port)
{
	*controller_port = (char)((mos_uint8_t)('A') + os_pin/8 );
	return (os_pin % 8 );
	
}

static void pinmode(mos_uint8_t os_pin, mos_uint8_t value)
{
	mos_uint8_t controller_pin;
	char controller_port;
	
	controller_pin = ospin2controllerpin(os_pin, &controller_port);
	setpin(controller_port,controller_pin, value);
	
}

static void write(mos_uint8_t os_pin, mos_uint8_t value)
{
	mos_uint8_t controller_pin;
	char controller_port;
	
	controller_pin = ospin2controllerpin(os_pin, &controller_port);
	digital_write(controller_port,controller_pin, value);
	
}

static mos_uint8_t read(mos_uint8_t os_pin)
{
	mos_uint8_t controller_pin;
	char controller_port;
	
	controller_pin = ospin2controllerpin(os_pin, &controller_port);
	return digital_read(controller_port,controller_pin);
	
}

const struct digital_driver avr_xmega_digital_driver = {
	pinmode,
	write,
	read
};

#endif

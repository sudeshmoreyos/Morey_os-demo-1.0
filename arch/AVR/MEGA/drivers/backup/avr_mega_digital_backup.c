#include "avr_mega_digital.h"

#ifdef PLATFORM_SUPPORT_DIGITAL_IO

static void setpin(char port,mos_uint8_t pin, mos_uint8_t value)
{
    if (pin < 8 && value <= INPUT_PULLUP)
    {                        
        if(value == OUTPUT)
        {
            #if defined(PORTA_AVAILABLE)
                if( port == 'A')
                    DDRA |= 1 << pin;
            #endif
                        
            #if defined(PORTB_AVAILABLE)
                if( port == 'B')
                    DDRB |= 1 << pin;
            #endif
                        
            #if defined(PORTC_AVAILABLE)
                if( port == 'C')
                    DDRC |= 1 << pin;
            #endif
                        
            #if defined(PORTD_AVAILABLE)
                if( port == 'D')
                    DDRD |= 1 << pin;
            #endif
                
            #if defined(PORTE_AVAILABLE)
                if( port == 'E')
                    DDRE |= 1 << pin;                        
            #endif
			
			#if defined(PORTF_AVAILABLE)
                if( port == 'F')
                    DDRF |= 1 << pin;
            #endif
                        
            #if defined(PORTG_AVAILABLE)
                if( port == 'G')
                    DDRG |= 1 << pin;
            #endif
                        
            #if defined(PORTH_AVAILABLE)
                if( port == 'H')
                    DDRH |= 1 << pin;
            #endif
                        
            #if defined(PORTJ_AVAILABLE)
                if( port == 'J')
                    DDRJ |= 1 << pin;
            #endif
			
			#if defined(PORTK_AVAILABLE)
                if( port == 'K')
                    DDRK |= 1 << pin;                        
            #endif
                
            #if defined(PORTL_AVAILABLE)
                if( port == 'L')
                    DDRL |= 1 << pin;                        
            #endif
			
        }
            
        else
        {
            #if defined(PORTA_AVAILABLE)
                if( port == 'A')
                    DDRA &= ~(1 << pin);
            #endif
                        
            #if defined(PORTB_AVAILABLE)
                if( port == 'B')
                    DDRB &= ~(1 << pin);
            #endif
                        
            #if defined(PORTC_AVAILABLE)
                if( port == 'C')
                    DDRC &= ~(1 << pin);
            #endif
                        
            #if defined(PORTD_AVAILABLE)
                if( port == 'D')
                    DDRD &= ~(1 << pin);
            #endif
                        
            #if defined(PORTE_AVAILABLE)
                if( port == 'E')
                    DDRE &= ~(1 << pin);                        
            #endif
			
			#if defined(PORTF_AVAILABLE)
                if( port == 'F')
                    DDRF &= ~(1 << pin);
            #endif
                        
            #if defined(PORTG_AVAILABLE)
                if( port == 'G')
                    DDRG &= ~(1 << pin);
            #endif
                        
            #if defined(PORTH_AVAILABLE)
                if( port == 'H')
                    DDRH &= ~(1 << pin);
            #endif
                        
            #if defined(PORTJ_AVAILABLE)
                if( port == 'J')
                    DDRJ &= ~(1 << pin);
            #endif
                        
            #if defined(PORTK_AVAILABLE)
                if( port == 'K')
                    DDRK &= ~(1 << pin);                        
            #endif

            #if defined(PORTL_AVAILABLE)
                if( port == 'L')
                    DDRL &= ~(1 << pin);                        
            #endif            
        }
            
        if(value == INPUT_PULLUP)
        {
            #if defined(PORTA_AVAILABLE)
                if( port == 'A')
                    PORTA |= 1 << pin;
            #endif
                        
            #if defined(PORTB_AVAILABLE)
                if( port == 'B')
                    PORTB |= 1 << pin;
            #endif
                        
            #if defined(PORTC_AVAILABLE)
                if( port == 'C')
                    PORTC |= 1 << pin;
            #endif
                        
            #if defined(PORTD_AVAILABLE)
                if( port == 'D')
                    PORTD |= 1 << pin;
            #endif
                
            #if defined(PORTE_AVAILABLE)
                if( port == 'E')
                    PORTE |= 1 << pin;                        
            #endif
			
			#if defined(PORTF_AVAILABLE)
                if( port == 'F')
                    PORTF |= 1 << pin;
            #endif
                        
            #if defined(PORTG_AVAILABLE)
                if( port == 'G')
                    PORTG |= 1 << pin;
            #endif
                        
            #if defined(PORTH_AVAILABLE)
                if( port == 'H')
                    PORTH |= 1 << pin;
            #endif
                        
            #if defined(PORTJ_AVAILABLE)
                if( port == 'J')
                    PORTJ |= 1 << pin;
            #endif
                
            #if defined(PORTK_AVAILABLE)
                if( port == 'K')
                    PORTK |= 1 << pin;                        
            #endif
			
            #if defined(PORTL_AVAILABLE)
                if( port == 'L')
                    PORTL |= 1 << pin;                        
            #endif
        }
            
        else
        {
            #if defined(PORTA_AVAILABLE)
                if( port == 'A')
                    PORTA &= ~(1 << pin);
            #endif
                        
            #if defined(PORTB_AVAILABLE)
                if( port == 'B')
                    PORTB &= ~(1 << pin);
            #endif
                        
            #if defined(PORTC_AVAILABLE)
                if( port == 'C')
                    PORTC &= ~(1 << pin);
            #endif
                        
            #if defined(PORTD_AVAILABLE)
                if( port == 'D')
                    PORTD &= ~(1 << pin);
            #endif
                        
            #if defined(PORTE_AVAILABLE)
                if( port == 'E')
                    PORTE &= ~(1 << pin);                        
            #endif
			
			#if defined(PORTF_AVAILABLE)
                if( port == 'F')
                    PORTF &= ~(1 << pin);
            #endif
                        
            #if defined(PORTG_AVAILABLE)
                if( port == 'G')
                    PORTG &= ~(1 << pin);
            #endif
                        
            #if defined(PORTH_AVAILABLE)
                if( port == 'H')
                    PORTH &= ~(1 << pin);
            #endif
                        
            #if defined(PORTJ_AVAILABLE)
                if( port == 'J')
                    PORTJ &= ~(1 << pin);
            #endif
                        
            #if defined(PORTK_AVAILABLE)
                if( port == 'K')
                    PORTK &= ~(1 << pin);                        
            #endif

            #if defined(PORTL_AVAILABLE)
                if( port == 'L')
                    PORTL &= ~(1 << pin);                        
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
                PORTA |= 1 << pin;
        #endif
                        
        #if defined(PORTB_AVAILABLE)
            if( port == 'B')
                PORTB |= 1 << pin;
        #endif
                        
        #if defined(PORTC_AVAILABLE)
            if( port == 'C')
                PORTC |= 1 << pin;
        #endif
                        
        #if defined(PORTD_AVAILABLE)
            if( port == 'D')
                PORTD |= 1 << pin;
        #endif
                
        #if defined(PORTE_AVAILABLE)
            if( port == 'E')
                PORTE |= 1 << pin;                        
        #endif
		
		#if defined(PORTF_AVAILABLE)
            if( port == 'F')
                PORTF |= 1 << pin;
        #endif
                        
        #if defined(PORTG_AVAILABLE)
            if( port == 'G')
                PORTG |= 1 << pin;
        #endif
                        
        #if defined(PORTH_AVAILABLE)
            if( port == 'H')
                PORTH |= 1 << pin;
        #endif
                        
        #if defined(PORTJ_AVAILABLE)
            if( port == 'J')
                PORTJ |= 1 << pin;
        #endif
                
        #if defined(PORTK_AVAILABLE)
            if( port == 'K')
                PORTK |= 1 << pin;                        
        #endif
		
		#if defined(PORTL_AVAILABLE)
            if( port == 'L')
                PORTL |= 1 << pin;                        
        #endif
    }            
    
	else if (value == LOW)
    {
        #if defined(PORTA_AVAILABLE)
            if( port == 'A')
                PORTA &= ~(1 << pin);
        #endif
                        
        #if defined(PORTB_AVAILABLE)
            if( port == 'B')
                PORTB &= ~(1 << pin);
        #endif
                        
        #if defined(PORTC_AVAILABLE)
            if( port == 'C')
                PORTC &= ~(1 << pin);
        #endif
                        
        #if defined(PORTD_AVAILABLE)
            if( port == 'D')
                PORTD &= ~(1 << pin);
        #endif
                        
        #if defined(PORTE_AVAILABLE)
            if( port == 'E')
                PORTE &= ~(1 << pin);                        
        #endif

		#if defined(PORTF_AVAILABLE)
            if( port == 'F')
                PORTF &= ~(1 << pin);
        #endif
                        
        #if defined(PORTG_AVAILABLE)
            if( port == 'G')
                PORTG &= ~(1 << pin);
        #endif
                        
        #if defined(PORTH_AVAILABLE)
            if( port == 'H')
                PORTH &= ~(1 << pin);
        #endif
                        
        #if defined(PORTJ_AVAILABLE)
            if( port == 'J')
                PORTJ &= ~(1 << pin);
        #endif
                        
        #if defined(PORTK_AVAILABLE)
            if( port == 'K')
                PORTK &= ~(1 << pin);                        
        #endif
		
		#if defined(PORTL_AVAILABLE)
            if( port == 'L')
                PORTL &= ~(1 << pin);                        
        #endif
    }                
	
	else if (value == TOGGLE)
    {
        #if defined(PORTA_AVAILABLE)
            if( port == 'A')
                PORTA ^= (1 << pin);
        #endif
                        
        #if defined(PORTB_AVAILABLE)
            if( port == 'B')
                PORTB ^= (1 << pin);
        #endif
                        
        #if defined(PORTC_AVAILABLE)
            if( port == 'C')
                PORTC ^= (1 << pin);
        #endif
                        
        #if defined(PORTD_AVAILABLE)
            if( port == 'D')
                PORTD ^= (1 << pin);
        #endif
                        
        #if defined(PORTE_AVAILABLE)
            if( port == 'E')
                PORTE ^= (1 << pin);                       
        #endif

        #if defined(PORTF_AVAILABLE)
            if( port == 'F')
                PORTF ^= (1 << pin);
        #endif
                        
        #if defined(PORTG_AVAILABLE)
            if( port == 'G')
                PORTG ^= (1 << pin);
        #endif
                        
        #if defined(PORTH_AVAILABLE)
            if( port == 'H')
                PORTH ^= (1 << pin);
        #endif
                        
        #if defined(PORTJ_AVAILABLE)
            if( port == 'J')
                PORTJ ^= (1 << pin);
        #endif
                        
        #if defined(PORTK_AVAILABLE)
            if( port == 'K')
                PORTK ^= (1 << pin);                       
        #endif 
		
        #if defined(PORTL_AVAILABLE)
            if( port == 'L')
                PORTL ^= (1 << pin);                       
        #endif 
    }                

}

static mos_uint8_t digital_read(char port, char pin)
{
    #if defined(PORTA_AVAILABLE)
        if( port == 'A')
            return (PINA & (1 << pin)) >> pin ;
    #endif
                            
    #if defined(PORTB_AVAILABLE)
        if( port == 'B')
            return (PINB & (1 << pin)) >> pin ;
    #endif
                            
    #if defined(PORTC_AVAILABLE)
        if( port == 'C')
            return (PINC & (1 << pin)) >> pin ;
    #endif
                            
    #if defined(PORTD_AVAILABLE)
        if( port == 'D')
            return (PIND & (1 << pin)) >> pin ;
    #endif
                    
    #if defined(PORTE_AVAILABLE)
        if( port == 'E')
            return (PINE & (1 << pin)) >> pin ;                        
    #endif	
	
	#if defined(PORTF_AVAILABLE)
        if( port == 'F')
            return (PINF & (1 << pin)) >> pin ;
    #endif
                            
    #if defined(PORTG_AVAILABLE)
        if( port == 'G')
            return (PING & (1 << pin)) >> pin ;
    #endif
                            
    #if defined(PORTH_AVAILABLE)
        if( port == 'H')
            return (PINH & (1 << pin)) >> pin ;
    #endif
                            
    #if defined(PORTJ_AVAILABLE)
        if( port == 'J')
            return (PINJ & (1 << pin)) >> pin ;
    #endif
                    
    #if defined(PORTK_AVAILABLE)
        if( port == 'K')
            return (PINK & (1 << pin)) >> pin ;                        
    #endif
	
	#if defined(PORTL_AVAILABLE)
        if( port == 'L')
            return (PINL & (1 << pin)) >> pin ;                        
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

const struct digital_driver avr_mega_digital_driver = {
	pinmode,
	write,
	read
};

#endif

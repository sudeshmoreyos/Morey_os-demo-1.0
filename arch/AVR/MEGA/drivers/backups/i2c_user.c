#include <io.h>
#include "i2c_user.h"

void i2c_init1()
{
    TWSR = 0x01;
    TWBR = 0x18;
    TWCR = 0x04;  
    // TWCR = 0x44; By doing so I2C is enabled and acknowledge is activated in master/slave read mode.
    // Not for multiple byte transfer receiver must acknowledge transmitter. No Acknowledge is signal to
    // transmitter that no more data should be transmitted. 
}

unsigned char i2c_start1()
{
    TWCR = (1<<TWINT) | ( 1<< TWSTA ) | (1<< TWEN );
    while( ( TWCR & (1<< TWINT)) == 0);
    // TWCR &= ~(1<<TWSTA); // as per datasheet start bit must be cleared by software after start bit is transmitted.
    return (TWSR & 0xF8);
}

unsigned char i2c_write1(unsigned char data)                           
{
    TWDR = data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while( ( TWCR & (1<< TWINT)) == 0);
    return (TWSR & 0xF8);
}

unsigned char i2c_read1(unsigned char * data, int acknowledge)
{
    if(acknowledge == 0)
    TWCR = ( 1 << TWINT ) | ( 1<< TWEN); 
    else
    TWCR = ( 1 << TWINT ) | ( 1<< TWEN) | (1<<TWEA);
    
    
    while( ( TWCR & (1 << TWINT) ) ==0);
    *data = TWDR;
    return (TWSR & 0xF8);
}

void i2c_stop1()
{
    TWCR = (1<<TWINT)| (1<< TWEN ) | ( 1<< TWSTO ) ;
}

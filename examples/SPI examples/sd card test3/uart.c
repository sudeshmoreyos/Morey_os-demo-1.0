#include "uart.h"
#include "Serial.h"

void UART_putc(unsigned char data)
{
	Serial.write(data);
}

void UART_puts(char* charString)
{
	Serial.print(charString);
}

void UART_puthex8(mos_uint8_t val)
{
    // extract upper and lower nibbles from input value
    mos_uint8_t upperNibble = (val & 0xF0) >> 4;
    mos_uint8_t lowerNibble = val & 0x0F;

    // convert nibble to its ASCII hex equivalent
    upperNibble += upperNibble > 9 ? 'A' - 10 : '0';
    lowerNibble += lowerNibble > 9 ? 'A' - 10 : '0';

    // print the characters
    UART_putc(upperNibble);
    UART_putc(lowerNibble);
}

unsigned char UART_getc(void)
{
	return Serial.read();
}

void UART_puts_p(char* ps)
{
	Serial.print(ps);
}

void UART_pputs(char* ps)
{
	Serial.print(ps);
}


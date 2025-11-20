#ifndef _uart_h
#define _uart_h

#include "morey_os.h"
// UART functions
void UART_putc(unsigned char data);
void UART_puts(char* charString);
void UART_puthex8(mos_uint8_t val);
unsigned char UART_getc(void);
void UART_puts_p(char* ps);
void UART_pputs(char* ps);

#endif

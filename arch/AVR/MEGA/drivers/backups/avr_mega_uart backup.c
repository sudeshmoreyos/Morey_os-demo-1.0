#include "avr_mega_uart.h"

#ifdef PLATFORM_SUPPORT_UART

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
	#include <avr/interrupt.h>
#endif

#ifdef RX_BUFFER_CONF
	#define RX_BUFFER_SIZE RX_BUFFER_CONF
#else
	#define RX_BUFFER_SIZE 64
#endif

static void begin(mos_uint32_t baudrate, mos_uint16_t databits, mos_uint8_t paritymode, mos_uint8_t stopbits)
{
    mos_uint32_t x;
    mos_uint8_t ucsrc = 0;
    ucsrc |= (1 << URSEL);
    UCSRB |= (1 << TXEN) | (1 << RXEN) | (1 << RXCIE);
   
    switch (databits)
    {
    case 6:
        ucsrc |= (1 << UCSZ0);
        break;
    case 7:
        ucsrc |= (1 << UCSZ1);
        break;
    case 9:
    {
        ucsrc |= (1 << UCSZ0) | (1 << UCSZ1);
        UCSRB |= (1 << UCSZ2);
        break;
    }
    default:
        ucsrc |= (1 << UCSZ0) | (1 << UCSZ1);
        break;
    }

    switch (paritymode)
    {
		case '1':
			ucsrc |= (1 << UPM1) | (1 << UPM0);
			break;
		case '2':
			ucsrc |= (1 << UPM1) | (0 << UPM0);
			break;
			case '0':
        ucsrc |= (0 << UPM1) | (0 << UPM0);
        break;

		default:
			ucsrc |= (0 << UPM1) | (1 << UPM0);
			break;
    }
	
    if (stopbits == '2')
    {
        ucsrc |= (1 << USBS);
    }
    else
    {
        ucsrc |= (0 << USBS);
    }
	
    UCSRC = ucsrc;
	x = (mos_uint32_t)(((float)CONTROLLER_FREQ / (16 * (float)baudrate)) - 1);
	UBRRH = (mos_uint8_t)(x >> 8);
	UBRRL = (mos_uint8_t)(x & 0xFF);
	
	// Global Interrupt should not be enabled here
	// As it will be enabled by OS
	//GLOBAL_INTERRUPT_ENABLE();
}

static void write(mos_uint8_t data)
{
    while (!(UCSRA & (1 << UDRE)));
    UDR = data;
}

static void print(char *data)
{
    while (*data)
    {
        write(*data);
        data++;
    }
}

mos_uint8_t rx_buffer[RX_BUFFER_SIZE];
mos_uint16_t rx_wr_index=0,rx_rd_index=0,rx_counter = 0;
mos_uint8_t rx_buffer_overflow=0;

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
	ISR(USART_RXC_vect)
#elif COMPILER == CODE_VISION_AVR
	interrupt [USART_RXC] void usart_rx_isr(void)
#endif
{
    mos_uint8_t status,data;
    status = UCSRA;
    data = UDR;

    if ((status & ((1<<FE)|(1<<PE)|(1<<DOR))) == 0)
    {
        rx_buffer[rx_wr_index++] = data;
        if(rx_wr_index == RX_BUFFER_SIZE)
            rx_wr_index = 0;
        if(++rx_counter == RX_BUFFER_SIZE)
        {
            rx_counter = 0;
            rx_buffer_overflow=1;
        }
    }
}

static mos_uint8_t read(void)
{
    mos_uint8_t data;
    if(rx_counter > 0)
    {
		data = rx_buffer[rx_rd_index++];
		if(rx_rd_index == RX_BUFFER_SIZE)
			rx_rd_index = 0;
		GLOBAL_INTERRUPT_DISABLE();
		--rx_counter;
		GLOBAL_INTERRUPT_ENABLE();
		return data;
    }
    else
    {
        return 0x00;
    }
}

static int available(void)
{
    if(rx_buffer_overflow == 1)
        return -1;
    else
        return (int)rx_counter;
}

static void println(char *data)
{
	print(data);
	write('\n');
}

const struct serial_driver avr_mega_uart_driver = {
	begin,
	write,
	read,
	print,
	available,
	println
};

#endif
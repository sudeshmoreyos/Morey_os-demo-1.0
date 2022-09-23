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

#ifdef SERIAL_TX_INTERRUPT_ENABLE
	#ifdef TX_BUFFER_CONF
		#define TX_BUFFER_SIZE TX_BUFFER_CONF
	#else
		#define TX_BUFFER_SIZE 64
	#endif
#endif

#ifdef SERIAL_TX_DISABLE
	#ifdef SERIAL_RX_DISABLE
		#error If Serial driver is enabled, both Tx and Rx cannot be disabled
	#endif
#endif

#include "../../../../os/util/ringbuf.h"

// define generic Macros for UART registers
#define UART_STATUS 	UCSRA
#define UART_CONTROL1 	UCSRB
#define UART_CONTROL2 	UCSRC
#define UART_DATA 		UDR
#define UART_BAUD_LOW 	UBRRL
#define UART_BAUD_HIGH 	UBRRH

// define generic Macros for UART Bits
#define UART_STATUS_DOUBLE_SPEED_BIT 	1
#define UART_STATUS_PARITY_ERROR_BIT 	2
#define UART_STATUS_DATA_OVERRUN_BIT 	3
#define UART_STATUS_FRAME_ERROR_BIT  	4
#define UART_STATUS_UDR_EMPTY_BIT 	 	5
#define UART_STATUS_TX_COMPLETE_BIT	 	6

#define UART_CONTROL1_TX_ENABLE_BIT		3
#define UART_CONTROL1_RX_ENABLE_BIT		4
#define UART_CONTROL1_UDRIE_ENABLE_BIT	5
#define UART_CONTROL1_RXCIE_ENABLE_BIT	7

#define UART_CONTROL2_CHAR_SIZE_BIT0	1
#define UART_CONTROL2_CHAR_SIZE_BIT1	2
#define UART_CONTROL2_STOP_SIZE_BIT		3
#define UART_CONTROL2_PARITY_MODE_BIT0	4
#define UART_CONTROL2_PARITY_MODE_BIT1	5
#define UART_CONTROL2_REG_SEL_BIT		7

// define generic Macros for UART config Macros
#define RX_BUFFER_SIZE_TEMP 					RX_BUFFER_SIZE
#define TX_BUFFER_SIZE_TEMP 					TX_BUFFER_SIZE

#ifdef 											SERIAL_TX_INTERRUPT_ENABLE
	#define SERIAL_TX_INTERRUPT_ENABLE_TEMP
#endif

#ifdef 											SERIAL_TX_DISABLE
	#define SERIAL_TX_DISABLE_TEMP
#endif

#ifdef 											SERIAL_RX_DISABLE
	#define SERIAL_RX_DISABLE_TEMP
#endif


#ifndef SERIAL_RX_DISABLE_TEMP
	mos_uint8_t bufferRx[RX_BUFFER_SIZE_TEMP];
	ringbuf_struct_t buffer_struct_rx;
#endif

#ifndef SERIAL_TX_DISABLE_TEMP
	#ifdef SERIAL_TX_INTERRUPT_ENABLE_TEMP
		mos_uint8_t bufferTx[TX_BUFFER_SIZE_TEMP];
		ringbuf_struct_t buffer_struct_tx;
	#endif
#endif

static void begin(mos_uint32_t baudrate, mos_uint8_t mode)
{
    float x,X,E1,E2;
	mos_uint32_t y,Y;
    mos_uint8_t ucsrc = 0;
	
    ucsrc = (1 << UART_CONTROL2_REG_SEL_BIT);
	
	#ifndef SERIAL_TX_DISABLE_TEMP
		UART_CONTROL1 = (1 << UART_CONTROL1_TX_ENABLE_BIT);
		#ifdef SERIAL_TX_INTERRUPT_ENABLE_TEMP
			ringbuf_init(&buffer_struct_tx, bufferTx, TX_BUFFER_SIZE_TEMP);
		#endif
	#endif
	
	#ifndef SERIAL_RX_DISABLE_TEMP
		UART_CONTROL1 |= (1 << UART_CONTROL1_RX_ENABLE_BIT) | (1 << UART_CONTROL1_RXCIE_ENABLE_BIT);
		ringbuf_init(&buffer_struct_rx, bufferRx, RX_BUFFER_SIZE_TEMP);
	#endif
	
	
	// do settings for selected databits mode
    switch (mode >> 4)
    {
		case 6:
			ucsrc |= (1 << UART_CONTROL2_CHAR_SIZE_BIT0);
			break;
			
		case 7:
			ucsrc |= (1 << UART_CONTROL2_CHAR_SIZE_BIT1);
			break;
		
		case 8:
			ucsrc |= (1 << UART_CONTROL2_CHAR_SIZE_BIT0) | (1 << UART_CONTROL2_CHAR_SIZE_BIT1);
			break;
			
		// Currently 9 data bit mode is not supported
		/*	
		case 9:
		{
			ucsrc |= (1 << UCSZ0) | (1 << UCSZ1);
			UART_CONTROL1 |= (1 << UCSZ2);
			break;
		}
		*/
    }

	// do settings for selected parity bit mode
    switch ((mode>>2) & 0x3)
    {
		case ODD:
			ucsrc |= (1 << UART_CONTROL2_PARITY_MODE_BIT1) | (1 << UART_CONTROL2_PARITY_MODE_BIT0);
			break;
		case EVEN:
			ucsrc |= (1 << UART_CONTROL2_PARITY_MODE_BIT1) | (0 << UART_CONTROL2_PARITY_MODE_BIT0);
			break;
		case NONE:
			ucsrc |= (0 << UART_CONTROL2_PARITY_MODE_BIT1) | (0 << UART_CONTROL2_PARITY_MODE_BIT0);
			break;
    }
	
	// do settings for selected stop bit mode
    if ((mode & 0x3) == 2)
    {
        ucsrc |= (1 << UART_CONTROL2_STOP_SIZE_BIT);
    }
    else
    {
        ucsrc |= (0 << UART_CONTROL2_STOP_SIZE_BIT);
    }
    
	UART_CONTROL2 = ucsrc;
	
	// Calculate baudrate error in normal speed mode
	x = (((float)CONTROLLER_FREQ/ (16 * (float)baudrate)) - 1);
	y = (mos_uint32_t) x;
	E1 = (((x - (float)y) / x ) * 100);
	if (E1 < 0)
	{
		 E1 = (E1 * (-1));
	}
	
	// Calculate baudrate error in double speed mode
	X = (((float)CONTROLLER_FREQ / (8 * (float)baudrate)) - 1);
	Y = (mos_uint32_t) X;
	E2 = (((X - (float)Y) / X ) * 100);
	if (E2 < 0)
	{
		 E2 = (E2 * (-1));
	}
	
	// Choose mode with lesser baudrate error
	if(E1 <= E2)
	{
		UART_BAUD_HIGH = (mos_uint8_t)(y >> 8);
		UART_BAUD_LOW = (mos_uint8_t)(y & 0xFF);
	}
	else
	{
		UART_BAUD_HIGH = (mos_uint8_t)(Y >> 8);
		UART_BAUD_LOW = (mos_uint8_t)(Y & 0xFF);
		UART_STATUS |= (1<<UART_STATUS_DOUBLE_SPEED_BIT);
	}
}

// UART Rx Functions
#ifdef SERIAL_RX_DISABLE_TEMP

	static int available(void)
	{
		return 0;
	}

	static mos_uint8_t read(void)
	{
		return 0xFF;
	}

	static void flush(void)
	{
		return;
	}

#else
	#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
		ISR(USART_RXC_vect)
	#elif COMPILER == CODEVISION_AVR
		interrupt [USART_RXC] void usart_rx_isr(void)
	#endif
	{
		mos_uint8_t status,data;
		status = UART_STATUS;
		data = UART_DATA;

		if ((status & ((1<<UART_STATUS_FRAME_ERROR_BIT)|(1<<UART_STATUS_PARITY_ERROR_BIT)|(1<<UART_STATUS_DATA_OVERRUN_BIT))) == 0)
		{
			ringbuf_write(&buffer_struct_rx,data);
		}
	}

	static int available(void)
	{
		return (int)ringbuf_count(&buffer_struct_rx);
	}

	static mos_uint8_t read(void)
	{
		return ringbuf_read(&buffer_struct_rx);
	}

	static void flush(void)
	{
		ringbuf_flush(&buffer_struct_rx);
	}

#endif

// UART Tx Functions
#ifdef SERIAL_TX_DISABLE_TEMP

	static void write(mos_uint8_t data)
	{
		(void) data;
		return;
	}

	static void print(char *data)
	{
		(void) data;
		return;
	}

	static void println(char *data)
	{
		(void) data;
		return;
	 }

	static void printBytes(mos_uint8_t * data, mos_uint16_t len)
	{
		(void) data;
		(void) len;
		return;
	}
	
	static mos_uint8_t txComplete(void)
	{
		return 1;
	}

#else
	
	#ifdef SERIAL_TX_INTERRUPT_ENABLE_TEMP
		#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
			ISR(USART_UDRE_vect)
		#elif COMPILER == CODEVISION_AVR
			interrupt [USART_UDRE] void usart_udre_isr(void)
		#endif
		{
			if(ringbuf_count(&buffer_struct_tx))
				UART_DATA = ringbuf_read(&buffer_struct_tx);
			else
			{
				GLOBAL_INTERRUPT_DISABLE();
				UART_CONTROL1 &= ~(1<<UART_CONTROL1_UDRIE_ENABLE_BIT);
				GLOBAL_INTERRUPT_ENABLE();
			}
		}
	#endif

	static void write(mos_uint8_t data)
	{
		#ifdef SERIAL_TX_INTERRUPT_ENABLE_TEMP
			ringbuf_write(&buffer_struct_tx,data);
			if((UART_CONTROL1 & (1<<UART_CONTROL1_UDRIE_ENABLE_BIT))==0)
			{
				GLOBAL_INTERRUPT_DISABLE();
				UART_CONTROL1 |= (1<<UART_CONTROL1_UDRIE_ENABLE_BIT);
				GLOBAL_INTERRUPT_ENABLE();
			}
		#else
			while (!(UART_STATUS & (1 << UART_STATUS_UDR_EMPTY_BIT)));
			UART_DATA = data;
		#endif
	}

	static void print(char *data)
	{
		while (*data)
		{
			write(*data);
			data++;
		}
	}

	static void println(char *data)
	{
		 print(data);
		 write('\n');
	 }

	static void printBytes(mos_uint8_t * data, mos_uint16_t len)
	{
		while (len)
		{
			write(*data);
			data++;
			len--;
		}
	}
	
	static mos_uint8_t txComplete(void)
	{
		#ifdef SERIAL_TX_INTERRUPT_ENABLE_TEMP
			if(ringbuf_count(&buffer_struct_tx) > 0) // check if Tx ring buffer is empty
				return 0;
			else if ( (UART_STATUS & (1<<UART_STATUS_TX_COMPLETE_BIT)) == 0) // check if transmit shif register of mega8 is empty
				return 0;
			else
				return 1;
		#else
			if ( (UART_STATUS & (1<<UART_STATUS_TX_COMPLETE_BIT)) == 0) // check if transmit shif register of mega8 is empty
				return 0;
			else
				return 1;
		#endif
	}

#endif

const struct serial_driver avr_mega_uart_driver = {
	begin,
	available,
	read,
	flush,
	write,
	print,
	println,
	printBytes,
	txComplete,
};

// Un-define generic Macros for UART registers
#undef UART_STATUS
#undef UART_CONTROL1
#undef UART_CONTROL2
#undef UART_DATA
#undef UART_BAUD_LOW
#undef UART_BAUD_HIGH

// Un-define generic Macros for UART Bits
#undef UART_STATUS_DOUBLE_SPEED_BIT
#undef UART_STATUS_PARITY_ERROR_BIT
#undef UART_STATUS_DATA_OVERRUN_BIT
#undef UART_STATUS_FRAME_ERROR_BIT
#undef UART_STATUS_UDR_EMPTY_BIT
#undef UART_STATUS_TX_COMPLETE_BIT

#undef UART_CONTROL1_TX_ENABLE_BIT
#undef UART_CONTROL1_RX_ENABLE_BIT
#undef UART_CONTROL1_UDRIE_ENABLE_BIT
#undef UART_CONTROL1_RXCIE_ENABLE_BIT

#undef UART_CONTROL2_CHAR_SIZE_BIT0
#undef UART_CONTROL2_CHAR_SIZE_BIT1
#undef UART_CONTROL2_STOP_SIZE_BIT
#undef UART_CONTROL2_PARITY_MODE_BIT0
#undef UART_CONTROL2_PARITY_MODE_BIT1
#undef UART_CONTROL2_REG_SEL_BIT

// Un-define generic Macros for UART config Macros
#undef RX_BUFFER_SIZE_TEMP
#undef TX_BUFFER_SIZE_TEMP

#ifdef SERIAL_TX_INTERRUPT_ENABLE_TEMP
	#undef SERIAL_TX_INTERRUPT_ENABLE_TEMP
#endif

#ifdef SERIAL_TX_DISABLE_TEMP
	#undef SERIAL_TX_DISABLE_TEMP
#endif

#ifdef SERIAL_RX_DISABLE_TEMP
	#undef SERIAL_RX_DISABLE_TEMP
#endif

#endif
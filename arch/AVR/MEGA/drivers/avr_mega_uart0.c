//-------------------Macros to change for UARTx------------------//
// avr_mega_uart0
// UART0_AVAILABLE
// UART0_ENABLE
// UART0_TX_DISABLE
// UART0_RX_DISABLE
// UART0_RX_BUFFER_CONF
// UART0_RX_BUFFER_DEFAULT
// UART0_TX_INTERRUPT_ENABLE
// UART0_TX_BUFFER_CONF
// UART0_TX_BUFFER_DEFAULT
// avr_mega_uart0_driver
// MCU_UART0_RX_INTERRUPT_ISR
// MCU_UART0_TX_INTERRUPT_ISR
// USART0_RX_vect
// USART0_UDRE_vect

#include "avr_mega_uart0.h"

#if defined(PLATFORM_SUPPORT_UART) && defined(UART0_AVAILABLE) && (UART0_ENABLE == 1)

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
	#include <avr/interrupt.h>
#endif


#ifndef UART0_RX_DISABLE
	#if defined(UART0_RX_BUFFER_CONF)
		#define RX_BUFFER_SIZE_TEMP UART0_RX_BUFFER_CONF
	#elif defined(UART0_RX_BUFFER_DEFAULT)
		#define RX_BUFFER_SIZE_TEMP UART0_RX_BUFFER_DEFAULT
	#else
		#define RX_BUFFER_SIZE_TEMP 64
	#endif
#endif

#ifndef UART0_TX_DISABLE
	#ifdef UART0_TX_INTERRUPT_ENABLE
		#if defined(UART0_TX_BUFFER_CONF)
			#define TX_BUFFER_SIZE_TEMP UART0_TX_BUFFER_CONF
		#elif defined(UART0_TX_BUFFER_DEFAULT)
			#define TX_BUFFER_SIZE_TEMP UART0_TX_BUFFER_DEFAULT
		#else
			#define TX_BUFFER_SIZE_TEMP 64
		#endif
	#endif
#endif

#ifdef UART0_TX_DISABLE
	#ifdef UART0_RX_DISABLE
		#error If Serial driver is enabled, both Tx and Rx cannot be disabled
	#endif
#endif

#include "../../../../os/util/ringbuf.h"

// Do changes here
// define generic Macros for UART registers
#define UART_STATUS 			UCSR0A
#define UART_CONTROL1 			UCSR0B
#define UART_CONTROL2 			UCSR0C
#define UART_DATA 			UDR0
#define UART_BAUD_LOW 			UBRR0L
#define UART_BAUD_HIGH 			UBRR0H
#define UART_DRIVER			avr_mega_uart0_driver

// Unfortunately Interrupt routines are not same for all controllers in AVR
// Hence These ISR routines must be imported from controller specific macros 
// if it is different from standard ISRs
#ifdef MCU_UART0_RX_INTERRUPT_ISR
	#define UART_RX_INTERRUPT_ISR	MCU_UART0_RX_INTERRUPT_ISR
#else
	#define UART_RX_INTERRUPT_ISR	USART0_RX_vect
#endif

#ifdef MCU_UART0_RX_INTERRUPT_ISR
	#define UART_TX_INTERRUPT_ISR	MCU_UART0_TX_INTERRUPT_ISR
#else
	#define UART_TX_INTERRUPT_ISR	USART0_UDRE_vect
#endif

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
#define UART_CONTROL1_TXCIE_ENABLE_BIT	6
#define UART_CONTROL1_RXCIE_ENABLE_BIT	7

#define UART_CONTROL2_CHAR_SIZE_BIT0	1
#define UART_CONTROL2_CHAR_SIZE_BIT1	2
#define UART_CONTROL2_STOP_SIZE_BIT		3
#define UART_CONTROL2_PARITY_MODE_BIT0	4
#define UART_CONTROL2_PARITY_MODE_BIT1	5
#define UART_CONTROL2_REG_SEL_BIT		7

// define generic Macros for UART config Macros

#ifdef 											UART0_TX_INTERRUPT_ENABLE
	#define UART_TX_INTERRUPT_ENABLE_TEMP
#endif

#ifdef 											UART0_TX_DISABLE
	#define UART_TX_DISABLE_TEMP
#endif

#ifdef 											UART0_RX_DISABLE
	#define UART_RX_DISABLE_TEMP
#endif

#ifndef UART_RX_DISABLE_TEMP
	static mos_uint8_t bufferRx[RX_BUFFER_SIZE_TEMP];
	static ringbuf_struct_t buffer_struct_rx;
#endif

#ifndef UART_TX_DISABLE_TEMP
	#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
		static mos_uint8_t bufferTx[TX_BUFFER_SIZE_TEMP];
		static ringbuf_struct_t buffer_struct_tx;
	#endif
#endif

static void begin(mos_uint32_t baudrate, mos_uint8_t mode)
{
    float x,X,error1,error2;
	mos_uint32_t y,Y;
    mos_uint8_t ucsrc = 0;
	
    ucsrc = (1 << UART_CONTROL2_REG_SEL_BIT);
	
	#ifndef UART_TX_DISABLE_TEMP
		UART_CONTROL1 = (1 << UART_CONTROL1_TX_ENABLE_BIT);
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			//UART_CONTROL1 |= (1<<UART_CONTROL1_TXCIE_ENABLE_BIT);		
			ringbuf_init(&buffer_struct_tx, bufferTx, TX_BUFFER_SIZE_TEMP);
		#endif
	#endif
	
	#ifndef UART_RX_DISABLE_TEMP
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
	error1 = (((x - (float)y) / x ) * 100);
	if (error1 < 0)
	{
		 error1 = (error1 * (-1));
	}
	
	// Calculate baudrate error in double speed mode
	X = (((float)CONTROLLER_FREQ / (8 * (float)baudrate)) - 1);
	Y = (mos_uint32_t) X;
	error2 = (((X - (float)Y) / X ) * 100);
	if (error2 < 0)
	{
		 error2 = (error2 * (-1));
	}
	
	// Choose Single speed mode if error is less than 0.5%
	// Otherwise choose mode with lesser baudrate error
	if(error1 < 0.5 || error1 <= error2)
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
	
	#ifdef UART_RX_DISABLE_TEMP
		ringbuf_flush(&buffer_struct_rx);
	#endif
}

static void end(void)
{
	UART_CONTROL1 = 0;
	UART_CONTROL2 = 0;
}
// UART Rx Functions
#ifdef UART_RX_DISABLE_TEMP

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
		ISR(UART_RX_INTERRUPT_ISR)
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

	static unsigned int available(void)
	{
		return (unsigned int)ringbuf_count(&buffer_struct_rx);
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
#ifdef UART_TX_DISABLE_TEMP

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
	
	#ifdef PLATFORM_SUPPORT_CONST_PRINT
	
		static void constPrintArch(const char *data)
		{
			(void) data;
			return;
		}

		static void constPrintlnArch(const char *data)
		{
			(void) data;
			return;
		}
	
	#endif

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
	
	#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
		#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
			ISR(UART_TX_INTERRUPT_ISR)
		#endif
		{
			//UART_CONTROL1 &= ~(1<<UART_CONTROL1_UDRIE_ENABLE_BIT);
			UART_DATA = ringbuf_read_noAtomic(&buffer_struct_tx);
			UART_STATUS |= (1<<UART_STATUS_TX_COMPLETE_BIT);
			if(ringbuf_count(&buffer_struct_tx)==0)
				UART_CONTROL1 &= ~(1<<UART_CONTROL1_UDRIE_ENABLE_BIT);				
				//UART_CONTROL1 |= (1<<UART_CONTROL1_UDRIE_ENABLE_BIT);				
		}
	#endif

	static void write(mos_uint8_t data)
	{
		
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			mos_uint8_t atomic_status = 0;
			
			atomic_status = IS_ATOMIC_ON();
			if(!atomic_status)
				ATOMIC_ON();
			
			if( ( ringbuf_count(&buffer_struct_tx) == 0 ) && ( ( UART_STATUS & (1 << UART_STATUS_UDR_EMPTY_BIT) ) != 0) )
					UART_DATA = data;
			else if( !ringbuf_isfull(&buffer_struct_tx) )
			{
				ringbuf_write(&buffer_struct_tx,data);
				if((UART_CONTROL1 & (1<<UART_CONTROL1_UDRIE_ENABLE_BIT))==0)
					UART_CONTROL1 |= (1<<UART_CONTROL1_UDRIE_ENABLE_BIT);
			}
			
			if(!atomic_status)
				ATOMIC_OFF();
		#else
			while (!(UART_STATUS & (1 << UART_STATUS_UDR_EMPTY_BIT)));
			UART_DATA = data;
		#endif
	}

	static void print(char *data)
	{
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			mos_uint8_t atomic_status = 0;
			
			atomic_status = IS_ATOMIC_ON();
			if(!atomic_status)
				ATOMIC_ON();
		#endif
			
		while (*data)
		{
			write((mos_uint8_t)(*data));
			data++;
		}
		
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			if(!atomic_status)
				ATOMIC_OFF();
		#endif
		
	}

	static void println(char *data)
	{
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			ATOMIC_ON();
		#endif
		
		print(data);
		write('\r');
		write('\n');
		
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			ATOMIC_OFF();
		#endif
	}
	
	#ifdef PLATFORM_SUPPORT_CONST_PRINT
	
		static void constPrintArch(const char *data)
		{
			#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
				mos_uint8_t atomic_status = 0;
				
				atomic_status = IS_ATOMIC_ON();
				if(!atomic_status)
					ATOMIC_ON();
			#endif
			
			while (pgm_read_byte(data))
			{
				write(pgm_read_byte(data));
				data++;
			}
			
			#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
				if(!atomic_status)
					ATOMIC_OFF();
			#endif
		}

		static void constPrintlnArch(const char *data)
		{
			#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
				ATOMIC_ON();
			#endif
			
			constPrintArch(data);
			write('\r');
			write('\n');
			
			#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
				ATOMIC_OFF();
			#endif
		}
	
	#endif

	static void printBytes(mos_uint8_t * data, mos_uint16_t len)
	{
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			ATOMIC_ON();
		#endif
		while (len)
		{
			write(*data);
			data++;
			len--;
		}
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			ATOMIC_OFF();
		#endif
	}
	
	static mos_uint8_t txComplete(void)
	{
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			if(ringbuf_count(&buffer_struct_tx) == 0)
			{
				if(UART_STATUS & (1 << UART_STATUS_TX_COMPLETE_BIT))
					return 1;
				else
					return 0;
			}
			else
				return 0;
		#else
			if(UART_STATUS & (1 << UART_STATUS_TX_COMPLETE_BIT))
				return 1;
			else
				return 0;
		#endif
	}
#endif

const struct serial_driver UART_DRIVER = {
	begin,
	end,
	available,
	read,
	flush,
	write,
	print,
	println,
	#ifdef PLATFORM_SUPPORT_CONST_PRINT
		constPrintArch,
		constPrintlnArch,
	#endif
	printBytes,
	txComplete,
};

// Un-define generic Macros for UART registers
#undef UART_RX_INTERRUPT_ISR
#undef UART_TX_INTERRUPT_ISR
#undef UART_STATUS
#undef UART_CONTROL1
#undef UART_CONTROL2
#undef UART_DATA
#undef UART_BAUD_LOW
#undef UART_BAUD_HIGH
#undef UART_DRIVER

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

#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
	#undef UART_TX_INTERRUPT_ENABLE_TEMP
#endif

#ifdef UART_TX_DISABLE_TEMP
	#undef UART_TX_DISABLE_TEMP
#endif

#ifdef UART_RX_DISABLE_TEMP
	#undef UART_RX_DISABLE_TEMP
#endif

#endif 
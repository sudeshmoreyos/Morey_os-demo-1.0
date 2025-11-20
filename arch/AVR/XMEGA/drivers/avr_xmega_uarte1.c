#include "avr_xmega_uarte1.h"
#if defined(PLATFORM_SUPPORT_UART) && defined(UARTE1_AVAILABLE) && ( UARTE1_ENABLE == 1 )

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
	#include <avr/interrupt.h>
#endif

#if defined(UARTE1_RX_BUFFER_CONF)
	#define UARTE1_RX_BUFFER_SIZE UARTE1_RX_BUFFER_CONF
#elif defined(UARTE1_RX_BUFFER_DEFAULT)
	#define UARTE1_RX_BUFFER_SIZE UARTE1_RX_BUFFER_DEFAULT
#else
	#define UARTE1_RX_BUFFER_SIZE 64
#endif

#ifdef UARTE1_TX_INTERRUPT_ENABLE
	#if defined(UARTE1_TX_BUFFER_CONF)
		#define UARTE1_TX_BUFFER_SIZE UARTE1_TX_BUFFER_CONF
	#elif defined(UARTE1_TX_BUFFER_DEFAULT)
		#define UARTE1_TX_BUFFER_SIZE UARTE1_TX_BUFFER_DEFAULT
	#else
		#define UARTE1_TX_BUFFER_SIZE 64
	#endif
#endif

#ifdef UARTE1_TX_DISABLE
	#ifdef UARTE1_RX_DISABLE
		#error If Serial driver is enabled, both Tx and Rx cannot be disabled
	#endif
#endif

#include "../../../../os/util/ringbuf.h"
#include "../../../../os/drivers/Digital.h"

// define generic Macros for UART registers
#define UART_REMAP 		PORTE.REMAP
#define UART_STATUS 	USARTE1.STATUS
#define UART_CONTROL1 	USARTE1.CTRLB
#define UART_CONTROL2 	USARTE1.CTRLC
#define UART_INTERRUPT	USARTE1.CTRLA
#define UART_DATA 		USARTE1.DATA
#define UART_BAUD_LOW 	USARTE1.BAUDCTRLA
#define UART_BAUD_HIGH 	USARTE1.BAUDCTRLB

// define generic Macros for UART Bits

#define UART_RXTX_REMAP_BIT 	4

//#define UART_STATUS_DOUBLE_SPEED_BIT 	1
#define UART_STATUS_PARITY_ERROR_BIT 	2
#define UART_STATUS_DATA_OVERRUN_BIT 	3
#define UART_STATUS_FRAME_ERROR_BIT  	4
#define UART_STATUS_UDR_EMPTY_BIT 	 	5
#define UART_STATUS_TX_COMPLETE_BIT	 	6

#define UART_CONTROL1_DOUBLE_SPEED_BIT 	2
#define UART_CONTROL1_TX_ENABLE_BIT		3
#define UART_CONTROL1_RX_ENABLE_BIT		4
//#define UART_CONTROL1_UDRIE_ENABLE_BIT	5
//#define UART_CONTROL1_TXCIE_ENABLE_BIT	6
//#define UART_CONTROL1_RXCIE_ENABLE_BIT	7

#define UART_CONTROL2_CHAR_SIZE_BIT0	0
#define UART_CONTROL2_CHAR_SIZE_BIT1	1
#define UART_CONTROL2_STOP_SIZE_BIT		3
#define UART_CONTROL2_PARITY_MODE_BIT0	4
#define UART_CONTROL2_PARITY_MODE_BIT1	5
//#define UART_CONTROL2_REG_SEL_BIT		7

#define UART_INTERRUPT_DREINTLVL_BIT0	0
#define UART_INTERRUPT_DREINTLVL_BIT1	1
#define UART_INTERRUPT_TXCINTLVL_BIT0	2
#define UART_INTERRUPT_TXCINTLVL_BIT1	3
#define UART_INTERRUPT_RXCINTLVL_BIT0	4
#define UART_INTERRUPT_RXCINTLVL_BIT1	5

// define generic Macros for UART config Macros
#define RX_BUFFER_SIZE_TEMP 					UARTE1_RX_BUFFER_SIZE
#define TX_BUFFER_SIZE_TEMP 					UARTE1_TX_BUFFER_SIZE

#ifdef 											UARTE1_TX_INTERRUPT_ENABLE
	#define UART_TX_INTERRUPT_ENABLE_TEMP
#endif

#ifdef 											UARTE1_TX_DISABLE
	#define UART_TX_DISABLE_TEMP
#endif

#ifdef 											UARTE1_RX_DISABLE
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
		static mos_uint8_t tx_complete_flag = 0;
	#endif
#endif

static void begin(mos_uint32_t baudrate, mos_uint8_t mode)
{
    mos_uint8_t temp1 = 0, temp2 = 0;
	
	UART_INTERRUPT = 0x00;
	UART_CONTROL1 = 0x00;
	UART_CONTROL2 = 0x00;
	
	// reads RX and TX pins location based on chosen platform
	// RX pin location is saved in temp1 while TX pin location is saved in temp2
	#ifdef UARTE_RXTX_REMAP
		temp1 = E2;
		temp2 = E3;
		UART_REMAP |= 1 << UART_RXTX_REMAP_BIT;
	#else
		temp1 = E6;
		temp2 = E7;
	#endif
	
	// STEP-1 : Configure Digital modes of RX TX Pins
	
	#ifndef UART_TX_DISABLE_TEMP
		// set TX pin output value as HIGH
		Digital.write(temp2, HIGH);
		// set TX pin in Digital OUTPUT Mode
		Digital.pinmode(temp2, OUTPUT);
	#endif
	
	#ifndef UART_TX_DISABLE_TEMP
		// set RX pin Digital INPUT mode
		Digital.pinmode(temp1, INPUT);
	#endif
	
	// STEP-2 : Set baudrate and frame format
	UART_BAUD_LOW = 0x2E;
	UART_BAUD_HIGH = 0x98;	
	
	temp1 = 0;
	// do settings for selected databits mode
    switch (mode >> 4)
    {
		case 5:
			temp1 |= (0 << UART_CONTROL2_CHAR_SIZE_BIT1) | (0 << UART_CONTROL2_CHAR_SIZE_BIT0);
			break;
			
		case 6:
			temp1 |= (0 << UART_CONTROL2_CHAR_SIZE_BIT1)| (1 << UART_CONTROL2_CHAR_SIZE_BIT0);
			break;
			
		case 7:
			temp1 |= (1 << UART_CONTROL2_CHAR_SIZE_BIT1)| (0 << UART_CONTROL2_CHAR_SIZE_BIT0);
			break;
		
		case 8:
			temp1 |= (1 << UART_CONTROL2_CHAR_SIZE_BIT1) | (1 << UART_CONTROL2_CHAR_SIZE_BIT0);
			break;
			
		// Currently 9 data bit mode is not supported
		/*	
		case 9:
		{
			temp1 |= (1 << UCSZ0) | (1 << UCSZ1);
			UART_CONTROL1 |= (1 << UCSZ2);
			break;
		}
		*/
    }

	// do settings for selected parity bit mode
    switch ((mode>>2) & 0x3)
    {
		case ODD:
			temp1 |= (1 << UART_CONTROL2_PARITY_MODE_BIT1) | (1 << UART_CONTROL2_PARITY_MODE_BIT0);
			break;
		case EVEN:
			temp1 |= (1 << UART_CONTROL2_PARITY_MODE_BIT1) | (0 << UART_CONTROL2_PARITY_MODE_BIT0);
			break;
		case NONE:
			temp1 |= (0 << UART_CONTROL2_PARITY_MODE_BIT1) | (0 << UART_CONTROL2_PARITY_MODE_BIT0);
			break;
    }
	
	// do settings for selected stop bit mode
    if ((mode & 0x3) == 2)
    {
        temp1 |= (1 << UART_CONTROL2_STOP_SIZE_BIT);
    }
    else
    {
        temp1 |= (0 << UART_CONTROL2_STOP_SIZE_BIT);
    }
    
	UART_CONTROL2 = temp1;
	
	// STEP-3 : Enable RX and TX
	temp1=0;
	temp2=0;
	#ifndef UART_TX_DISABLE_TEMP
		temp1 |= (1 << UART_CONTROL1_TX_ENABLE_BIT);
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			temp2 |= (1<<UART_INTERRUPT_TXCINTLVL_BIT0)|(0<<UART_INTERRUPT_TXCINTLVL_BIT1);
			ringbuf_init(&buffer_struct_tx, bufferTx, TX_BUFFER_SIZE_TEMP);
		#endif
	#endif
	
	#ifndef UART_RX_DISABLE_TEMP
		temp1 |= (1 << UART_CONTROL1_RX_ENABLE_BIT);
		temp2 |= (1<<UART_INTERRUPT_RXCINTLVL_BIT0)|(0<<UART_INTERRUPT_RXCINTLVL_BIT1);
		ringbuf_init(&buffer_struct_rx, bufferRx, RX_BUFFER_SIZE_TEMP);
	#endif
	
	UART_INTERRUPT = temp2;
	UART_CONTROL1 = temp1;
}

// UART Rx Functions
#ifdef UART_RX_DISABLE_TEMP

	static unsigned int available(void)
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
		ISR(USARTE1_RXC_vect)
	#elif COMPILER == CODEVISION_AVR
		interrupt [USARTE1_RXC] void usarte1_rxc_isr(void)
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
		return (unsigned int) ringbuf_count(&buffer_struct_rx);
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
			//ISR(USART_UDRE_vect)
			ISR(USARTE1_TXC_vect)
		#elif COMPILER == CODEVISION_AVR
			interrupt [USARTE1_TXC] void usarte1_txc_isr(void)
		#endif
		{
			if(ringbuf_count(&buffer_struct_tx))
				UART_DATA = ringbuf_read(&buffer_struct_tx);
			else
				tx_complete_flag = 1;
		}
	#endif

	static void write(mos_uint8_t data)
	{
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			tx_complete_flag = 0;
			if( ( ringbuf_count(&buffer_struct_tx) == 0 ) && ( ( UART_STATUS & (1 << UART_STATUS_UDR_EMPTY_BIT) ) != 0) )
				UART_DATA = data;
			else
				ringbuf_write(&buffer_struct_tx,data);
		#else
			while ((UART_STATUS & (1 << UART_STATUS_UDR_EMPTY_BIT))==0);
			UART_DATA = data;
		#endif
	}

	static void print(char *data)
	{
		while (*data)
		{
			write((mos_uint8_t)(*data));
			data++;
		}
	}

	static void println(char *data)
	{
		 print(data);
		 write('\r');
		 write('\n');
	}
	
	static void constPrintArch(const char *data)
	{
		while (pgm_read_byte(*data) != 0x00)
		{
			write(pgm_read_byte(*data));
			data++;
		}
	}

	static void constPrintlnArch(const char *data)
	{
		constPrintArch(data);
		write('\r');
		write('\n');
	}

	static void printBytes(mos_uint8_t * data, mos_uint16_t len)
	{
		while (len)
		{
			write((mos_uint8_t)(*data));
			data++;
			len--;
		}
	}
	
	static mos_uint8_t txComplete(void)
	{
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			return tx_complete_flag;
		#else
			if ( (UART_STATUS & (1<<UART_STATUS_TX_COMPLETE_BIT)) == 0) // check if transmit shift register of mega8 is empty
				return 0;
			else
				return 1;
		#endif
	}


#endif

const struct serial_driver avr_xmega_uarte1_driver = {
	begin,
	available,
	read,
	flush,
	write,
	print,
	println,
	constPrintArch,
	constPrintlnArch,
	printBytes,
	txComplete,
};

// Un-define generic Macros for UART registers
#undef UART_STATUS
#undef UART_CONTROL1
#undef UART_CONTROL2
#undef UART_INTERRUPT
#undef UART_DATA
#undef UART_BAUD_LOW
#undef UART_BAUD_HIGH

// Un-define generic Macros for UART Bits
//#undef UART_STATUS_DOUBLE_SPEED_BIT 	
#undef UART_STATUS_PARITY_ERROR_BIT 	
#undef UART_STATUS_DATA_OVERRUN_BIT 	
#undef UART_STATUS_FRAME_ERROR_BIT  	
#undef UART_STATUS_UDR_EMPTY_BIT 	 	
#undef UART_STATUS_TX_COMPLETE_BIT	 	

#undef UART_CONTROL1_DOUBLE_SPEED_BIT 	
#undef UART_CONTROL1_TX_ENABLE_BIT		
#undef UART_CONTROL1_RX_ENABLE_BIT		
//#undef UART_CONTROL1_UDRIE_ENABLE_BIT	
//#undef UART_CONTROL1_TXCIE_ENABLE_BIT	
//#undef UART_CONTROL1_RXCIE_ENABLE_BIT	

#undef UART_CONTROL2_CHAR_SIZE_BIT0	
#undef UART_CONTROL2_CHAR_SIZE_BIT1	
#undef UART_CONTROL2_STOP_SIZE_BIT	
#undef UART_CONTROL2_PARITY_MODE_BIT0
#undef UART_CONTROL2_PARITY_MODE_BIT1
//#undef UART_CONTROL2_REG_SEL_BIT

#undef UART_INTERRUPT_DREINTLVL_BIT0
#undef UART_INTERRUPT_DREINTLVL_BIT1
#undef UART_INTERRUPT_TXCINTLVL_BIT0
#undef UART_INTERRUPT_TXCINTLVL_BIT1
#undef UART_INTERRUPT_RXCINTLVL_BIT0
#undef UART_INTERRUPT_RXCINTLVL_BIT1

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
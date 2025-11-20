//-------------------Macros to change for UARTx------------------//
// avr_mega0_uart1
// UART1_AVAILABLE
// UART1_ENABLE
// UART1_TX_DISABLE
// UART1_RX_DISABLE
// UART1_RX_BUFFER_CONF
// UART1_RX_BUFFER_DEFAULT
// UART1_TX_INTERRUPT_ENABLE
// UART1_TX_BUFFER_CONF
// UART1_TX_BUFFER_DEFAULT
// avr_mega0_uart1_driver
// MCU_UART1_RX_INTERRUPT_ISR
// MCU_UART1_TX_INTERRUPT_ISR
// USART1_RXC_vect
// USART1_DRE_vect
// UART1_TX_PIN
// UART1_RX_PIN
// UART1_AT_C4_C5_ENABLE

#include "avr_mega0_uart1.h"

#if defined(PLATFORM_SUPPORT_UART) && defined(UART1_AVAILABLE) && (UART1_ENABLE == 1)

#include "Digital.h"

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
	#include <avr/interrupt.h>
#endif

#ifndef UART1_RX_DISABLE
	#if defined(UART1_RX_BUFFER_CONF)
		#define RX_BUFFER_SIZE_TEMP UART1_RX_BUFFER_CONF
	#elif defined(UART1_RX_BUFFER_DEFAULT)
		#define RX_BUFFER_SIZE_TEMP UART1_RX_BUFFER_DEFAULT
	#else
		#define RX_BUFFER_SIZE_TEMP 64
	#endif
#endif

#ifndef UART1_TX_DISABLE
	#ifdef UART1_TX_INTERRUPT_ENABLE
		#if defined(UART1_TX_BUFFER_CONF)
			#define TX_BUFFER_SIZE_TEMP UART1_TX_BUFFER_CONF
		#elif defined(UART1_TX_BUFFER_DEFAULT)
			#define TX_BUFFER_SIZE_TEMP UART1_TX_BUFFER_DEFAULT
		#else
			#define TX_BUFFER_SIZE_TEMP 64
		#endif
	#endif
#endif

#ifdef UART1_TX_DISABLE
	#ifdef UART1_RX_DISABLE
		#error If Serial driver is enabled, both Tx and Rx cannot be disabled
	#endif
#endif

#include "../../../../os/util/ringbuf.h"

// Do changes here
// define generic Macros for UART registers
#define UART_STATUS 			USART1.STATUS
#define UART_ERROR_STATUS		USART1.RXDATAH
#define UART_CONTROL1 			USART1.CTRLA
#define UART_CONTROL2 			USART1.CTRLB
#define UART_CONTROL3 			USART1.CTRLC
#define UART_RX_DATA 			USART1.RXDATAL
#define UART_TX_DATA 			USART1.TXDATAL
#define UART_BAUD				USART1.BAUD
#define UART_PORTMUX			PORTMUX.USARTROUTEA
#define UART_PORTMUX_BIT		2
#define UART_DRIVER				avr_mega0_uart1_driver

#define UART_RX_INTERRUPT_ISR	USART1_RXC_vect
#define UART_TX_INTERRUPT_ISR	USART1_DRE_vect

#define UART_TX_PIN				UART1_TX_PIN
#define UART_RX_PIN				UART1_RX_PIN

// define generic Macros for UART Bits

// Status register bits
#define UART_STATUS_DREIF_EMPTY_BIT 	5
#define UART_STATUS_TXCIF_COMPLETE_BIT	6

// Control1 register bits
#define UART_CONTROL1_DREIE_ENABLE_BIT	5
#define UART_CONTROL1_RXCIE_ENABLE_BIT	7

// Control2 register bits
#define UART_CONTROL2_TX_ENABLE_BIT		6
#define UART_CONTROL2_RX_ENABLE_BIT		7
#define UART_CONTROL2_NORMAL_MODE_SEL	(0x00 << 1)
#define UART_CONTROL2_CLK2X_MODE_SEL	(0x01 << 1)
#define UART_CONTROL2_GENAUTO_MODE_SEL	(0x02 << 1)
#define UART_CONTROL2_LINAUTO_MODE_SEL	(0x03 << 1)

// Control3 register bits
#define UART_CONTROL3_PARITY_NONE_SEL	(0 << 4)
#define UART_CONTROL3_PARITY_EVEN_SEL	(2 << 4)
#define UART_CONTROL3_PARITY_ODD_SEL	(3 << 4)
#define UART_CONTROL3_STOP_ONE_SEL		(0 << 3)
#define UART_CONTROL3_STOP_TWO_SEL		(1 << 3)
#define UART_CONTROL3_CHAR_SIZE5_SEL	0
#define UART_CONTROL3_CHAR_SIZE6_SEL	1
#define UART_CONTROL3_CHAR_SIZE7_SEL	2
#define UART_CONTROL3_CHAR_SIZE8_SEL	3

// Error Status register bits
#define UART_STATUS_PARITY_ERROR_BIT 	1
#define UART_STATUS_FRAME_ERROR_BIT  	2
#define UART_STATUS_DATA_OVERRUN_BIT 	6

// define generic Macros for UART config Macros
#ifdef	UART1_TX_INTERRUPT_ENABLE
	#define UART_TX_INTERRUPT_ENABLE_TEMP
#endif

#ifdef 	UART1_TX_DISABLE
	#define UART_TX_DISABLE_TEMP
#endif

#ifdef 	UART1_RX_DISABLE
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

#ifdef SERIAL_LINE_ENABLE
	static volatile mos_uint8_t terminator_string[10];
	static volatile mos_uint8_t terminator_len=0;
	
	// count of fully terminated strings
	static volatile mos_uint16_t string_len = 0;
	
	// ISR terminator matching index
	static volatile mos_uint8_t isr_match_index = 0;
	
	// read-side terminator matcher
	static mos_uint8_t read_match_index = 0;

	// Flag: set by read() when it finishes exactly one string
	static volatile mos_uint8_t string_completed_in_read = 0;
	
	static void terminator(void * term, mos_uint8_t len);

#endif

static void rxEmpty(void);

static void begin(mos_uint32_t baudrate, mos_uint8_t mode)
{
    float x,X,error1,error2;
	mos_uint16_t y,Y;
	
	// Temperorary variables to hold control2 and control3 register values for final atomic update.
    mos_uint8_t control2_temp=0, control3_temp = 0;
	
	// Do settings to enable Tx
	#ifndef UART_TX_DISABLE_TEMP
		// enable Tx in control2 temp variable
		control2_temp |= (1 << UART_CONTROL2_TX_ENABLE_BIT);
		// If uart Tx is enabled initiate Tx buffer, Tx interrupt is enabled in write function
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP	
			ringbuf_init(&buffer_struct_tx, bufferTx, TX_BUFFER_SIZE_TEMP);
		#endif
	#endif
	
	// Do settings to enable Rx, Rx by default work in interrupt mode
	#ifndef UART_RX_DISABLE_TEMP
		// enable Rx in control2 temp variable
		control2_temp |= (1 << UART_CONTROL2_RX_ENABLE_BIT);
		// Only RX interrupt bit has to be enabled in UART_CONTROL1, hence temp variable is not used
		UART_CONTROL1 = (1 << UART_CONTROL1_RXCIE_ENABLE_BIT);
		// Initiate Rx buffer
		ringbuf_init(&buffer_struct_rx, bufferRx, RX_BUFFER_SIZE_TEMP);		
	#endif	
	
	// do settings for selected databits mode
    switch (mode >> 4)
    {
		case 5:
			control3_temp |= UART_CONTROL3_CHAR_SIZE5_SEL;
			break;
			
		case 6:
			control3_temp |= UART_CONTROL3_CHAR_SIZE6_SEL;
			break;
			
		case 7:
			control3_temp |= UART_CONTROL3_CHAR_SIZE7_SEL;
			break;
		
		case 8:
			control3_temp |= UART_CONTROL3_CHAR_SIZE8_SEL;
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
			control3_temp |= UART_CONTROL3_PARITY_ODD_SEL;
			break;
		case EVEN:
			control3_temp |= UART_CONTROL3_PARITY_EVEN_SEL;
			break;
		case NONE:
			control3_temp |= UART_CONTROL3_PARITY_NONE_SEL;
			break;
    }
	
	// do settings for selected stop bit mode
    if ((mode & 0x3) == 2)
    {
        control3_temp |= UART_CONTROL3_STOP_TWO_SEL;
    }
    else
    {
        control3_temp |= UART_CONTROL3_STOP_ONE_SEL;
    }
	
	// Calculate baudrate error in normal speed mode

	x = ((float)CONTROLLER_FREQ * 64.0f) / (16.0f * (float)baudrate);
	x += 0.5f;
	y = (mos_uint16_t) x;
	error1 = (((x - (float)y) / x ) * 100);
	if (error1 < 0)
	{
		 error1 = (error1 * (-1));
	}
	
	// Calculate baudrate error in double speed mode
	X = ((float)CONTROLLER_FREQ * 64.0f) / (8.0f * (float)baudrate);
	X += 0.5f;
	Y = (mos_uint16_t) X;
	error2 = (((X - (float)Y) / X ) * 100);
	if (error2 < 0)
	{
		 error2 = (error2 * (-1));
	}
	
	// Choose Single speed mode if error is less than 0.5%
	// Otherwise choose mode with lesser baudrate error
	if(error1 < 0.5 || error1 <= error2)
	{
		UART_BAUD = x;
		control2_temp |= UART_CONTROL2_NORMAL_MODE_SEL;
	}
	else
	{
		UART_BAUD = X;
		control2_temp |= UART_CONTROL2_CLK2X_MODE_SEL;
	}
	
	#ifdef SERIAL_LINE_ENABLE
		// set \r\n as ter bytes, also Empty Rx buffer and reset Rx global variables
		terminator("\r\n",2);
	#endif
	
	UART_CONTROL3 = control3_temp;
	UART_CONTROL2 = control2_temp;
	
	#ifdef UART1_AT_C4_C5_ENABLE
		UART_PORTMUX |= (0x01 << UART_PORTMUX_BIT);
	#else
		UART_PORTMUX &= ~(0x03 << UART_PORTMUX_BIT);
	#endif
	
	// Set Tx pin as output after enabling uart, otherwise garbage is printed
	#ifndef UART_TX_DISABLE_TEMP
		// set Tx pin as output
		Digital.pinmode(UART_TX_PIN,OUTPUT);
	#endif
	
	#ifndef UART_RX_DISABLE_TEMP
		// Set Rx pin as input
		Digital.pinmode(UART_RX_PIN,INPUT);
	#endif
}

static void end(void)
{	
	#ifndef UART_RX_DISABLE_TEMP
		Digital.pinmode(UART_RX_PIN,INPUT);
		rxEmpty();
	#endif
	
	#ifndef UART_TX_DISABLE_TEMP
		Digital.pinmode(UART_TX_PIN,INPUT);
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			ringbuf_flush(&buffer_struct_tx);
		#endif
	#endif
	
	UART_CONTROL1 = 0;
	UART_CONTROL2 = 0;
	UART_CONTROL3 = 0;
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

	static void rxEmpty(void)
	{
		return;
	}
	
	#ifdef SERIAL_LINE_ENABLE
		static void terminator(void * term_temp, mos_uint8_t len)
		{
			(void) term_temp;
			(void) len;
			return;
		}
		
		static unsigned int stringAvailable(void)
		{
			return 0;
		}
		
		static unsigned int readString(char * data, int max_len)
		{
			(void) data;
			return 0;
		}
	#endif

#else
	#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
		ISR(UART_RX_INTERRUPT_ISR)	// Uart Rx Interrupt service to automatically read Rx data
	#endif
	{		
		mos_uint8_t status,data;
		
		status = UART_ERROR_STATUS;	// read error status
		data = UART_RX_DATA;		// Read single rx byte
		
		// Check if there is no framer error or parity error or Rx hardware buffer overflow error
		if ((status & ((1<<UART_STATUS_FRAME_ERROR_BIT)|(1<<UART_STATUS_PARITY_ERROR_BIT)|(1<<UART_STATUS_DATA_OVERRUN_BIT))) == 0)
		{
			// write byte in rx buffer
			ringbuf_write(&buffer_struct_rx,data);
			
			#ifdef SERIAL_LINE_ENABLE
				// Check if terminator_len is non-zero, else no point in checking for terminator characters
				if(terminator_len)
				{
					// check if data = terminator[isr_match_index]
					// isr_match_index holds the current terminator array index if previous terminator char was matched.
					if(terminator_string[isr_match_index] == data)
					{
						// increment isr_match_index to match next terminator char
						isr_match_index++;
						
						// if all terminator chars are matched in exact order
						if(isr_match_index == terminator_len)
						{
							// reset isr_match_index
							isr_match_index = 0;
							
							// increment string_len since a valid string terminated by terminator is found
							string_len++;
						}
					}
					// if not (i.e. data != terminator[isr_match_index]), reset isr_match_index to check from first terminator char
					else
					{
						// simple overlap handling: if the current byte equals first terminator byte,
						// set index = 1 (we matched first byte), else reset to 0
						if (data == terminator_string[0])
							isr_match_index = 1;
						else
							isr_match_index = 0;
					}
				}
			#endif
		}
	}
	
	// Function to return number of Rx bytes available in Rx buffer
	static unsigned int available(void)
	{
		return (unsigned int)ringbuf_count(&buffer_struct_rx);
	}
	
	// Function to read a single byte from Rx buffer
	#ifdef SERIAL_LINE_ENABLE
		static mos_uint8_t read(void)
		{			
			// Check if atomic is off
			mos_uint8_t temp;

			temp = IS_ATOMIC_OFF();
			
			// if atomic is off then switch it on
			if(temp)
				ATOMIC_ON();
					
			// check if rx buffer is empty
			if(!ringbuf_count(&buffer_struct_rx))
			{
				// If Rx buffer is empty string_len must be zero, hence setting it zero
				// This step must be done atomic since interrupt can change its value in between
				string_len = 0;
				
				// if atomic was switched on then only switch it off
				if(temp)
					ATOMIC_OFF();
				
				return 0xFF;
			}
			
			// if atomic was switched on then only switch it off
			if(temp)
				ATOMIC_OFF();
				
			// read single byte from Rx buffer
			mos_uint8_t data = ringbuf_read(&buffer_struct_rx);
			
			// Check if terminator_len is zero, else no point in checking for terminator characters
			if(!terminator_len)
				return data;
				
			// check if data = terminator[read_match_index]
			// read_match_index holds the current terminator array index if previous terminator char was matched. 
			if(data == terminator_string[read_match_index])
			{
				// increment read_match_index to match next terminator char
				read_match_index++;
				
				// if all terminator chars are matched in exact order
				if(read_match_index == terminator_len)
				{
					// reset read_match_index
					read_match_index = 0;
					
					// This step is for readString function. 
					// It indicates that string has been read successfully
					// reset string_completed_in_read if it is set by readString function
					if(string_completed_in_read)
						string_completed_in_read = 0;
					
					temp = IS_ATOMIC_OFF();
						
					// if atomic is off then switch it on
					if(temp)
						ATOMIC_ON();
					
					// Check if string_len is non zero, Decrement only if string_len is non zero.
					// Although not mandatory, it is always good to write safe codes for any potential logical code errors
					// This step must be done atomic since interrupt can change its value in between
					if(string_len)
						string_len--;
					
					// if atomic was switched on then only switch it off
					if(temp)
						ATOMIC_OFF();
				}			
			}
			
			// if not (i.e. data != terminator[read_match_index]), reset read_match_index to check from first terminator char
			else
			{
				// simple overlap handling: if the current byte equals first terminator byte,
				// set read_match_index = 1 (we matched first byte), else reset to 0
				if (data == terminator_string[0])
					read_match_index = 1;
				else
					read_match_index = 0;
			}			
			return data;
		}
	#else
		static mos_uint8_t read(void)
		{
			return ringbuf_read(&buffer_struct_rx);
		}
	#endif
	
	//Function to delete all bytes in Rx buffer
	#ifdef SERIAL_LINE_ENABLE
	
		static void rxEmpty(void)
		{
			mos_uint8_t temp;
			
			// Check if atomic is off
			temp = IS_ATOMIC_OFF();
			
			// if atomic is off then switch it on
			if(temp)
				ATOMIC_ON();
			
			// reset Uart Rx specific global variables
			string_len = 0;
			isr_match_index = 0;
			read_match_index = 0;

			// Empty (delete all bytes in) Rx buffer
			ringbuf_flush(&buffer_struct_rx);
			
			// if atomic was switched on then only switch it off
			if(temp)
				ATOMIC_OFF();
		}
		
	#else
		static void rxEmpty(void)
		{
			ringbuf_flush(&buffer_struct_rx);
		}
	#endif
	
	#ifdef SERIAL_LINE_ENABLE
	
		// set terminator bytes
		static void terminator(void * term_temp, mos_uint8_t len)
		{			
			mos_uint8_t * term = (mos_uint8_t *) term_temp;
			// Set terminator_len
			terminator_len = len;
			
			// make sure terminator length is <= 10
			// Setting it zero effectively, remove readString functionality
			if( len <= 10)
			{
				// run while loop len times
				while(len)
				{
					// save terminator bytes in terminator variable
					// bytes are saved in reverse order for easy coding
					terminator_string[len-1] = term[len-1];
					
					// decrement len, effectively saving len bytes in terminator variable
					len--;
				}
			}
			
			// Flushing discards all previous data in Rx buffer
			// Since old strings are no more terminated by new terminator bytes hence invalid
			rxEmpty();
		}
		
		// returns number of valid strings available in Rx buffer which are terminated by terminator
		static unsigned int stringAvailable(void)
		{
			unsigned int temp;
			mos_uint8_t atomic_state;
			
			atomic_state = IS_ATOMIC_OFF();
			if(atomic_state)
				ATOMIC_ON();
			
			temp = string_len;
			
			if(atomic_state)
				ATOMIC_OFF();
			
			return temp;			
		}
		
		// Function to read one string from Rx Buffer
		static unsigned int readString(char * data, int max_len)
		{
			// Check if string_len is non zero i.e. Rx buffer contains valid strings
			if(string_len)
			{
				unsigned int count=0;
				
				// this is global variable, which is reset by read function after a valid string is read from Rx Buffer
				string_completed_in_read = 1;
				
				// keep storing bytes till valid string is read or Rx buffer is empty
				while(string_completed_in_read && ringbuf_count(&buffer_struct_rx))
				{
					// store bytes from rx buffer into data variable up to max_len-1
					// one space in data variable is left for Null character at the end
					if(count < max_len-1)
					{
						*data = read();
						data++;
					}
					// if data variable is full, simply read from rx buffer to discard bytes
					else
						read();
					
					count++;
				}
				
				// Add Null character in the end
				*data ='\0';
				
				// Reset string_completed_in_read variable in case Rx buffer is empty before valid string is read
				string_completed_in_read = 0;
				
				// return number of bytes in the read string
				return count;
			}
			return 0;
		}
	#endif

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
			// Read data byte from Tx buffer and write to transmit buffer register
			UART_TX_DATA = ringbuf_read(&buffer_struct_tx);
			
			// if Tx buffer is empty
			if(ringbuf_count(&buffer_struct_tx)==0)
			{
				// Disable DRE interrupt (no more data)
				UART_CONTROL1 &= ~(1<<UART_CONTROL1_DREIE_ENABLE_BIT);
				
				// Clear TXCIF now so that txComplete() will work correctly
				// TXCIF is cleared by writing 1 to it
				UART_STATUS |= (1 << UART_STATUS_TXCIF_COMPLETE_BIT);
			}
		}
	#endif
	
	// Function to transmit single byte
	static void write(mos_uint8_t data)
	{		
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			mos_uint8_t atomic_status = 0;
			
			// writing directly to transmit buffer or into Tx buffer must be atomic
			atomic_status = IS_ATOMIC_OFF();
			if(atomic_status)
				ATOMIC_ON();
			
			// If Tx buffer is empty as well as Transmit buffer register is also empty
			// Then directly write data byte to Transmit buffer register instead of Tx buffer
			if( ( ringbuf_count(&buffer_struct_tx) == 0 ) && (( UART_STATUS & (1 << UART_STATUS_DREIF_EMPTY_BIT))) )
			{
					// Clear TXCIF before sending first byte
					UART_STATUS |= (1 << UART_STATUS_TXCIF_COMPLETE_BIT);
					// Write byte to transmit buffer register
					UART_TX_DATA = data;
			}
			// Other wise if Tx buffer is not full then
			else if( !ringbuf_isfull(&buffer_struct_tx) )
			{
				// Write byte to Tx buffer
				ringbuf_write(&buffer_struct_tx,data);
				// Enable interrupt on transmit buffer register empty
				UART_CONTROL1 |= (1<<UART_CONTROL1_DREIE_ENABLE_BIT);
			}
			
			if(atomic_status)
				ATOMIC_OFF();
		#else
			// Wait untill transmit buffer register is not empty
			while (!(UART_STATUS & (1 << UART_STATUS_DREIF_EMPTY_BIT)))
				;
			
			// Clear TXCIF before sending
			UART_STATUS |= (1 << UART_STATUS_TXCIF_COMPLETE_BIT);
			
			// Write byte to transmit buffer register
			UART_TX_DATA = data;
		#endif
	}
	
	// Function to print to Uart Tx
	static void print(char *data)
	{
		// return if null pointer is passed
		if(!data)
			return;
		
		// Write till null character is found in data string
		while (*data)
		{
			write((mos_uint8_t)(*data));
			data++;
		}
	}

	static void println(char *data)
	{
		// return if null pointer is passed
		if(!data)
			return;
		
		// Print data to Uart Tx, append carriage return and new line char in the end
		print(data);
		write('\r');
		write('\n');
	}
	
	#ifdef PLATFORM_SUPPORT_CONST_PRINT
	
		// Function similar to print function, print data string saved in flash
		static void constPrintArch(const char *data)
		{	
			while (pgm_read_byte(data))
			{
				write(pgm_read_byte(data));
				data++;
			}
		}
		
		// Function similar to println function, print data string saved in flash
		static void constPrintlnArch(const char *data)
		{			
			constPrintArch(data);
			write('\r');
			write('\n');
		}
	
	#endif
	
	// Function to print bytes (not characters)
	static void printBytes(mos_uint8_t * data, mos_uint16_t len)
	{	
		// Print data to Uart Tx of length len
		while (len)
		{
			write(*data);
			data++;
			len--;
		}
	}
	
	// Function to check if complete data is transmitted by the UART
	static mos_uint8_t txComplete(void)
	{
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			// If Tx buffer is not empty, means data is not fully transmitted yet
			if(ringbuf_count(&buffer_struct_tx))
				return 0;
		#endif
		
		// If TX complete bit status is 1 then full data is transmitted else not
		if(UART_STATUS & (1 << UART_STATUS_TXCIF_COMPLETE_BIT))
			return 1;
		else
			return 0;
	}
#endif

const struct serial_driver UART_DRIVER = {
	begin,
	end,
	available,
	read,
	rxEmpty,
	write,
	print,
	println,
	#ifdef PLATFORM_SUPPORT_CONST_PRINT
		constPrintArch,
		constPrintlnArch,
	#endif
	printBytes,
	txComplete,
	#ifdef SERIAL_LINE_ENABLE
		terminator,
		stringAvailable,
		readString,
	#endif
};

// Un-define generic Macros for UART registers

#undef UART_STATUS
#undef UART_ERROR_STATUS
#undef UART_CONTROL1
#undef UART_CONTROL2
#undef UART_CONTROL3
#undef UART_RX_DATA
#undef UART_TX_DATA
#undef UART_BAUD
#undef UART_PORTMUX
#undef UART_DRIVER

#undef UART_RX_INTERRUPT_ISR
#undef UART_TX_INTERRUPT_ISR

// define generic Macros for UART Bits

// Status register bits
#undef UART_STATUS_DREIF_EMPTY_BIT
#undef UART_STATUS_TXCIF_COMPLETE_BIT

// Control1 register bits
#undef UART_CONTROL1_DREIE_ENABLE_BIT
#undef UART_CONTROL1_RXCIE_ENABLE_BIT

// Control2 register bits
#undef UART_CONTROL2_TX_ENABLE_BIT
#undef UART_CONTROL2_RX_ENABLE_BIT
#undef UART_CONTROL2_NORMAL_MODE_SEL
#undef UART_CONTROL2_CLK2X_MODE_SEL
#undef UART_CONTROL2_GENAUTO_MODE_SEL
#undef UART_CONTROL2_LINAUTO_MODE_SEL

// Control3 register bits
#undef UART_CONTROL3_PARITY_NONE_SEL
#undef UART_CONTROL3_PARITY_EVEN_SEL
#undef UART_CONTROL3_PARITY_ODD_SEL
#undef UART_CONTROL3_STOP_ONE_SEL
#undef UART_CONTROL3_STOP_TWO_SEL
#undef UART_CONTROL3_CHAR_SIZE5_SEL
#undef UART_CONTROL3_CHAR_SIZE6_SEL
#undef UART_CONTROL3_CHAR_SIZE7_SEL
#undef UART_CONTROL3_CHAR_SIZE8_SEL

// Error Status register bits
#undef UART_STATUS_PARITY_ERROR_BIT
#undef UART_STATUS_FRAME_ERROR_BIT
#undef UART_STATUS_DATA_OVERRUN_BIT

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
//-------------------Macros to change for UARTx------------------//
// avr_mega_uart
// UART_AVAILABLE
// UART_ENABLE
// UART_TX_DISABLE
// UART_RX_DISABLE
// UART_RX_BUFFER_CONF
// UART_RX_BUFFER_DEFAULT
// UART_TX_INTERRUPT_ENABLE
// UART_TX_BUFFER_CONF
// UART_TX_BUFFER_DEFAULT
// avr_mega_uart_driver
// MCU_UART_RX_INTERRUPT_ISR
// MCU_UART_TX_INTERRUPT_ISR
// USART_RX_vect
// USART_UDRE_vect

#include "avr_mega_uart.h"

#if defined(PLATFORM_SUPPORT_UART) && defined(UART_AVAILABLE) && (UART_ENABLE == 1)

#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
	#include <avr/interrupt.h>
#endif


#ifndef UART_RX_DISABLE
	#if defined(UART_RX_BUFFER_CONF)
		#define RX_BUFFER_SIZE_TEMP UART_RX_BUFFER_CONF
	#elif defined(UART_RX_BUFFER_DEFAULT)
		#define RX_BUFFER_SIZE_TEMP UART_RX_BUFFER_DEFAULT
	#else
		#define RX_BUFFER_SIZE_TEMP 64
	#endif
#endif

#ifndef UART_TX_DISABLE
	#ifdef UART_TX_INTERRUPT_ENABLE
		#if defined(UART_TX_BUFFER_CONF)
			#define TX_BUFFER_SIZE_TEMP UART_TX_BUFFER_CONF
		#elif defined(UART_TX_BUFFER_DEFAULT)
			#define TX_BUFFER_SIZE_TEMP UART_TX_BUFFER_DEFAULT
		#else
			#define TX_BUFFER_SIZE_TEMP 64
		#endif
	#endif
#endif

#ifdef UART_TX_DISABLE
	#ifdef UART_RX_DISABLE
		#error If Serial driver is enabled, both Tx and Rx cannot be disabled
	#endif
#endif

#include "../../../../os/util/ringbuf.h"

// Do changes here
// define generic Macros for UART registers
#define UART_STATUS 		UCSRA
#define UART_CONTROL1 		UCSRB
#define UART_CONTROL2 		UCSRC
#define UART_DATA 			UDR
#define UART_BAUD_LOW 		UBRRL
#define UART_BAUD_HIGH 		UBRRH
#define UART_DRIVER			avr_mega_uart_driver

// Unfortunately Interrupt routines are not same for all controllers in AVR
// Hence These ISR routines must be imported from controller specific macros 
// if it is different from standard ISRs
#ifdef MCU_UART_RX_INTERRUPT_ISR
	#define UART_RX_INTERRUPT_ISR	MCU_UART_RX_INTERRUPT_ISR
#else
	#define UART_RX_INTERRUPT_ISR	USART_RXC_vect
#endif

#ifdef MCU_UART_RX_INTERRUPT_ISR
	#define UART_TX_INTERRUPT_ISR	MCU_UART_TX_INTERRUPT_ISR
#else
	#define UART_TX_INTERRUPT_ISR	USART_UDRE_vect
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

#ifdef	UART_TX_INTERRUPT_ENABLE
	#define UART_TX_INTERRUPT_ENABLE_TEMP
#endif

#ifdef 	UART_TX_DISABLE
	#define UART_TX_DISABLE_TEMP
#endif

#ifdef 	UART_RX_DISABLE
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
	
	static void terminator(mos_uint8_t * term, mos_uint8_t len);
#endif

static void flush(void);

static void begin(mos_uint32_t baudrate, mos_uint8_t mode)
{
    float x,X,error1,error2;
	mos_uint32_t y,Y;
	
	// Temperorary variables to hold control2 and control3 register values for final atomic update.
    mos_uint8_t uart_control1_temp = 0, uart_control2_temp = 0;
	
	// Do settings to enable Tx
	#ifndef UART_TX_DISABLE_TEMP
		// enable Tx in control1 temp variable
		uart_control1_temp |= (1 << UART_CONTROL1_TX_ENABLE_BIT);
		// If uart Tx is enabled initiate Tx buffer, Tx interrupt is enabled in write function
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP	
			ringbuf_init(&buffer_struct_tx, bufferTx, TX_BUFFER_SIZE_TEMP);
		#endif
	#endif
	
	// Do settings to enable Rx, Rx by default work in interrupt mode
	#ifndef UART_RX_DISABLE_TEMP
		// Enable Rx and RXC interrupt bit
		uart_control1_temp |= (1 << UART_CONTROL1_RX_ENABLE_BIT) | (1 << UART_CONTROL1_RXCIE_ENABLE_BIT);
		// Initiate Rx buffer
		ringbuf_init(&buffer_struct_rx, bufferRx, RX_BUFFER_SIZE_TEMP);
	#endif
	
    uart_control2_temp = (1 << UART_CONTROL2_REG_SEL_BIT);
	
	// do settings for selected databits mode
    switch (mode >> 4)
    {
		case 5:
			uart_control2_temp |= (0 << UART_CONTROL2_CHAR_SIZE_BIT0) | (0 << UART_CONTROL2_CHAR_SIZE_BIT1);
			break;
			
		case 6:
			uart_control2_temp |= (1 << UART_CONTROL2_CHAR_SIZE_BIT0);
			break;
			
		case 7:
			uart_control2_temp |= (1 << UART_CONTROL2_CHAR_SIZE_BIT1);
			break;
		
		case 8:
			uart_control2_temp |= (1 << UART_CONTROL2_CHAR_SIZE_BIT0) | (1 << UART_CONTROL2_CHAR_SIZE_BIT1);
			break;
			
		// Currently 9 data bit mode is not supported
		/*	
		case 9:
		{
			uart_control2_temp |= (1 << UCSZ0) | (1 << UCSZ1);
			UART_CONTROL1 |= (1 << UCSZ2);
			break;
		}
		*/
    }

	// do settings for selected parity bit mode
    switch ((mode>>2) & 0x3)
    {
		case ODD:
			uart_control2_temp |= (1 << UART_CONTROL2_PARITY_MODE_BIT1) | (1 << UART_CONTROL2_PARITY_MODE_BIT0);
			break;
		case EVEN:
			uart_control2_temp |= (1 << UART_CONTROL2_PARITY_MODE_BIT1) | (0 << UART_CONTROL2_PARITY_MODE_BIT0);
			break;
		case NONE:
			uart_control2_temp |= (0 << UART_CONTROL2_PARITY_MODE_BIT1) | (0 << UART_CONTROL2_PARITY_MODE_BIT0);
			break;
    }
	
	// do settings for selected stop bit mode
    if ((mode & 0x3) == 2)
    {
        uart_control2_temp |= (1 << UART_CONTROL2_STOP_SIZE_BIT);
    }
    else
    {
        uart_control2_temp |= (0 << UART_CONTROL2_STOP_SIZE_BIT);
    }
	
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
	
	#ifdef SERIAL_LINE_ENABLE
		// set \r\n as ter bytes, also flush Rx buffer and reset Rx global variables
		terminator((mos_uint8_t *)"\r\n",2);
	#endif
	
	UART_CONTROL2 = uart_control2_temp;
	UART_CONTROL1 = uart_control1_temp;
}

static void end(void)
{
	UART_CONTROL1 = 0;
	UART_CONTROL2 = 0;
	UART_STATUS = 0;
	flush();
	#ifndef UART_TX_DISABLE_TEMP
		#ifdef UART_TX_INTERRUPT_ENABLE_TEMP
			ringbuf_flush(&buffer_struct_tx);
		#endif
	#endif
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
	
	#ifdef SERIAL_LINE_ENABLE
		static void terminator(unit8_t * term, mos_uint8_t len)
		{
			(void) term;
			(void) len;
			return;
		}
		
		static unsigned int stringAvailable(void)
		{
			return 0;
		}
		
		static unsigned int readString(char * data)
		{
			(void) data;
			return 0;
		}
	#endif

#else
	#if ( (COMPILER == AVR_STUDIO) || ( COMPILER == WIN_AVR ) || ( COMPILER == AVR_GCC ))
		ISR(UART_RX_INTERRUPT_ISR)
	#endif
	{
		mos_uint8_t status,data;
		
		status = UART_STATUS;
		data = UART_DATA;
		
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
		static void flush(void)
		{
			mos_uint8_t temp;
			
			// Check if atomic is off
			temp = IS_ATOMIC_OFF();
			
			// if atomic is off then switch it on
			if(temp)
				ATOMIC_ON();
			
			// reset Uart Rx specific global variables
			string_len = 0;
			read_match_index = 0;
			isr_match_index = 0;
			
			// flush (delete all bytes in) Rx buffer
			ringbuf_flush(&buffer_struct_rx);
			
			// if atomic was switched on then only switch it off
			if(temp)
				ATOMIC_OFF();
		}
	#else
		static void flush(void)
		{
			ringbuf_flush(&buffer_struct_rx);
		}
	#endif
	
	#ifdef SERIAL_LINE_ENABLE
	
		// set terminator bytes
		static void terminator(mos_uint8_t * term, mos_uint8_t len)
		{			
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
			flush();
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
		static unsigned int readString(char * data)
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
					*data = read();
					data++;
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
			// Read data byte from Tx buffer and write to uart data buffer register
			UART_DATA = ringbuf_read(&buffer_struct_tx);
			
			// if Tx buffer is empty
			if(ringbuf_count(&buffer_struct_tx)==0)
			{				
				// Disable DRE interrupt (no more data)
				UART_CONTROL1 &= ~(1<<UART_CONTROL1_UDRIE_ENABLE_BIT);
				
				// Clear TXCIF now so that txComplete() will work correctly
				// TXCIF is cleared by writing 1 to it
				UART_STATUS |= (1 << UART_STATUS_TX_COMPLETE_BIT);
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
			if( ( ringbuf_count(&buffer_struct_tx) == 0 ) && ( UART_STATUS & (1 << UART_STATUS_UDR_EMPTY_BIT) ) )
			{
				// Clear TXCIF before sending first byte
				UART_STATUS |= (1 << UART_STATUS_TX_COMPLETE_BIT);
				// Write byte to transmit buffer register
				UART_DATA = data;
			}
			// Other wise if Tx buffer is not full then
			else if( ringbuf_isfull(&buffer_struct_tx) == 0)
			{
				// Write byte to Tx buffer
				ringbuf_write(&buffer_struct_tx,data);
				// Enable interrupt on transmit buffer register empty
				UART_CONTROL1 |= (1<<UART_CONTROL1_UDRIE_ENABLE_BIT);
			}
			
			if(atomic_status)
				ATOMIC_OFF();
		#else
			// Wait untill transmit buffer register is not empty
			while (!(UART_STATUS & (1 << UART_STATUS_UDR_EMPTY_BIT)))
				;
			// Clear TX complete bit before sending
			UART_STATUS |= (1 << UART_STATUS_TX_COMPLETE_BIT);
			// Write byte to transmit buffer register
			UART_DATA = data;
		#endif
	}
	
	// Function to print to Uart Tx
	static void print(char *data)
	{			
		// return if null pointer is passed
		if (!data) return;
		
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
		if (!data) return;
	
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
		if(UART_STATUS & (1 << UART_STATUS_TX_COMPLETE_BIT))
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
	#ifdef SERIAL_LINE_ENABLE
		terminator,
		stringAvailable,
		readString,
	#endif
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
#undef UART_CONTROL1_TXCIE_ENABLE_BIT
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
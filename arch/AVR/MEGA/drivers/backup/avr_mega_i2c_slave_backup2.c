#include "avr_mega_i2c_slave.h"

// Macro PLATFORM_SUPPORT_I2C checks if chosen Architechture supports I2c or not
// Macro I2C_AVAILABLE checks if chosen controller has I2c support or not
// Macro I2C_SLAVE_ENABLE has to be defined in config file to use I2cSlave driver
// By adding this macro, in case code do not need I2C support, this source file is not compiled
// thereby reduce code space. Please note since I2cSlave driver is based on interrupts
// while linking this file, it is always included. Hence by using I2C_SLAVE_ENABLE macro we save memory
#if defined(PLATFORM_SUPPORT_I2C) && defined(I2C_AVAILABLE) && defined(I2C_SLAVE_ENABLE)

#warning this is running
// Includes TWI macros
#include <util/twi.h>
// Includes interrupts support
#include <avr/interrupt.h>
// Includes Ringbuf utility
#include "util/ringbuf.h"

// I2c Slave RX buffer can be configured by two Macros : I2C_SLAVE_RX_BUFFER_CONF or I2C_SLAVE_RX_BUFFER_DEFAULT
// Macro I2C_SLAVE_RX_BUFFER_CONF is optionally defined by user/coder in config file and it has higher priority
// Macro I2C_SLAVE_RX_BUFFER_DEFAULT is defined in controller_macros.h file of selected controller and it has lower priority
// If neither of the macros is defined default value chosen is 64 bytes
#if defined(I2C_SLAVE_RX_BUFFER_CONF)
	#define I2C_SLAVE_RX_BUFFER_SIZE I2C_SLAVE_RX_BUFFER_CONF
#elif defined(I2C_SLAVE_RX_BUFFER_DEFAULT)
	#define I2C_SLAVE_RX_BUFFER_SIZE I2C_SLAVE_RX_BUFFER_DEFAULT
#else
	#define I2C_SLAVE_RX_BUFFER_SIZE 64
#endif

// I2c Slave TX buffer can be configured by two Macros : I2C_SLAVE_TX_BUFFER_CONF or I2C_SLAVE_TX_BUFFER_DEFAULT
// Macro I2C_SLAVE_TX_BUFFER_CONF is optionally defined by user/coder in config file and it has higher priority
// Macro I2C_SLAVE_TX_BUFFER_DEFAULT is defined in controller_macros.h file of selected controller and it has lower priority
// If neither of the macros is defined default value chosen is 64 bytes
#if defined(I2C_SLAVE_TX_BUFFER_CONF)
	#define I2C_SLAVE_TX_BUFFER_SIZE I2C_SLAVE_TX_BUFFER_CONF
#elif defined(I2C_SLAVE_TX_BUFFER_DEFAULT)
	#define I2C_SLAVE_TX_BUFFER_SIZE I2C_SLAVE_TX_BUFFER_DEFAULT
#else
	#define I2C_SLAVE_TX_BUFFER_SIZE 64
#endif

// define generic Macros for I2C registers
#define I2C_STATUS 			TWSR
#define I2C_CONTROL 		TWCR
#define I2C_SLAVE_ADDRESS 	TWAR
#define I2C_DATA 			TWDR

// define generic Macros for I2C Slave config Macros
#define RX_BUFFER_SIZE_TEMP	I2C_SLAVE_RX_BUFFER_SIZE
#define TX_BUFFER_SIZE_TEMP I2C_SLAVE_TX_BUFFER_SIZE

// create varaibles for RX and TX buffers
static mos_uint8_t bufferRx[RX_BUFFER_SIZE_TEMP];
static mos_uint8_t bufferTx[TX_BUFFER_SIZE_TEMP];
static ringbuf_struct_t buffer_struct_rx;
static ringbuf_struct_t buffer_struct_tx;
static mos_uint8_t rx_complete_status = 0,tx_complete_status = 0;

// Create callback functions and required variables
static void (*rxCallback)(void);
static void (*txCallback)(void);
static mos_uint8_t enable_rx_callback = 0;
static mos_uint8_t enable_tx_callback = 0;


// Function to begin I2C slave
static void begin (mos_uint8_t slave_address)
{ 
  // set rx tx complete status to 0
  rx_complete_status = 0;
  tx_complete_status = 0;
  
  ringbuf_init(&buffer_struct_rx, bufferRx, RX_BUFFER_SIZE_TEMP);
  ringbuf_init(&buffer_struct_tx, bufferTx, TX_BUFFER_SIZE_TEMP);
  
  // load address into TWI address register
  I2C_SLAVE_ADDRESS = slave_address << 1;
  
  // set the I2C_CONTROL to enable address matching and enable TWI, clear TWINT by writing 1 and enable TWI interrupt
  I2C_CONTROL = (1<<TWIE) | (1<<TWEA) | (1<<TWINT) | (1<<TWEN);
}

// Function to end I2C slave
static void end (void)
{ 
  // clear acknowledge and enable bits
  I2C_CONTROL = 0;
  I2C_SLAVE_ADDRESS = 0;
}

// I2C Slave interrupt function
ISR(TWI_vect)
{
	mos_uint8_t i2c_stat, clear_acknowledge = 0;
  
	switch(i2c_stat = I2C_STATUS & 0xF8)
	{
		// Slave Receiver mode : Slave own address has been acknowledged by Slave
		case TW_SR_SLA_ACK:
		
			// prepare to receive data bytes
			// Set rx_complete_status to 0 indicating data reception is not complete
			rx_complete_status = 0;
			
			// Flush RX buffer
			ringbuf_flush(&buffer_struct_rx);
			break;
			
		// Slave Receiver mode : data received from master, acknowledgement sent by Slave
		case TW_SR_DATA_ACK:
			
			// Save data in buffer
			ringbuf_write(&buffer_struct_rx, I2C_DATA);
			
			// If buffer is about to get full in next data reception
			if(ringbuf_count(&buffer_struct_rx)>= RX_BUFFER_SIZE_TEMP-1)
				clear_acknowledge = 1;
			
			break;
			
		// Slave Receiver mode : data received from master, while No acknowledgement was sent by Slave
		case TW_SR_DATA_NACK:
			
			// Save data in buffer
			ringbuf_write(&buffer_struct_rx, I2C_DATA);
			
			// This status happens only when buffer is full and slave can read no more data.
			// Hence we set the rx_complete_status to 1, to indicate Data Reception is complete
			rx_complete_status = 1;
			
			// Received data can now be read by read function
			
			// If enabled, we run rxCallback function so that user can read data from Rx buffer
			if(enable_rx_callback == 1)
				rxCallback();
			
			// It is essential to again enable acknowledge by writing 1 to TWEA bit
			// Otherwise slave will not acknowledge to master and this will create error in I2C communication
			
			break;
		
		// Stop or Repeated start condition is received
		case TW_SR_STOP:
			
			// This status indicates end of data reception by slave
			// Hence we set the rx_complete_status to 1, to indicate Data Reception is complete
			rx_complete_status = 1;
			
			// If enabled, we run rxCallback function so that user can read data from Rx buffer
			if(enable_rx_callback == 1)
				rxCallback();
			
			break;
			
		// Slave Transmitter mode : Slave own address has been acknowledged	by Slave
		case TW_ST_SLA_ACK:
			
			// prepare to transmit data bytes
			// Set tx_complete_status to 0 indicating data transmission is not complete
			tx_complete_status = 0;
			
			// Data must have been written to TX buffer before master requests for data
			// This can be done using print function before I2C communication starts
			// Or Calling print funtion inside txCallback function.
			// If no data is written in TX Buffer then by default slave sends 0xFF data
			
			// if enabled, we run txCallback function so that user can write data to the Tx buffer
			if(enable_tx_callback == 1)
					txCallback();
			
			// Write one byte data from TX buffer to I2C data register
			I2C_DATA = ringbuf_read(&buffer_struct_tx);
			break;
			
		// Slave Transmitter mode : Master has receieved data, master sent acknowledge to slave
		case TW_ST_DATA_ACK:
				
			// Write next one byte data from TX buffer to I2C data register
			I2C_DATA = ringbuf_read(&buffer_struct_tx);
			break;
			
		// Slave Transmitter mode : Master has receieved data, master sent no acknowledge to slave
		case TW_ST_DATA_NACK:
			
			// This status indicates end of data transmission to master by the slave
			// Hence we set the tx_complete_status to 1, to indicate Data Transmission is complete
			tx_complete_status = 1;
			break;
			
		// Bus error
		case TW_BUS_ERROR:
			// some sort of erroneous state, prepare TWI to be readdressed
			// Set I2C Control Register to 0, this resets I2C
			I2C_CONTROL = 0;
			break;
	}
 
 
	switch(clear_acknowledge)
	{
		case 0:
			// clear TWI interrupt flag TWINT bit by writing 1
			// keep TWI interupt enabled by writing 1 to TWIE bit
			// Keep TWEA bit high to send acknowledge to master in next operation
			// Keept TWEN bit 1 to continue with I2C mode
			I2C_CONTROL = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
			break;
			
		case 1:
			// clear TWI interrupt flag TWINT bit by writing 1
			// keep TWI interupt enabled by writing 1 to TWIE bit
			// Keept TWEN bit 1 to continue with I2C mode
			// clear TWEA bit to not send acknowledge in next operation
			// to indicate master not to send any more data
			I2C_CONTROL = (1<<TWIE) | (1<<TWINT) | (0<<TWEA) | (1<<TWEN);
			break;
	}
} 

// Function to check if slave data reception is complete
static mos_uint8_t rxComplete (void)
{
	// if rx_complete_status is 1, means data reception is complete
	return rx_complete_status;
}

// Function to read data from Rx data buffer
// This function must be called after checking rxComplete function
static void read (void * data_rx_void, mos_uint16_t * data_rx_len)
{
	mos_uint8_t * data_rx;
	mos_uint8_t atomic_state;
	
	// This type casting is done so that input data to read function can be either char or mos_uint8_t
	data_rx = (mos_uint8_t *) data_rx_void;
	
	if(rx_complete_status)
	{
		atomic_state = IS_ATOMIC_OFF();
		 
		if(atomic_state)
			ATOMIC_ON();
		
		// read length of data received in Rx data buffer
		*data_rx_len = ringbuf_count(&buffer_struct_rx);
		
		// Read data from Rx Data buffer
		while(ringbuf_count(&buffer_struct_rx))
		{
			*data_rx = ringbuf_read(&buffer_struct_rx);
			data_rx++;
		}
		
		// set rx_complete_status to 0, so that rxComplete function return 0
		rx_complete_status = 0;
		
		if(atomic_state)
			ATOMIC_OFF();
	}
	else
	{		
		// since data reception is not complete set data read length to 0
		*data_rx_len = 0;
	}		

}

// Function to check if slave data transmission is complete
// This function must be called after running print function
static mos_uint8_t txComplete (mos_uint16_t * data_tx_len)
{
	// read length of data transmitted to master from Tx data buffer
	*data_tx_len = ringbuf_count(&buffer_struct_tx);
	
	// if tx_complete_status is 1, means data transmission is complete
	return tx_complete_status;	
}

// Function to write data to Tx data buffer
static mos_uint8_t print ( void * data_tx_void, mos_uint16_t data_tx_len)
{
	mos_uint8_t * data_tx;
	
	// This type casting is done so that output data to print function can be either char or mos_uint8_t
	data_tx = (mos_uint8_t *) data_tx_void;
	
	// If data to be transmitted is less than or equal Tx buffer size
	if(data_tx_len <= TX_BUFFER_SIZE_TEMP)
	{
		mos_uint8_t atomic_state;
		
		atomic_state = IS_ATOMIC_OFF();
		
		if(atomic_state)
			ATOMIC_ON();
		
		// Delete previous data
		// Note this step is important because ringbuf functions do no delete previous data
		ringbuf_flush(&buffer_struct_tx);
		
		// Copy transmit data to TX buffer
		while(data_tx_len)
		{
			ringbuf_write(&buffer_struct_tx,*data_tx);
			data_tx++;
			data_tx_len--;
		}
		
		// set tx_complete_status to 0, so that txComplete function return 0
		tx_complete_status = 0;
		
		if(atomic_state)
			ATOMIC_OFF();
		
		// return success indicating that complete data could be transmitted
		return 1;
	}
	else
	{
		// return failure indicating that complete data could not be transmitted
		return 0;
	}
}

// Function to automatically run a callback after completion of data reception by slave
static void attachRxCallback(void (*rx_callback_function)(void))
{
	// attach user function to Rx callback function
	rxCallback = rx_callback_function;
	// Enable auto execution of Rx callback at the end of data reception
	enable_rx_callback = 1;
}

// Function to automatically run a callback after completion of data transmission by slave
static void attachTxCallback(void (*tx_callback_function)(void))
{
	// attach user function to Tx callback function
	txCallback = tx_callback_function;
	// Enable auto execution of Tx callback before the start of data transmission
	enable_tx_callback = 1;
}

// Function to stop Rx callback functionality
static void detachRxCallback(void)
{
	// detach user function from Rx callback function
	rxCallback = NULL;
	// Disable auto execution of Rx callback at the end of data reception
	enable_rx_callback = 0;
}

// Function to stop Tx callback functionality
static void detachTxCallback(void)
{
	// detach user function from Tx callback function
	txCallback = NULL;
	// Disable auto execution of Tx callback before the start of data transmission
	enable_tx_callback = 0;
}

// Attaching all functions to avr mega slave driver
const struct i2c_slave_driver avr_mega_i2c_slave_driver = {
	begin,
	end,
	rxComplete,
	read,
	txComplete,
	print,
	attachRxCallback,
	attachTxCallback,
	detachRxCallback,
	detachTxCallback,
};

// undefine generic Macros for I2C registers
#undef I2C_STATUS
#undef I2C_CONTROL
#undef I2C_SLAVE_ADDRESS
#undef I2C_DATA

// define generic Macros for I2C Slave config Macros
#undef RX_BUFFER_SIZE_TEMP
#undef TX_BUFFER_SIZE_TEMP

#endif
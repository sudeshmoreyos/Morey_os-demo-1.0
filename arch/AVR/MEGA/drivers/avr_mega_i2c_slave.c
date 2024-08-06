#include "avr_mega_i2c_slave.h"

// Macro PLATFORM_SUPPORT_I2C checks if chosen Architechture supports I2c or not
// Macro I2C_AVAILABLE checks if chosen controller has I2c support or not
// Macro I2C_SLAVE_ENABLE has to be defined in config file to use I2cSlave driver
// By adding this macro, in case code do not need I2C support, this source file is not compiled
// thereby reduce code space. Please note since I2cSlave driver is based on interrupts
// while linking this file, it is always included. Hence by using I2C_SLAVE_ENABLE macro we save memory
#if defined(PLATFORM_SUPPORT_I2C) && defined(I2C_AVAILABLE) && defined(I2C_SLAVE_ENABLE)

// Includes TWI macros
#include <util/twi.h>
// Includes interrupts support
#include <avr/interrupt.h>

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
static mos_uint16_t bufferRxIndex = 0, bufferTxIndex = 0;
static mos_uint16_t bufferTxDataLen = 0;
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
  mos_uint8_t i2c_stat;
  
  switch(i2c_stat = I2C_STATUS & 0xF8)
  {
	// Slave Receiver mode : Slave own address has been acknowledged by Slave
	case TW_SR_SLA_ACK:
	
		// prepare to receive data bytes
		bufferRxIndex = 0;		
		rx_complete_status = 1;
		
		// clear TWI interrupt flag TWINT bit by writing 1
		// keep TWI interupt enabled by writing 1 to TWIE bit
		// Keep TWEA bit high to send acknowledge to master in next operation
		// Keept TWEN bit 1 to continue with I2C mode
		I2C_CONTROL = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
		
	// Slave Receiver mode : data received from master, acknowledgement sent by Slave
    case TW_SR_DATA_ACK:
		
		// Save data in buffer if buffer is not full
		if(bufferRxIndex < RX_BUFFER_SIZE_TEMP)
		{
			// save received byte in Rx buffer
			bufferRx[bufferRxIndex] = I2C_DATA;
			
			// increment Rx bufer index
			bufferRxIndex++;
		}
		
		// If buffer is about to get full in next data reception
		if(bufferRxIndex >= RX_BUFFER_SIZE_TEMP-1)
		{
			// clear TWEA bit to not send acknowledge in next operation
			// to indicate master not to send any more data
			I2C_CONTROL = (1<<TWIE) | (1<<TWINT) | (0<<TWEA) | (1<<TWEN);
			
			// break statement is used to not execute default state at the end
			break;
		}
		
		// clear TWI interrupt flag TWINT bit by writing 1
		// keep TWI interupt enabled by writing 1 to TWIE bit
		// Keep TWEA bit high to send acknowledge to master in next operation
		// Keept TWEN bit 1 to continue with I2C mode
		I2C_CONTROL = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
		
	// Slave Receiver mode : data received from master, while No acknowledgement was sent by Slave
	case TW_SR_DATA_NACK:
		
		// Save data in buffer if buffer is not full
		if(bufferRxIndex < RX_BUFFER_SIZE_TEMP)
		{
			// save received byte in Rx buffer
			bufferRx[bufferRxIndex] = I2C_DATA;
			// increment Rx bufer index
			bufferRxIndex++;
		}
		
		// This status happens only when buffer is full and slave can read no more data.
		if(rx_complete_status == 1)
		{
			// Hence we increment rx_complete_status to 2 and indicate that Slave Rx is complete
			rx_complete_status++;
			// If enabled, we run rxCallback function so that user can read data from Rx buffer
			if(enable_rx_callback == 1)
				rxCallback();
		}
		
		// clear TWI interrupt flag TWINT bit by writing 1
		// keep TWI interupt enabled by writing 1 to TWIE bit
		// Keep TWEA bit high to send acknowledge to master in next operation
		// Keept TWEN bit 1 to continue with I2C mode
		I2C_CONTROL = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
		
		// It is essential to again enable acknowledge by writing 1 to TWEA bit
		// Otherwise slave will not acknowledge to master and this will create error in I2C communication
	
	// Stop or Repeated start condition is received
	case TW_SR_STOP:
		// This status indicates end of data reception by slave
		
		if(rx_complete_status == 1)
		{
			// Hence we increment rx_complete_status to 2 and indicate that Slave Rx is complete
			rx_complete_status++;
			
			// If enabled, we run rxCallback function so that user can read data from Rx buffer
			if(enable_rx_callback == 1)
				rxCallback();
		}
		
		// clear TWI interrupt flag TWINT bit by writing 1
		// keep TWI interupt enabled by writing 1 to TWIE bit
		// Keep TWEA bit high to send acknowledge to master in next operation
		// Keept TWEN bit 1 to continue with I2C mode
		I2C_CONTROL = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
		
	// Slave Transmitter mode : Slave own address has been acknowledged	by Slave
    case TW_ST_SLA_ACK:
	
		// prepare to transmit data bytes
		bufferTxIndex = 0;
		tx_complete_status = 1;
		
		// Increment Tx buffer index if it is less than Tx data length
		if(bufferTxIndex < bufferTxDataLen)
		{
			// Write data I2C data register
			I2C_DATA = bufferTx[bufferTxIndex];
			
			//increment tx buffer index
			bufferTxIndex++;
		}
		// if master still requesting data, when compelete data is already sent
		else
			// send 0xFF which is a garbage data
			I2C_DATA = 0xFF;
		
		// if enabled, we run txCallback function so that user can write data to the Tx buffer
		if(enable_tx_callback == 1)
				txCallback();
		
		// clear TWI interrupt flag TWINT bit by writing 1
		// keep TWI interupt enabled by writing 1 to TWIE bit
		// Keep TWEA bit high to send acknowledge to master in next operation
		// Keept TWEN bit 1 to continue with I2C mode
		I2C_CONTROL = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
		
	// Slave Transmitter mode : Master has receieved data, master sent acknowledge to slave
    case TW_ST_DATA_ACK:
	
		// Increment Tx buffer index if it is less than Tx data length
		if(bufferTxIndex < bufferTxDataLen)
		{
			// Write data I2C data register
			I2C_DATA = bufferTx[bufferTxIndex];
			
			//increment tx buffer index
			bufferTxIndex++;
		}
		// if master still requesting data, when compelete data is already sent
		else
			// send 0xFF which is a garbage data
			I2C_DATA = 0xFF;
		
		// clear TWI interrupt flag TWINT bit by writing 1
		// keep TWI interupt enabled by writing 1 to TWIE bit
		// Keep TWEA bit high to send acknowledge to master in next operation
		// Keept TWEN bit 1 to continue with I2C mode
		I2C_CONTROL = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
		
	// Slave Transmitter mode : Master has receieved data, master sent no acknowledge to slave
    case TW_ST_DATA_NACK:
		
		// This status indicates end of data transmission to master by the slave
				
		if(tx_complete_status == 1)
		{			
			// So we increment tx_complete_status to 2, to indicate completion of transmission 
			tx_complete_status++;
			// We set Tx data length to zero to be on safer side
			bufferTxDataLen=0;
		}
		
		// clear TWI interrupt flag TWINT bit by writing 1
		// keep TWI interupt enabled by writing 1 to TWIE bit
		// Keep TWEA bit high to send acknowledge to master in next operation
		// Keept TWEN bit 1 to continue with I2C mode
		I2C_CONTROL = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
		
	// Bus error
    case TW_BUS_ERROR:
		// some sort of erroneous state, prepare TWI to be readdressed
		// Set I2C Control Register to 0, this resets I2C
		I2C_CONTROL = 0;
		
		// clear TWI interrupt flag TWINT bit by writing 1
		// keep TWI interupt enabled by writing 1 to TWIE bit
		// Keep TWEA bit high to send acknowledge to master in next operation
		// Keept TWEN bit 1 to continue with I2C mode
		I2C_CONTROL = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
		
    default:
		
		// clear TWI interrupt flag TWINT bit by writing 1
		// keep TWI interupt enabled by writing 1 to TWIE bit
		// Keep TWEA bit high to send acknowledge to master in next operation
		// Keept TWEN bit 1 to continue with I2C mode
		I2C_CONTROL = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
		break;
  }

} 

// Function to check if slave data reception is complete
static mos_uint8_t rxComplete (void)
{
	// if rx_complete_status is 2, means data reception is complete
	if (rx_complete_status == 2)
		return 1;
	else
		return 0;
}

// Function to read data from Rx data buffer
// This function must be called after checking rxComplete function
static void read (void * data_rx_void, mos_uint16_t * data_rx_len)
{
	mos_uint16_t i;
	mos_uint8_t * data_rx;
	
	// This type casting is done so that input data to read function can be either char or mos_uint8_t
	data_rx = (mos_uint8_t *) data_rx_void;
	
	// Read data from Rx Data buffer
	for(i=0;i<bufferRxIndex;i++)
	{
		data_rx[i] = bufferRx[i];
	}
	
	// set rx_complete_status to 0, so that rxComplete function return 0
	rx_complete_status = 0;
	// read length of data received in Rx data buffer
	*data_rx_len = bufferRxIndex;
}

// Function to check if slave data transmission is complete
// This function must be called after running print function
static mos_uint8_t txComplete (mos_uint16_t * data_tx_len)
{
	// read length of data transmitted to master from Tx data buffer
	*data_tx_len = bufferTxIndex;
	
	// if tx_complete_status is 2, means data transmission is complete
	if (tx_complete_status == 2)
		return 1;
	else
		return 0;	
}

// Function to write data to Tx data buffer
static mos_uint8_t print ( void * data_tx_void, mos_uint16_t data_tx_len)
{
	mos_uint16_t i=0;
	mos_uint8_t return_value;
	mos_uint8_t * data_tx;
	
	// This type casting is done so that output data to print function can be either char or mos_uint8_t
	data_tx = (mos_uint8_t *) data_tx_void;
	
	// If data to be transmitted is less than Tx buffer size
	if(data_tx_len <= TX_BUFFER_SIZE_TEMP)
	{
		// Set transmit data length to input data length
		bufferTxDataLen = data_tx_len;
		// return success indicating that xomplete data could be transmitted
		return_value = 1;
	}
	else
	{
		// Set transmit data length to Tx buffer size
		bufferTxDataLen = TX_BUFFER_SIZE_TEMP;
		// return failure indicating that complete data could not be transmitted
		return_value = 0;
	}
	
	// Fill Tx data buffer
	for(i=0;i<bufferTxDataLen;i++)
	{
		bufferTx[i] = data_tx[i];
	}
	
	// set tx_complete_status to 0, so that txComplete function return 0
	tx_complete_status = 0;
	
	return return_value;
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
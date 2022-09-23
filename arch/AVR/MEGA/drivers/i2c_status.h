#ifndef I2C_STATUS_H
#define I2C_STATUS_H

// these are status codes generated in master mode

// status for master transmitter mode
#define MASTER_START 							0x08    // master initate I2c start, success status for master
#define MASTER_START_REPEAT 					0x10    // master initate I2c repeat-start, success status for master
#define MASTER_ADDRESS_WRITE_ACKNOWLEDGE 		0x18    // master send address + write to slave, slave responds with acknowledge
#define MASTER_ADDRESS_WRITE_NOACKNOWLEDGE 		0x20  	// master send address + write to slave, slave responds with no-acknowledge
#define MASTER_DATA_SEND_ACKNOWLEDGE 			0x28    // after address + write master send data to slave, slave responds with acknowledge
#define MASTER_DATA_SEND_NOACKNOWLEDGE 			0x30    // after address + write master send data to slave, slave responds with no-acknowledge
#define MASTER_ARBITRATION_LOST_ADDRESS_WRITE 	0x38	// master send address + (write or read) to slave, Arbitration lost by master

// status for master receiver mode                  
#define MASTER_ADDRESS_READ_ACKNOWLEDGE 		0x40    // master send address + read to slave, slave responds with acknowledge
#define MASTER_ADDRESS_READ_NOACKNOWLEDGE 		0x48   	// master send address + read to slave, slave responds with no-acknowledge
#define MASTER_DATA_READ_ACKNOWLEDGE 			0x50    // after address + read, slave send data to master, master responds with acknowledge
#define MASTER_DATA_READ_NOACKNOWLEDGE 			0x58    // after address + read, slave send data to master, master responds with no-acknowledge

// these are status codes generated in slave mode

// status for slave receiver mode
#define SLAVE_ADDRESS_WRITE_ACKNOWLEDGE 			0x60	// master send address + write to slave, slave responds with acknowledge
#define SLAVE_ADDRESS_WRITE_NOACKNOWLEDGE 			0x68  	// master send address + write to slave, slave responds with no-acknowledge
#define SLAVE_GENERAL_CALL_ACKNOWLEDGE 				0x70	// Slave recives a general call, slave responds with acknowledge
#define SLAVE_DATA_SENDBYMASTER_ACKNOWLEDGE 		0x80    // after address + write master send data to slave, slave responds with acknowledge
#define SLAVE_DATA_SENDBYMASTER_NOACKNOWLEDGE 		0x88    // after address + write master send data to slave, slave responds with no-acknowledge
#define SLAVE_DATA_SENDBY_GENERALCALL_ACKNOWLEDGE 	0x90  	// after general call data received by slave, slave responds with acknowledge
#define SLAVE_DATA_SENDBY_GENERALCALL_NOACKNOWLEDGE 0x98    // after general call data received by slave, slave responds with no-acknowledge
#define SLAVE_STOP_OK 0xA0                         			// stop condition or repeated start condition received by slave
//not covered states
// 0x78 = after arbitration lost as master, general call is received, acknwledge has been sent

// status for slave transmitter mode
#define SLAVE_ADDRESS_READ_ACKNOWLEDGE 			0xA8		// master send address + read to slave, slave responds with acknowledge
#define SLAVE_DATA_READBYMASTER_ACKNOWLEDGE 	0xB8       	// after address + read, slave send data to master, master responds with acknowledge
#define SLAVE_DATA_READBYMASTER_NOACKNOWLEDGE 	0xC0      	// after address + read, slave send data to master, master responds with no-acknowledge
//not covered states
// 0xB0 = after arbitration lost as master, own slave address + read is received, acknwledge has been sent
// 0xC8 = last byte in TWDR is transmitted as slave, TWEA = 0, Acknowledge has been received

// Miscellaneaous states
#define NO_RELEVANT_STATE 						0xF8		// NO relevant state information, TWINT = 0								
#define BUS_ERROR 								0x00		// Bus error due to illegal Start or Stop condition

#endif

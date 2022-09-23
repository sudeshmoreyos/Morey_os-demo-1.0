#ifndef _i2c_status_h
#define _i2c_status_h

// these are status codes generated in master mode

// status for master transmitter mode
#define master_start 							0x08    // master initate I2c start, success status for master
#define master_start_repeat 					0x10    // master initate I2c repeat-start, success status for master
#define master_address_write_acknowledge 		0x18    // master send address + write to slave, slave responds with acknowledge
#define master_address_write_noacknowledge 		0x20  	// master send address + write to slave, slave responds with no-acknowledge
#define master_data_send_acknowledge 			0x28    // after address + write master send data to slave, slave responds with acknowledge
#define master_data_send_noacknowledge 			0x30    // after address + write master send data to slave, slave responds with no-acknowledge
#define master_arbitration_lost_address_write 	0x38	// master send address + (write or read) to slave, Arbitration lost by master

// status for master receiver mode                  
#define master_address_read_acknowledge 		0x40    // master send address + read to slave, slave responds with acknowledge
#define master_address_read_noacknowledge 		0x48   	// master send address + read to slave, slave responds with no-acknowledge
#define master_data_read_acknowledge 			0x50    // after address + read, slave send data to master, master responds with acknowledge
#define master_data_read_noacknowledge 			0x58    // after address + read, slave send data to master, master responds with no-acknowledge

// these are status codes generated in slave mode

// status for slave receiver mode
#define slave_address_write_acknowledge 			0x60	// master send address + write to slave, slave responds with acknowledge
#define slave_address_write_noacknowledge 			0x68  	// master send address + write to slave, slave responds with no-acknowledge
#define slave_general_call_acknowledge 				0x70	// Slave recives a general call, slave responds with acknowledge
#define slave_data_sendbymaster_acknowledge 		0x80    // after address + write master send data to slave, slave responds with acknowledge
#define slave_data_sendbymaster_noacknowledge 		0x88    // after address + write master send data to slave, slave responds with no-acknowledge
#define slave_data_sendby_generalcall_acknowledge 	0x90  	// after general call data received by slave, slave responds with acknowledge
#define slave_data_sendbymaster_noacknowledge 		0x98    // after general call data received by slave, slave responds with no-acknowledge
#define slave_stop_ok 0xA0                         			// stop condition or repeated start condition received by slave
//not covered states
// 0x78 = after arbitration lost as master, general call is received, acknwledge has been sent

// status for slave transmitter mode
#define slave_address_read_acknowledge 			0xA8		// master send address + read to slave, slave responds with acknowledge
#define slave_data_readbymaster_acknowledge 	0xB8       	// after address + read, slave send data to master, master responds with acknowledge
#define slave_data_readbymaster_noacknowledge 	0xC0      	// after address + read, slave send data to master, master responds with no-acknowledge
//not covered states
// 0xB0 = after arbitration lost as master, own slave address + read is received, acknwledge has been sent
// 0xC8 = last byte in TWDR is transmitted as slave, TWEA = 0, Acknowledge has been received

// Miscellaneaous states
#define no_relevant_state 						0xF8		// NO relevant state information, TWINT = 0								
#define BUS_ERROR 								0x00		// Bus error due to illegal Start or Stop condition

#endif

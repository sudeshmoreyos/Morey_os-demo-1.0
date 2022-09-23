#ifndef _i2c_status_h
#define _i2c_status_h

// these are status codes generated in master mode

// status for master transmitter mode
#define master_start_ok 0x08                        // master initate I2c start, success status for master
#define master_start_repeat_ok 0x10                 // master initate I2c repeat-start, success status for master
#define master_address_write_acknowledge_ok 0x18    // master send address + write to slave, slave responds with acknowledge
#define master_address_write_noacknowledge_ok 0x20  // master send address + write to slave, slave responds with no-acknowledge
#define master_data_send_acknowledge_ok 0x28        // after address + write master send data to slave, slave responds with acknowledge
#define master_data_send_noacknowledge_ok 0x30      // after address + write master send data to slave, slave responds with no-acknowledge

// status for master receiver mode                  
#define master_address_read_acknowledge_ok 0x40     // master send address + read to slave, slave responds with acknowledge
#define master_address_read_noacknowledge_ok 0x48   // master send address + read to slave, slave responds with no-acknowledge
#define master_data_read_acknowledge_ok 0x50        // after address + read, slave send data to master, master responds with acknowledge
#define master_data_read_noacknowledge_ok 0x58      // after address + read, slave send data to master, master responds with no-acknowledge

// these are status codes generated in slave mode

// status for slave receiver mode
#define slave_address_write_acknowledge_ok 0x60    // master send address + write to slave, slave responds with acknowledge
#define slave_address_write_noacknowledge_ok 0x68  // master send address + write to slave, slave responds with no-acknowledge
#define slave_data_sendbymaster_acknowledge_ok 0x80        // after address + write master send data to slave, slave responds with acknowledge
#define slave_data_sendbymaster_noacknowledge_ok 0x88      // after address + write master send data to slave, slave responds with no-acknowledge
#define slave_stop_ok 0xA0                         // stop condition or repeated start condition received by slave

// status for slave transmitter mode
#define slave_address_read_acknowledge_ok 0xA8     // master send address + read to slave, slave responds with acknowledge
#define slave_address_read_noacknowledge_ok 0xB0   // master send address + read to slave, slave responds with no-acknowledge
#define slave_data_readbymaster_acknowledge_ok 0xB8        // after address + read, slave send data to master, master responds with acknowledge
#define slave_data_readbymaster_noacknowledge_ok 0xC0      // after address + read, slave send data to master, master responds with no-acknowledge

#endif

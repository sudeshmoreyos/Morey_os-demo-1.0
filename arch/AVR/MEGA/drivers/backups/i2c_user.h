#ifndef _i2c_user_h
#define _i2c_user_h
void i2c_init1();
unsigned char i2c_start1();
unsigned char i2c_write1(unsigned char data);
unsigned char i2c_read1(unsigned char * data, int acknowledge);
void i2c_stop1();
#endif

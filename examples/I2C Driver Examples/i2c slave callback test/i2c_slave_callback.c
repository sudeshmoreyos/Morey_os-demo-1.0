// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "I2cSlave.h"
#include "Serial.h"

void i2c_callback(void)
{	
	char data[60];
	mos_uint16_t i=0,len;
	
	I2cSlave.read(data,&len);
	
	while(len)
	{
		Serial.write(data[i]);
		i++;
		len--;		
	}
}

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{
	Serial.begin(9600, SERIAL_8N1);
	I2cSlave.begin(0x10);
	I2cSlave.attachRxCallback(&i2c_callback);
}

// Delcare all processes here
TASK_CREATE(i2c_slave,"i2c slave");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&i2c_slave);

TASK_RUN(i2c_slave)
{
	// Declare all variables here, please read documentation to understand 
	// which variables should be declared as static variables
	
	
	// Process starts here  
	BEGIN();
	Serial.println("Slave : ");

	while(1)
	{		
		DELAY_SEC(0.1);
	}
	  
  // process ends here
  END();
}
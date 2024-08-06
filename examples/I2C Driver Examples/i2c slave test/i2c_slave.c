// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "I2cSlave.h"
#include "lib/debug.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{
	DEBUG_INIT(9600);
	I2cSlave.begin(0x10);
}

// Delcare all processes here
TASK_CREATE(i2c_slave,"i2c slave");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&i2c_slave);

TASK_RUN(i2c_slave)
{
	// Declare all variables here, please read documentation to understand 
	// which variables should be declared as static variables            
	char data[50];
	mos_uint16_t len;
	
	
	// Process starts here  
	BEGIN();
	Serial.println("Slave : ");
	I2cSlave.print("bye",4);

	while(1)
	{		
		if(I2cSlave.rxComplete())
		{
			I2cSlave.read(data,&len);
			Serial.constPrint("Data = ");
			Serial.println(data);
		}
		
		if(I2cSlave.txComplete(&len))
		{
			Serial.constPrintln("Data sent!");
			I2cSlave.print("bye",4);
		}		
		DELAY_SEC(0.1);
	}
	  
  // process ends here
  END();
}
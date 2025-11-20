// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Serial.h"
#include "SpiSlave.h"

void callback(mos_uint8_t data)
{
	Serial.print(" Data recieved = ");
	Serial.write(data);
	Serial.println("");
	SpiSlave.transfer(data);
}

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	SpiSlave.begin(&callback);
	Serial.begin(9600,SERIAL_8N1);
}

// Delcare all processes here
TASK_CREATE(test1,"test-1");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&test1);

TASK_RUN(test1)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables
  
  // Process starts here  
  BEGIN();    
  Serial.println("Welcome Slave");
  
  while(1)
  {
	DELAY_SEC(0.1);
  }
  
  // process ends here
  END();
}

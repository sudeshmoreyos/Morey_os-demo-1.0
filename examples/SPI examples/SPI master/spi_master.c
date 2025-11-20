// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Serial.h"
#include "SpiMaster.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	SpiMaster.begin(B2);
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
  char x;
  
  // Process starts here  
  BEGIN();    
  Serial.println("Welcome");
  //SpiMaster.slaveUnselect();
  //SpiMaster.slaveSelect();
  while(1)
  {
	if(Serial.available())
	{
		SpiMaster.slaveSelect();
		while(Serial.available())
		{
			x = Serial.read();
			x = SpiMaster.transfer(x);
			Serial.print(" Data recieved = ");
			Serial.write(x);
			Serial.println("");
		}
		SpiMaster.slaveUnselect();		
	}
	DELAY_SEC(0.1);
  }
  
  // process ends here
  END();
}

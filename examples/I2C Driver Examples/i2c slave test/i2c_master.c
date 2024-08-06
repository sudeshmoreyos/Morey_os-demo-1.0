// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "I2cMaster.h"
#include "lib/debug.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Serial.begin(9600, SERIAL_8N1);
	I2cMaster.begin(100000);
}

// Delcare all processes here
TASK_CREATE(master_tx,"master transmit");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&master_tx);

TASK_RUN(master_tx)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables 
char temp[10];  

  // Process starts here  
  BEGIN();  
	Serial.constPrintln("Master : ");

  while(1)
  {
	Serial.constPrintln("sending data");
	if(I2cMaster.transfer(0x10,"hello",6,temp,4)==0)
	{
		Serial.constPrintln("success");
		Serial.constPrint("Data received = ");
		Serial.println(temp);
	}
	else
		Serial.constPrintln("failed");
	DELAY_SEC(10);
  }
  
  // process ends here
  END();
}

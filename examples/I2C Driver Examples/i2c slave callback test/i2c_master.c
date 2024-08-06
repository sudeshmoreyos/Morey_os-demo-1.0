// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "I2cMaster.h"
#include "Serial.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Serial.begin(2400, SERIAL_8N1);
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
	char temp[60];
	int i;

	// Process starts here  
	BEGIN();  
	Serial.constPrintln("Master : ");

	while(1)
	{
		if(Serial.available())
		{
			i=0;
			while(Serial.available())
			{
				if(i<50)
				{
					temp[i] = Serial.read();
					i++;
				}
				else
					break;
			}
		
			if(I2cMaster.transfer(0x10,temp,i,temp,0)==0)
			{
				Serial.constPrintln("success");
			}
			else
				Serial.constPrintln("failed");
		}
		DELAY_SEC(0.1);
	}
  
	// process ends here
	END();
}

// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"
#include "lib/debug.h"
#include "Serial.h"

#define led D6
#define led2 D5

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Digital.pinmode(led,OUTPUT);
	Digital.pinmode(led2,OUTPUT);
	Serial.begin(115200,SERIAL_8N1);
}

// Delcare all processes here
TASK_CREATE(LED1,"LED-1");
TASK_CREATE(LED2,"LED-2");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&LED1,&LED2);

TASK_RUN(LED1)
{
	// Declare all variables here, please read documentation to understand 
	// which variables should be declared as static variables            

	// Process starts here  
	BEGIN();    
	
	Serial.constPrintln("Welcome");

	while(1)
	{
		Digital.write(led,HIGH);
		DEBUG_USER_LN("LED On");
		DELAY_SEC_PRECISE(0.751);

		Digital.write(led,LOW);
		DEBUG_USER_LN("LED Off");
		DELAY_SEC_PRECISE(1.249);
	}
  
	// process ends here
	END();
}


TASK_RUN(LED2)
{
	// Declare all variables here, please read documentation to understand 
	// which variables should be declared as static variables            

	// Process starts here  
	BEGIN();    

	while(1)
	{
		Digital.write(led2,HIGH);
		DEBUG_USER_LN("        LED2 On");
		DELAY_SEC_PRECISE(2.251);

		Digital.write(led2,LOW);
		DEBUG_USER_LN("        LED2 Off");
		DELAY_SEC_PRECISE(2.749);            
	}

	// process ends here
	END();
}


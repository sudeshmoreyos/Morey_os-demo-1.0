// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "drivers/Serial.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Serial.begin(9600,SERIAL_8N1);
}

// Delcare all processes here
PROCESS(serial_test,"Serial test");
PROCESS(serial_read,"Serial read");

// Delcare autostart  processes here. Atleast one process must be autostarted;
AUTOSTART_PROCESSES(&serial_test, &serial_read);

PROCESS_THREAD(serial_test)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            

  // Process starts here  
  BEGIN();    

  while(1)
  {
	Serial.println("Hello world");
	DELAY_SEC(1);
  }
  
  // process ends here
  END();
}

PROCESS_THREAD(serial_read)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables  
	static char x;

  // Process starts here  
  BEGIN();    

  while(1)
  {
	if(Serial.available()>0)
	{
		x=Serial.read();
		if(x=='a')
			Serial.println("Hi");
		else if (x=='b')
			Serial.println("Bye");
	}
	
	DELAY_SEC(0.1);
  }
  
  // process ends here
  END();
}
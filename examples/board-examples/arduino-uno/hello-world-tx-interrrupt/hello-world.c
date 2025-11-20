// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"
#include "Serial.h"
#include "util/delay_blocking.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Serial.begin(9600,SERIAL_8N1);
	Digital.pinmode(pin13,OUTPUT);
}

// Delcare all processes here
TASK_CREATE(hello,"hello");
TASK_CREATE(reply,"reply");
TASK_CREATE(led,"LED");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&hello, &reply, &led);

TASK_RUN(hello)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            

  // Process starts here  
  BEGIN();    

  while(1)
  {
	Serial.println("Hello World");
	DELAY_SEC(2);
  }
  
  // process ends here
  END();
}

TASK_RUN(reply)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            
	static char x;
  // Process starts here  
  BEGIN();    

  while(1)
  {
    if(Serial.available())
	{
		x = Serial.read();
		if(x=='a')
			Serial.println("Hey");
		if(x=='b')
			Serial.println("Bye");
	}
	DELAY_SEC(0.01); 
  }
  
  // process ends here
  END();
}

TASK_RUN(led)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            

  // Process starts here  
  BEGIN();    

  while(1)
  {
	Digital.write(pin13,HIGH);
    DELAY_SEC_PRECISE(0.1);
	
	Digital.write(pin13,LOW);
    DELAY_SEC_PRECISE(0.1);            
  }
  
  // process ends here
  END();
}

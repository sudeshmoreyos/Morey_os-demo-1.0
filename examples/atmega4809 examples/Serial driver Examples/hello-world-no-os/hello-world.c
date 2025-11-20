// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"
#include "Serial.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Serial.begin(9600,SERIAL_8N1);
	Digital.pinmode(B4,OUTPUT);
}

// Delcare all processes here
TASK_CREATE(hello,"hello");

// Delcare autostart  processes here. Atleast one process must be autostarted;
AUTOSTART_PROCESSES(&hello);

TASK_RUN(hello)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            
	char x;
  // Process starts here  
  BEGIN();    
  
  DELAY_SEC(2);
  
  Serial.println("Welcome");

  while(1)
  {
	/*
	Digital.write(B4,HIGH);
	Serial.constPrintln("Hello");
	DELAY_SEC(2);
	
	Digital.write(B4,LOW);
	Serial.constPrintln("Bye");
	DELAY_SEC(2);
	*/
	
	while(Serial.available())
	{
		x = Serial.read();
		if(x=='a')
			Serial.println("Hello");
		else if(x=='b')
			Serial.println("Bye");
	}
	DELAY_SEC(0.1);
  }
  
  // process ends here
  END();
}
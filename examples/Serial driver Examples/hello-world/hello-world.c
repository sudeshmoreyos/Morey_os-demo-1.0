// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"
#include "Serial.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Serial.begin(4800,SERIAL_8N1);
	Digital.pinmode(B5,OUTPUT);
	Digital.pinmode(B7,OUTPUT);
}

// Delcare all processes here
TASK_CREATE(hello,"hello");
TASK_CREATE(reply,"reply");
TASK_CREATE(led,"LED");

// Delcare autostart  processes here. Atleast one process must be autostarted;
AUTOSTART_PROCESSES(&hello, &reply,&led);

TASK_RUN(hello)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            

  // Process starts here  
  BEGIN();    

  while(1)
  {
	Digital.write(B5,HIGH);
	Serial.constPrintln("Hello World I am Sudesh Morey and who are you?");
	//while(Serial.txComplete()==0);
	Digital.write(B5,LOW);
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
			Serial.constPrintln("Hey how are you I mean");
		if(x=='b')
			Serial.constPrintln("No thank you Bye");
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
	Digital.write(B7,HIGH);
    DELAY_SEC_PRECISE(0.1);
	
	Digital.write(B7,LOW);
    DELAY_SEC_PRECISE(0.1);            
  }
  
  // process ends here
  END();
}

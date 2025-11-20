// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"
#include "Serial.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Digital.pinmode(E0,OUTPUT);
	Digital.pinmode(E1,OUTPUT);
	Serial.begin(9600,SERIAL_8N1);
	Serial1.begin(9600,SERIAL_8N1);
}

// Delcare all processes here
TASK_CREATE(LED1,"LED-1");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&LED1);

TASK_RUN(LED1)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            
	char x;
  // Process starts here  
  BEGIN();    

  while(1)
  {
	Digital.write(E0,HIGH);
	Digital.write(E1,LOW);
	Serial.println("Serial-0 : LED1-ON");
	Serial1.println("Serial-1 : LED2-OFF");
    DELAY_SEC_PRECISE(0.51);

	Digital.write(E0,LOW);
	Digital.write(E1,HIGH);
	Serial.println("Serial-0 : LED1-OFF");
	Serial1.println("Serial-1 : LED2-ON");
    DELAY_SEC_PRECISE(0.49); 

	while(Serial1.available())
	{
		Serial1.print("Inside Serial-1");
		x = Serial1.read();
		Serial1.print("Data = ");
		Serial1.write(x);
		Serial1.println("");
		if(x=='a')
			Serial1.println("Hello");
		else if(x=='b')
			Serial1.println("Bye");
	}
	
	while(Serial.available())
	{
		Serial.print("Inside Serial-0");
		x = Serial.read();
		Serial.print("Data = ");
		Serial.write(x);
		Serial.println("");
		if(x=='a')
			Serial.println("Hello");
		else if(x=='b')
			Serial.println("Bye");
		else if(x=='\n')
			Serial.println("New Line");
	}
  }
  
  // process ends here
  END();
}
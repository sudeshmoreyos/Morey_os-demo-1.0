// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"
#include "lib/debug.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Digital.pinmode(E0,OUTPUT);
	Digital.pinmode(E1,OUTPUT);
	DEBUG_INIT(115200);
	Serial1.begin(9600,SERIAL_8N1);
}

// Delcare all processes here
TASK_CREATE(LED1,"LED-1");
TASK_CREATE(LED2,"LED-2");
TASK_CREATE(serial_read,"Serial Read");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&LED1,&LED2,&serial_read);

TASK_RUN(LED1)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            

  // Process starts here  
  BEGIN();    

  while(1)
  {
	Digital.write(E0,HIGH);
	DEBUG_USER_LN("									LED1-ON");
    DELAY_SEC_PRECISE(5.4);

	Digital.write(E0,LOW);
	DEBUG_USER_LN("									LED1-OFF");
    DELAY_SEC_PRECISE(4.6);           
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
	Digital.write(E1,HIGH);
	Serial1.println("LED2-ON");
    DELAY_SEC_PRECISE(2.3);
	
	Digital.write(E1,LOW);
	Serial1.println("LED2-OFF");
    DELAY_SEC_PRECISE(7.7);            
  }
  
  // process ends here
  END();
}

TASK_RUN(serial_read)
{
	char x;
	BEGIN();
	while(1)
	{
		if(Serial.available())
		{
			Serial.print("Inside Serial0");
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
		
		if(Serial1.available())
		{
			Serial1.print("Inside Serial1");
			x = Serial1.read();
			Serial1.print("Data = ");
			Serial1.write(x);
			Serial1.println("");
			if(x=='a')
				Serial1.println("Hello");
			else if(x=='b')
				Serial1.println("Bye");
		}
		DELAY_SEC(0.05);
	}	
	END();
}

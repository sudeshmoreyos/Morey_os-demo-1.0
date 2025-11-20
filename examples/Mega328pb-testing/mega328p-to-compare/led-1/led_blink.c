// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"
#include "lib/debug.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Digital.pinmode(B5,OUTPUT);
	Digital.pinmode(B4,OUTPUT);
	DEBUG_INIT(115200);
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
	Digital.write(B5,HIGH);
	DEBUG_USER_LN("		LED1-ON");
    DELAY_SEC_PRECISE(5.4);

	Digital.write(B5,LOW);
	DEBUG_USER_LN("		LED1-OFF");
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
	Digital.write(B4,HIGH);
	DEBUG_USER_LN("				LED2-ON");
    DELAY_SEC_PRECISE(2.3);
	
	Digital.write(B4,LOW);
	DEBUG_USER_LN("				LED2-OFF");
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
			x = Serial.read();
			Serial.print("Data =");
			Serial.write(x);
			Serial.println("");
			if(x=='a')
				Serial.println("Hello");
			else if(x=='b')
				Serial.println("Bye");
		}
		DELAY_SEC(0.05);
	}	
	END();
}

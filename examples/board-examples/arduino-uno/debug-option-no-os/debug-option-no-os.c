// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"
#include "lib/debug.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Digital.pinmode(pin13,OUTPUT);
	Serial.begin(9600,SERIAL_8N1);
}

// Delcare all processes here
TASK_CREATE(LED1,"LED-1");

// Delcare autostart  processes here. Atleast one process must be autostarted;
AUTOSTART_PROCESSES(&LED1);

TASK_RUN(LED1)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            

  // Process starts here  
  BEGIN();    

  while(1)
  {
	Digital.write(pin13,HIGH);
	DEBUG_USER_LN("LED INBUILT ON");
    DELAY_SEC_PRECISE(1);

	Digital.write(pin13,LOW);
	DEBUG_USER_LN("LED INBUILT OFF");
    DELAY_SEC_PRECISE(1);           
  }
  
  // process ends here
  END();
}
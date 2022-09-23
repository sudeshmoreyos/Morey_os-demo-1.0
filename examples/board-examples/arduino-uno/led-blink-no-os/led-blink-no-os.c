// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Digital.pinmode(pin13,OUTPUT);
	Digital.pinmode(pinA0,OUTPUT);
}

// Delcare all processes here
TASK_CREATE(LED1,"LED-1");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&LED1);

TASK_RUN(LED1)
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

// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Digital.pinmode(pin12,OUTPUT);
	Digital.pinmode(pin13,OUTPUT);
}

// Delcare all processes here
TASK_CREATE(LED1,"LED-1");
TASK_CREATE(LED2,"LED-2");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&LED1, &LED2);

TASK_RUN(LED1)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            

  // Process starts here  
  BEGIN();    

  while(1)
  {
	Digital.write(pin12,HIGH);
    	DELAY_SEC_PRECISE(5);

	Digital.write(pin12,LOW);
    	DELAY_SEC_PRECISE(5);           
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
	Digital.write(pin13,HIGH);
    	DELAY_SEC_PRECISE(1);
	
	Digital.write(pin13,LOW);
    	DELAY_SEC_PRECISE(3);            
  }
  
  // process ends here
  END();
}

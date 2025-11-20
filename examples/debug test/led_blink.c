// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"
#include "lib/debug.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Digital.pinmode(B0,OUTPUT);
	Digital.pinmode(B5,OUTPUT);
	DEBUG_INIT(57600);
}

// Delcare all processes here
TASK_CREATE(LED1,"LED-1");
TASK_CREATE(LED2,"LED-2");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&LED2);

TASK_RUN(LED1)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            

  // Process starts here  
  BEGIN();    

  while(1)
  {
	DEBUG_USER_CONST_LN("B0 On!");
	Digital.write(B0,HIGH);
    DELAY_SEC_PRECISE(5);
	
	DEBUG_USER_CONST_LN("B0 Off!");
	Digital.write(B0,LOW);
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
	DEBUG_USER_CONST_LN("B5 On!");
	Digital.write(B5,HIGH);
    DELAY_SEC_PRECISE(1);
	
	DEBUG_USER_CONST_LN("B5 Off!");
	Digital.write(B5,LOW);
    DELAY_SEC_PRECISE(3);            
  }
  
  // process ends here
  END();
}

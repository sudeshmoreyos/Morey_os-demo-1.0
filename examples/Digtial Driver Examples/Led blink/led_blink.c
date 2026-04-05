// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Digital.pinmode(B5,OUTPUT);
	Digital.pinmode(B4,OUTPUT);
}

// Delcare all tasks here
TASK_CREATE(Led1,"LED-1");
TASK_CREATE(Led2,"LED-2");

// Delcare autostart tasks here. Atleast one task must be autostarted;
TASK_AUTOSTART(&Led1, &Led2);

TASK_RUN(Led1)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            

  // Task begins here  
  BEGIN();    

  while(1)
  {

	Digital.write(B5,HIGH);
    DELAY_SEC(1);

	Digital.write(B5,LOW);
    DELAY_SEC(1);           
  }
  
  // process ends here
  END();
}


TASK_RUN(Led2)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            

  // Task begins here  
  BEGIN();    

  while(1)
  {

	Digital.write(B4,HIGH);
    DELAY_SEC_PRECISE(0.1);

	Digital.write(B4,LOW);
    DELAY_SEC_PRECISE(0.1);           
  }
  
  // process ends here
  END();
}

// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Digital.pinmode(pin13,OUTPUT);
	Digital.pinmode(pin12,OUTPUT);
	Digital.pinmode(pin2,INPUT);
}

// Delcare all processes here
TASK_CREATE(led_blink,"led blink");
TASK_CREATE(input_test,"input test");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&led_blink, &input_test);

TASK_RUN(led_blink)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            

  // Process starts here  
  BEGIN();    

  while(1)
  {
	Digital.write(pin13,HIGH);
    DELAY_SEC(0.5);

	Digital.write(pin13,LOW);
    DELAY_SEC(0.5);           
  }
  
  // process ends here
  END();
}

TASK_RUN(input_test)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            

  // Process starts here  
  BEGIN();    

  while(1)
  {
	if(Digital.read(pin2)==1)
		Digital.write(pin12,HIGH);
	else
		Digital.write(pin12,LOW);
	
    DELAY_SEC(0.1);            
  }
  
  // process ends here
  END();
}

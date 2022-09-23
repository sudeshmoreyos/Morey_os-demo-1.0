// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"
#include "Serial.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Serial.begin(9600,SERIAL_8N1);
}

// Delcare all processes here
TASK_CREATE(hello,"hello");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&hello);

TASK_RUN(hello)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            

  // Process starts here  
  BEGIN();    

  while(1)
  {
	Serial.println("Hello World");
	DELAY_SEC(2);           
  }
  
  // process ends here
  END();
}

// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"
#include "TimerDelay.h"
int count =0;
void callback(void)
{
	Digital.write(B5,TOGGLE);
	count++;
	if(count==20)
		TimerDelay.end(TIMER_2);
}

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Digital.pinmode(B5,OUTPUT);
	TimerDelay.begin(TIMER_2,1000,&callback);
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
    DELAY_SEC_PRECISE(5);           
  }
  
  // process ends here
  END();
}
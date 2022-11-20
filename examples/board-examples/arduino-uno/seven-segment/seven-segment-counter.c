// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "lib/SevenSegment.h"
SevenSegment_t seg1={pin0,pin1,pin2,pin3,pin4,pin5,pin6,pin7},seg2={pinA0,pinA1,pinA2,pinA3,pinA4,pinA5,pin8,pin9};

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	SevenSegment_begin(&seg1, ANODE);
	SevenSegment_begin(&seg2, CATHODE);
}

// Delcare all processes here
TASK_CREATE(SEVEN1,"SEVEN-1");
TASK_CREATE(SEVEN2,"SEVEN-2");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&SEVEN1, &SEVEN2);

TASK_RUN(SEVEN1)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            
   static int i=0;
  // Process starts here  
  BEGIN();

  while(1)
  {
	for(i=0; i<19; i++)
	{
		SevenSegment_print(&seg1, i);
		DELAY_SEC_PRECISE(0.5);
	} 
    for(i=0; i<19; i++)
	{
		SevenSegment_printDot(&seg1, i);	
		DELAY_SEC_PRECISE(0.5);
	}
    for(i=0; i<19; i++)
	{
		SevenSegment_printReverse(&seg1, i);
		DELAY_SEC_PRECISE(0.5);
	}
    for(i=0; i<19; i++)
	{
		SevenSegment_printDotReverse(&seg1, i);
		DELAY_SEC_PRECISE(0.5);
	}

	SevenSegment_customPrint(&seg1,1,0,0,0,0,0,0,0);
	DELAY_SEC_PRECISE(0.5);
	
	SevenSegment_customPrint(&seg1,0,1,0,0,0,0,0,0);
	DELAY_SEC_PRECISE(0.5);
	
	SevenSegment_customPrint(&seg1,0,0,1,0,0,0,0,0);
	DELAY_SEC_PRECISE(0.5);
	
	SevenSegment_customPrint(&seg1,0,0,0,1,0,0,0,0);
	DELAY_SEC_PRECISE(0.5);
	
	SevenSegment_customPrint(&seg1,0,0,0,0,1,0,0,0);
	DELAY_SEC_PRECISE(0.5);
	
	SevenSegment_customPrint(&seg1,0,0,0,0,0,1,0,0);
	DELAY_SEC_PRECISE(0.5);
	
	SevenSegment_customPrint(&seg1,0,0,0,0,0,0,1,0);
	DELAY_SEC_PRECISE(0.5);
	
	SevenSegment_customPrint(&seg1,0,0,0,0,0,0,0,1);
	DELAY_SEC_PRECISE(0.5);
  }
  // process ends here
  END();
}

TASK_RUN(SEVEN2)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            
   static int i=0;
  // Process starts here  
  BEGIN();    

  while(1)
  {
	for(i=0; i<19; i++)
	{
		SevenSegment_print(&seg2, i);
		DELAY_SEC_PRECISE(0.5);
	} 
    for(i=0; i<19; i++)
	{
		SevenSegment_printDot(&seg2, i);	
		DELAY_SEC_PRECISE(0.5);
	}
    for(i=0; i<19; i++)
	{
		SevenSegment_printReverse(&seg2, i);
		DELAY_SEC_PRECISE(0.5);
	}
    for(i=0; i<19; i++)
	{
		SevenSegment_printDotReverse(&seg2, i);
		DELAY_SEC_PRECISE(0.5);
	}	

	SevenSegment_customPrint(&seg2,1,0,0,0,0,0,0,0);
	DELAY_SEC_PRECISE(0.5);
	
	SevenSegment_customPrint(&seg2,0,1,0,0,0,0,0,0);
	DELAY_SEC_PRECISE(0.5);
	
	SevenSegment_customPrint(&seg2,0,0,1,0,0,0,0,0);
	DELAY_SEC_PRECISE(0.5);
	
	SevenSegment_customPrint(&seg2,0,0,0,1,0,0,0,0);
	DELAY_SEC_PRECISE(0.5);
	
	SevenSegment_customPrint(&seg2,0,0,0,0,1,0,0,0);
	DELAY_SEC_PRECISE(0.5);
	
	SevenSegment_customPrint(&seg2,0,0,0,0,0,1,0,0);
	DELAY_SEC_PRECISE(0.5);
	
	SevenSegment_customPrint(&seg2,0,0,0,0,0,0,1,0);
	DELAY_SEC_PRECISE(0.5);
	
	SevenSegment_customPrint(&seg2,0,0,0,0,0,0,0,1);
	DELAY_SEC_PRECISE(0.5);
	
  }
  // process ends here
  END();
}

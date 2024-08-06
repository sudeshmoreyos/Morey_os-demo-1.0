// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "lib/sevensegment.h"
SevenSegment_t seg1={D0,D1,D2,D3,D4,D5,D6,D7},seg2={C0,C1,C2,C3,C4,C5,B0,B1};

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	SevenSegment_begin(&seg1, ANODE);
	SevenSegment_begin(&seg2, CATHODE);
}

// Delcare all processes here
PROCESS(SEVEN1,"SEVEN-1");
PROCESS(SEVEN2,"SEVEN-2");

// Delcare autostart  processes here. Atleast one process must be autostarted;
AUTOSTART_PROCESSES(&SEVEN1, &SEVEN2);

PROCESS_THREAD(SEVEN1)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            
   static int i=0;
  // Process starts here  
  BEGIN();

  while(1)
  {
	for(i=0; i<16; i++)
	{
		SevenSegment_print(&seg1, i);
		DELAY_SEC_PRECISE(0.5);
	} 
    for(i=0; i<16; i++)
	{
		SevenSegment_printDot(&seg1, i);	
		DELAY_SEC_PRECISE(0.5);
	}
    for(i=0; i<16; i++)
	{
		SevenSegment_printReverse(&seg1, i);
		DELAY_SEC_PRECISE(0.5);
	}
    for(i=0; i<16; i++)
	{
		SevenSegment_printDotReverse(&seg1, i);
		DELAY_SEC_PRECISE(0.5);
	}	 	
  }
  // process ends here
  END();
}

PROCESS_THREAD(SEVEN2)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            
   static int i=0;
  // Process starts here  
  BEGIN();    

  while(1)
  {
	for(i=0; i<16; i++)
	{
		SevenSegment_print(&seg2, i);
		DELAY_SEC_PRECISE(0.5);
	} 
    for(i=0; i<16; i++)
	{
		SevenSegment_printDot(&seg2, i);	
		DELAY_SEC_PRECISE(0.5);
	}
    for(i=0; i<16; i++)
	{
		SevenSegment_printReverse(&seg2, i);
		DELAY_SEC_PRECISE(0.5);
	}
    for(i=0; i<16; i++)
	{
		SevenSegment_printDotReverse(&seg2, i);
		DELAY_SEC_PRECISE(0.5);
	}	 	
  }
  // process ends here
  END();
}

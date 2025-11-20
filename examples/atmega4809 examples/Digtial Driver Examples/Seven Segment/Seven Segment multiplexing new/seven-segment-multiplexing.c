// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"
#include "TimerDelay.h"
#include "lib/SevenSegmentMulti.h"

SevenSegment_t seg1 = {B0,B1,B2,B3,B4,B5,D6,D7};
SevenSegmentMulti_t multi_seg1;
mos_uint8_t seg_pins1[4] = {C3, C2, C1, C0};
mos_uint8_t dot[4] = {0,0,0,0};
mos_uint8_t reverse[4] = {0,0,1,1};

void timer_callback(void)
{
	SevenSegmentMulti_callback(&multi_seg1);
}

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	SevenSegment_begin(&seg1, ANODE);
	SevenSegmentMulti_begin(&multi_seg1,&seg1,seg_pins1, 4);
	
	TimerDelay.begin(TIMER_DELAY0,1,&timer_callback);
}

// Delcare all processes here
TASK_CREATE(multi1,"Multi Seg-1");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&multi1);

TASK_RUN(multi1)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            
   static int i=0;
  // Process starts here  
  BEGIN();
  
  while(1)
  {
	for(i=0; i<10000; i++)
	{			
		if((i/5)%2)
		{
			dot[1] = 1;
			dot[2] = 1;
		}
		else
		{
			dot[1] = 0;
			dot[2] = 0;
		}
		
		//SevenSegmentMulti_printDotReverse(&multi_seg1, i, dot, reverse);
		SevenSegmentMulti_printDotReverse(&multi_seg1, i, dot, reverse);
		DELAY_SEC_PRECISE(0.1);
	}	 	
  }
  // process ends here
  END();
}

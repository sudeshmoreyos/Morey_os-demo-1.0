// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "TimerDelay.h"
#include "lib/SevenSegmentMulti.h"

SevenSegment_t seg1 = {pin2,pin3,pin4,pin5,pin6,pin7,pin8,pin9};
SevenSegmentMulti_t multi_seg1;
mos_uint8_t seg_pins1[4] = {pinA0, pinA1, pinA2, pinA3};

void timer_callback(void)
{
	SevenSegmentMulti_callback(&multi_seg1);
}

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	SevenSegment_begin(&seg1, CATHODE);
	SevenSegmentMulti_begin(&multi_seg1,&seg1,4,seg_pins1);
	
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
		SevenSegmentMulti_print(&multi_seg1, i);
		DELAY_SEC_PRECISE(0.01);
	}	 	
  }
  // process ends here
  END();
}

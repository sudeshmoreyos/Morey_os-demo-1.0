// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"
#include "TimerDelay.h"
#include "lib/SevenSegmentMulti.h"

SevenSegment_t seg1 = {B7,B6,B5,B4,B3,B2,B1,B0}, seg2={C4,C5,D0,D1,D2,D3,D4,D5};
SevenSegmentMulti_t multi_seg1, multi_seg2;
mos_uint8_t seg_pins1[4] = {C0, C1, C2, C3}, seg_pins2[2] = {D6, D7};

void timer_callback(void)
{
	SevenSegmentMulti_callback(&multi_seg1);
	SevenSegmentMulti_callback(&multi_seg2);
}

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	SevenSegment_begin(&seg1, CATHODE);
	SevenSegmentMulti_begin(&multi_seg1,&seg1,4,seg_pins1);
	
	SevenSegment_begin(&seg2, ANODE);
	SevenSegmentMulti_begin(&multi_seg2,&seg2,2,seg_pins2);
	
	TimerDelay.begin(TIMER_DELAY0,1,&timer_callback);
}

// Delcare all processes here
PROCESS(multi1,"Multi Seg-1");
PROCESS(multi2,"Multi Seg-2");

// Delcare autostart  processes here. Atleast one process must be autostarted;
AUTOSTART_PROCESSES(&multi1, &multi2);

PROCESS_THREAD(multi1)
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
		DELAY_SEC_PRECISE(0.1);
	}	 	
  }
  // process ends here
  END();
}

PROCESS_THREAD(multi2)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            
   static int i=0;
  // Process starts here  
  BEGIN();
  while(1)
  {
	for(i=99; i>=0; i--)
	{
		SevenSegmentMulti_print(&multi_seg2, i);
		DELAY_SEC_PRECISE(1);
	}	 	
  }
  // process ends here
  END();
}
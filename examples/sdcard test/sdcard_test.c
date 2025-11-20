// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "lib/sdcard_new.h"
#include "Serial.h"
#include "Digital.h"
#include "lib/MosTimer.h"
#include "util/stdlib.h"
#include "util/delay_blocking.h"

MosTimer_t timer_obj;

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    	
	delay_ms_blocking(5000);
	Serial.begin(9600,SERIAL_8N1);	
	//Digital.pinmode(B1,OUTPUT);
	
	Serial.println("Welcome");
}

// Delcare all processes here
TASK_CREATE(test1,"LED-1");
TASK_CREATE(test2,"MosTimer Test");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&test1);

TASK_RUN(test1)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables   
	static mos_uint8_t buffer[512];
	static int i,x;

  // Process starts here  
  BEGIN();
  
  x = sdcard_begin(&SpiMaster,B2);
  if(x==0)
	  Serial.println("Success");

  while(1)
  {
	Serial.println("Enter text to save : ");
	
	while(!Serial.available())
	{
		DELAY_SEC(0.1);
	}
	
	DELAY_SEC(1);
	 
	i = 0;
	while(Serial.available())
	{
		buffer[i] = Serial.read();
		i++;
	}
	
	sdcard_write (buffer, 0, 1);	
    DELAY_SEC(1);
	
	for(i=0;i<512;i++)
		buffer[i] = '\0';

	sdcard_read (buffer, 0 , 1);
    DELAY_SEC_PRECISE(1);

	Serial.println("Read Text : ");
	Serial.println(buffer);
  }
  
  // process ends here
  END();
}

TASK_RUN(test2)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables   
  static int count = 0;
  static char s[20];

  // Process starts here  
  BEGIN();
		MosTimer_set(&timer_obj, 2);
  while(1)
  {
	

	while(!MosTimer_expired(&timer_obj))
	{
		DELAY_SEC(0.1);		
	}
	MosTimer_reset(&timer_obj);
	count++;
	Serial.print(" Mos Timer Expired : ");
	itoa(count,s);
	Serial.println(s);
	
  }
  // process ends here
  END();
  
}
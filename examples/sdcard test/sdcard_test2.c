// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "lib/sdcard_new.h"
#include "Serial.h"
#include "Digital.h"
#include "util/stdlib.h"


// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Serial.begin(9600,SERIAL_8N1);
}

// Delcare all processes here
TASK_CREATE(test1,"Test-1");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&test1);

TASK_RUN(test1)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables
  
  mos_uint8_t x,buffer[512];
  int i;

  // Process starts here  
  BEGIN();
  
  DELAY_SEC(5);
  
  x = sdcard_begin(&SpiMaster,B2);
  if(x==0)
	  Serial.println("Success");
  else
	  Serial.println("failed");
	
  while(1)
  {
	Serial.flush();
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
		if(buffer[i] == '\n')
		{
			buffer[i]='\0';
			break;
		}
		i++;
	}
	Serial.println(buffer);
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

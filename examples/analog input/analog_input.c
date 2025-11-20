// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "AnalogInput.h"
#include "Serial.h"
#include "util/stdlib.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Serial.begin(9600,SERIAL_8N1);
	AnalogInput.begin(AI_SPEED_MODE0 , AI_REF_INTERNAL2v56);
}

// Delcare all processes here
PROCESS(analog_example,"Analog Example");

// Delcare autostart  processes here. Atleast one process must be autostarted;
AUTOSTART_PROCESSES(&analog_example);

PROCESS_THREAD(analog_example)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            
	mos_uint16_t x=0;
	float volt=0;
	char s[10];
  // Process starts here  
  BEGIN();    

  while(1)
  {
	x = AnalogInput.read(AI_CHANNEL_BG0V0);
	volt = x * 2.56/1023;
	ftoa(volt,2,s);
	Serial.print("Volt = ");
	Serial.println(s);
	DELAY_SEC(1);
  }
  
  // process ends here
  END();
}
// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"
#include "Serial.h"
#include "lib/debug.h"
#include "util/delay_blocking.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Digital.pinmode(B5,OUTPUT);
	Serial.begin(115200,SERIAL_8N1);
	Serial.terminator("#\r\n",3);
}

// Delcare all processes here
TASK_CREATE(LED1,"LED-1");
TASK_CREATE(read_serial,"Read Serial");
TASK_CREATE(serial_echo,"Serial Echo");

// Delcare autostart  processes here. Atleast one process must be autostarted;
// TASK_AUTOSTART(&LED1, &serial_echo);
TASK_AUTOSTART(&LED1, &read_serial);

TASK_RUN(LED1)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            

  // Process starts here  
  BEGIN();    

  while(1)
  {
	// Serial.println("Hello from Serial-0");
	// Serial1.println("Hello from Serial-1");
	Digital.write(B5,HIGH);
	
	// Serial.begin(115200,SERIAL_8N1);
	DEBUG_USER_CONST_LN("LED ON");
	
	/*
	while(Serial.txComplete()==0)
	{
		delay_ms_blocking(1);
	}	
	Serial.end();
	*/
	
    DELAY_SEC_PRECISE(0.255);

	Digital.write(B5,LOW);
	
	// Serial.begin(115200,SERIAL_8N1);
	DEBUG_USER_CONST_LN("LED OFF");
	/*
	while(Serial.txComplete()==0)
	{
		delay_ms_blocking(1);
	}
	Serial.end();
	*/
	
    DELAY_SEC_PRECISE(0.745);           
  }
  
  // process ends here
  END();
}

TASK_RUN(read_serial)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            
	char x[50];
	int y;
  // Process starts here  
  BEGIN();    

  while(1)
  {
		while(Serial.stringAvailable())
		{
			y = Serial.readString(x, sizeof(x));
			if(y > sizeof(x))
				Serial.println("Read Buffer size limit reached");
			
			Serial.print("String read = ");
			Serial.println(x);
		}
		DELAY_SEC(0.01);
  }
  
  // process ends here
  END();
}

TASK_RUN(serial_echo)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables            

  // task starts here  
  BEGIN();    

  while(1)
  {
		if(Serial.available())
		{
			while(Serial.available())
			{
				Serial.write(Serial.read());
			}
			Serial.printBytes((mos_uint8_t*)"Done\r\n",6);
		}
		DELAY_SEC(0.01);
  }
  
  // process ends here
  END();
}

// functions to write
// delay_Sec with reference to a point int the code
// tick tock tick_tock function.
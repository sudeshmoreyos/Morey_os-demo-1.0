// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "lib/lcd.h"
#include "Digital.h"

lcd_t lcd1;

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	lcd_begin(&lcd1,B5,B4,B3,B2,B1,B0);
	lcd_charLen(&lcd1,16,2);
	Digital.pinmode(C0,OUTPUT);
}

// Delcare all processes here
PROCESS(lcd_test,"lcd_test");
PROCESS(led,"led");

// Delcare autostart  processes here. Atleast one process must be autostarted;
AUTOSTART_PROCESSES(&lcd_test,&led);

PROCESS_THREAD(lcd_test)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables 
	static int i = 0;

  // Process starts here  
  BEGIN();

  while(1)
  {
	for(i=0;i<16;i++)
	{
		lcd_clear(&lcd1);
		lcd_setCursor(&lcd1,i,0);
		lcd_print(&lcd1,"Morey_os");
		DELAY_SEC_PRECISE(1); 
	}
	
	for(i=0;i<16;i++)
	{
		lcd_clear(&lcd1);
		lcd_setCursor(&lcd1,i,1);
		lcd_print(&lcd1,"Morey_os");
		DELAY_SEC_PRECISE(1); 
	}
	
  }
  
  // process ends here
  END();
}

PROCESS_THREAD(led)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables

  // Process starts here  
  BEGIN();

  while(1)
  {
	Digital.write(C0,HIGH);
	DELAY_SEC(0.1);	
	Digital.write(C0,LOW);
	DELAY_SEC(0.1);
  }
  
  // process ends here
  END();
}
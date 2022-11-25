// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "lib/LiquidCrystal.h"
#include "Digital.h"

LiquidCrystal_t lcd={pin13,pin12,pin11,pin10,pin9,pin8};

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	lcd_begin(&lcd,16,2);
	Digital.pinmode(pina0,OUTPUT);
}

// Delcare all processes here
TASK_CREATE(lcd_test,"lcd_test");
TASK_CREATE(led,"led");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&lcd_test,&led);

TASK_RUN(lcd_test)
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
		lcd_clear(&lcd);
		lcd_setCursor(&lcd,i,0);
		lcd_print(&lcd,"Morey_os");
		DELAY_SEC_PRECISE(1); 
	}
	
	for(i=0;i<16;i++)
	{
		lcd_clear(&lcd);
		lcd_setCursor(&lcd,i,1);
		lcd_print(&lcd,"Morey_os");
		DELAY_SEC_PRECISE(1); 
	}
	  
	for(i=0;i<16;i++)
	{
		lcd_clear(&lcd);
		lcd_setCursor(&lcd,i,0);
		lcd_write(&lcd,'A');
		DELAY_SEC_PRECISE(1); 
	}
	
	for(i=0;i<16;i++)
	{
		lcd_clear(&lcd);
		lcd_setCursor(&lcd,i,1);
		lcd_write(&lcd,'A');
		DELAY_SEC_PRECISE(1); 
	}
	
  }
  
  // process ends here
  END();
}

TASK_RUN(led)
{
  // Declare all variables here, please read documentation to understand 
  // which variables should be declared as static variables

  // Process starts here  
  BEGIN();

  while(1)
  {
	Digital.write(pinA0,HIGH);
	DELAY_SEC(0.1);	
	Digital.write(pinA0,LOW);
	DELAY_SEC(0.1);
  }
  
  // process ends here
  END();
}

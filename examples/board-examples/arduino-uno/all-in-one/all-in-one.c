#include "morey_os.h"
#include "Digital.h"
#include "lib/LiquidCrystal.h"
#include "Serial.h"
#include "lib/debug.h"

LiquidCrystal_t lcd = {pin13,pin12,pin11,pin10,pin9,pin8};

void setup(void)
{
	Digital.pinmode(pinA1,OUTPUT);
	Digital.pinmode(pinA0,OUTPUT);
	lcd_begin(&lcd,16,2);
	Serial.begin(9600,SERIAL_8N1);
	DEBUG_INIT(9600);
}

TASK_CREATE(LED,"my_led");
TASK_CREATE(LED1,"my_led1");
TASK_CREATE(LCD,"my_lcd");
TASK_CREATE(serial,"my_serial");

TASK_AUTOSTART(&LED,&LED1,&LCD,&serial);

TASK_RUN(LED)
{
	BEGIN();
	while(1)
	{
		DEBUG_USER_LN("LED ON");
		Digital.write(pinA1,HIGH);
		DELAY_SEC_PRECISE(1);
		
		DEBUG_USER_LN("LED OFF");
		Digital.write(pinA1,LOW);
		DELAY_SEC_PRECISE(1);
		
	}
	END();
}

TASK_RUN(LED1)
{
	BEGIN();
	while(1)
	{
		Digital.write(pinA0,HIGH);
		DELAY_SEC_PRECISE(.1);
		Digital.write(pinA0,LOW);
		DELAY_SEC_PRECISE(.1);
	}
	END();
}

TASK_RUN(LCD)
{
	BEGIN();
	while(1)
	{
		lcd_clear(&lcd);
		lcd_print(&lcd,"NEEVU");
		DELAY_SEC_PRECISE(1);
		lcd_clear(&lcd);
		lcd_setCursor(&lcd,11,1);
		lcd_print(&lcd,"MOREY");
		DELAY_SEC_PRECISE(1);
	}
	END();
}

TASK_RUN(serial)
{
	BEGIN();
	while(1)
	{
		DEBUG_USER("");
		Serial.println("NEEVU");
		DELAY_SEC_PRECISE(1);
		DEBUG_USER("");
		Serial.println("MOREY");
		DELAY_SEC_PRECISE(1);
	}
	END();
}
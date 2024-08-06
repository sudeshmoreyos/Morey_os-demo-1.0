// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Serial.h"
#include "lib/rtc_ds1307.h"
#include "util/stdlib.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Serial.begin(9600,SERIAL_8N1);
	rtc1307_begin(&I2cSoft, 100000, RTC1307_OUT_ZERO);
}

PROCESS(rtc,"RTC");

// Delcare autostart  processes here. Atleast one process must be autostarted;
AUTOSTART_PROCESSES(&rtc);

PROCESS_THREAD(rtc)
{
	mos_uint8_t year, month, date, day, hour, minute, second;
	static char s[5];
	
	BEGIN();
	
	Serial.constPrintln("Welcome");
	rtc1307_setTime(11,15,0);
	rtc1307_setDate(3,18,5,22);
	while(1)
	{
		Serial.constPrint("Date = ");
		
		rtc1307_getDate(&day,&date,&month,&year);
		
		itoa(date,s);
		Serial.print(s);
		Serial.write('-');
		
		itoa(month,s);
		Serial.print(s);
		Serial.write('-');
		
		itoa(year+2000,s);
		Serial.print(s);				
		
		Serial.constPrint(", Time = ");
		
		rtc1307_getTime(&hour,&minute,&second);
		
		itoa(hour,s);
		Serial.print(s);
		Serial.write(':');
		
		itoa(minute,s);
		Serial.print(s);
		Serial.write(':');
		
		itoa(second,s);
		Serial.println(s);
		
		DELAY_SEC_PRECISE(1);
	}
	END();
}
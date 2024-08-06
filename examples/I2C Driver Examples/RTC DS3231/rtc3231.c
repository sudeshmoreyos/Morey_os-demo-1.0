// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Serial.h"
#include "lib/rtc_ds3231.h"
#include "util/stdlib.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Serial.begin(9600,SERIAL_8N1);
	rtc3231_begin(&I2cMaster, 100000, RTC3231_OUT_ONE, RTC3231_SQR_32KHZ_DISABLE);
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
	rtc3231_setTime(11,15,0);
	rtc3231_setDate(3,18,5,22);
	while(1)
	{
		Serial.constPrint("Date = ");
		
		rtc3231_getDate(&day,&date,&month,&year);
		
		itoa(date,s);
		Serial.print(s);
		Serial.write('-');
		
		itoa(month,s);
		Serial.print(s);
		Serial.write('-');
		
		itoa(year+2000,s);
		Serial.print(s);				
		
		Serial.constPrint(", Time = ");
		
		rtc3231_getTime(&hour,&minute,&second);
		
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
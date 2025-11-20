// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Serial.h"
#include "lib/rtc_ds3231.h"
#include "util/stdlib.h"

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Serial6.begin(115200,SERIAL_8N1);
	I2C1.masterBegin(100000);
}

PROCESS(rtc,"RTC");

// Delcare autostart  processes here. Atleast one process must be autostarted;
AUTOSTART_PROCESSES(&rtc);

PROCESS_THREAD(rtc)
{
	static mos_uint8_t year, month, date, day, hour, minute, second;
	static char s[5];
	
	BEGIN();
	DELAY_SEC_PRECISE(10);
	Serial6.println("Welcome");
	//rtc3231_setTime(17,36,10);
	//rtc3231_setDate(6,25,6,22);
	//DELAY_SEC_PRECISE(2);
	while(1)
	{
		Serial6.print("Date = ");
		
		rtc3231_getDate(&day,&date,&month,&year);
		
		itoa(date,s);
		Serial6.print(s);
		Serial6.write('-');
		
		itoa(month,s);
		Serial6.print(s);
		Serial6.write('-');
		
		itoa(year+2000,s);
		Serial6.print(s);				
		
		Serial6.print(", Time = ");
		
		rtc3231_getTime(&hour,&minute,&second);
		
		itoa(hour,s);
		Serial6.print(s);
		Serial6.write(':');
		
		itoa(minute,s);
		Serial6.print(s);
		Serial6.write(':');
		
		itoa(second,s);
		Serial6.println(s);
		
		DELAY_SEC_PRECISE(1);
	}
	END();
}
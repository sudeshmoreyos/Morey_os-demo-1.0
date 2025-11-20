#include "morey_os.h"
#include "i2cMaster.h"
#include "Digital.h"
#include "Serial.h"

void setup(void)
{
	I2cMaster.begin(100000);
	Digital.pinmode(D3,OUTPUT);
	Digital.pinmode(D4,OUTPUT);
	Serial.begin(9600,SERIAL_8N1);
}

TASK_CREATE(i2c_test,"I2c Test");

TASK_AUTOSTART(&i2c_test);

TASK_RUN(i2c_test)
{
	char data_rx[50];
	
	BEGIN();
	
	while(1)
	{
		//Serial.println("Welcome");
		I2cMaster.transfer(0x10,"Welcome", 7,data_rx,50);
		Serial.println(data_rx);
			//Serial.println("Error-1");
			
		
		DELAY_SEC(5);
		
		Digital.write(D3,LOW);
		Digital.write(D4,LOW);
		
		I2cMaster.transfer(0x10,"I am Sudesh", 11,data_rx,50);	
		Serial.println(data_rx);		
		DELAY_SEC(5);
		
		Digital.write(D3,HIGH);
		Digital.write(D4,LOW);
		
		I2cMaster.transfer(0x10,"Who are you?", 12,data_rx,50);	
		Serial.println(data_rx);
		DELAY_SEC(5);
		
		Digital.write(D3,LOW);
		Digital.write(D4,HIGH);
		
		I2cMaster.transfer(0x10,"Take care", 10,data_rx,50);		
		Serial.println(data_rx);
		DELAY_SEC(5);
		
		Digital.write(D3,HIGH);
		Digital.write(D4,HIGH);
	}
	
	END();
}
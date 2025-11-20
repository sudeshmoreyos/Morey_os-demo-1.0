// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "SpiMaster.h"
#include "Serial.h"
#include "TimerDelay.h"
#include "lib/ff.h"
#include "lib/diskio.h"
#include "Digital.h"

FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;			/* File object needed for each open file */
int count=0;

void timer_delay_callback1(void)
{
	disk_timerproc();
	count++;
	if(count==100)
	{
		count=0;
		Digital.write(B0,TOGGLE);
	}
}

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	SpiMaster.begin(B2);
	Serial.begin(9600,SERIAL_8N1);
	TimerDelay.begin(TIMER_DELAY0 , 10, &timer_delay_callback1);
	Digital.pinmode(B1,OUTPUT);
	Digital.pinmode(B0,OUTPUT);
}

// Delcare all processes here
TASK_CREATE(test1,"test-1");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&test1);

TASK_RUN(test1)
{
    // array to hold responses
	UINT bw;
	FRESULT fr;
	
	BEGIN();
	DELAY_SEC(10);
	Serial.println("Welcome!");
	DELAY_SEC(1);
	
	f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */
	
	Serial.println("SD mount!");
	DELAY_SEC(1);
	
	fr = f_open(&Fil, "newfile.txt", FA_WRITE | FA_CREATE_ALWAYS);	/* Create a file */
	if (fr == FR_OK) 
	{
		Serial.println("File Open Success!");
		f_write(&Fil, "It works!\r\n It works Again!\r\n", 29, &bw);	/* Write data to the file */
		fr = f_close(&Fil);							/* Close the file */
		if (fr == FR_OK && bw == 29) {		/* Lights green LED if data written well */
			//Digital.write(B5,HIGH);	/* Set PB4 high */
			Serial.println("File write Success!");
		}
		else
		{
			Serial.println("File write Failure!");
			Serial.print("Error = ");
			Serial.write(fr+'0');
			Serial.println("");
		}
	}
	else
	{
		Serial.println("File Open Failure!");
		Serial.print("Error = ");
		Serial.write(fr+'0');
		Serial.println("");
		Serial.print("Error2 = ");
		Serial.write(FR_WRITE_PROTECTED+'0');
		Serial.println("");
	}


    END();
}

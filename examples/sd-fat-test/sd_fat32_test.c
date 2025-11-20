// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Serial.h"
#include "lib/ff.h"

FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;		/* File object needed for each open file */
int count=0;

// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Serial.begin(9600,SERIAL_8N1);
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
	DELAY_SEC(2);
	Serial.println("Welcome!");
	DELAY_SEC(2);
	
	f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */
	Serial.println("SD Mount!");
	DELAY_SEC(2);
	
	fr = f_open(&Fil, "newfile.txt", FA_WRITE | FA_CREATE_ALWAYS);	/* Create a file */
	if (fr == FR_OK) 
	{
		Serial.println("File Open Success!");
		DELAY_SEC(2);
		f_write(&Fil, "It works!\r\n It works Again!\r\n", 29, &bw);	/* Write data to the file */
		fr = f_close(&Fil);							/* Close the file */
		if (fr == FR_OK && bw == 29) {		/* Lights green LED if data written well */
			Serial.println("File write Success!");
			DELAY_SEC(2);
		}
		else
		{
			Serial.println("File write Failure!");
			Serial.print("Error = ");
			Serial.write( fr+'0');
			Serial.println("");
			DELAY_SEC(2);
		}
	}
	
	else
	{
		Serial.println("File open Failure!");
		Serial.print("Error = ");
		Serial.write( fr+'0');
		Serial.println("");
		DELAY_SEC(2);
	}
    END();
}

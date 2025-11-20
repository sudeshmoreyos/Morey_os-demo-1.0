// Declare here all header files used in the code.h , OS related files are included by default
#include "morey_os.h"
#include "Digital.h"
#include "Serial.h"
#include "ff.h"		/* Declarations of FatFs API */

FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;			/* File object needed for each open file */


// Declare all initialization functions of controller peripherals in the setup function below
void setup(void)
{    
	Serial.begin(9600,SERIAL_8N1);
	Digital.pinmode(B5,OUTPUT);
}

// Delcare all processes here
TASK_CREATE(test1,"test-1");

// Delcare autostart  processes here. Atleast one process must be autostarted;
TASK_AUTOSTART(&test1);

TASK_RUN(test1)
{
    static UINT bw;
	static FRESULT fr;

	BEGIN();
	
	DELAY_SEC(5);
	Serial.println("Welcome!");
	DELAY_SEC(1);
	
	f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */
	
	Serial.println("F_mount!");
	DELAY_SEC(1);
	
	fr = f_open(&Fil, "newfile.txt", FA_WRITE | FA_CREATE_ALWAYS);	/* Create a file */
	if (fr == FR_OK) 
	{
		f_write(&Fil, "It works!\r\n", 11, &bw);	/* Write data to the file */
		fr = f_close(&Fil);							/* Close the file */
		if (fr == FR_OK && bw == 11) 				/* Lights green LED if data written well */
		{		
			Digital.write(B5,HIGH);
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

#include "../../../../os/lib/LiquidCrystal.h"

#ifdef PLATFORM_SUPPORT_CONST_PRINT
	#include "avr/pgmspace.h"

	void lcd_constPrintArch(LiquidCrystal_t * lcd_struct, const char * data_string)
	{
		while (pgm_read_byte(data_string))
		{
			lcd_write(lcd_struct, pgm_read_byte(data_string));
			data_string++;
		}
	}
#endif
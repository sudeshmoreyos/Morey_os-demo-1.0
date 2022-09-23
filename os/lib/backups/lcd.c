#include "lcd.h"

#ifdef PLATFORM_SUPPORT_DIGITAL_IO

#include "Digital.h"
#include "util/delay_blocking.h"

static void write2datapins(lcd_t * lcd_struct, mos_uint8_t data)
{
	mos_uint8_t i=0;
	for(i=0;i<4;i++)
		Digital.write(lcd_struct->lcd_pins[i+2], (data>>i) & 0x01);
	
	Digital.write(lcd_struct->lcd_pins[1],LOW);
	delay_us_blocking(1);
	Digital.write(lcd_struct->lcd_pins[1],HIGH);
	delay_us_blocking(1);
	Digital.write(lcd_struct->lcd_pins[1],LOW);
	delay_us_blocking(100);
}

static void lcd_command_data(lcd_t * lcd_struct, mos_uint8_t data, mos_uint8_t mode)
{
	mos_uint8_t i=0;
	
	Digital.write(lcd_struct->lcd_pins[0], mode);	
	write2datapins(lcd_struct, data >> 4);
	write2datapins(lcd_struct, data & 0x0F);
	for(i=0;i<4;i++)
		Digital.write(lcd_struct->lcd_pins[i+2], 1);
} 

void lcd_begin(lcd_t * lcd_struct, mos_uint8_t rs, mos_uint8_t en,\
	mos_uint8_t d4,mos_uint8_t d5,mos_uint8_t d6,mos_uint8_t d7)
{
	mos_uint8_t i;
	
	lcd_struct->lcd_pins[0] = rs;
	lcd_struct->lcd_pins[1] = en;
	lcd_struct->lcd_pins[2] = d4;
	lcd_struct->lcd_pins[3] = d5;
	lcd_struct->lcd_pins[4] = d6;
	lcd_struct->lcd_pins[5] = d7;
	
	for(i=0;i<6;i++)
		Digital.pinmode(lcd_struct->lcd_pins[i], OUTPUT);
	
	delay_ms_blocking(16);
	
	lcd_struct->cols = 16;
	lcd_struct->rows = 1;
	lcd_struct->x = 0;
	lcd_struct->y = 0;
	
	Digital.write(lcd_struct->lcd_pins[0], LOW);
	
	// Set to 4 bit mode
	write2datapins(lcd_struct, 0x03);
	delay_ms_blocking(5);
	write2datapins(lcd_struct, 0x03);
	delay_ms_blocking(5);
	write2datapins(lcd_struct, 0x03);
	delay_us_blocking(150);
	write2datapins(lcd_struct, 0x02);
	
	// default initialize to 4bitmode, 2 line and 5x8 dots
	lcd_charLen(lcd_struct,16,2);

	// default initialize to lcd display on, cursor off and blink off
	lcd_command_data(lcd_struct, 0x0C, 0);
	
	// default initialize to lcd left entry, decrement shift
	lcd_command_data(lcd_struct, 0x06, 0);
	
	// Clear LCD
	lcd_clear(lcd_struct);

}

void lcd_charLen(lcd_t * lcd_struct, mos_uint8_t cols, mos_uint8_t rows)
{
	lcd_struct->cols = cols;
	lcd_struct->rows = rows;
	
	if(rows == 1)
		lcd_command_data(lcd_struct, 0x20, 0);
	else
		lcd_command_data(lcd_struct, 0x28, 0);
}

void lcd_clear(lcd_t * lcd_struct)
{
	lcd_command_data(lcd_struct, 0x01, 0);
	delay_ms_blocking(2);
	lcd_struct->x=0;
	lcd_struct->y=0;
}

void lcd_setCursor(lcd_t * lcd_struct, mos_uint8_t x, mos_uint8_t y)
{
	if(y==0)
		lcd_command_data(lcd_struct, 0x80 + x, 0);
	
	else if(y==1)
		lcd_command_data(lcd_struct, 0xC0 + x, 0);
	
	lcd_struct->x = x;
	lcd_struct->y = y;
	
}

void lcd_write(lcd_t * lcd_struct, char data)
{	
	if(lcd_struct->x >= lcd_struct->cols)
	{
		lcd_struct->x = 0;
		lcd_struct->y++;
		if(lcd_struct->y >= lcd_struct->rows)
		{
			lcd_struct->y = 0;
		}
		
		lcd_setCursor(lcd_struct, lcd_struct->x, lcd_struct->y);
	}
	
	lcd_command_data(lcd_struct, data, 1);
	delay_ms_blocking(1);
	
	lcd_struct->x++;
}

void lcd_print(lcd_t * lcd_struct, char * data_string)
{
	while(*data_string)
	{
		lcd_write(lcd_struct, *data_string);
		data_string++;
	}	
}

#endif

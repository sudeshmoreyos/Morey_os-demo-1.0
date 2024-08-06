#include "LiquidCrystal.h"

#ifdef PLATFORM_SUPPORT_DIGITAL_IO

#include "Digital.h"
#include "util/delay_blocking.h"

static void write2datapins(LiquidCrystal_t* lcd_struct, mos_uint8_t data)
{	
	Digital.write(lcd_struct->lcd_pin_d4, (data>>0) & 0x01);
	Digital.write(lcd_struct->lcd_pin_d5, (data>>1) & 0x01);
	Digital.write(lcd_struct->lcd_pin_d6, (data>>2) & 0x01);
	Digital.write(lcd_struct->lcd_pin_d7, (data>>3) & 0x01);
	
	Digital.write(lcd_struct->lcd_pin_en,LOW);
	delay_us_blocking(1);
	Digital.write(lcd_struct->lcd_pin_en,HIGH);
	delay_us_blocking(1);
	Digital.write(lcd_struct->lcd_pin_en,LOW);
	delay_us_blocking(100);
}

static void lcd_command_data(LiquidCrystal_t* lcd_struct, mos_uint8_t data, mos_uint8_t mode)
{
	Digital.write(lcd_struct->lcd_pin_rs, mode);	
	write2datapins(lcd_struct, data >> 4);
	write2datapins(lcd_struct, data & 0x0F);
	
	Digital.write(lcd_struct->lcd_pin_d4, 1);
	Digital.write(lcd_struct->lcd_pin_d5, 1);
	Digital.write(lcd_struct->lcd_pin_d6, 1);
	Digital.write(lcd_struct->lcd_pin_d7, 1);
}
	
void lcd_begin(LiquidCrystal_t* lcd_struct, mos_uint8_t cols, mos_uint8_t rows)
{
	Digital.pinmode(lcd_struct->lcd_pin_rs, OUTPUT);
	Digital.pinmode(lcd_struct->lcd_pin_en, OUTPUT);
	Digital.pinmode(lcd_struct->lcd_pin_d4, OUTPUT);
	Digital.pinmode(lcd_struct->lcd_pin_d5, OUTPUT);
	Digital.pinmode(lcd_struct->lcd_pin_d6, OUTPUT);
	Digital.pinmode(lcd_struct->lcd_pin_d7, OUTPUT);
	
	delay_ms_blocking(16);
	
	lcd_struct->cols = cols;
	lcd_struct->rows = rows;
	lcd_struct->x = 0;
	lcd_struct->y = 0;
	
	Digital.write(lcd_struct->lcd_pin_rs, LOW);
	
	// Set to 4 bit mode
	write2datapins(lcd_struct, 0x03);
	delay_ms_blocking(5);
	write2datapins(lcd_struct, 0x03);
	delay_ms_blocking(5);
	write2datapins(lcd_struct, 0x03);
	delay_us_blocking(150);
	write2datapins(lcd_struct, 0x02);
	
	// default initialize to 4bitmode, 2 line and 5x8 dots
	//lcd_charLen(lcd_struct,16,2);

	// default initialize to lcd display on, cursor off and blink off
	lcd_command_data(lcd_struct, 0x0C, 0);
	
	// default initialize to lcd left entry, decrement shift
	lcd_command_data(lcd_struct, 0x06, 0);
	
	// Clear LCD
	lcd_clear(lcd_struct);
	
	if(rows == 1)
		lcd_command_data(lcd_struct, 0x20, 0);
	else
		lcd_command_data(lcd_struct, 0x28, 0);

}

void lcd_clear(LiquidCrystal_t* lcd_struct)
{
	lcd_command_data(lcd_struct, 0x01, 0);
	delay_ms_blocking(2);
	lcd_struct->x=0;
	lcd_struct->y=0;
}

void lcd_setCursor(LiquidCrystal_t* lcd_struct, mos_uint8_t x, mos_uint8_t y)
{
	if(y==0)
		lcd_command_data(lcd_struct, 0x80 + x, 0);
	
	else if(y==1)
		lcd_command_data(lcd_struct, 0xC0 + x, 0);
	
	lcd_struct->x = x;
	lcd_struct->y = y;
	
}

void lcd_write(LiquidCrystal_t* lcd_struct, char data)
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

void lcd_print(LiquidCrystal_t* lcd_struct, char * data_string)
{
	while(*data_string)
	{
		lcd_write(lcd_struct, *data_string);
		data_string++;
	}	
}

#endif

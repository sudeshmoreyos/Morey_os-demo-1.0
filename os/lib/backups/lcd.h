#ifndef LCD_H
#define LCD_H

#include "../../platform.h"
typedef struct lcd_struct 
{
	mos_uint8_t lcd_pins[6];
	mos_uint8_t cols;
	mos_uint8_t rows;
	mos_uint8_t x;
	mos_uint8_t y;	
} lcd_t;

void lcd_begin(lcd_t * lcd_struct, mos_uint8_t rs, mos_uint8_t en, \
	mos_uint8_t d4,mos_uint8_t d5,mos_uint8_t d6,mos_uint8_t d7);
void lcd_charLen(lcd_t * lcd_struct, mos_uint8_t cols, mos_uint8_t rows);
void lcd_clear(lcd_t * lcd_struct);
void lcd_setCursor(lcd_t * lcd_struct, mos_uint8_t x, mos_uint8_t y);
void lcd_write(lcd_t * lcd_struct, char data);
void lcd_print(lcd_t * lcd_struct, char * data_string);


#endif
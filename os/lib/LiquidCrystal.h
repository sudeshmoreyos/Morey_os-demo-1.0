#ifndef LIQUID_CRYSTAL_H
#define LIQUID_CRYSTAL_H

#include "../../platform.h"
typedef struct lcd_struct 
{
	mos_uint8_t lcd_pin_rs;
	mos_uint8_t lcd_pin_en;
	mos_uint8_t lcd_pin_d4;
	mos_uint8_t lcd_pin_d5;
	mos_uint8_t lcd_pin_d6;
	mos_uint8_t lcd_pin_d7;
	mos_uint8_t cols;
	mos_uint8_t rows;
	mos_uint8_t x;
	mos_uint8_t y;
} LiquidCrystal_t;

void lcd_begin(LiquidCrystal_t * lcd_struct, mos_uint8_t cols, mos_uint8_t rows);
void lcd_clear(LiquidCrystal_t * lcd_struct);
void lcd_setCursor(LiquidCrystal_t * lcd_struct, mos_uint8_t x, mos_uint8_t y);
void lcd_write(LiquidCrystal_t * lcd_struct, char data);
void lcd_print(LiquidCrystal_t * lcd_struct, char * data_string);
#ifdef PLATFORM_SUPPORT_CONST_PRINT
	#include "../../arch/arch_liquid_crystal.h"
	void lcd_constPrintArch(LiquidCrystal_t * lcd_struct, const char * data_string);
#endif

#endif
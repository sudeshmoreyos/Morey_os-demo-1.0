/*********************************************************
 **														**
 **		Company : DRMZ SYSTEM INNOVATIONS PVT LTD		**
 **     Project : Morey_os                              **
 **     Author  : Sudesh Morey                          **
 **     version : v1.4 									**
 **   	File    : SevenSegment.c						**
 **														**
 ********************************************************/ 
 
#include "SevenSegment.h"

#ifdef PLATFORM_SUPPORT_DIGITAL_IO
#include "../drivers/Digital.h"

static const mos_uint8_t digit_values[19] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, \
												0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x80, 0x40, 0x00};

mos_uint8_t digit2symbol(mos_uint8_t digit)
{
	return digit_values[digit];
}

mos_uint8_t reverse_symbol(mos_uint8_t symbol)
{
	mos_uint8_t a,b;	
		
	for(a=0; a<3; a++)  //swaping 0, 1 and 2 bit with 3,4 and 5 respectively for upside down seven segment 
	{
		b=a+3;
		if(((symbol&(1<<a))>>a)^((symbol&(1<<b))>>b)) // checcking if both bits are different 
		{
			symbol = symbol ^ (1 << a);
			symbol = symbol ^ (1 << b);
		}
	}
	
	return symbol;
}

void SevenSegment_begin (struct seven_segment_struct * seven_segment_obj, mos_uint8_t segment_type)
{	
	seven_segment_obj->segment_type =  segment_type;
	
	if(seven_segment_obj->segment_pin_a != NOT_USED_PIN )
		Digital.pinmode(seven_segment_obj->segment_pin_a,OUTPUT);

	if(seven_segment_obj->segment_pin_b != NOT_USED_PIN )
		Digital.pinmode(seven_segment_obj->segment_pin_b,OUTPUT);
	
	if(seven_segment_obj->segment_pin_c != NOT_USED_PIN )
		Digital.pinmode(seven_segment_obj->segment_pin_c,OUTPUT);

	if(seven_segment_obj->segment_pin_d != NOT_USED_PIN )
		Digital.pinmode(seven_segment_obj->segment_pin_d,OUTPUT);

	if(seven_segment_obj->segment_pin_e != NOT_USED_PIN )
		Digital.pinmode(seven_segment_obj->segment_pin_e,OUTPUT);

	if(seven_segment_obj->segment_pin_f != NOT_USED_PIN )
		Digital.pinmode(seven_segment_obj->segment_pin_f,OUTPUT);
	
	if(seven_segment_obj->segment_pin_g != NOT_USED_PIN )
		Digital.pinmode(seven_segment_obj->segment_pin_g,OUTPUT);

	if(seven_segment_obj->segment_pin_h != NOT_USED_PIN )
		Digital.pinmode(seven_segment_obj->segment_pin_h,OUTPUT);	
}

void SevenSegment_symbolPrint (struct seven_segment_struct * seven_segment_obj, mos_uint8_t symbol)
{
	if(seven_segment_obj->segment_type == ANODE)
		symbol = ~symbol;
	
	if(seven_segment_obj->segment_pin_a != NOT_USED_PIN )
		Digital.write(seven_segment_obj->segment_pin_a, (symbol & (1<<0))>>0);	
	if(seven_segment_obj->segment_pin_b != NOT_USED_PIN )
		Digital.write(seven_segment_obj->segment_pin_b, (symbol & (1<<1))>>1);	
	if(seven_segment_obj->segment_pin_c != NOT_USED_PIN )
		Digital.write(seven_segment_obj->segment_pin_c, (symbol & (1<<2))>>2);	
	if(seven_segment_obj->segment_pin_d != NOT_USED_PIN )
		Digital.write(seven_segment_obj->segment_pin_d, (symbol & (1<<3))>>3);
	if(seven_segment_obj->segment_pin_e != NOT_USED_PIN )
		Digital.write(seven_segment_obj->segment_pin_e, (symbol & (1<<4))>>4);	
	if(seven_segment_obj->segment_pin_f != NOT_USED_PIN )
		Digital.write(seven_segment_obj->segment_pin_f, (symbol & (1<<5))>>5);	
	if(seven_segment_obj->segment_pin_g != NOT_USED_PIN )
		Digital.write(seven_segment_obj->segment_pin_g, (symbol & (1<<6))>>6);	
	if(seven_segment_obj->segment_pin_h != NOT_USED_PIN )
		Digital.write(seven_segment_obj->segment_pin_h, (symbol & (1<<7))>>7);		
}

void SevenSegment_print (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit)
{	
	SevenSegment_symbolPrint(seven_segment_obj,digit_values[digit]);	
}

void SevenSegment_printDot (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit)
{	
	SevenSegment_symbolPrint(seven_segment_obj,digit_values[digit] | (1<<7));
}

void SevenSegment_printReverse (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit)
{	
	SevenSegment_symbolPrint(seven_segment_obj,reverse_symbol(digit_values[digit]));	
}

void SevenSegment_printDotReverse (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit)
{
	SevenSegment_symbolPrint(seven_segment_obj , ( reverse_symbol(digit_values[digit]) | (1<<7) ));
}

void SevenSegment_customPrint (struct seven_segment_struct * seven_segment_obj, \
mos_uint8_t custom_data_pin_a, mos_uint8_t custom_data_pin_b, mos_uint8_t custom_data_pin_c, mos_uint8_t custom_data_pin_d, \
mos_uint8_t custom_data_pin_e, mos_uint8_t custom_data_pin_f, mos_uint8_t custom_data_pin_g, mos_uint8_t custom_data_pin_h)
{
	mos_uint8_t symbol=0;
	
	if(custom_data_pin_h > 0)
	{
		symbol++;		
	}
	symbol <<= 1;
	
	if(custom_data_pin_g > 0)
	{
		symbol++;
	}
	symbol <<= 1;
	
	if(custom_data_pin_f > 0)
	{
		symbol++;
	}
	symbol <<= 1;
	
	if(custom_data_pin_e > 0)
	{
		symbol++;
	}
	symbol <<= 1;
	
	if(custom_data_pin_d > 0)
	{
		symbol++;		
	}
	symbol <<= 1;
	
	if(custom_data_pin_c > 0)
	{
		symbol++;
	}
	symbol <<= 1;
	
	if(custom_data_pin_b > 0)
	{
		symbol++;
	}
	symbol <<= 1;
	
	if(custom_data_pin_a > 0)
	{
		symbol++;
	}
	
	SevenSegment_symbolPrint(seven_segment_obj, symbol);
	
}


#endif
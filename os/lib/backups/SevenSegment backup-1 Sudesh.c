#include "SevenSegment.h"
#include "../drivers/Digital.h"

mos_uint8_t digit_values[19] = {0x3F, 0x06, 0x7C, 0x4F, 0x66, 0xD6, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x80, 0x00, 0x40};

static mos_uint8_t reverse(mos_uint8_t digit);

void SevenSegment_init (struct seven_segment_struct * seven_segment_obj, mos_uint8_t segment_type, mos_uint8_t * segment_pins)
{
	mos_uint8_t i;
	
	seven_segment_obj->segment_type =  segment_type;
	seven_segment_obj->segment_pins =  segment_pins;
	
	for(i=0;i<8;i++)
	{
		if(segment_pins[i] != NOT_USED_PIN )
			Digital.pinmode(segment_pins[i],OUTPUT);
	}	
}

void SevenSegment_print (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit)
{
	mos_uint8_t j, digit_type = 0, digit_value=0;
	
	digit_type = ( digit & 0xF0 ) >> 4;
	digit = digit & 0xF;
	
	if(seven_segment_obj->segment_type == CATHODE)
		digit_value = digit_values[digit];
	else
		digit_value = ~digit_values[digit];
	
	
	for(j=0;j<8;j++)
	{
		if(seven_segment_obj->segment_pins[j] != NOT_USED_PIN )
		{
			if(digit_type <= 1 )
			{
				Digital.write(seven_segment_obj->segment_pins[j], (digit_value & (1<<j))>>j);				
			}
			
			else if(digit_type <= 3 )
			{
				digit = reverse(digit);
				Digital.write(seven_segment_obj->segment_pins[j], (digit_value & (1<<j))>>j);					
			}
			
			if(digit_type == 1 || digit_type == 3)
			{
				if(seven_segment_obj->segment_type == CATHODE)
					Digital.write(seven_segment_obj->segment_pins[7], HIGH);		
				else
					Digital.write(seven_segment_obj->segment_pins[7], LOW);				
			}

			if(digit_type == 4)
			{
				digit_value += 16;
				Digital.write(seven_segment_obj->segment_pins[j], (digit_value & (1<<j))>>j);
			}
		}
	}
}

void SevenSegment_custom_print (struct seven_segment_struct * seven_segment_obj, mos_uint8_t * custom_data)
{
	mos_uint8_t j;		

	for(j=0;j<8;j++)
	{
		if(seven_segment_obj->segment_pins[j] != NOT_USED_PIN )
		{
			if(seven_segment_obj->segment_type == CATHODE)
			{
				if(custom_data[j] == 0)
					Digital.write(seven_segment_obj->segment_pins[j], LOW);
				else
					Digital.write(seven_segment_obj->segment_pins[j], HIGH);
			}
			else
			{
				if(custom_data[j] == 0)
					Digital.write(seven_segment_obj->segment_pins[j], HIGH);
				else
					Digital.write(seven_segment_obj->segment_pins[j], LOW);
			}
		}
	}
}

static mos_uint8_t reverse(mos_uint8_t digit)
{
	mos_uint8_t reverse_digit = 0, a, b, c, d, e, f, g, h;
	
	a = (digit & (1<<0)) >> 0;
	b = (digit & (1<<1)) >> 1;
	c = (digit & (1<<2)) >> 2;
	d = (digit & (1<<3)) >> 3;
	e = (digit & (1<<4)) >> 4;
	f = (digit & (1<<5)) >> 5;
	g = (digit & (1<<6)) >> 6;
	h = (digit & (1<<7)) >> 7;
	
	
	reverse_digit |= d<<0;
	reverse_digit |= f<<1;
	reverse_digit |= e<<2;
	reverse_digit |= a<<3;
	reverse_digit |= c<<4;
	reverse_digit |= b<<5;
	reverse_digit |= g<<6;
	reverse_digit |= h<<7;
	
	return reverse_digit;
}



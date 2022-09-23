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

const mos_uint8_t digit_values[19] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x80, 0x40, 0x00};

void SevenSegment_begin (struct seven_segment_struct * seven_segment_obj, mos_uint8_t segment_type, mos_uint8_t pina, mos_uint8_t pinb, \
	mos_uint8_t pinc, mos_uint8_t pind,mos_uint8_t pine, mos_uint8_t pinf,mos_uint8_t ping, mos_uint8_t pinh)
{
	mos_uint8_t i;
	
	seven_segment_obj->segment_type =  segment_type;
	seven_segment_obj->segment_pins[0] =  pina;
	seven_segment_obj->segment_pins[1] =  pinb;
	seven_segment_obj->segment_pins[2] =  pinc;
	seven_segment_obj->segment_pins[3] =  pind;
	seven_segment_obj->segment_pins[4] =  pine;
	seven_segment_obj->segment_pins[5] =  pinf;
	seven_segment_obj->segment_pins[6] =  ping;
	seven_segment_obj->segment_pins[7] =  pinh;
	
	for(i=0;i<8;i++)
	{
		if(seven_segment_obj->segment_pins[i] != NOT_USED_PIN )
			Digital.pinmode(seven_segment_obj->segment_pins[i],OUTPUT);
	}	
}

void SevenSegment_print (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit)
{
	mos_uint8_t j, digit_value=0;
	
	if(seven_segment_obj->segment_type == CATHODE)
		digit_value = digit_values[digit];
	else
		digit_value = ~digit_values[digit];
    
	for(j=0;j<8;j++) 
	{
		if(seven_segment_obj->segment_pins[j] != NOT_USED_PIN )
			Digital.write(seven_segment_obj->segment_pins[j], (digit_value & (1<<j))>>j);
	}	
}

void SevenSegment_printDot (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit)
{	
	SevenSegment_print (seven_segment_obj, digit);
	
	if(seven_segment_obj->segment_type == CATHODE)
		Digital.write(seven_segment_obj->segment_pins[7], HIGH);		
	else
		Digital.write(seven_segment_obj->segment_pins[7], LOW);	
}

void SevenSegment_printReverse (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit)
{
	mos_uint8_t a,b,digit_value=0,j=0;
	
	if(seven_segment_obj->segment_type == CATHODE)
		digit_value = digit_values[digit];
	else
		digit_value = ~digit_values[digit];		
		
	for(a=0; a<3; a++)  //swaping 0, 1 and 2 bit with 3,4 and 5 respectively for upside down seven segment 
	{
		b=a+3;
		if(((digit_value&(1<<a))>>a)^((digit_value&(1<<b))>>b)) // checcking if both bits are different 
		{
			digit_value = digit_value ^ (1 << a);
			digit_value = digit_value ^ (1 << b);
		}
	}
    
	for(j=0;j<8;j++) 
	{
		if(seven_segment_obj->segment_pins[j] != NOT_USED_PIN )
			Digital.write(seven_segment_obj->segment_pins[j], (digit_value & (1<<j))>>j);
	}	
}

void SevenSegment_printDotReverse (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit)
{
	SevenSegment_printReverse (seven_segment_obj, digit);
	
	if(seven_segment_obj->segment_type == CATHODE)
		Digital.write(seven_segment_obj->segment_pins[7], HIGH);		
	else
		Digital.write(seven_segment_obj->segment_pins[7], LOW);
}

void SevenSegment_customPrint (struct seven_segment_struct * seven_segment_obj, mos_uint8_t * custom_data)
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


#endif
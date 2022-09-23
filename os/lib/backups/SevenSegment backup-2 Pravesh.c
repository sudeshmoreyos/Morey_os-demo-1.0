#include "SevenSegment.h"
#include "../drivers/Digital.h"

mos_uint8_t digit_values[18] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x80, 0x00};

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

void SevenSegment_print (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit, mos_uint8_t position)
{
	mos_uint8_t j, digit_type = 0, digit_value=0;
	mos_uint8_t p1,p2;
	digit_type = ( digit & 0xF0 ) >> 4;
	digit = digit & 0xF;
	
	if(seven_segment_obj->segment_type == CATHODE)
		digit_value = digit_values[digit];
	else
		digit_value = ~digit_values[digit];
	
	if(position == 0)
	{
	 for(p1=0; p1<3; p1++)  //swaping 0, 1 and 2 bit with 3,4 and 5 respectively for upside down seven segment 
	 {
		 p2=p1+3;
		 if(((digit_value&(1<<p1))>>p1)^((digit_value&(1<<p2))>>p2))
		 {
			 digit_value = digit_value ^ (1 << p1);
			 digit_value = digit_value ^ (1 << p2);
		 }
	 }
	 /*p1=6; p2=7;  // swaping 6th and 7th bit for upside down seven segment 
     if(((digit_value&(1<<p1))>>p1)^((digit_value&(1<<p2))>>p2))
	 {
	  digit_value = digit_value ^ (1 << p1);
	  digit_value = digit_value ^ (1 << p2);
	 }*/	 
		
	}
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
	mos_uint8_t reverse_digit = 0;
	
	reverse_digit = digit;
	
	return reverse_digit;
}
#include "SevenSegmentMulti.h"

#ifdef PLATFORM_SUPPORT_DIGITAL_IO
#include "../drivers/Digital.h"

static void memset_temp(mos_uint8_t * data, mos_uint8_t len)
{
	while(len)
	{
		data[len-1] = 0;
		len--;
	}
}

void SevenSegmentMulti_begin (SevenSegmentMulti_t * seven_segment_multi_obj, SevenSegment_t * seven_segment_obj, \
	mos_uint8_t segment_count, mos_uint8_t * segment_common_pins)
{
	mos_uint8_t i;
	seven_segment_multi_obj -> seven_segment_obj = seven_segment_obj;
	seven_segment_multi_obj -> segment_count = segment_count;
	seven_segment_multi_obj ->segment_common_pins = segment_common_pins;
	seven_segment_multi_obj -> count = 0;

	for(i=0; i<segment_count;i++)
	{
		Digital.pinmode(seven_segment_multi_obj ->segment_common_pins[i], OUTPUT);
		
		if(seven_segment_multi_obj->seven_segment_obj->segment_type == CATHODE)
			Digital.write(seven_segment_multi_obj ->segment_common_pins[i], HIGH);
		else if(seven_segment_multi_obj->seven_segment_obj->segment_type == ANODE)
			Digital.write(seven_segment_multi_obj ->segment_common_pins[i], LOW);
	}
}

void SevenSegmentMulti_print (SevenSegmentMulti_t * seven_segment_multi_obj, mos_uint16_t number)
{
	mos_uint8_t dot_position[seven_segment_multi_obj->segment_count];
	mos_uint8_t reverse_position[seven_segment_multi_obj->segment_count];
	
	memset_temp(dot_position, seven_segment_multi_obj->segment_count);
	memset_temp(reverse_position, seven_segment_multi_obj->segment_count);
	
	SevenSegmentMulti_printDotReverse(seven_segment_multi_obj, number, dot_position, reverse_position);
}

void SevenSegmentMulti_printDot(SevenSegmentMulti_t * seven_segment_multi_obj, mos_uint16_t number,\
	mos_uint8_t * dot_position)
{
	mos_uint8_t reverse_position[seven_segment_multi_obj->segment_count];
	
	memset_temp(reverse_position, seven_segment_multi_obj->segment_count);
	SevenSegmentMulti_printDotReverse(seven_segment_multi_obj, number, dot_position, reverse_position);
}

void SevenSegmentMulti_printDotReverse(SevenSegmentMulti_t * seven_segment_multi_obj, mos_uint16_t number,\
	mos_uint8_t * dot_position, mos_uint8_t * reverse_position)
{
	mos_uint8_t i = 0;
	
	for(i = (seven_segment_multi_obj -> segment_count); i>0; i--)
	{
		seven_segment_multi_obj -> digit_display[i-1] = number%10;
		number = number/10;
		if(dot_position[i-1] > 0)
		{
			seven_segment_multi_obj -> digit_display[i-1] += 16;
		}
		
		if(reverse_position[i-1] > 0)
		{
			seven_segment_multi_obj -> digit_display[i-1] += 32;
		}
	}
}

void SevenSegmentMulti_callback(SevenSegmentMulti_t * seven_segment_multi_obj)
{
	mos_uint8_t i;
		
	for(i=0; i<seven_segment_multi_obj->segment_count;i++)
	{
		if(seven_segment_multi_obj->seven_segment_obj->segment_type == CATHODE)
			Digital.write(seven_segment_multi_obj ->segment_common_pins[i], HIGH);
		
		else if(seven_segment_multi_obj->seven_segment_obj->segment_type == ANODE)
			Digital.write(seven_segment_multi_obj ->segment_common_pins[i], LOW);			
	}
	SevenSegment_print (seven_segment_multi_obj->seven_segment_obj, \
	seven_segment_multi_obj->digit_display[seven_segment_multi_obj->count]);
	
	if(seven_segment_multi_obj->seven_segment_obj->segment_type == CATHODE)
		Digital.write(seven_segment_multi_obj->segment_common_pins[seven_segment_multi_obj->count], LOW);
	
	else if(seven_segment_multi_obj->seven_segment_obj->segment_type == ANODE)
		Digital.write(seven_segment_multi_obj->segment_common_pins[seven_segment_multi_obj->count], HIGH);
	
	seven_segment_multi_obj->count++;
	if(seven_segment_multi_obj->count >= seven_segment_multi_obj->segment_count)
		seven_segment_multi_obj->count = 0;
	
}

#endif

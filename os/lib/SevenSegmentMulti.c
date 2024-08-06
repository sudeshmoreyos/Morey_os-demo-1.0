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
	mos_uint8_t * segment_common_pins, mos_uint8_t segment_count)
{
	mos_uint8_t i;
	seven_segment_multi_obj -> seven_segment_obj = seven_segment_obj;
	seven_segment_multi_obj -> segment_count = segment_count;
	seven_segment_multi_obj -> segment_common_pins = segment_common_pins;
	seven_segment_multi_obj -> count = 0;
	
	if( seven_segment_multi_obj->seven_segment_obj->segment_type == CATHODE)
		seven_segment_multi_obj -> switch_on = LOW;
	
	else if( seven_segment_multi_obj->seven_segment_obj->segment_type == ANODE)
		seven_segment_multi_obj -> switch_on = HIGH;
	

	for(i=0; i<segment_count;i++)
	{
		seven_segment_multi_obj->symbol_display[i] = 0;
		Digital.pinmode(seven_segment_multi_obj->segment_common_pins[i], OUTPUT);
		
		if(seven_segment_multi_obj->seven_segment_obj->segment_type == CATHODE)
			Digital.write(seven_segment_multi_obj ->segment_common_pins[i], HIGH);
		else if(seven_segment_multi_obj->seven_segment_obj->segment_type == ANODE)
			Digital.write(seven_segment_multi_obj ->segment_common_pins[i], LOW);
	}
}

void SevenSegmentMulti_callback(SevenSegmentMulti_t * seven_segment_multi_obj)
{
	mos_uint8_t i;		
		
	for(i=0; i<seven_segment_multi_obj->segment_count;i++)
		Digital.write(seven_segment_multi_obj->segment_common_pins[i], !(seven_segment_multi_obj->switch_on));
	
	SevenSegment_symbolPrint(seven_segment_multi_obj->seven_segment_obj, \
		seven_segment_multi_obj->symbol_display[seven_segment_multi_obj->count]);
	
	Digital.write(seven_segment_multi_obj->segment_common_pins[seven_segment_multi_obj->count], seven_segment_multi_obj->switch_on);
	
	seven_segment_multi_obj->count++;
	if(seven_segment_multi_obj->count >= seven_segment_multi_obj->segment_count)
		seven_segment_multi_obj->count = 0;	
}

void SevenSegmentMulti_config(SevenSegmentMulti_t * seven_segment_multi_obj, mos_uint8_t mode)
{
	if(mode == NORMAL_MODE)
	{
		if( seven_segment_multi_obj->seven_segment_obj->segment_type == CATHODE)
			seven_segment_multi_obj -> switch_on = LOW;
	
		else if( seven_segment_multi_obj->seven_segment_obj->segment_type == ANODE)
			seven_segment_multi_obj -> switch_on = HIGH;
	}
	
	else if(mode == INVERTED_MODE)
	{
		if( seven_segment_multi_obj->seven_segment_obj->segment_type == CATHODE)
			seven_segment_multi_obj -> switch_on = HIGH;
	
		else if( seven_segment_multi_obj->seven_segment_obj->segment_type == ANODE)
			seven_segment_multi_obj -> switch_on = LOW;
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

void SevenSegmentMulti_printReverse (SevenSegmentMulti_t * seven_segment_multi_obj, mos_uint16_t number, \
	mos_uint8_t * reverse_position)
{
	mos_uint8_t dot_position[seven_segment_multi_obj->segment_count];
	
	memset_temp(dot_position, seven_segment_multi_obj->segment_count);
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
	mos_uint8_t i = 0, temp_symbol;
	
	for(i = (seven_segment_multi_obj -> segment_count); i>0; i--)
	{
		temp_symbol = digit2symbol(number%10);
		number = number/10;
		
		if(reverse_position[i-1] > 0)
			temp_symbol = reverse_symbol(temp_symbol);
		
		if(dot_position[i-1] > 0)
			temp_symbol |= 1<<7;
		
		seven_segment_multi_obj -> symbol_display[i-1] = temp_symbol;
	}
}

void SevenSegmentMulti_customPrint (struct seven_segment_multi_struct * seven_segment_multi_obj, mos_uint8_t * * custom_data)
{
	mos_uint8_t i, j, temp_symbol;
	
	for(i = (seven_segment_multi_obj -> segment_count); i>0; i--)
	{	
		temp_symbol=0;
		for(j=7;j>=0;j--)
		{
			temp_symbol <<= 1;
			if(custom_data[(seven_segment_multi_obj -> segment_count)-i][j] > 0)
				temp_symbol |= 1;
		}			
		seven_segment_multi_obj -> symbol_display[i-1] = temp_symbol;
	}	
}

#endif


#ifndef SEVENSEGMENTMULTI_H
#define SEVENSEGMENTMULTI_H

#include "../../platform.h"

#ifdef PLATFORM_SUPPORT_DIGITAL_IO
#include "SevenSegment.h"

typedef struct seven_segment_multi_struct 
{
	SevenSegment_t * seven_segment_obj;
	mos_uint8_t segment_count;
	mos_uint8_t * segment_common_pins;
	mos_uint8_t digit_display[8];
	mos_uint8_t count;
} SevenSegmentMulti_t;

void SevenSegmentMulti_begin (SevenSegmentMulti_t * seven_segment_multi_obj, SevenSegment_t * seven_segment_obj, \
	mos_uint8_t segment_count, mos_uint8_t * segment_common_pins);
void SevenSegmentMulti_print (SevenSegmentMulti_t * seven_segment_multi_obj, mos_uint16_t number);
void SevenSegmentMulti_printDot(SevenSegmentMulti_t * seven_segment_multi_obj, mos_uint16_t number,\
	mos_uint8_t * dot_position);
void SevenSegmentMulti_printDotReverse(SevenSegmentMulti_t * seven_segment_multi_obj, mos_uint16_t number,\
	mos_uint8_t * dot_position, mos_uint8_t * reverse_position);
void SevenSegmentMulti_printDigits(SevenSegmentMulti_t * seven_segment_multi_obj, mos_uint16_t * digits);
void SevenSegmentMulti_callback(SevenSegmentMulti_t * seven_segment_multi_obj);
//void SevenSegment_custom_print (struct seven_segment_struct * seven_segment_obj, mos_uint8_t * custom_data);
	
#endif
#endif
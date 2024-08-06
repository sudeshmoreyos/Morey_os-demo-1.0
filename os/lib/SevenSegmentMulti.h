#ifndef SEVENSEGMENTMULTI_H
#define SEVENSEGMENTMULTI_H

#include "../../platform.h"

#ifdef PLATFORM_SUPPORT_DIGITAL_IO
#include "SevenSegment.h"

#define NORMAL_MODE 0
#define INVERTED_MODE 1

typedef struct seven_segment_multi_struct 
{
	SevenSegment_t * seven_segment_obj;
	mos_uint8_t segment_count;
	mos_uint8_t * segment_common_pins;
	mos_uint8_t symbol_display[8];
	mos_uint8_t count;
	mos_uint8_t switch_on;
	mos_uint8_t switch_off;
} SevenSegmentMulti_t;

void SevenSegmentMulti_begin (SevenSegmentMulti_t * seven_segment_multi_obj, SevenSegment_t * seven_segment_obj, \
	mos_uint8_t * segment_common_pins, mos_uint8_t segment_count);
void SevenSegmentMulti_callback(SevenSegmentMulti_t * seven_segment_multi_obj);
void SevenSegmentMulti_config(SevenSegmentMulti_t * seven_segment_multi_obj, mos_uint8_t mode);
void SevenSegmentMulti_print (SevenSegmentMulti_t * seven_segment_multi_obj, mos_uint16_t number);
void SevenSegmentMulti_printReverse (SevenSegmentMulti_t * seven_segment_multi_obj, mos_uint16_t number, mos_uint8_t * reverse_position);
void SevenSegmentMulti_printDot(SevenSegmentMulti_t * seven_segment_multi_obj, mos_uint16_t number,\
	mos_uint8_t * dot_position);
void SevenSegmentMulti_printDotReverse(SevenSegmentMulti_t * seven_segment_multi_obj, mos_uint16_t number,\
	mos_uint8_t * dot_position, mos_uint8_t * reverse_position);
void SevenSegmentMulti_customPrint (struct seven_segment_multi_struct * seven_segment_multi_obj, mos_uint8_t * * custom_data);
	
#endif
#endif
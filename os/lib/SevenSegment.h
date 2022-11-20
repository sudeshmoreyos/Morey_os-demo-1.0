/*********************************************************
 **														**
 **		Company : DRMZ SYSTEM INNOVATIONS PVT LTD		**
 **     Project : Morey_os                              **
 **     Author  : Sudesh Morey                          **
 **     version : v1.4 									**
 **   	File    : SevenSegment.h						**
 **														**
 ********************************************************/ 

#ifndef SEVENSEGMENT_H
#define SEVENSEGMENT_H

#include "../../platform.h"

#ifdef PLATFORM_SUPPORT_DIGITAL_IO

#define CATHODE 0
#define ANODE 1
#define NOT_USED_PIN 0xFF

#define DIGIT0 0x0
#define DIGIT1 0x1
#define DIGIT2 0x2
#define DIGIT3 0x3
#define DIGIT4 0x4
#define DIGIT5 0x5
#define DIGIT6 0x6
#define DIGIT7 0x7
#define DIGIT8 0x8
#define DIGIT9 0x9
#define DIGITA 0xA
#define DIGITB 0xB
#define DIGITC 0xC
#define DIGITD 0xD
#define DIGITE 0xE
#define DIGITF 0xF
#define DIGIT_DOT 0x10
#define DIGIT_DASH 0x11
#define DIGIT_BLANK 0x12

typedef struct seven_segment_struct 
{
	mos_uint8_t segment_pin_a;
	mos_uint8_t segment_pin_b;
	mos_uint8_t segment_pin_c;
	mos_uint8_t segment_pin_d;
	mos_uint8_t segment_pin_e;
	mos_uint8_t segment_pin_f;
	mos_uint8_t segment_pin_g;
	mos_uint8_t segment_pin_h;	
	mos_uint8_t segment_type;	
} SevenSegment_t;

void SevenSegment_begin (struct seven_segment_struct * seven_segment_obj, mos_uint8_t segment_type);
void SevenSegment_print (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit);
void SevenSegment_printDot (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit);
void SevenSegment_printReverse (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit);
void SevenSegment_printDotReverse (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit);
void SevenSegment_customPrint (struct seven_segment_struct * seven_segment_obj, \
mos_uint8_t custom_data_pin_a, mos_uint8_t custom_data_pin_b, mos_uint8_t custom_data_pin_c, mos_uint8_t custom_data_pin_d, \
mos_uint8_t custom_data_pin_e, mos_uint8_t custom_data_pin_f, mos_uint8_t custom_data_pin_g, mos_uint8_t custom_data_pin_h);

#endif	
#endif

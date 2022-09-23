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
	mos_uint8_t segment_type;
	mos_uint8_t segment_pins[8];
} SevenSegment_t;

void SevenSegment_begin (struct seven_segment_struct * seven_segment_obj, mos_uint8_t segment_type, mos_uint8_t pina, mos_uint8_t pinb, \
	mos_uint8_t pinc, mos_uint8_t pind,mos_uint8_t pine, mos_uint8_t pinf,mos_uint8_t ping, mos_uint8_t pinh);
void SevenSegment_print (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit);
void SevenSegment_printDot (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit);
void SevenSegment_printReverse (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit);
void SevenSegment_printDotReverse (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit);
void SevenSegment_customPrint (struct seven_segment_struct * seven_segment_obj, mos_uint8_t * custom_data);

#endif	
#endif
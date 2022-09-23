#ifndef SEVENSEGMENT_H
#define SEVENSEGMENT_H

#include "../../platform.h"

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
#define DIGIT_DOT0 0x10
#define DIGIT_DOT1 0x11
#define DIGIT_DOT2 0x12
#define DIGIT_DOT3 0x13
#define DIGIT_DOT4 0x14
#define DIGIT_DOT5 0x15
#define DIGIT_DOT6 0x16
#define DIGIT_DOT7 0x17
#define DIGIT_DOT8 0x18
#define DIGIT_DOT9 0x19
#define DIGIT_DOTA 0x1A
#define DIGIT_DOTB 0x1B
#define DIGIT_DOTC 0x1C
#define DIGIT_DOTD 0x1D
#define DIGIT_DOTE 0x1E
#define DIGIT_DOTF 0x1F
#define DIGIT_REVERSE0 0x20
#define DIGIT_REVERSE1 0x21
#define DIGIT_REVERSE2 0x22
#define DIGIT_REVERSE3 0x23
#define DIGIT_REVERSE4 0x24
#define DIGIT_REVERSE5 0x25
#define DIGIT_REVERSE6 0x26
#define DIGIT_REVERSE7 0x27
#define DIGIT_REVERSE8 0x28
#define DIGIT_REVERSE9 0x29
#define DIGIT_REVERSEA 0x2A
#define DIGIT_REVERSEB 0x2B
#define DIGIT_REVERSEC 0x2C
#define DIGIT_REVERSED 0x2D
#define DIGIT_REVERSEE 0x2E
#define DIGIT_REVERSEF 0x2F
#define DIGIT_REVERSE_DOT0 0x30
#define DIGIT_REVERSE_DOT1 0x31
#define DIGIT_REVERSE_DOT2 0x32
#define DIGIT_REVERSE_DOT3 0x33
#define DIGIT_REVERSE_DOT4 0x34
#define DIGIT_REVERSE_DOT5 0x35
#define DIGIT_REVERSE_DOT6 0x36
#define DIGIT_REVERSE_DOT7 0x37
#define DIGIT_REVERSE_DOT8 0x38
#define DIGIT_REVERSE_DOT9 0x39
#define DIGIT_REVERSE_DOTA 0x3A
#define DIGIT_REVERSE_DOTB 0x3B
#define DIGIT_REVERSE_DOTC 0x3C
#define DIGIT_REVERSE_DOTD 0x3D
#define DIGIT_REVERSE_DOTE 0x3E
#define DIGIT_REVERSE_DOTF 0x3F
#define DIGIT_DOT 0x40
#define DIGIT_BLANK 0x41
	

typedef struct seven_segment_struct 
{
	mos_uint8_t segment_type;
	mos_uint8_t * segment_pins;
} SevenSegment_t;

void SevenSegment_init (struct seven_segment_struct * seven_segment_obj, mos_uint8_t segment_type, mos_uint8_t * segment_pins);
void SevenSegment_print (struct seven_segment_struct * seven_segment_obj, mos_uint8_t digit, mos_uint8_t position);
void SevenSegment_custom_print (struct seven_segment_struct * seven_segment_obj, mos_uint8_t * custom_data);
	
#endif
#ifndef RINGBUF_H
#define RINGBUF_H

#include "../../platform.h"

typedef struct ringbuf_struct 
{
	mos_uint8_t * buffer;
	mos_uint16_t buffer_size;
	volatile mos_uint16_t write_index;
	volatile mos_uint16_t read_index;
	volatile mos_uint16_t counter;
} ringbuf_struct_t;

void ringbuf_init(ringbuf_struct_t * ringbuf, mos_uint8_t * data_buffer, mos_uint16_t data_buffer_size);
mos_uint8_t ringbuf_write(ringbuf_struct_t * ringbuf, mos_uint8_t data);
mos_uint8_t ringbuf_read(ringbuf_struct_t * ringbuf);
mos_uint16_t ringbuf_count(const ringbuf_struct_t * ringbuf);
mos_uint8_t ringbuf_isfull(const ringbuf_struct_t * ringbuf);
void ringbuf_flush(ringbuf_struct_t * ringbuf);
mos_uint16_t ringbuf_space(const ringbuf_struct_t *ringbuf);
mos_uint8_t ringbuf_peek(const ringbuf_struct_t *ringbuf);
#endif
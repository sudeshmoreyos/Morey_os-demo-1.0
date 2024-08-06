#include "ringbuf.h"

// platform.h is included to include definition of os data types mos_uint8_t, mos_uint16_t;
// platform.h also includes function Macros GLOBAL_INTERRUPT_DISABLE() and GLOBAL_INTERRUPT_ENABLE();

void ringbuf_init(ringbuf_struct_t * ringbuf, mos_uint8_t * data_buffer, mos_uint16_t data_buffer_size)
{
	ringbuf->buffer = data_buffer;
	ringbuf->buffer_size = data_buffer_size;
	ringbuf_flush(ringbuf);
}

mos_uint8_t ringbuf_write(ringbuf_struct_t * ringbuf, mos_uint8_t data)
{
	mos_uint8_t atomic_status;
	
	if(ringbuf->full)
	{
		return 0;
	}
	else
	{
		ringbuf->buffer[ringbuf->write_index] = data;
		ringbuf->write_index++;
		ringbuf->write_index %= ringbuf->buffer_size;
		
		atomic_status = IS_ATOMIC_OFF();		
		if(atomic_status)
			ATOMIC_ON();
		
		ringbuf->counter++;		
		if(ringbuf->counter == ringbuf->buffer_size)
		{
			ringbuf->full = 1;
		}
		
		if(atomic_status)
			ATOMIC_OFF();
	}
	return 1;
}

mos_uint8_t ringbuf_read(ringbuf_struct_t * ringbuf)
{
	mos_uint8_t x=0xFF;
	mos_uint8_t atomic_status;
	
	if( ringbuf->counter == 0 )
	{
		return 0xFF;
	}
	else
	{
		x = ringbuf->buffer[ringbuf->read_index];
		ringbuf->read_index++;
		ringbuf->read_index %= ringbuf->buffer_size;
		
		atomic_status = IS_ATOMIC_OFF();		
		if(atomic_status)
			ATOMIC_ON();
		
		ringbuf->counter--;
		ringbuf->full = 0;
		
		if(atomic_status)
			ATOMIC_OFF();		
	}
	return x;
}

mos_uint16_t ringbuf_count(ringbuf_struct_t * ringbuf)
{
	return ringbuf->counter;
}

mos_uint8_t ringbuf_isfull(ringbuf_struct_t * ringbuf)
{
	return ringbuf->full;
}

void ringbuf_flush(ringbuf_struct_t * ringbuf)
{
	mos_uint8_t atomic_status;
	
	atomic_status = IS_ATOMIC_OFF();		
	if(atomic_status)
		ATOMIC_ON();
	
	ringbuf->write_index = 0;
	ringbuf->read_index = 0;
	ringbuf->counter = 0;
	ringbuf->full = 0;
	
	if(atomic_status)
		ATOMIC_OFF();
}

/*

mos_uint8_t ringbuf_write_noAtomic(ringbuf_struct_t * ringbuf, mos_uint8_t data)
{
	if(ringbuf->full)
	{
		return 0;
	}
	else
	{
		ringbuf->buffer[ringbuf->write_index] = data;
		ringbuf->write_index++;
		ringbuf->write_index %= ringbuf->buffer_size;
		
		ringbuf->counter++;		
		if(ringbuf->counter == ringbuf->buffer_size)
		{
			ringbuf->full = 1;
		}
	}
	return 1;
}

mos_uint8_t ringbuf_read_noAtomic(ringbuf_struct_t * ringbuf)
{
	mos_uint8_t x=0xFF;
	
	if( ringbuf->counter == 0 )
	{
		return 0xFF;
	}
	else
	{
		x = ringbuf->buffer[ringbuf->read_index];
		ringbuf->read_index++;
		ringbuf->read_index %= ringbuf->buffer_size;
		
		ringbuf->counter--;
		ringbuf->full = 0;		
	}
	return x;
}

void ringbuf_flush_noAtomic(ringbuf_struct_t * ringbuf)
{	
	ringbuf->write_index = 0;
	ringbuf->read_index = 0;
	ringbuf->counter = 0;
	ringbuf->full = 0;
}
*/
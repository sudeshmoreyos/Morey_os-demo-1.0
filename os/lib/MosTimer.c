#ifndef DISABLE_OS

#include "MosTimer.h"
#include "../../platform.h"

void MosTimer_set(struct mos_timer_struct * mos_timer_obj, float time_sec)
{
	MosTimer_reset(mos_timer_obj);
	mos_timer_obj->expiry_seconds = time_sec;
}

void MosTimer_reset(struct mos_timer_struct * mos_timer_obj)
{
	get_clock_time( &(mos_timer_obj->start_second), &(mos_timer_obj->start_millisecond) );
}
		
mos_uint8_t MosTimer_expired(struct mos_timer_struct * mos_timer_obj)
{
	clock_second_t current_second=0;
	clock_millisecond_t current_millisecond=0;
	float time_elapsed;
	
	get_clock_time( &current_second, &current_millisecond );
	
	time_elapsed = (float)current_second - (float)(mos_timer_obj->start_second) \
		+ ( (float)current_millisecond - (float)(mos_timer_obj->start_millisecond) )/1000.0;
		
	return time_elapsed >= (mos_timer_obj->expiry_seconds) ? 1 : 0;	
}

#endif

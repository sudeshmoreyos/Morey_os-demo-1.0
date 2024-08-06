#ifndef MOSTIMER_H
#define MOSTIMER_H

#include "../sys/clock.h"

typedef struct mos_timer_struct 
{
	clock_second_t start_second;
	clock_millisecond_t start_millisecond;
	float expiry_seconds;
} MosTimer_t;

void MosTimer_set(struct mos_timer_struct * mos_timer_obj, float time_sec);
void MosTimer_reset(struct mos_timer_struct * mos_timer_obj);
mos_uint8_t MosTimer_expired(struct mos_timer_struct * mos_timer_obj);

#endif
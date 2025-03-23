#ifndef __YAC_TIMER_H__
#define __YAC_TIMER_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct yac_timer {
	uint64_t delta;
	uint64_t old;
	float accumulator;
	float rate;
} yac_timer;

yac_timer yac_timer_new(const float rate);

/// Returns true when logic should be updated.
bool yac_timer_update(yac_timer *timer);

#endif

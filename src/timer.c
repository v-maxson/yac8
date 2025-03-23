#include "timer.h"
#include "SDL.h"

yac_timer yac_timer_new(const float rate)
{
	yac_timer timer;
	timer.delta = 0;
	timer.old = 0;
	timer.accumulator = 0;
	timer.rate = rate;
	return timer;
}

bool yac_timer_update(yac_timer *timer)
{
	const uint64_t now = SDL_GetPerformanceCounter();
	timer->delta = now - timer->old;
	timer->old = now;
	timer->accumulator += (float)timer->delta / SDL_GetPerformanceFrequency();
	if (timer->accumulator >= timer->rate) {
		timer->accumulator = 0;
		return true;
	}
	return false;
}
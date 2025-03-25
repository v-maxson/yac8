#include "timer.h"
#include "SDL.h"

yac_timer yac_timer_new(const uint64_t rate)
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
	timer->delta = (double)(now - timer->old);
	timer->old = now;

	// Convert to seconds
	double deltaTime = timer->delta / (double)SDL_GetPerformanceFrequency();

	// Cap the delta time to prevent spiral of death after pauses/debugging
	if (deltaTime > 0.25) // Max 0.25 seconds
		deltaTime = 0.25;

	timer->accumulator += deltaTime;

	// Only return true once per time step
	if (timer->accumulator >= 1.0 / (double)timer->rate) {
		timer->accumulator -= 1.0 / (double)timer->rate;
		return true;
	}
	return false;
}
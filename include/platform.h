#ifndef __YAC_PLATFORM_LAYER_H__
#define __YAC_PLATFORM_LAYER_H__

#include <SDL.h>
#include "memory.h"

typedef struct yac_platform {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
} yac_platform;

typedef struct yac_platform_config {
	const char *window_title;
	int window_width;
	int window_height;
} yac_platform_config;

yac_platform yac_platform_new(const yac_platform_config config);
void yac_platform_del(yac_platform *platform_layer);

void yac_platform_render(const yac_platform *platform_layer, const yac_memory_bool *display_memory,
	const size_t display_width, const size_t display_height, const uint32_t color);

#endif

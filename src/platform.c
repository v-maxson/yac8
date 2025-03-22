#include "platform.h"

yac_platform yac_platform_new(const yac_platform_config config)
{
	SDL_Window *window =
		SDL_CreateWindow(config.window_title, SDL_WINDOWPOS_CENTERED,
				 SDL_WINDOWPOS_CENTERED, config.window_width,
				 config.window_height, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer =
		SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture *texture = SDL_CreateTexture(
		renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
		config.window_width, config.window_height);

	return (yac_platform){ .window = window,
			       .renderer = renderer,
			       .texture = texture };
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void yac_platform_del(yac_platform *platform_layer)
{
	SDL_DestroyTexture(platform_layer->texture);
	SDL_DestroyRenderer(platform_layer->renderer);
	SDL_DestroyWindow(platform_layer->window);
}

void yac_platform_render(const yac_platform *platform_layer,
			 const yac_memory_bool *display_memory,
			 const size_t display_width,
			 const size_t display_height, const uint32_t color)
{
	// Map display memory to an array of uint32_t ARGB color values. True should
	// map to the color value, false should map to its inverse.
	uint32_t *pixels =
		malloc(display_width * display_height * sizeof(uint32_t));
	for (size_t i = 0; i < display_width * display_height; i++) {
		pixels[i] = display_memory->data[i] ? color : ~color;
	}

	SDL_UpdateTexture(platform_layer->texture, NULL, pixels,
			  sizeof(uint32_t) * display_width);
	SDL_RenderClear(platform_layer->renderer);
	SDL_RenderCopy(platform_layer->renderer, platform_layer->texture, NULL,
		       NULL);
	SDL_RenderPresent(platform_layer->renderer);

	free(pixels);
}
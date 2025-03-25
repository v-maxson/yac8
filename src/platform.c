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
	const size_t total_pixels = display_width * display_height;
	uint32_t *pixels = malloc(total_pixels * sizeof(uint32_t));
	if (pixels == NULL) {
		// Handle memory allocation failure
		SDL_SetError("Failed to allocate pixel buffer");
		return;
	}

	// Process pixels in chunks of 4 for better performance
	size_t i = 0;
	for (; i + 3 < total_pixels; i += 4) {
		pixels[i] = display_memory->data[i] ? color : ~color;
		pixels[i+1] = display_memory->data[i+1] ? color : ~color;
		pixels[i+2] = display_memory->data[i+2] ? color : ~color;
		pixels[i+3] = display_memory->data[i+3] ? color : ~color;
	}
	// Handle remaining pixels
	for (; i < total_pixels; i++) {
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
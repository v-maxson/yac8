#include <stdlib.h>
#include <SDL2/SDL.h>
#include <sodium.h>
#include "cpu.h"
#include "platform.h"

int main(void)
{
	// Initialize SDL2.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		fprintf(stderr, "Failed to initialize SDL2: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	// Initialize libsodium.
	if (sodium_init() == -1) {
		fprintf(stderr, "Failed to initialize libsodium\n");
		return EXIT_FAILURE;
	}

	const int display_width = 64;
	const int display_height = 32;

	const yac_cpu_config config = {
		.memory_size = 4096,
		.registers_size = 16,
		.display_width = display_width,
		.display_height = display_height,
	};
	yac_cpu *cpu = yac_cpu_new(config);

	const yac_platform_config platform_config = {
		.window_title = "Yet Another CHIP-8 Emulator",
		.window_width = display_width * 10,
		.window_height = display_height * 10,
	};
	yac_platform platform_layer = yac_platform_new(platform_config);

	while (1) {
		if (!yac_cpu_cycle(cpu)) {
			break;
		}

		if (cpu->redraw_requested) {
			yac_platform_render(&platform_layer,
					    &cpu->display_memory, display_width,
					    display_height, 0xFFFFFFFF);
			cpu->redraw_requested = false;
		}
	}

	yac_cpu_del(cpu);
	yac_platform_del(&platform_layer);

	return EXIT_SUCCESS;
}
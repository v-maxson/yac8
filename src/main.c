#include <stdlib.h>
#include <SDL.h>
#include <sodium.h>
#include "cpu.h"
#include "platform.h"
#include "timer.h"
#include "cli.h"

int main(const int argc, char *argv[])
{
	const yac_cli_args args = yac_cli_parse(argc, argv);

	// Initialize SDL2.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
		fprintf(stderr, "Failed to initialize SDL2: %s\n",
			SDL_GetError());
		return EXIT_FAILURE;
	}

	// Initialize libsodium.
	if (sodium_init() == -1) {
		fprintf(stderr, "Failed to initialize libsodium\n");
		return EXIT_FAILURE;
	}

	const int display_width = 64;
	const int display_height = 32;
	const int scalar = args.display_scale;

	const yac_cpu_config config = {
		.memory_size = 4096,
		.registers_size = 16,
		.display_width = display_width,
		.display_height = display_height,
	};
	yac_cpu *cpu = yac_cpu_new(config);

	// Load ROM.
	if (!yac_cpu_load_rom(cpu, args.rom_path))
		return EXIT_FAILURE;

	const yac_platform_config platform_config = {
		.window_title = "Yet Another CHIP-8 Emulator",
		.window_width = display_width,
		.window_height = display_height,
		.scalar = scalar
	};
	yac_platform platform_layer = yac_platform_new(platform_config);

	yac_timer timer = yac_timer_new(args.clock_speed);
	bool running = true;
	SDL_Event event;
	while (running) {
		if (!yac_timer_update(&timer))
			continue;
		if (!yac_cpu_cycle(cpu))
			break;

		if (cpu->redraw_requested) {
			yac_platform_render(&platform_layer,
					    &cpu->display_memory, display_width,
					    display_height, 0xFFFFFFFF);
			cpu->redraw_requested = false;
		}

		// Get key state.
		int num_keys;
		const uint8_t *keys = SDL_GetKeyboardState(&num_keys);
		// Check if any of the pressed keys are in the WASD_KEYMAP,
		// and set the corresponding key in the CPU.
		for (int i = 0; i < 16; i++) {
			cpu->keys[i] = keys[WASD_KEYMAP[i]];
		}

		// Handle events.
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			default:;
			}
		}
	}

	yac_cpu_del(cpu);
	yac_platform_del(&platform_layer);

	return EXIT_SUCCESS;
}
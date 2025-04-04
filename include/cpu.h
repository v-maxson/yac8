#ifndef __YAC_CPU_H__
#define __YAC_CPU_H__


#include <stdbool.h>
#include <SDL.h>
#include "memory.h"
#include "stack.h"

extern const int WASD_KEYMAP[16];

typedef struct yac_cpu {
	// Memory and registers
	yac_memory_u8 memory;
	yac_memory_bool display_memory;
	yac_memory_u8 registers;
	uint16_t i;
	uint16_t pc;
	yac_stack *stack;

	// Timers
	uint8_t delay_timer;
	uint8_t sound_timer;

	// Display
	size_t display_width;
	size_t display_height;
	bool redraw_requested;

	// Input
	bool keys[16];
} yac_cpu;

typedef struct yac_cpu_config {
	size_t memory_size;
	size_t registers_size;
	size_t display_width;
	size_t display_height;
	size_t stack_size;
} yac_cpu_config;

yac_cpu *yac_cpu_new(const yac_cpu_config config);

void yac_cpu_del(yac_cpu *cpu);

/// Executes a single cycle of the CHIP-8 CPU.
/// Returns true if the CPU should continue executing, false otherwise.
bool yac_cpu_cycle(yac_cpu *cpu);

/// Loads a ROM into the CPU's memory.
/// Returns true if the ROM was loaded successfully, false otherwise.
bool yac_cpu_load_rom(yac_cpu *cpu, const char *rom_path);

#endif

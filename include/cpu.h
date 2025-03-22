#ifndef __YAC_CPU_H__
#define __YAC_CPU_H__

#include <stdbool.h>
#include "memory.h"

/// The CHIP-8 CPU.
typedef struct yac_cpu {
	// Memory and registers
	yac_memory_u8 memory;
	yac_memory_u32 display_memory;
	yac_memory_u8 registers;
	uint16_t i;
	uint16_t pc;
	// TODO - STACK

	// Timers
	uint8_t delay_timer;
	uint8_t sound_timer;

	// Display
	size_t display_width;
	size_t display_height;
} yac_cpu;

typedef struct yac_cpu_config {
	size_t memory_size;
	size_t registers_size;
	size_t display_width;
	size_t display_height;
	// TODO - STACK_SIZE
} yac_cpu_config;

/// Creates a new CHIP-8 CPU.
yac_cpu *yac_cpu_new(const yac_cpu_config config);

/// Deletes a CHIP-8 CPU.
void yac_cpu_del(yac_cpu *cpu);

/// Executes a single cycle of the CHIP-8 CPU.
/// Returns true if the CPU should continue executing, false otherwise.
bool yac_cpu_cycle(yac_cpu *cpu);

#endif

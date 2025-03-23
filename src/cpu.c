#include <stdio.h>
#include "cpu.h"
#include "instruction.h"

yac_cpu *yac_cpu_new(const yac_cpu_config config)
{
	yac_cpu *cpu = malloc(sizeof(yac_cpu));
	if (cpu == NULL) {
		return NULL;
	}

	cpu->memory = yac_memory_u8_new(config.memory_size);
	cpu->display_memory = yac_memory_bool_new(config.display_width * config.display_height);
	cpu->registers = yac_memory_u8_new(config.registers_size);
	cpu->i = 0;
	cpu->pc = 0;
	cpu->stack = yac_stack_new(config.stack_size);
	cpu->delay_timer = 0;
	cpu->sound_timer = 0;
	cpu->display_width = config.display_width;
	cpu->display_height = config.display_height;
	cpu->redraw_requested = false;
	return cpu;
}

void yac_cpu_del(yac_cpu *cpu)
{
	yac_memory_u8_del(&cpu->memory);
	yac_memory_bool_del(&cpu->display_memory);
	yac_memory_u8_del(&cpu->registers);
	yac_stack_del(cpu->stack);
	free(cpu);
}

bool yac_cpu_cycle(yac_cpu *cpu)
{
	// Fetch instruction.
	const yac_instruction instruction = yac_decode_instruction(
		cpu->memory.data[cpu->pc], cpu->memory.data[cpu->pc + 1]);

	// TODO - Fetch instruction and execute it.
	printf("Fetched instruction: 0x%04X\n", instruction.raw);

	if (cpu->pc + 2 >= cpu->memory.size) {
		return false;
	}
	cpu->pc += 2;

	return true;
}
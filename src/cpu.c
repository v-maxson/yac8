#include <stdio.h>
#include "cpu.h"
#include "instruction.h"
#include "instruction_set.h"

yac_cpu *yac_cpu_new(const yac_cpu_config config)
{
	yac_cpu *cpu = malloc(sizeof(yac_cpu));
	if (cpu == NULL) {
		return NULL;
	}

	cpu->memory = yac_memory_u8_new(config.memory_size);
	cpu->display_memory = yac_memory_bool_new(config.display_width *
						  config.display_height);
	cpu->registers = yac_memory_u8_new(config.registers_size);
	cpu->i = 0;
	cpu->pc = 0x200;
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

	// Execute
	instruction_logic *logic = instruction_set[instruction.opcode];
	if (logic == NULL) {
		fprintf(stderr, "Unknown opcode: 0x%02X\n", instruction.opcode);
		return false;
	}
	printf("Executing instruction: 0x%04X\n", instruction.raw);
	logic(cpu, instruction);

	// Increment PC
	if (cpu->pc + 2 >= cpu->memory.size) {
		return false;
	}
	cpu->pc += 2;

	return true;
}

bool yac_cpu_load_rom(yac_cpu *cpu, const char *rom_path)
{
	FILE *rom_file = fopen(rom_path, "rb");
	if (rom_file == NULL) {
		fprintf(stderr, "Failed to open ROM file: %s\n", rom_path);
		return false;
	}

	fseek(rom_file, 0, SEEK_END);
	long rom_size = ftell(rom_file);
	rewind(rom_file);

	if (rom_size > cpu->memory.size - 0x200) {
		fprintf(stderr, "ROM file is too large: %ld bytes\n", rom_size);
		fclose(rom_file);
		return false;
	}

	fread(&cpu->memory.data[0x200], 1, rom_size, rom_file);
	fclose(rom_file);

	return true;
}
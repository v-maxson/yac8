#include <sodium.h>
#include "instruction_set.h"

void op_0(yac_cpu *cpu, const yac_instruction instruction)
{
	switch (instruction.kk) {
	case 0xE0:
		// CLS
		for (size_t i = 0; i < cpu->display_memory.size; i++) {
			cpu->display_memory.data[i] = false;
		}
		cpu->redraw_requested = true;
		break;
	case 0xEE:
		// RET
		uint16_t popped;
		if (!yac_stack_pop(cpu->stack, &popped))
			return;
		cpu->pc = popped;
		break;
	default:;
	}
}

void op_1(yac_cpu *cpu, const yac_instruction instruction)
{
	// JP addr
	cpu->pc =
		instruction.nnn -
		2; // -2 because the PC is incremented after the instruction is fetched.
}

void op_2(yac_cpu *cpu, const yac_instruction instruction)
{
	// CALL addr
	if (!yac_stack_push(cpu->stack, cpu->pc))
		return;
	cpu->pc = instruction.nnn - 2;
}

void op_3(yac_cpu *cpu, const yac_instruction instruction)
{
	// SE Vx, kk
	if (cpu->registers.data[instruction.x] == instruction.kk)
		cpu->pc += 2;
}

void op_4(yac_cpu *cpu, const yac_instruction instruction)
{
	// SNE Vx, kk
	if (cpu->registers.data[instruction.x] != instruction.kk)
		cpu->pc += 2;
}

void op_5(yac_cpu *cpu, const yac_instruction instruction)
{
	// SE Vx, Vy
	if (cpu->registers.data[instruction.x] ==
	    cpu->registers.data[instruction.y])
		cpu->pc += 2;
}

void op_6(yac_cpu *cpu, const yac_instruction instruction)
{
	// LD Vx, kk
	cpu->registers.data[instruction.x] = instruction.kk;
}

void op_7(yac_cpu *cpu, const yac_instruction instruction)
{
	// ADD Vx, kk
	cpu->registers.data[instruction.x] += instruction.kk;
}

void op_8(yac_cpu *cpu, const yac_instruction instruction)
{
	switch (instruction.n) {
	case 0: // LD Vx, Vy
		cpu->registers.data[instruction.x] =
			cpu->registers.data[instruction.y];
		break;
	case 1: // OR Vx, Vy
		cpu->registers.data[instruction.x] |=
			cpu->registers.data[instruction.y];
		break;
	case 2: // AND Vx, Vy
		cpu->registers.data[instruction.x] &=
			cpu->registers.data[instruction.y];
		break;
	case 3: // XOR Vx, Vy
		cpu->registers.data[instruction.x] ^=
			cpu->registers.data[instruction.y];
		break;
	case 4: // ADD Vx, Vy
		// Set VF to 1 if there is a carry, 0 otherwise.
		if (cpu->registers.data[instruction.x] >
		    0xFF - cpu->registers.data[instruction.y])
			cpu->registers.data[0xF] = 1;
		else
			cpu->registers.data[0xF] = 0;

		cpu->registers.data[instruction.x] +=
			cpu->registers.data[instruction.y];
		break;
	case 5: // SUB Vx, Vy
		// Set VF to 0 if there is a borrow, 1 otherwise.
		if (cpu->registers.data[instruction.x] >
		    cpu->registers.data[instruction.y])
			cpu->registers.data[0xF] = 1;
		else
			cpu->registers.data[0xF] = 0;

		cpu->registers.data[instruction.x] -=
			cpu->registers.data[instruction.y];
		break;
	case 6: // SHR Vx {, Vy}
		if (instruction.x == instruction.y) {
			cpu->registers.data[0xF] =
				cpu->registers.data[instruction.x] & 0x1;
			cpu->registers.data[instruction.x] >>= 1;
		} else {
			cpu->registers.data[0xF] =
				cpu->registers.data[instruction.y] & 0x1;
			cpu->registers.data[instruction.x] =
				cpu->registers.data[instruction.y] >> 1;
		}
		break;
	case 7: // SUBN Vx, Vy
		// Set VF to 0 if there is a borrow, 1 otherwise.
		if (cpu->registers.data[instruction.y] >
		    cpu->registers.data[instruction.x])
			cpu->registers.data[0xF] = 1;
		else
			cpu->registers.data[0xF] = 0;
		if (instruction.x == instruction.y) {
			cpu->registers.data[instruction.x] =
				cpu->registers.data[instruction.y] -
				cpu->registers.data[instruction.x];
		} else {
			cpu->registers.data[instruction.x] =
				cpu->registers.data[instruction.x] -
				cpu->registers.data[instruction.y];
		}
		break;
	case 0xE: // SHL Vx {, Vy}
		if (instruction.x == instruction.y) {
			cpu->registers.data[0xF] =
				cpu->registers.data[instruction.x] >> 7;
			cpu->registers.data[instruction.x] <<= 1;
		} else {
			cpu->registers.data[0xF] =
				cpu->registers.data[instruction.y] >> 7;
			cpu->registers.data[instruction.x] =
				cpu->registers.data[instruction.y] << 1;
		}
	default:;
	}
}

void op_9(yac_cpu *cpu, const yac_instruction instruction)
{
	// SNE Vx, Vy
	if (cpu->registers.data[instruction.x] !=
	    cpu->registers.data[instruction.y])
		cpu->pc += 2;
}

void op_A(yac_cpu *cpu, const yac_instruction instruction)
{
	// LD I, addr
	cpu->i = instruction.nnn;
}

void op_B(yac_cpu *cpu, const yac_instruction instruction)
{
	// JP V0, addr
	cpu->pc = cpu->registers.data[0] + instruction.nnn - 2;
}

void op_C(yac_cpu *cpu, const yac_instruction instruction)
{
	// RND Vx, kk
	cpu->registers.data[instruction.x] = randombytes_uniform(UINT8_MAX) &
					     instruction.kk;
}

void op_D(yac_cpu *cpu, const yac_instruction instruction)
{
	// DRW Vx, Vy, n
	// The interpreter reads n bytes from memory,
	// starting at the address stored in I.
	// These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
	// Sprites are XORed onto the existing screen.
	// If this causes any pixels to be erased, VF is set to 1,
	// otherwise it is set to 0. If the sprite is positioned so
	// part of it is outside the coordinates of the display, it wraps around
	// to the opposite side of the screen. See instruction 8xy3 for more
	// information on XOR, and section 2.4, Display, for more information on
	// the Chip-8 screen and sprites.
	uint8_t x = cpu->registers.data[instruction.x];
	uint8_t y = cpu->registers.data[instruction.y];
	uint8_t n = instruction.n;
	cpu->registers.data[0xF] = 0;
	for (size_t i = 0; i < n; i++) {
		uint8_t byte = cpu->memory.data[cpu->i + i];
		for (size_t j = 0; j < 8; j++) {
			uint8_t bit = (byte >> (7 - j)) & 0x1;
			bool *pixel =
				&cpu->display_memory.data[((y + i) % 32) * 64 +
							  ((x + j) % 64)];
			if (*pixel && bit)
				cpu->registers.data[0xF] = 1;
			*pixel ^= bit;
		}
	}
	cpu->redraw_requested = true;
}

void op_E(yac_cpu *cpu, const yac_instruction instruction)
{
	switch (instruction.kk) {
	case 0x9E:
		// SKP Vx
		if (cpu->keys[cpu->registers.data[instruction.x]])
			cpu->pc += 2;
		break;
	case 0xA1:
		// SKNP Vx
		if (!cpu->keys[cpu->registers.data[instruction.x]])
			cpu->pc += 2;
		break;
	default:;
	}
}

void op_F(yac_cpu *cpu, const yac_instruction instruction)
{
	switch (instruction.kk) {
	case 0x07:
		// LD Vx, DT
		cpu->delay_timer = cpu->registers.data[instruction.x];
		break;
	case 0x0A:
		// LD Vx, K
		// Wait for a key, then store the key in Vx
		// (blocking operation, all instruction halted until next key event)
		bool key_pressed = false;
		for (size_t i = 0; i < 16; i++) {
			if (cpu->keys[i]) {
				cpu->registers.data[instruction.x] = i;
				key_pressed = true;
				break;
			}
		}
		if (!key_pressed)
			cpu->pc -= 2;
		break;
	case 0x15:
		// LD DT, Vx
		cpu->delay_timer = cpu->registers.data[instruction.x];
		break;
	case 0x18:
		// LD ST, Vx
		cpu->sound_timer = cpu->registers.data[instruction.x];
		break;
	case 0x1E:
		// ADD I, Vx
		cpu->i += cpu->registers.data[instruction.x];
		break;
	case 0x29:
		// LD F, Vx
		cpu->i = cpu->registers.data[instruction.x] * 5;
		break;
	case 0x33:
		// LD B, Vx
		cpu->memory.data[cpu->i] = cpu->registers.data[instruction.x] / 100;
		cpu->memory.data[cpu->i + 1] =
			(cpu->registers.data[instruction.x] / 10) % 10;
		cpu->memory.data[cpu->i + 2] = cpu->registers.data[instruction.x] % 10;
		break;
	case 0x55:
		// LD [I], Vx
		// Store registers V0 through Vx in memory starting at location I.
		for (size_t i = 0; i <= instruction.x; i++) {
			cpu->memory.data[cpu->i + i] = cpu->registers.data[0 + i];
		}
		break;
	case 0x65:
		// LD Vx, [I]
		// Read registers V0 through Vx from memory starting at location I.
		for (size_t i = 0; i <= instruction.x; i++) {
			cpu->registers.data[0 + i] = cpu->memory.data[cpu->i + i];
		}
		break;
	default:;
	}
}

const instruction_logic *instruction_set[16] = { op_0, op_1, op_2, op_3,
						 op_4, op_5, op_6, op_7,
						 op_8, op_9, op_A, op_B,
						 op_C, op_D, op_E, op_F };
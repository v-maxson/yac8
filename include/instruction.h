#ifndef __YAC_INSTRUCTION_H__
#define __YAC_INSTRUCTION_H__

#include <stdint.h>

typedef struct yac_instruction {
	uint16_t raw;
	uint8_t opcode;
	uint8_t x;
	uint8_t y;
	uint8_t n;
	uint8_t kk;
	uint16_t nnn;
} yac_instruction;

/// Decodes a CHIP-8 instruction given the raw instruction bytes.
yac_instruction yac_decode_instruction(uint8_t msb, uint8_t lsb);

#endif

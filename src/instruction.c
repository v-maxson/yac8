#include "instruction.h"

yac_instruction yac_decode_instruction(const uint8_t msb, const uint8_t lsb)
{
	yac_instruction instr;
	instr.raw = (msb << 8) | lsb;
	instr.opcode = (msb & 0xF0) >> 4;
	instr.x = msb & 0x0F;
	instr.y = (lsb & 0xF0) >> 4;
	instr.kk = lsb;
	instr.nnn = (msb & 0x0F) << 8 | lsb;
	return instr;
}
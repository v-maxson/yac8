#ifndef __YAC_INSTRUCTION_SET_H__
#define __YAC_INSTRUCTION_SET_H__

#include "cpu.h"
#include "instruction.h"

typedef void(instruction_logic)(yac_cpu *cpu,
				const yac_instruction instruction);

extern const instruction_logic *instruction_set[16];

#endif

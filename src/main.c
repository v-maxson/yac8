#include <stdlib.h>
#include "cpu.h"

int main(void)
{
	const yac_cpu_config config = {
		.memory_size = 4096,
		.registers_size = 16,
		.display_width = 64,
		.display_height = 32
	};

	yac_cpu *cpu = yac_cpu_new(config);

	while (1) {
		if (!yac_cpu_cycle(cpu)) {
			break;
		}
	}

	yac_cpu_del(cpu);

	return EXIT_SUCCESS;
}
#ifndef __YAC_CLI_H_
#define __YAC_CLI_H_

#include <argp.h>

typedef struct yac_cli_args {
	const char *rom_path;
	int clock_speed;
	int display_scale;
} yac_cli_args;

yac_cli_args yac_cli_parse(const int argc, char *argv[]);

#endif

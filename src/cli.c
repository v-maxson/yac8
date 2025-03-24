#include "cli.h"
#include "yac_version.h"
#include <argp.h>
#include <stdlib.h>

const char *argp_program_version = "yac " YAC_VERSION_STRING;
const char *argp_program_bug_address = YAC_REPO_URL "/issues";
static char doc[] = "Yet Another CHIP-8 Emulator";
static char args_doc[] = "ROM_PATH";

static struct argp_option options[] = { { "frequency", 'f', "FREQUENCY", 0,
					  "Clock speed in Hz" },
					{ 0 } };

struct arguments {
	char *args[1]; // ROM_PATH
	int frequency;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct arguments *arguments = state->input;

	switch (key) {
	case 'f':
		arguments->frequency = (int)strtol(arg, NULL, 10);
		break;
	case ARGP_KEY_ARG:
		if (state->arg_num >= 1)
			argp_usage(state);
		arguments->args[state->arg_num] = arg;
		break;
	case ARGP_KEY_END:
		if (state->arg_num < 1)
			argp_usage(state);
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

yac_cli_args yac_cli_parse(const int argc, char *argv[])
{
	struct arguments arguments;
	arguments.frequency = 60;

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	yac_cli_args yac_cli_args;
	yac_cli_args.rom_path = arguments.args[0];
	yac_cli_args.clock_speed = arguments.frequency;

	return yac_cli_args;
}
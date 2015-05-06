/*
 * disnes_cfg.c
 *
 *  Created on: 4 juillet 2012
 *      Author: msoula
 */

#include "disnes_cfg.h"

#include <getopt.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "utils/config.h"
#include "utils/logger.h"

#define VERSION		("0.1")

#define USAGE  "disnes usage:\n" \
	"\t--banks        | -b <list of banks to disassemble>\n" \
	"\t--bank-size    | -s <size (in bytes) of one bank (default: 0x4000)>\n" \
	"\t--bank-addr    | -a <starting address of output bank>\n" \
	"\t--no-branches  | -q <do not print branches>\n" \
	"\t--version      | -v <version>\n" \
	"\t--help         | -h <aide>\n"


/*!< tableaux des noms d'options (courts et longs) indexés par identifiant. */
static option_t option_names[] = {
		{'b', "banks"},
		{'s', "bank-size"},
		{'a', "bank-addr"},
		{'q', "no-branches"},
		{'v', "version"},
		{'h', "help"}
};

/**
 * @enum E_OPTION_ID
 * @brief Constantes des types de paramètres d'appel.
 *
 * E_OPTION_ID permet d'identifier les paramètres d'appel.
 */
typedef enum {
	BANKS_OPTION,		/*!< banks to disasm. */
	BANK_SIZE_OPTION,	/*!< size of one bank. */
	BANK_ADDR_OPTION,	/*!< starting address of output bank. */
	NO_BRANCHES_OPTION,	/*!< don't print branches. */
	VERSION_OPTION,		/*!< version. */
	HELP_OPTION			/*!< aide. */
} E_OPTION_ID;

static int check_config(void *cfg);
static const char *get_usage();
static const char *get_version();
static void *init_config(void);
static int treat_option(void *cfg, int opt, const char *arg);
static void print_config(void *cfg);
static void uinit_config(void *cfg);

static cfg_if_t _cfg_if = {
	get_usage,
	get_version,
	init_config,
	treat_option,
	check_config,
	print_config,
	uinit_config
};

disnes_cfg_t *disnes_cfg_init(int argc, char **argv) {
	config_init(&_cfg_if);

	/* définition des arguments */
	struct option long_options[] = {
			{option_names[BANKS_OPTION].long_option, required_argument, NULL, option_names[BANKS_OPTION].short_option},
			{option_names[BANK_SIZE_OPTION].long_option, required_argument, NULL, option_names[BANK_SIZE_OPTION].short_option},
			{option_names[BANK_ADDR_OPTION].long_option, required_argument, NULL, option_names[BANK_ADDR_OPTION].short_option},
			{option_names[NO_BRANCHES_OPTION].long_option, no_argument, NULL, option_names[NO_BRANCHES_OPTION].short_option},
			{option_names[HELP_OPTION].long_option, no_argument, NULL, option_names[HELP_OPTION].short_option},
			{option_names[VERSION_OPTION].long_option, no_argument, NULL, option_names[VERSION_OPTION].short_option},
			{NULL, 0, NULL, 0}
	};

	const char short_options[] = {
			option_names[BANKS_OPTION].short_option,':',
			option_names[BANK_SIZE_OPTION].short_option,':',
			option_names[BANK_ADDR_OPTION].short_option,':',
			option_names[NO_BRANCHES_OPTION].short_option,
			option_names[HELP_OPTION].short_option,
			option_names[VERSION_OPTION].short_option, '\0',
	};

	disnes_cfg_t *cfg;
	if(NULL == (cfg = config_parse_opts(argc, argv, short_options, long_options))) {
		LOG_ERROR("disnes_cfg_init() failed - cannot parse args to initialize config structure.");
		return NULL;
	}

	if(argc > optind) {
		cfg->rom = argv[optind];
	}

	return cfg;
}

void disnes_cfg_uinit(disnes_cfg_t *cfg) {
	if(cfg)
		uinit_config(cfg);
}

/************************** Private functions *********************************/
int check_config(void *cfg) {
	if(!cfg)
		return 0;

	return NULL != ((disnes_cfg_t *) cfg)->rom;
}
/******************************************************************************/
const char *get_usage(void) {
	return USAGE;
}
/******************************************************************************/
const char *get_version(void) {
	return VERSION;
}
/******************************************************************************/
void *init_config(void) {
	disnes_cfg_t *cfg;

	/* initializing default config structure */
	if(NULL == (cfg = (disnes_cfg_t *) malloc(sizeof(disnes_cfg_t)))) {
		LOG_ERROR("init_config() failed - cannot allocate memory to initialize a new config structure.");
		return NULL;
	}

	int i = 0;
	for(i = 0; i < BANK_LIST_SIZE; i++)
		cfg->banks[i] = 0;
	cfg->bank_size = DEFAULT_BANK_SIZE;
	cfg->bank_addr = DEFAULT_BANK_ADDR;
	cfg->no_branches = DEFAULT_NO_BRANCHES;
	cfg->rom = NULL;

	return cfg;
}
/******************************************************************************/
int treat_option(void *cfg, int opt, const char *arg) {
	int res = 0;
	if(opt == option_names[BANKS_OPTION].short_option) {

		// parsing list of banks
		char *saveptr_comma, *saveptr_hyphen;
		char *str = strdup(arg);
		int min;

		char *token = strtok_r(str, ",", &saveptr_comma), *subtoken;
		while(token) {
			subtoken = strtok_r(token, "-", &saveptr_hyphen);

			min = -1;
			while(subtoken) {
				long int val = strtol(subtoken, NULL, 0);
				/* Vérification de certaines erreurs possibles */
				if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
					   || (errno != 0 && val == 0)) {
					config_print_help("invalid value for parameter bank-size.");
					exit(EXIT_FAILURE);
				}

				if(min == -1) {
					min = val;
				}
				else {
					while(min <= val)
						((disnes_cfg_t *) cfg)->banks[min++] = 1;
					min = -1;
				}
				subtoken = strtok_r(NULL, "-", &saveptr_hyphen);
			}
			if(min != -1)
				((disnes_cfg_t *) cfg)->banks[min] = 1;
			token = strtok_r(NULL, ",", &saveptr_comma);
		}
		free(str);
	}
	else if(opt == option_names[BANK_SIZE_OPTION].short_option) {

		long int val = strtol(arg, NULL, 16);
		/* Vérification de certaines erreurs possibles */
		if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
			   || (errno != 0 && val == 0)) {
			config_print_help("invalid value for parameter bank-size.");
			exit(EXIT_FAILURE);
		}

		((disnes_cfg_t *) cfg)->bank_size = val;
	}
	else if(opt == option_names[BANK_ADDR_OPTION].short_option) {

		long int val = strtol(arg, NULL, 16);
		/* Vérification de certaines erreurs possibles */
		if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
			   || (errno != 0 && val == 0)) {
			config_print_help("invalid value for parameter bank-addr.");
			exit(EXIT_FAILURE);
		}

		((disnes_cfg_t *) cfg)->bank_addr = val & 0xFFFF;
	}
	else if(opt == option_names[NO_BRANCHES_OPTION].short_option) {
		((disnes_cfg_t *) cfg)->no_branches = 1;
	}
	else if(opt == option_names[VERSION_OPTION].short_option) {
		config_print_version();
		exit(EXIT_SUCCESS);
	}
	else if(opt == option_names[HELP_OPTION].short_option) {
		config_print_help(NULL);
		exit(EXIT_SUCCESS);
	}

	return res;
}
/******************************************************************************/
void print_config(void *cfg) {
	char *banks_str;
	if(NULL == (banks_str = (char *) calloc(256, sizeof(char)))) {
		return;
	}

	int i;
	int pos = 0;
	for(i = 0; i < BANK_LIST_SIZE; i++) {
		if(((disnes_cfg_t *) cfg)->banks[i])
			pos += sprintf(&banks_str[pos], "%d%s", i, (BANK_LIST_SIZE - 1 == i) ? "" : ", ");
	}


	LOG_INFO("Disnes parameters:\n"
			"\trom:           : %s\n"
			"\tbanks          : %s\n"
			"\tbank size      : %04X\n"
			"\tbank addr      : %04X\n"
			"\tno branches    : %s\n",
			((disnes_cfg_t *) cfg)->rom,
			banks_str,
			((disnes_cfg_t *) cfg)->bank_size,
			((disnes_cfg_t *) cfg)->bank_addr,
			((disnes_cfg_t *) cfg)->no_branches ? "yes" : "no"
	);

	free(banks_str);
}
/******************************************************************************/
void uinit_config(void *cfg) {
	if(cfg)
		free(((disnes_cfg_t *) cfg));
}

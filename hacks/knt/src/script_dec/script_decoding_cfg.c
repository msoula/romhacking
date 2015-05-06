/*
 * script_decoding_cfg.c
 *
 *  Created on: 16 nov. 2011
 *      Author: rid
 */

#include "script_decoding_cfg.h"

#include <getopt.h>
#include <stdlib.h>
#include "../utils/config.h"
#include "../utils/logger.h"

#define VERSION		("0.1")

#define USAGE  "spc_scr_enc usage:\n" \
	"\t--cpu_mem    | -c <cpu memory configuration filepath>\n" \
	"\t--table      | -t <table filepath>\n" \
	"\t--rom        | -r <original rom filepath>\n" \
	"\t--xml        | -x <output xml script filepath>\n" \
	"\t--version    | -v <version>\n" \
	"\t--help       | -h <aide>\n"


/*!< tableaux des noms d'options (courts et longs) indexés par identifiant. */
static option_t option_names[] = {
		{'c', "cpu_mem"},
		{'t', "table"},
		{'r', "rom"},
		{'x', "xml"},
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
	CPU_MEM_OPTION,			/*!< cpu memory configuration filepath. */
	TABLE_OPTION,			/*!< table filepath. */
	ROM_OPTION,				/*!< original rom filepath. */
	XML_OPTION,				/*!< output xml script filepath. */
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
	get_usage, get_version, init_config, treat_option, NULL, check_config, print_config, uinit_config
};

scr_dec_cfg_t *scr_dec_cfg_init(int argc, char **argv) {
	config_init(&_cfg_if);

	/* définition des arguments */
	struct option long_options[] = {
			{option_names[CPU_MEM_OPTION].long_option, required_argument, NULL, option_names[CPU_MEM_OPTION].short_option},
			{option_names[TABLE_OPTION].long_option, required_argument, NULL, option_names[TABLE_OPTION].short_option},
			{option_names[ROM_OPTION].long_option, required_argument, NULL, option_names[ROM_OPTION].short_option},
			{option_names[XML_OPTION].long_option, required_argument, NULL, option_names[XML_OPTION].short_option},
			{option_names[HELP_OPTION].long_option, no_argument, NULL, option_names[HELP_OPTION].short_option},
			{option_names[VERSION_OPTION].long_option, no_argument, NULL, option_names[VERSION_OPTION].short_option},
			{NULL, 0, NULL, 0}
	};

	const char short_options[] = {
			option_names[CPU_MEM_OPTION].short_option,':',
			option_names[TABLE_OPTION].short_option,':',
			option_names[ROM_OPTION].short_option,':',
			option_names[XML_OPTION].short_option,':',
			option_names[HELP_OPTION].short_option,
			option_names[VERSION_OPTION].short_option, '\0',
	};

	scr_dec_cfg_t *cfg;
	if(NULL == (cfg = config_parse_args(argc, argv, short_options, long_options))) {
		LOG_ERROR("scr_dec_load_cfg() failed - cannot parse args to initialize config structure.");
		return NULL;
	}

	return cfg;
}

void scr_dec_cfg_uinit(scr_dec_cfg_t *cfg) {
	if(cfg)
		uinit_config(cfg);
}

/************************** Private functions *********************************/
int check_config(void *cfg) {
	if(!cfg)
		return 0;

	return ((((scr_dec_cfg_t *) cfg)->cpu_mem != DEFAULT_CPU_MEM_VALUE)
			&& (((scr_dec_cfg_t *) cfg)->rom != DEFAULT_ROM_VALUE)
			&& (((scr_dec_cfg_t *) cfg)->table != DEFAULT_TABLE_VALUE)
			&& (((scr_dec_cfg_t *) cfg)->xml != DEFAULT_XML_VALUE));
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
	scr_dec_cfg_t *cfg;

	/* initializing default config structure */
	if(NULL == (cfg = (scr_dec_cfg_t *) malloc(sizeof(scr_dec_cfg_t)))) {
		LOG_ERROR("init_config() failed - cannot allocate memory to initialize a new config structure.");
		return NULL;
	}

	cfg->cpu_mem = DEFAULT_CPU_MEM_VALUE;
	cfg->rom = DEFAULT_ROM_VALUE;
	cfg->table = DEFAULT_TABLE_VALUE;
	cfg->xml = DEFAULT_XML_VALUE;

	return cfg;
}
/******************************************************************************/
int treat_option(void *cfg, int opt, const char *arg) {
	int res = 0;
	if(opt == option_names[CPU_MEM_OPTION].short_option) {
		((scr_dec_cfg_t *) cfg)->cpu_mem = arg;
	}
	else if(opt == option_names[ROM_OPTION].short_option) {
		((scr_dec_cfg_t *) cfg)->rom = arg;
	}
	else if(opt == option_names[TABLE_OPTION].short_option) {
		((scr_dec_cfg_t *) cfg)->table = arg;
	}
	else if(opt == option_names[XML_OPTION].short_option) {
		((scr_dec_cfg_t *) cfg)->xml = arg;
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
	LOG_INFO("Script encoding parameters:\n"
			"\tcpu memory config:    %s\n"
			"\ttable file:           %s\n"
			"\toriginal rom:         %s\n"
			"\toutput file:          %s\n",
			((scr_dec_cfg_t *) cfg)->cpu_mem,
			((scr_dec_cfg_t *) cfg)->table,
			((scr_dec_cfg_t *) cfg)->rom,
			((scr_dec_cfg_t *) cfg)->xml
	);
}
/******************************************************************************/
void uinit_config(void *cfg) {
	if(cfg)
		free(((scr_dec_cfg_t *) cfg));
}

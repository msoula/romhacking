/*
 * script_encoding_cfg.c
 *
 *  Created on: 16 nov. 2011
 *      Author: rid
 */

#include "script_encoding_cfg.h"

#include <getopt.h>
#include <stdlib.h>
#include "../utils/config.h"
#include "../utils/logger.h"

#define VERSION		("0.1")

#define USAGE  "spc_scr_enc usage:\n" \
	"\t--cpumem     | -c <cpu memory configuration filepath>\n" \
    "\t--dict       | -d <dictionary filepath>\n" \
    "\t--rom        | -r <rom filepath>\n" \
	"\t--script     | -s <input xml script filepath>\n" \
	"\t--table      | -t <table filepath>\n" \
	"\t--version    | -v <version>\n" \
	"\t--help       | -h <aide>\n"

/*!< tableaux des noms d'options (courts et longs) indexés par identifiant. */
static option_t option_names[] = {
        { 'c', "cpu_mem" },
        { 'd', "dict" },
        { 'r', "rom" },
        { 's', "script" },
        { 't', "table" },
        { 'v', "version" },
        { 'h', "help" }
};

/**
 * @enum E_OPTION_ID
 * @brief Constantes des types de paramètres d'appel.
 *
 * E_OPTION_ID permet d'identifier les paramètres d'appel.
 */
typedef enum {
    CPU_MEM_OPTION, /*!< cpu memory configuration filepath. */
    DICT_OPTION,    /*!< dictionary filepath. */
    ROM_OPTION,     /*!< rom filepath. */
    SCRIPT_OPTION,  /*!< input xml script filepath. */
    TABLE_OPTION,   /*!< table filepath. */
    VERSION_OPTION, /*!< version. */
    HELP_OPTION     /*!< help. */
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
        NULL,
        check_config,
        print_config,
        uinit_config
};

scr_enc_cfg_t *scr_enc_cfg_init(int argc, char **argv) {
    config_init(&_cfg_if);

    /* définition des arguments */
    struct option long_options[] = {
            { option_names[CPU_MEM_OPTION].long_option, required_argument, NULL, option_names[CPU_MEM_OPTION].short_option },
            { option_names[DICT_OPTION].long_option, required_argument, NULL, option_names[DICT_OPTION].short_option },
            { option_names[ROM_OPTION].long_option, required_argument, NULL, option_names[ROM_OPTION].short_option },
            { option_names[SCRIPT_OPTION].long_option, required_argument, NULL, option_names[SCRIPT_OPTION].short_option },
            { option_names[TABLE_OPTION].long_option, required_argument, NULL, option_names[TABLE_OPTION].short_option },
            { option_names[HELP_OPTION].long_option, no_argument, NULL, option_names[HELP_OPTION].short_option },
            { option_names[VERSION_OPTION].long_option, no_argument, NULL, option_names[VERSION_OPTION].short_option },
            { NULL, 0, NULL, 0 }
    };

    const char short_options[] = {
            option_names[CPU_MEM_OPTION].short_option, ':',
            option_names[DICT_OPTION].short_option, ':',
            option_names[ROM_OPTION].short_option, ':',
            option_names[SCRIPT_OPTION].short_option, ':',
            option_names[TABLE_OPTION].short_option, ':',
            option_names[HELP_OPTION].short_option,
            option_names[VERSION_OPTION].short_option,
            '\0',
    };

    scr_enc_cfg_t *cfg;
    if (NULL == (cfg = config_parse_args(argc, argv, short_options, long_options))) {
        LOG_ERROR("scr_enc_load_cfg() failed - cannot parse args to initialize config structure.");
        return NULL;
    }

    return cfg;
}

void scr_enc_cfg_uinit(scr_enc_cfg_t *cfg) {
    if (cfg)
        uinit_config(cfg);
}

/************************** Private functions *********************************/
int check_config(void *cfg) {
    if (!cfg)
        return 0;

    return ((((scr_enc_cfg_t *) cfg)->cpu_mem != NULL)
            && (((scr_enc_cfg_t *) cfg)->dict != NULL)
            && (((scr_enc_cfg_t *) cfg)->rom != NULL)
            && (((scr_enc_cfg_t *) cfg)->script != NULL)
            && (((scr_enc_cfg_t *) cfg)->table != NULL));
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
    scr_enc_cfg_t *cfg;

    /* initializing default config structure */
    if (NULL == (cfg = (scr_enc_cfg_t *) malloc(sizeof(scr_enc_cfg_t)))) {
        fprintf(stderr, "cannot allocate memory to initialize a new config structure.");
        return NULL;
    }

    cfg->cpu_mem = DEFAULT_CPU_MEM_VALUE;
    cfg->dict    = DEFAULT_DICT_VALUE;
    cfg->script  = DEFAULT_ROM_VALUE;
    cfg->script  = DEFAULT_SCRIPT_VALUE;
    cfg->table   = DEFAULT_TABLE_VALUE;

    return cfg;
}
/******************************************************************************/
int treat_option(void *cfg, int opt, const char *arg) {
    int res = 0;
    if (opt == option_names[CPU_MEM_OPTION].short_option) {
        ((scr_enc_cfg_t *) cfg)->cpu_mem = arg;
    } else if (opt == option_names[DICT_OPTION].short_option) {
        ((scr_enc_cfg_t *) cfg)->dict = arg;
    } else if (opt == option_names[ROM_OPTION].short_option) {
        ((scr_enc_cfg_t *) cfg)->rom = arg;
    } else if (opt == option_names[SCRIPT_OPTION].short_option) {
        ((scr_enc_cfg_t *) cfg)->script = arg;
    } else if (opt == option_names[TABLE_OPTION].short_option) {
        ((scr_enc_cfg_t *) cfg)->table = arg;
    } else if (opt == option_names[VERSION_OPTION].short_option) {
        config_print_version();
        exit(EXIT_SUCCESS);
    } else if (opt == option_names[HELP_OPTION].short_option) {
        config_print_help(NULL);
        exit(EXIT_SUCCESS);
    }
    return res;
}
/******************************************************************************/
void print_config(void *cfg) {
    LOG_INFO(
            "Script encoding parameters:\n"
            "cpu memory config:    %s\n"
            "table file:           %s\n"
            "script file :         %s\n"
            "dictionnary file :    %s\n"
            "rom file :            %s\n",
            ((scr_enc_cfg_t * ) cfg)->cpu_mem,
            ((scr_enc_cfg_t * ) cfg)->table,
            ((scr_enc_cfg_t * ) cfg)->script,
            ((scr_enc_cfg_t * ) cfg)->dict,
            ((scr_enc_cfg_t * ) cfg)->rom);
}
/******************************************************************************/
void uinit_config(void *cfg) {
    if (cfg)
        free(((scr_enc_cfg_t *) cfg));
}

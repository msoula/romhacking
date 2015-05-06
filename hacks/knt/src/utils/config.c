/**
 * @file config.c
 * @brief Implémentation des fonctions d'analyse des paramètres d'appel.
 * @author Mathieu S.
 * @version 0.1
 * @date 21 juil. 2011
 * 
 * L'analyse des paramètres d'appel permet de préparer la configuration du
 * programme à partir des arguments transmis à l'appel.
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include "../utils/logger.h"

static cfg_if_t *_cfg_if = NULL;

void config_init(cfg_if_t *cfg) {
	_cfg_if = cfg;
}

/**
 * @fn static void config_print_help(const char *error_msg)
 * @brief Affiche sur la sortie standard le message d'aide du programme.
 *
 * @param error_msg le message d'erreur à afficher suite au message d'aide.
 */
void config_print_help(const char *error_msg) {
	if(!_cfg_if || !_cfg_if->usage)
		return;

	if(error_msg) {
		fprintf(stderr, "%s\n", error_msg);
		fprintf(stderr, "%s\n", _cfg_if->usage());
	}
	else
		fprintf(stdout, "%s\n", _cfg_if->usage());
}

/**
 * @fn static void config_print_version(void)
 * @brief Affiche sur la sortie standard la version de l'application.
 */
void config_print_version(void) {
	fprintf(stdout, "Build time: " __DATE__ " " __TIME__ );
	if(_cfg_if && _cfg_if->version)
		fprintf(stdout, "Version: %s\n", _cfg_if->version());
}

void *config_parse_args(int argc, char **argv, const char *shortopts, const struct option *longopts) {
	if(!_cfg_if) {
		LOG_ERROR("config_parse_args() failed - config loader has not been initialized.");
		return NULL;
	}

	void *cfg;
	if(NULL == (cfg = _cfg_if->init_cfg())) {
		LOG_ERROR("config_parse_args() failed - cannot initialize config structure.");
		return NULL;
	}

	if(_cfg_if->treat_option) {
		int c = 0;
		while(-1 != (c = getopt_long(argc, argv, shortopts, longopts, NULL))) {
			_cfg_if->treat_option(cfg, c, optarg);
		}
	}

	if(_cfg_if->treat_args)
		if(1 == _cfg_if->treat_args(cfg, argc, argv)) {
			config_print_help("invalid arguments.");
			_cfg_if->uinit_cfg(cfg);
			return NULL;
		}

	if(!_cfg_if->check_cfg(cfg)) {
		config_print_help("invalid arguments.");
		_cfg_if->uinit_cfg(cfg);
		return NULL;
	}

	_cfg_if->print_cfg(cfg);

	return cfg;
}

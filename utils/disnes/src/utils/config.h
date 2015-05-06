/**
 * @file config.h
 * @brief Déclaration des fonctions d'analyse des paramètres d'appel.
 * @author Mathieu S.
 * @version 0.1
 * @date 21 juil. 2011
 * 
 * L'analyse des paramètres d'appel permet de préparer la configuration du
 * programme à partir des arguments transmis à l'appel.
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <getopt.h>

typedef struct _cfg_if {
	const char *(*usage)(void);
	const char *(*version)(void);

	void *(*init_cfg)(void);
	int (*treat_option)(void *cfg, int c, const char *arg);
	int (*check_cfg)(void *cfg);
	void (*print_cfg)(void *cfg);
	void (*uinit_cfg)(void *cfg);
} cfg_if_t;


/**
 * @struct option_t
 * @brief Structure des options du programme.
 *
 * option_t regroupe les noms (courts et longs) d'une option du programme.
 */
typedef struct {
	char short_option;
	char *long_option;
} option_t;


void config_init(cfg_if_t *cfg);

void config_print_version(void);

void config_print_help(const char *error_msg);

/**
 * @fn config_t *config_parse_args(int argc, char **argv)
 * @brief Initialise les paramètres du programme à partir des paramètres
 * transmis à l'appel.
 *
 * @param argc le nombre de paramètres d'appel.
 * @param argv un tableau contenant l'ensemble des paramètres d'appel.
 * @return une structure contenant les paramètres de l'application, ou NULL
 * en cas d'erreur.
 */
void *config_parse_opts(int argc, char *const *argv, const char *shortopts, const struct option *longopts);

#endif /* CONFIG_H_ */

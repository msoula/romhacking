/*
 * main.c
 *
 *  Created on: 11 déc. 2014
 *      Author: rid
 *
 * L'agrandissement de la rom 'Kyatto Ninden Teyandee (J) [!].nes' se déroule
 * de la manière suivante:
 *   - doubler la PRG-ROM disponible;
 *   - déplacer la 7e bank de 16kB à la fin de la PRG-ROM et remplir la place
 *     libérée par des zéros;
 *   - déplacer la function située entre 0xEF8D et 0xEFD9 en tête de la bank
 *     libérée.
 *   - à la place du code libéré, ajouter le code ci-dessous.
 */

#include "spcdef.h"

#include "utils/file.h"
#include "utils/spc_md5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const size_t PRGBANKSIZ = 16384;
static const size_t CHRBANKSIZ = 8192;

static int   check_args(int argc, char **argv);
static char *get_new_filename(const char *from_filename);
static int   expand(const char *filename);

int main(int argc, char **argv) {

    int res;

    if(0 != check_args(argc, argv))
        return 1;

    const char *old_filename = argv[1];
    char       *new_filename;

    /* verify checksum*/
    if(0 == md5_check_file(old_filename, JP_SPC_MD5)) {
        fprintf(stderr, "ERROR - invalid file checksum.\n");
        res = 1;
        goto clean;
    }

    new_filename = get_new_filename(old_filename);

    /* copy file */
    file_clone(new_filename, old_filename);

    /* expand rom */
    expand(new_filename);

    /* insert screentitle */

    /* insert font fr */

    res = 0;

clean:
    if(NULL != new_filename)
        free(new_filename);

    return res;
}

/******************************************************************************/

int check_args(int argc, char **argv) {
    (void) argv;

    if(2 != argc) {
        fprintf(stderr, "ERROR - invalid number of arguments - expected 2\n");
        return 1;
    }

    return 0;
}

char *get_new_filename(const char *from_filename) {

    char *from_basename = basename(from_filename);
    char *name;
    if(NULL == (name = (char *) calloc(sizeof(char), strlen(from_basename) + 4 + 1))) { /* from_basename + ".new" + '\0' */
        fprintf(stderr, "ERROR - cannot create new filename from %s\n", from_basename);
        return NULL;
    }
    strcpy(name, from_basename);
    strcat(name, ".exp");
    return name;
}

int clone_file(const char *to_filename, const char *from_filename) {

    char *buf[BUFSIZ];
    FILE *in, *out;
    size_t len;

    /* open files */
    if(NULL == (in = fopen(from_filename, "rb"))) {
        fprintf(stderr, "ERROR - cannot open '%s' in READ mode\n", from_filename);
        return 1;
    }
    if(NULL == (out = fopen(to_filename, "wb"))) {
        fprintf(stderr, "ERROR - cannot open '%s' in WRITE mode\n", to_filename);
        return 1;
    }

    /* copy files */
    while(0 < (len = fread(buf, sizeof(char), BUFSIZ, in))) {
        if(len != fwrite(buf, sizeof(char), len, out)) {
            fprintf(stderr, "ERROR - cannot write %ld bytes into %s\n", len, to_filename);
        }
    }

    /* close files */
    fclose(out);
    fclose(in);

    return 0;
}

int expand(const char *filename) {

    char emptyprgrom[PRGBANKSIZ];
    char prgrom[PRGBANKSIZ];
    char chrrom[16 * CHRBANKSIZ];
    FILE *f = NULL;
    int idx = 0;

    if(NULL == (f = fopen(filename, "r+"))) {
        fprintf(stderr, "ERROR - cannot open '%s' in READ/WRITE mode\n", filename);
        goto error;
    }

    /* modify iNes PRG rom count */
    u_int8_t nb = 0x10;
    fseek(f, 0x04, SEEK_SET);
    fwrite(&nb, sizeof(u_int8_t), 1, f);

    /* we are going to double the number of PRG-ROM banks from PRG bank #7 */

    /* go to last prg bank pos */
    fseek(f, 0x10 + (7 * PRGBANKSIZ), SEEK_SET);

    /* now save both prg and chr rom banks */
    file_read_bytes(f, prgrom, PRGBANKSIZ, 1);

    for(idx = 0; idx < 16; idx++) {
        file_read_bytes(f, &chrrom[idx * CHRBANKSIZ], CHRBANKSIZ, 1);
    }

    /* go to last prg bank pos */
    fseek(f, 0x10 + (7 * PRGBANKSIZ), SEEK_SET);

    /* now double the number of prg rom */
    memset(emptyprgrom, '\0', PRGBANKSIZ);
    for(idx = 0; idx < 8; idx++) {
        file_write_bytes(f, emptyprgrom, PRGBANKSIZ, 1);
    }

    /* write saved prg rom and chr rom */
    file_write_bytes(f, prgrom, PRGBANKSIZ, 1);

    for(idx = 0; idx < 16; idx++) {
        file_write_bytes(f, &chrrom[idx * CHRBANKSIZ], CHRBANKSIZ, 1);
    }

    fclose(f);

    return 0;

error:
    if(NULL != f)
        fclose(f);
    return 1;
}

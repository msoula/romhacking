/*
 * script.c
 *
 *  Created on: 3 août 2011
 *      Author: rid
 */

#include "script.h"

#include <stdio.h>
#include <string.h>
#include <libxml/xmlreader.h>

#include "tags.h"
#include "script_enc/xml_script_reader.h"
#include "utils/logger.h"

script_t *script_init(void) {
    script_t *p_script;

    LOG_DEBUG("script_init() - initializing xml script object...");
    /* allocating memory to contain xml script */
    if (NULL == (p_script = (script_t *) malloc(sizeof(script_t)))) {
        LOG_ERROR("script_init() failed - cannot allocate memory to initialize xml script.");
        return NULL;
    }

    /* allocating memory to contain pointed texts */
    ptrlist_init(&(p_script->texts));
    return p_script;
}

int script_uinit(script_t * script) {
    LOG_DEBUG("Freeing xml script object...");
    if (!script) {
        LOG_ERROR("script_init() failed - xml script is not initialized - script is NULL.");
        return -1;
    }

    /* remove every text content */
    while (0 < ptrlist_count(&(script->texts))) {

        text_t *cur = NULL;
        ptrlist_remove(&(script->texts), 0, (void **) &cur);
        if (NULL != cur)
            text_destroy(cur);
    }
    ptrlist_uninit(&(script->texts));
    free(script), script = NULL;
    return 1;
}

void script_fprintf(script_t * script, FILE *output_fd) {
    int idx;

    if (output_fd == stdout || output_fd == stderr)
        fprintf(output_fd, "Content of SPC script:\n");

    for(idx = 0; idx  < ptrlist_count(&(script->texts)); idx++) {
        text_fprintf(ptrlist_get(&(script->texts), idx), output_fd);
    }

    if (output_fd == stdout || output_fd == stderr)
        fprintf(output_fd, "\nTotal number of texts: %d.", ptrlist_count(&(script->texts)));
}

size_t script_count(script_t *script) {
    return ptrlist_count(&(script->texts));
}

void script_sort_by_address(script_t * script) {

    int textcmp(const void *arg1, const void *arg2) {
        const text_t *t1 = *((const text_t **) arg1);
        const text_t *t2 = *((const text_t **) arg2);

        if (t1->cpu_address < t2->cpu_address)
            return -1;
        else if (t1->cpu_address > t2->cpu_address)
            return 1;
        else
            return 0;
    }

    ptrlist_sort(&(script->texts), textcmp);
}

void script_sort_by_pointer_index(script_t * script) {

    int textcmp(const void *arg1, const void *arg2) {
        const text_t *t1 = *((const text_t **) arg1);
        const text_t *t2 = *((const text_t **) arg2);

        if (t1->pointer_idx < t2->pointer_idx)
            return -1;
        else if (t1->pointer_idx > t2->pointer_idx)
            return 1;
        else
            return 0;
    }

    ptrlist_sort(&(script->texts), textcmp);
}

text_t *script_get_entry(script_t *script, int idx) {
    return ptrlist_get(&(script->texts), idx);
}

/*****************************************************************************/

text_t *text_init() {

    text_t *text;
    if (NULL == (text = (text_t *) malloc(sizeof(text_t)))) {
        LOG_ERROR("cannot allocate memory to initialize a new spc text.");
        return NULL;
    }
    text->cpu_address = 0;
    array_init(&(text->code), sizeof(u_int8_t));
    text->pointer_idx = 0;
    text->u8srt     = NULL;
    return text;

}

void text_destroy(text_t *t) {
    if(NULL != t->u8srt)
        free(t->u8srt);
    array_uninit(&(t->code));
    free(t);
}

int text_store_to_file(text_t *t, FILE *rom) {
    fwrite(&(t->code), t->code.dsize, t->code.count, rom);
    return 0;
}

void text_fprintf(text_t *t, FILE *output_fd) {
    fprintf(output_fd, "%02d - %04X - ", t->pointer_idx, t->cpu_address);
//    fwrite(t->u8srt, sizeof(u_int32_t), u8_strlen(t->u8srt), output_fd);
    fputc('\n', output_fd);
}

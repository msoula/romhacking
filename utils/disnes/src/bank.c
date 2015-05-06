/*
 * bank.c
 *
 *  Created on: 21 juin 2012
 *      Author: msoula
 */

#include "bank.h"
#include "jmps.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define JSR_INCREMENT	256

static void bank_add_jsr_off(bank_t *bank, u_int16_t offset);

static int bank_find_jsr_off(bank_t *bank, u_int16_t offset);

static void bank_treat_jmps(bank_t *bank);

bank_t *bank_init() {
    bank_t *bank;
    if (NULL == (bank = (bank_t *) malloc(sizeof(bank_t)))) {
        return NULL;
    }
    bank->first = bank->last = NULL;
    bank->size = 0;
    bank->start_address = 0x0000;

    // initializing jsr offsets array
    if (NULL == (bank->jsr_offsets = (u_int16_t *) malloc(JSR_INCREMENT * sizeof(u_int16_t)))) {
        bank_destroy(bank);
        return NULL;
    }
    bank->jsr_count = 0;
    bank->jsr_size = JSR_INCREMENT;

    return bank;
}
void bank_destroy(bank_t *bank) {
    command_t *cur = bank->first;
    command_t *tmp;
    while (cur) {
        tmp = cur->next;
        command_destroy(cur);
        cur = tmp;
    }
    if (bank->jsr_offsets)
        free(bank->jsr_offsets);
    free(bank);
}

int bank_load(bank_t *bank, void *data, size_t size) {

    u_int8_t *page = (u_int8_t *) data;

    int i = 0;
    command_t *cmd;
    while (i < (int) size) {
        if (NULL == (cmd = command_init())) {
            return 1;
        }

        cmd->offset = i;
        memcpy(cmd->bytes, &page[i], instruction_param_number(instruction_set[page[i]]) + 1);
        cmd->prev = NULL;
        cmd->next = NULL;

        bank_append(bank, cmd);

        i += instruction_param_number(instruction_set[page[i]]) + 1;
    }
    return 0;
}

int bank_append(bank_t *bank, command_t *cmd) {
    if (!bank->first) {
        bank->first = bank->last = cmd;
        return 0;
    }

    command_t *cur = bank->first;
    while (cur->next)
        cur = cur->next;

    cmd->prev = cur;
    cur->next = bank->last = cmd;
    return 0;
}

void bank_set_start_address(bank_t *bank, u_int16_t start_addr) {
    if(NULL == bank)
        return;

    bank->start_address = start_addr;
}

void bank_fprintf(FILE *f, bank_t *bank) {

    command_t *cur;

    /* first, we build the branches tree that stores every jumps in the code */
    jmps_init();
    bank_treat_jmps(bank);
    jmps_compute_levels();

    char line[1024];
    int length;
    int check_jsr = 0;

    cur = bank->first;
    while (cur) {
        if (check_jsr && bank_find_jsr_off(bank, bank->start_address + cur->offset)) {
            /* separate jsr/rts bloc */
            fprintf(f, ";---------------------------------------------------------------------------------------\n"
                       ";---------------------------------------------------------------------------------------\n"
                       ";\n;\n;\n");
        }
        check_jsr = 0;

        memset(line, '\0', 1024);

        length = jmps_format(line, cur->offset, RightToLeft);
        length += sprintf(&line[length], "%s", command_to_str(cur, bank->start_address));

        // if current command is RTS, check next offset is pointed by JSR command
        if (RTS == instruction_set[cur->bytes[0]].mnemonic) {
            check_jsr = 1;
        }

        fprintf(f, "%s\n", line);
        cur = cur->next;
    }

    jmps_destroy();
}

/******************************************************************************/
void bank_add_jsr_off(bank_t *bank, u_int16_t offset) {
    if (!bank_find_jsr_off(bank, offset)) {
        // inserting offset at pos
        if (bank->jsr_size == bank->jsr_count) {
            bank->jsr_offsets = (u_int16_t *) realloc(bank->jsr_offsets,
                    (bank->jsr_size + JSR_INCREMENT) * sizeof(u_int16_t));
            bank->jsr_size += JSR_INCREMENT;
        }
        bank->jsr_offsets[bank->jsr_count++] = offset;
    }
}
/******************************************************************************/
// fixme: classement et recherche dichotomique?
int bank_find_jsr_off(bank_t *bank, u_int16_t offset) {
    int found = 0;
    int pos;
    for (pos = 0; !found && pos < bank->jsr_count; pos++) {
        if (bank->jsr_offsets[pos] == offset) {
            found = 1;
        }
    }
    return found;
}
/******************************************************************************/
void bank_treat_jmps(bank_t *bank) {

    command_t *cur = bank->first, *tmp;
    unsigned int offset, dest_off, found;

    while (cur) {
        if (Relative == instruction_set[cur->bytes[0]].add_mode) {
            offset = cur->offset;
            dest_off = (cur->offset + 2 + (int8_t) cur->bytes[1]) & 0xFFFF;

            // check dest offset exists
            found = 0;
            tmp = bank->first;
            while (!found && tmp) {
                if (tmp->offset == dest_off)
                    found = 1;
                tmp = tmp->next;
            }

            if (found) {
                /* insert */
                jmps_add(offset, dest_off);
            }

        } else if (JSR == instruction_set[cur->bytes[0]].mnemonic) {

            // inserting jump address
            offset = (cur->bytes[2] * 0x100 + cur->bytes[1]);
            bank_add_jsr_off(bank, offset);

        }

        cur = cur->next;
    }

}

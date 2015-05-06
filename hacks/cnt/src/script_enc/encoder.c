/*
 * encoder.c
 *
 *      Author: rid
 */

#include "encoder.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "spcdef.h"
#include "utils/logger.h"
#include "utils/spc_utf8.h"
#include "utils/tags.h"
#include "utils/unicode_converter.h"
#include "utils/utf8_def.h"

extern tag_data_t script_tag_patterns[];

int encode(array_t *dest, const u_int32_t *src, dictionary_t *dict, table_t *table) {

    u_int8_t index;
    int nb_cars;
    const mte_t *mte;

    while (src[0] != '\0') {

        nb_cars = 0;

        E_SCRIPT_TAG_ID tag_id;
        int tag_length;
        if(src[0] == '\n') {
            /* skip pretty printing chars */
            nb_cars = 1;
        }
        else if (1 == is_u8_script_tag(src, &tag_id, &tag_length)) {

            index = (u_int8_t) (script_tag_patterns[tag_id].hex_value & 0xFF);
            array_add(dest, &index);

            if (script_tag_patterns[tag_id].is_value) {
                int tag_hex_value = 0x00;
                script_tag_get_value(src, &tag_hex_value);

                index = (u_int8_t) (tag_hex_value & 0xFF);
                array_add(dest, &index);
            }

            nb_cars += tag_length;
        }
        else if (NULL != (mte = dictionary_find(dict, src))) {

            index = (u_int8_t) (script_tag_patterns[E_SCRIPT_TAG_DICO_START].hex_value & 0xFF);
            array_add(dest, &index);
            index = (u_int8_t) (mte->idx & 0xFF);
            array_add(dest, &index);

            nb_cars = mte_length(mte);
        }
        else if(0 < (nb_cars = table_get_best_value_index(table, src, &index))) {
            /* insert index value from table */
            array_add(dest, &index);
        }
        else {
            /* caracter not found */
            LOG_ERROR("encode() failed - cannot encode %08X.", src[0]);
            return 1;
        }

        src += nb_cars;
    }
    return 0;
}

/******************************************************************************/

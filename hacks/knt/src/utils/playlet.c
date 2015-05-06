/*
 * playlet.c
 *
 *  Created on: 8 déc. 2014
 *      Author: rid
 */

#include "playlet.h"
#include "logger.h"

#include <stdlib.h>
#include <string.h>
#include <libxml/xmlreader.h>

static int anim_data_load_from_rom(array_t *anim_data, FILE *rom, int is_shared);
static int anim_data_store_to_rom(array_t *anim_data, FILE *rom);
static int anim_data_fprintf(array_t *anim_data);
static void treat_jump_882B(array_t *a, FILE *rom, u_int8_t param);
static void treat_bit_7(array_t *a, FILE *rom, u_int8_t param);
static void treat_bit_6(array_t *a, FILE *rom, u_int8_t param);
static void treat_function_06(array_t *a, FILE *rom, u_int8_t param);
static void treat_function_09(array_t *a, FILE *rom, u_int8_t param);
static void treat_function_0B(array_t *a, FILE *rom, u_int8_t param);
static void treat_function_0D(array_t *a, FILE *rom, u_int8_t param);
static void treat_function_0E(array_t *a, FILE *rom, u_int8_t param);
static void treat_function_0F(array_t *a, FILE *rom, u_int8_t param);
static void treat_function_11(array_t *a, FILE *rom, u_int8_t param);
static void treat_function_13(array_t *a, FILE *rom, u_int8_t param);
static void treat_function_14(array_t *a, FILE *rom, u_int8_t param);
static void treat_function_15(array_t *a, FILE *rom, u_int8_t param);

playlet_t *playlet_init(u_int8_t index, u_int8_t bank, u_int16_t address) {

    playlet_t *p;

    if(NULL == (p = (playlet_t *) malloc(sizeof(playlet_t)))) {
        LOG_ERROR("playlet_init() failed - cannot allocate memory");
        return NULL;
    }
    p->index   = index;
    p->bank    = bank;
    p->address = address;
    ptrlist_init(&(p->anims));

    return p;
}
void playlet_destroy(playlet_t *p) {

    /* remove every animation data */
    while(0 < ptrlist_count(&(p->anims))) {

        anim_t *cur = NULL;
        ptrlist_remove(&(p->anims), 0, (void **) &cur);
        if(NULL != cur)
            anim_destroy(cur);

    }

    ptrlist_uninit(&(p->anims));
    free(p);
}

int playlet_load_from_rom(playlet_t *p, FILE *rom, cpu_memory_t *cpu_mem) {

    u_int8_t byte_1, byte_2;
    slot_t slot;
    long int offset;

    /* save current offset */
    offset = file_get_current_position(rom);

    /* store current slot data at $A000 (ugly stuff incoming) */
    slot = cpu_mem->slots[1];

    /* load playlet's bank in memory */
    cpu_modify_slot(cpu_mem, 1, (slot_t) {p->bank, 0x10 + (p->bank * 0x2000), 0x10 + ((p->bank + 1) * 0x2000) - 1});

    /* go to bank position*/
    if (-1 == file_go_to(rom, cpu_get_offset_from_address(cpu_mem, p->address))) {
        LOG_ERROR("playlet_load_from_rom() failed - cannot go to position 0x%08lX.",
                cpu_get_offset_from_address(cpu_mem, p->address));
        return 1;
    }

    /* decode playlet */
    while(1) {
        anim_t *anim;

        /* read two bytes */
        byte_1 = file_load_byte(rom);
        byte_2 = file_load_byte(rom);
        if(0xFF == byte_1 && byte_2 == 0xFF) {
            /* end of playlet */
            break;
        }

        anim = anim_init();

        /* read animation length */
        anim->length = ((byte_2 * 0x100) + byte_1) & 0xFFFF;

        /* read animation data */
        if(0 == anim_load_from_rom(anim, rom)) {
            /* insert anim in playlet */
            ptrlist_add(&(p->anims), anim);
        }
        else {
            /* destroy invalid anim */
            anim_destroy(anim);
        }
    }

    /* restore bank in memory */
    cpu_modify_slot(cpu_mem, 1, slot);

    /* go back to current offset */
    if (-1 == file_go_to(rom, offset)) {
        LOG_ERROR("playlet_load_from_rom() failed - cannot go to position 0x%08lX.", offset);
    }

    return 0;
}

int playlet_store_to_rom(playlet_t *p, FILE *rom, cpu_memory_t *cpu_mem) {
    slot_t    slot;
    long int  offset;
    int       idx;
    u_int16_t eop = 0xFFFF;   /* end of playlet value */

    /* save current offset */
    offset = file_get_current_position(rom);

    /* store current slot data at $A000 (ugly stuff incoming) */
    slot = cpu_mem->slots[1];

    /* load playlet's bank in memory */
    cpu_modify_slot(cpu_mem, 1, (slot_t) {p->bank, 0x10 + (p->bank * 0x2000), 0x10 + ((p->bank + 1) * 0x2000) - 1});

    /* go to playlet position*/
    if (-1 == file_go_to(rom, cpu_get_offset_from_address(cpu_mem, p->address))) {
        LOG_ERROR("playlet_store_to_rom() failed - cannot go to position 0x%08lX.",
                cpu_get_offset_from_address(cpu_mem, p->address));
        return 1;
    }

    for(idx = 0; idx  < ptrlist_count(&(p->anims)); idx++) {
        anim_store_to_rom(ptrlist_get(&(p->anims), idx), rom);
    }
    fwrite(&eop, sizeof(u_int16_t), 1, rom);

    /* restore bank in memory */
    cpu_modify_slot(cpu_mem, 1, slot);

    /* go back to current offset */
    if (-1 == file_go_to(rom, offset)) {
        LOG_ERROR("playlet_store_to_rom() failed - cannot go to position 0x%08lX.", offset);
    }

    return 0;
}

int playlet_printf(playlet_t *p) {
    int idx;

    fprintf(stdout, "*** playlet at $%04X in bank 0x%02X:\n", p->address, p->bank);
    for(idx = 0; idx  < ptrlist_count(&(p->anims)); idx++) {
        anim_printf(ptrlist_get(&(p->anims), idx));
    }
    fprintf(stdout, "FFFF\n");
    return 0;
}

/******************************************************************************/

shared_anim_t *shared_anim_init(u_int8_t index, int16_t address) {
    shared_anim_t *sa;
    if(NULL == (sa = (shared_anim_t *) malloc(sizeof(shared_anim_t)))) {

        LOG_ERROR("shared_anim_init() failed - cannot allocate memory");
        return NULL;
    }

    sa->index   = index;
    sa->address = address;
    array_init(&(sa->data), sizeof(u_int8_t));
    ptrlist_init(&(sa->anims));

    return sa;
}
void shared_anim_destroy(shared_anim_t *sa) {

    /* remove every animation data */
    while(0 < ptrlist_count(&(sa->anims))) {

        anim_t *cur = NULL;
        ptrlist_remove(&(sa->anims), 0, (void **) &cur);
        if(NULL != cur)
            anim_destroy(cur);

    }

    ptrlist_uninit(&(sa->anims));
    array_uninit(&(sa->data));
    free(sa);

}
int shared_anim_load_from_rom(shared_anim_t *sa, FILE *rom, cpu_memory_t *cpu_mem) {

    array_t *last_anim_data;
    u_int8_t byte_1, byte_2;
    long int offset;

    /* save current offset */
    offset = file_get_current_position(rom);

    /* go to playlet position*/
    if (-1 == file_go_to(rom, cpu_get_offset_from_address(cpu_mem, sa->address))) {
        LOG_ERROR("shared_anim_load_from_rom() failed - cannot go to position 0x%08lX.",
                cpu_get_offset_from_address(cpu_mem, sa->address));
        return 1;
    }

    /* first, only read data */
    anim_data_load_from_rom(&(sa->data), rom, 1);
    last_anim_data = &(sa->data);

    /* decode playlet */
    while(1) {
        anim_t *anim;

        /* test two last read bytes */
        if(2 <= array_count(last_anim_data)) {
            byte_1 = *((u_int8_t*) array_get(last_anim_data, array_count(last_anim_data) - 2));
            byte_2 = *((u_int8_t*) array_get(last_anim_data, array_count(last_anim_data) - 1));
            if(0x16 == byte_1 && byte_2 == 0x80) { /* 0x1680 indicates the end of shared animation */
                /* end of playlet */
                break;
            }
        }

        anim = anim_init();

        /* read animation length */
        byte_1 = file_load_byte(rom);
        byte_2 = file_load_byte(rom);
        anim->length = ((byte_2 * 0x100) + byte_1) & 0xFFFF;

        /* read animation data */
        if(0 == anim_data_load_from_rom(&(anim->data), rom, 1)) {
            last_anim_data = &(anim->data);
            /* insert anim in playlet */
            ptrlist_add(&(sa->anims), anim);
        }
        else {
            /* destroy invalid anim */
            anim_destroy(anim);
        }
    }

    /* go back to current offset */
    if (-1 == file_go_to(rom, offset)) {
        LOG_ERROR("shared_anim_load_from_rom() failed - cannot go to position 0x%08lX.", offset);
    }

    return 0;

}
int shared_anim_store_to_rom(shared_anim_t *sa, FILE *rom, cpu_memory_t *cpu_mem) {

    long int  offset;
    int       idx;

    /* save current offset */
    offset = file_get_current_position(rom);

    /* go to playlet position*/
    if (-1 == file_go_to(rom, cpu_get_offset_from_address(cpu_mem, sa->address))) {
        LOG_ERROR("shared_anim_store_to_rom() failed - cannot go to position 0x%08lX.",
                cpu_get_offset_from_address(cpu_mem, sa->address));
        return 1;
    }

    /* first write data */
    anim_data_store_to_rom(&(sa->data), rom);

    /* then other animations */
    for(idx = 0; idx  < ptrlist_count(&(sa->anims)); idx++) {
        anim_t *a = ptrlist_get(&(sa->anims), idx);
        fwrite(&(a->length), sizeof(u_int16_t), 1, rom);
        anim_data_store_to_rom(&(a->data), rom);
    }

    /* go back to current offset */
    if (-1 == file_go_to(rom, offset)) {
        LOG_ERROR("shared_anim_store_to_rom() failed - cannot go to position 0x%08lX.", offset);
    }

    return 0;

}
int shared_anim_printf(shared_anim_t *sa) {
    int idx;

    fprintf(stdout, "*** shared_anim_printf at $%04X:\n", sa->address);

    /* first printf data */
    anim_data_fprintf(&(sa->data));
    fprintf(stdout, "\n");

    for(idx = 0; idx  < ptrlist_count(&(sa->anims)); idx++) {
        anim_t *a = ptrlist_get(&(sa->anims), idx);
        fprintf(stdout, "%02X%02X", (u_int8_t) a->length & 0xFF, (u_int8_t) ((a->length & 0xFF00) / 0x100));
        anim_data_fprintf(&(a->data));
        fprintf(stdout, "\n");
    }
    return 0;
}

/******************************************************************************/
anim_t *anim_init() {

    anim_t *a;
    if(NULL == (a = (anim_t *) malloc(sizeof(anim_t)))) {

        LOG_ERROR("anim_init() failed - cannot allocate memory");
        return NULL;
    }

    a->length = -1;
    array_init(&(a->data), sizeof(u_int8_t));

    return a;
}
void anim_destroy(anim_t *a) {
    array_uninit(&(a->data));
    free(a);
}
int anim_load_from_rom(anim_t *a, FILE *rom) {

    return anim_data_load_from_rom(&(a->data), rom, 0);

}
int anim_store_to_rom(anim_t *a, FILE *rom) {

    /* print length first */
    fwrite(&(a->length), sizeof(u_int16_t), 1, rom);
    anim_data_store_to_rom(&(a->data), rom);

    return 0;
}
int anim_printf(anim_t *a) {
    /* print length first */
    fprintf(stdout, "%02X%02X", (u_int8_t) a->length & 0xFF, (u_int8_t) ((a->length & 0xFF00) / 0x100));
    anim_data_fprintf(&(a->data));
    return 0;
}

/******************************************************************************/

int anim_data_load_from_rom(array_t *anim_data, FILE *rom, int is_shared) {
    u_int8_t type;
    u_int8_t param;

    /* decode anim */
    type = file_load_byte(rom);

    while(0xFF != type) {

        /* insert type into anim data */
        array_add(anim_data, &type);

        /* test bit#7 */
        if(0 != (type & 0x80)) {
            treat_bit_7(anim_data, rom, type & 0x3F);
        }
        /* test bit#6 */
        else if(0 != (type & 0x40)) {
            treat_bit_6(anim_data, rom, type & 0x3F);
        }
        else {
            u_int8_t func_idx = type & 0x3F;

            /* read bytes according value */
            switch(func_idx) {
            case 0x00:
            case 0x01:
            case 0x02:
                /* no more byte to read */
                break;
            case 0x03:
            case 0x04:
            case 0x05:
            case 0x07:
            case 0x08:
            case 0x0A:
            case 0x0C:
            case 0x10:
            case 0x12:
                /* read function param */
                param = file_load_byte(rom);
                array_add(anim_data, &param);
                break;
            case 0x16:
                /* read function param */
                param = file_load_byte(rom);
                array_add(anim_data, &param);

                if(is_shared && 0x80 == param)
                    goto end;

                break;
            case 0x06:
                /* read function param */
                param = file_load_byte(rom);
                array_add(anim_data, &param);
                treat_function_06(anim_data, rom, param);
                break;
            case 0x09:
                /* read function param */
                param = file_load_byte(rom);
                array_add(anim_data, &param);
                treat_function_09(anim_data, rom, param);
                break;
            case 0x0B:
                /* read function param */
                param = file_load_byte(rom);
                array_add(anim_data, &param);
                treat_function_0B(anim_data, rom, param);
                break;
            case 0x0D:
                /* read function param */
                param = file_load_byte(rom);
                array_add(anim_data, &param);
                treat_function_0D(anim_data, rom, param);
                break;
            case 0x0E:
                /* read function param */
                param = file_load_byte(rom);
                array_add(anim_data, &param);
                treat_function_0E(anim_data, rom, param);
                break;
            case 0x0F:
                /* read function param */
                param = file_load_byte(rom);
                array_add(anim_data, &param);
                treat_function_0F(anim_data, rom, param);
                break;
            case 0x11:
                /* read function param */
                param = file_load_byte(rom);
                array_add(anim_data, &param);
                treat_function_11(anim_data, rom, param);
                break;
            case 0x13:
                /* read function param */
                param = file_load_byte(rom);
                array_add(anim_data, &param);
                treat_function_13(anim_data, rom, param);
                break;
            case 0x14:
                /* read function param */
                param = file_load_byte(rom);
                array_add(anim_data, &param);
                treat_function_14(anim_data, rom, param);
                break;
            case 0x15:
                /* read function param */
                param = file_load_byte(rom);
                array_add(anim_data, &param);
                treat_function_15(anim_data, rom, param);
                break;
            default:
                LOG_ERROR("anim_load_from_rom() failed - invalid function index: %02X.", func_idx);
                return 1;
                break;
            }
        }

        type = file_load_byte(rom);
    }

    /* insert type into anim data */
    array_add(anim_data, &type);

end:
    return 0;
}

int anim_data_store_to_rom(array_t *anim_data, FILE *rom) {
    fwrite(anim_data->data, anim_data->dsize, anim_data->count, rom);
    return 0;
}

int anim_data_fprintf(array_t *anim_data) {
    int idx;

    /* print length first */
    for(idx = 0; idx  < array_count(anim_data); idx++) {
        u_int8_t byte = *((u_int8_t *) array_get(anim_data, idx));
        fprintf(stdout, "%02X", byte);
    }

    return 0;
}

/******************************************************************************/
void treat_jump_882B(array_t *anim_data, FILE *rom, u_int8_t param) {
    u_int8_t tmp;

    if(1 == param) {
        /* read one more byte */
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
    }
    else if (2 == param) {
        /* read 2 more bytes */
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
    }
    else {
        /* read 3 more bytes */
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
    }
}
/******************************************************************************/
void treat_bit_7(array_t *anim_data, FILE *rom, u_int8_t param) {

    u_int8_t tmp;

    if(0 != param) {

        /* read 5 more bytes */
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);

        while(param > 0) {
            /* read 4 more bytes */
            tmp = file_load_byte(rom);
            array_add(anim_data, &tmp);
            tmp = file_load_byte(rom);
            array_add(anim_data, &tmp);
            tmp = file_load_byte(rom);
            array_add(anim_data, &tmp);
            tmp = file_load_byte(rom);
            array_add(anim_data, &tmp);

            /* check if jump to $882B must be done */
            tmp = (tmp & 0x0F);
            if(0 != tmp) {
                treat_jump_882B(anim_data, rom, tmp);
            }

            param--;
        }
    }

}
/******************************************************************************/
void treat_bit_6(array_t *anim_data, FILE *rom, u_int8_t param) {
    (void) param;
    u_int8_t tmp;

    /* read 2 more bytes */
    tmp = file_load_byte(rom);
    array_add(anim_data, &tmp);
    tmp = file_load_byte(rom);
    array_add(anim_data, &tmp);

}
/******************************************************************************/
void treat_function_06(array_t *anim_data, FILE *rom, u_int8_t param) {
    u_int8_t tmp;

    if(0 != (param & 0x80)) {
        /* read 3 more bytes */
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
    }
    else {
        /* read 2 more bytes */
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
    }

}
/******************************************************************************/
void treat_function_09(array_t *anim_data, FILE *rom, u_int8_t param) {
    (void) param;
    u_int8_t tmp;

    /* read one more byte */
    tmp = file_load_byte(rom);
    array_add(anim_data, &tmp);
}
/******************************************************************************/
void treat_function_0B(array_t *anim_data, FILE *rom, u_int8_t param) {
    u_int8_t cpt;
    u_int8_t tmp;

    cpt = (param & 0x0F);
    while(0 < cpt) {
        /* read one more byte */
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
        cpt--;
    }

}
/******************************************************************************/
void treat_function_0D(array_t *anim_data, FILE *rom, u_int8_t param) {
    u_int8_t tmp;

    if(0 != param) {
        /* read one more byte */
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
    }

}
/******************************************************************************/
void treat_function_0E(array_t *anim_data, FILE *rom, u_int8_t param) {
    u_int8_t tmp;

    if(0 != param) {
        /* read one more byte */
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
    }

}
/******************************************************************************/
void treat_function_0F(array_t *anim_data, FILE *rom, u_int8_t param) {
    u_int8_t tmp;

    if(0 == (param & 0x80)) {
        /* read two more bytes */
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
    }
    else {
        /* read one more byte */
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
    }

}
/******************************************************************************/
void treat_function_11(array_t *anim_data, FILE *rom, u_int8_t param) {
    u_int8_t tmp = (param & 0x0F);

    if(0 != tmp) {
        treat_jump_882B(anim_data, rom, tmp);
    }
}
/******************************************************************************/
void treat_function_13(array_t *anim_data, FILE *rom, u_int8_t param) {
    u_int8_t tmp;

    if(0 == (param & 0x80)) {
        /* read one more byte */
        tmp = file_load_byte(rom);
        array_add(anim_data, &tmp);
    }

}
/******************************************************************************/
void treat_function_14(array_t *anim_data, FILE *rom, u_int8_t param) {
    (void) param;
    u_int8_t tmp;

    /* read 4 more bytes */
    tmp = file_load_byte(rom);
    array_add(anim_data, &tmp);
    tmp = file_load_byte(rom);
    array_add(anim_data, &tmp);
    tmp = file_load_byte(rom);
    array_add(anim_data, &tmp);
    tmp = file_load_byte(rom);
    array_add(anim_data, &tmp);

}
/******************************************************************************/
void treat_function_15(array_t *anim_data, FILE *rom, u_int8_t param) {
    (void) param;
    u_int8_t tmp;

    /* read one more byte */
    tmp = file_load_byte(rom);
    array_add(anim_data, &tmp);

}

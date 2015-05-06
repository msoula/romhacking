/*
 * jmps.c
 *
 *  Created on: 1 déc. 2014
 *      Author: rid
 */

#include "jmps.h"
#include "utils/ptrlist.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INCREMENT    16

#define MIN(x, y)    (((x) <= (y)) ? (x) : (y))
#define MAX(x, y)    (((x) > (y)) ? (x) : (y))

#define jmp_top(x)       ((x)->top)
#define jmp_bottom(x)    ((x)->bottom)

/**
 * jmp_include(x,y) indicates whether jmp x < jmp y.
 */
#define jmp_lt(x, y)    ((jmp_top((x)) < jmp_top((y))) || (jmp_top((x)) == jmp_top((y)) && jmp_bottom((x)) < jmp_bottom((y))))

/**
 * jmp_include(x,y) indicates whether jmp x > jmp y.
 */
#define jmp_gt(x, y)    ((jmp_top((x)) > jmp_top((y))) || (jmp_top((x)) == jmp_top((y)) && jmp_bottom((x)) > jmp_bottom((y))))

/**
 * jmp_include(x,y) indicates whether jmp x includes jmp y.
 * - x(0, 10) includes y(2, 9)
 * - x(0, 10) includes y(2, 10)
 * - x(1, 7) does not include y(0, 8)
 */
#define jmp_includes(x, y)    (jmp_top((x)) <= jmp_top((y)) && jmp_bottom((y)) <= jmp_bottom((x)))

/**
 * jmp_cross(x,y) indicates whether jmpes x,y are crossing.
 * - x(0, 5) crosses with y(4, 9)
 * - x(0, 3) crosses with y(3, 10)
 * - x(1, 7) does not cross with y(8, 10)
 */
#define jmp_crosses(x, y)    (jmp_top((x)) < jmp_top((y)) && jmp_bottom((x)) < jmp_bottom((y)) && jmp_top((y)) <= jmp_bottom((x)))

typedef struct jmp {
    int direction;         /*!< jmp direction (0: from bottom to top, 1: from top to bottom). */
    int top;               /*!< top offset. */
    int bottom;            /*!< bottom offset. */

    int level;

    ptrlist_t includes;   /*!< included jmps. */
    ptrlist_t crosses;    /*!< crossed jmps. */
} jmp_t;

static jmp_t *jmp_init(int top, int bottom, int direction);
static void jmp_destroy(jmp_t *jmp);
static void jmp_include(jmp_t *dest, jmp_t *jmp);
static void jmp_cross(jmp_t *dest, jmp_t *jmp);
static int jmp_compute_level(jmp_t *jmp);
static int jmp_format_level(jmp_t* jmp, char *dest, int offset, int level);

static ptrlist_t jmps;
static ptrlist_t leveled_jmps;

void jmps_init() {
    ptrlist_init(&jmps);
    ptrlist_init(&leveled_jmps);
}
void jmps_destroy() {
    int cpt;
    jmp_t *cur;

    /* free leveled jumps list */
    ptrlist_uninit(&leveled_jmps);

    /* destroying children first */
    cpt = 0;
    while(NULL != (cur = (jmp_t *) ptrlist_get(&jmps, cpt))) {
        jmp_destroy(cur);
        cpt++;
    }
    ptrlist_uninit(&leveled_jmps);
    ptrlist_uninit(&jmps);
}

void jmps_add(int from, int to) {
    jmp_t *new, *cur;
    int cpt;
    int top, bottom, direction;

    top       = MIN(from, to);
    bottom    = MAX(from, to);
    direction = (from == top);

    new = jmp_init(top, bottom, direction);

    /* jmps are ordered by top, then bottom */
    cpt = 0;
    while(NULL != (cur = (jmp_t *) ptrlist_get(&jmps, cpt)) && jmp_gt(new, cur)) {
        cpt++;
    }

    if(cpt < ptrlist_count(&jmps)) {
        /* insert new branch */
        ptrlist_set(&jmps, new, cpt);
    }
    else {
        /* add jmp to list */
        ptrlist_add(&jmps, new);
    }
}

void jmps_compute_levels() {
    int cpt, cpt2;
    jmp_t *cur, *next;

    /* copy every jmps to leveled jmps list */
    cpt = 0;
    while(NULL != (cur = (jmp_t *) ptrlist_get(&jmps, cpt))) {
        ptrlist_add(&leveled_jmps, cur);
        cpt++;
    }

    /* first pass, we treat crossed jmps */
    cpt = 0;
    while(NULL != (cur = (jmp_t *) ptrlist_get(&leveled_jmps, cpt))) {

        cpt2 = 0;
        if(cpt2 == cpt)
            cpt2++;
        while(NULL != (next = (jmp_t *) ptrlist_get(&leveled_jmps, cpt2))) {
            if(jmp_crosses(cur, next)) {
//                fprintf(stdout, "*** [%d:%d] crossed with [%d:%d]\n", cur->top, cur->bottom, next->top, next->bottom);
                jmp_cross(next, cur);
            }
            cpt2++;
            if(cpt2 == cpt)
                cpt2++;
        }

        cpt++;
    }

    /* second pass, we treat included jmps */
    cpt = 0;
    while(NULL != (cur = (jmp_t *) ptrlist_get(&leveled_jmps, cpt))) {

        int inserted = 0;

        cpt2 = 0;
        if(cpt2 == cpt)
            cpt2++;
        while(NULL != (next = (jmp_t *) ptrlist_get(&leveled_jmps, cpt2))) {
            if(jmp_includes(next, cur)) {
//                fprintf(stdout, "*** [%d:%d] included in [%d:%d]\n", cur->top, cur->bottom, next->top, next->bottom);
                jmp_include(next, cur);
                inserted = 1;
            }
            cpt2++;
            if(cpt2 == cpt)
                cpt2++;
        }

        if(inserted) {
            /* remove item from list, no need to increment counter */
            ptrlist_remove(&leveled_jmps, cpt, NULL);
        }
        else {
            cpt++;
        }
    }

    /* finally, we compute levels */
    cpt = 0;
    while(NULL != (cur = (jmp_t *) ptrlist_get(&leveled_jmps, cpt))) {
        jmp_compute_level(cur);
        cpt++;
    }
}

int jmps_format(char *dest, int offset, OutputOrientation orientation) {
    int cpt, i;
    jmp_t *cur;
    int max_level = jmps_get_level();

    for(i = 0; i < max_level; i++) {

        cpt = 0;
        while(NULL != (cur = (jmp_t *) ptrlist_get(&leveled_jmps, cpt))) {
            jmp_format_level(cur, dest, offset, i);
            cpt++;
        }

    }

    // completing string with space to get common length
    i = (int) strlen(dest);
    while (i < (max_level + 1) * 3)
        dest[i++] = ' ';

    if (RightToLeft == orientation) {
        // reverse string according orientation
        char *p = dest;
        char *q = &dest[i];
        for (--q; p < q; ++p, --q) {
            char tmp = *p;
            *p = *q;
            *q = tmp;
            if (*p == '<')
                *p = '>';
            if (*q == '<')
                *q = '>';
        }
        dest[i++] = ' ';
    }
    return i;
}

int jmps_get_level() {
    int cpt;
    jmp_t *cur;
    int level = 0;

    /* destroying children first */
    cpt = 0;
    while(NULL != (cur = (jmp_t *) ptrlist_get(&leveled_jmps, cpt))) {
        if(level < cur->level)
            level = cur->level + 1;
        cpt++;
    }

    return level;

}

/******************************************************************************/
jmp_t *jmp_init(int top, int bottom, int direction) {

    jmp_t *jmp;
    if (NULL == (jmp = (jmp_t *) malloc(sizeof(jmp_t)))) {
        return NULL;
    }

    jmp->level     = 0;
    jmp->top       = top;
    jmp->bottom    = bottom;
    jmp->direction = direction;
    ptrlist_init_size(&(jmp->includes), INCREMENT);
    ptrlist_init_size(&(jmp->crosses), INCREMENT);

    return jmp;
}
/******************************************************************************/
void jmp_destroy(jmp_t *jmp) {
    ptrlist_uninit(&(jmp->includes));
    ptrlist_uninit(&(jmp->crosses));
    free(jmp);
}
/******************************************************************************/
void jmp_include(jmp_t *dest, jmp_t *jmp) {

    int cpt;
    jmp_t *cur;
    int done;

    /* remove shared includes */
    cpt = 0;
    while(NULL != (cur = (jmp_t *) ptrlist_get(&(jmp->includes), cpt))) {

        if(ptrlist_contains(&(dest->includes), cur)) {
            ptrlist_remove_item(&(dest->includes), cur);
        }

        cpt++;
    }

    done = 0;
    cpt = 0;
    while(NULL != (cur = (jmp_t *) ptrlist_get(&(dest->includes), cpt))) {

        if(jmp_includes(cur, jmp)) {
            if(!ptrlist_contains(&(cur->includes), jmp)) {
                jmp_include(cur, jmp);
            }
            /* jmp is inserted in dest child */
            done = 1;
        }

        cpt++;
    }

    /* finally add new include if not included */
    if(!done) {
        ptrlist_add(&(dest->includes), jmp);
    }

}
/******************************************************************************/
void jmp_cross(jmp_t *dest, jmp_t *jmp) {

    /* add new cross if not already done */
    if(!ptrlist_contains(&(dest->crosses), jmp)) {
        ptrlist_add(&(dest->crosses), jmp);
    }

}
/******************************************************************************/
int jmp_compute_level(jmp_t *jmp)
{
    jmp_t *cur;
    int cpt, tmp, done;
    int level = 0;

    /* first, get maximum level from included branches */
    cpt = 0;
    while(NULL != (cur = (jmp_t *) ptrlist_get(&(jmp->includes), cpt))) {
        if(level <= (tmp = jmp_compute_level(cur))) {
            level = tmp + 1;
        }
        cpt++;
    }

    /* now, get first level compatible with crossed branches */
    done = 0;
    while(!done) {

        cpt = 0;
        while(NULL != (cur = (jmp_t *) ptrlist_get(&(jmp->crosses), cpt))) {
            if(level == cur->level)
                break;
            cpt++;
        }

        if(cpt < ptrlist_count(&(jmp->crosses))) {
            /* invalid level: try again with incremented value */
            level++;
        }
        else {
            /* level value has been found */
            done = 1;
        }
    }

    /* set branch level */
//    fprintf(stdout, "*** level of [%d:%d]: %d\n", jmp->top, jmp->bottom, level);
    jmp->level = level;
    return level;
}
/******************************************************************************/
int jmp_format_level(jmp_t* jmp, char *dest, int offset, int level) {
    jmp_t *cur;
    int cpt;

    if (level == jmp->level) {
        if (offset == (int) jmp->top) {
            // starting branch
            for (cpt = 0; cpt < (level + 1) * 3; cpt++) {
                if (0 == cpt && 0 == jmp->direction)
                    dest[cpt] = '<';
                else if (dest[cpt] == '\0' || dest[cpt] == ' ')
                    dest[cpt] = '-';
            }
            dest[cpt - 1] = '+';
        } else if (offset == (int) jmp->bottom) {
            // ending branch
            for (cpt = 0; cpt < (level + 1) * 3; cpt++) {
                if (0 == cpt && 1 == jmp->direction)
                    dest[cpt] = '<';
                else if (dest[cpt] == '\0' || dest[cpt] == ' ')
                    dest[cpt] = '-';
            }
            dest[cpt - 1] = '+';
        } else if (offset > (int) jmp_top(jmp) && offset < (int) jmp_bottom(jmp)) {
            // inter branch
            for (cpt = 0; cpt < (level + 1) * 3; cpt++) {
                if (dest[cpt] == '\0')
                    dest[cpt] = ' ';
            }
            dest[cpt - 1] = '|';
        }
        return 0;
    }

    cpt = 0;
    while(NULL != (cur = (jmp_t *) ptrlist_get(&(jmp->includes), cpt))) {
        if (offset >= (int) jmp_top(cur) && offset <= (int) jmp_bottom(cur)) {
            jmp_format_level(cur, dest, offset, level);
        }
        cpt++;
    }
    return 0;
}

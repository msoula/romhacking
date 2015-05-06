/*
 * jmps.h
 *
 *  Created on: 1 déc. 2014
 *      Author: rid
 */

#ifndef JMPS_H_
#define JMPS_H_

typedef enum _output_orientation {
    LeftToRight, RightToLeft
} OutputOrientation;

void jmps_init();
void jmps_destroy();

void jmps_add(int from, int to);
void jmps_compute_levels();
int jmps_format(char *dest, int offset, OutputOrientation orientation);
int jmps_get_level();

#endif /* JMPS_H_ */

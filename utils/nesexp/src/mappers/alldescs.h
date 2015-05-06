/*
 * alldescs.h
 *
 *  Created on: 14 juil. 2012
 *      Author: rid
 */

#ifndef ALLDESCS_H_
#define ALLDESCS_H_

#include "mappers.h"

extern mapper_desc_t nrom_desc;
extern mapper_desc_t mmc1_desc;
extern mapper_desc_t mmc3_desc;
extern mapper_desc_t mapper070_desc;

mapper_desc_t *mapper_descs[]={
&nrom_desc,
&mmc1_desc,
&mmc3_desc,
&mapper070_desc,
NULL
};

#endif /* ALLDESCS_H_ */

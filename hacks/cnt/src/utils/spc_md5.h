/*
 * spc_md5.h
 *
 *  Created on: 2 nov. 2011
 *      Author: rid
 */

#ifndef SPC_MD5_H_
#define SPC_MD5_H_

/**
 * @brief Check md5sum of given file content is equal to given md5 hash.
 *
 * @param file_name	file to check.
 * @param md5sum	md5 hash to compare.
 * @return 1 if file content hash is equal to md5sum.
 */
int md5_check_file(const char *file_name, const char *md5sum);


#endif /* SPC_MD5_H_ */

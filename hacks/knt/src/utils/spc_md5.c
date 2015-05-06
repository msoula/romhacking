/*
 * spc_md5.c
 *
 *  Created on: 2 nov. 2011
 *      Author: rid
 */

#include "spc_md5.h"

#include <errno.h>
#include <fcntl.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "logger.h"
#include "../spcdef.h"


static unsigned char md_value[EVP_MAX_MD_SIZE];
static unsigned int md_len;

//static void print_as_hex(const unsigned char *digest, int len) {
//	int i;
//	for (i = 0; i < len; i++) {
//		printf("%02x", digest[i]);
//	}
//	printf("\n");
//}

static void calculate_md5_of(const void *content, ssize_t len) {
	EVP_MD_CTX mdctx;

	EVP_DigestInit(&mdctx, EVP_md5());
	EVP_DigestUpdate(&mdctx, content, (size_t) len);
	EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
	EVP_MD_CTX_cleanup(&mdctx);
}

int md5_check_file(const char *file_name, const char *md5sum) {
	void *file_content;
	struct stat stat_buf;
	int i_rval = 0;
	int in_fd = -1;
	off_t size_of_file;
	ssize_t read_bytes;

	i_rval = stat(file_name, &stat_buf);
	size_of_file = stat_buf.st_size;
	file_content = malloc(size_of_file);
	if (NULL == file_content) {
		goto clean;
	}
	in_fd = open(file_name, 0, O_RDONLY);
	if (in_fd < 0) {
		goto clean;
	}
	/* slurp in all from the file at once */
	read_bytes = read(in_fd, file_content, size_of_file);
	if (read_bytes < 0) {
		LOG_ERROR("md5_check_file() failed - something has gone wrong while reading from the file\n");
		goto clean;
	}
	close(in_fd);
	calculate_md5_of(file_content, size_of_file);
	free(file_content);

	// comparing calculated md5 with given one
	int i;
	char md_str[EVP_MAX_MD_SIZE * 2];
	for (i = 0; i < (int) md_len; i++) {
		sprintf(&md_str[i * 2], "%02x", md_value[i]);
	}

	LOG_DEBUG("expected MD5 is '%s'.", md5sum);
	LOG_DEBUG("file MD5 is '%s'.", md_str);

	return (0 == strcasecmp(md5sum, md_str));

clean:
	if (file_content)
		free(file_content);
	if (in_fd > 0)
		close(in_fd);
	return 0;
}

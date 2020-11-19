#define _XOPEN_SOURCE 700
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#include "deonebook.h"

#define CBC 1
#define ECB 0
#define CTR 0
#include "tiny-AES-c/aes.c"

int decrypt(const unsigned char *key, const char *in, const char *out)
{
	int fd = open(in, O_RDONLY);
	if (fd == -1) {
		perror(in);
		return 1;
	}

	struct stat st;
	if (fstat(fd, &st) != 0) {
		perror(in);
		return 1;
	}

	unsigned char *buf = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (buf == MAP_FAILED) {
		perror(in);
		return 1;
	}

	close(fd);

	unsigned char iv[IV_SIZE];
	memset(iv, '\03', sizeof(iv));

	struct AES_ctx ctx = {0};
	AES_init_ctx_iv(&ctx, key, iv);
	AES_CBC_decrypt_buffer(&ctx, buf, st.st_size);

	fd = open(out, O_WRONLY | O_CREAT, 0644);
	if (fd == -1) {
		perror(out);
		return 1;
	}

	write(fd, buf, st.st_size);
	close(fd);

	return 0;
}

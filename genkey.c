#define _XOPEN_SOURCE 700
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "deonebook.h"

#define SCR_SIZE 8
#define CSD_SIZE 16
#define CID_SIZE 16
#define KEY_SIZE 16

static unsigned char twiddle(unsigned char buf[2])
{
	unsigned char c = 0;

	if (isdigit(buf[0])) {
		c = buf[0];
	} else if (isupper(buf[0])) {
		c = buf[0] - 0x37;
	} else if (islower(buf[0])) {
		c = buf[0] - 0x57;
	}
	c *= 0x10;

	if (isdigit(buf[1])) {
		c |= buf[1] - 0x30;
	} else if (isupper(buf[1])) {
		c |= buf[1] - 0x37;
	} else if (islower(buf[1])) {
		c |= buf[1] + 0xa9;
	}

	return c;
}

static bool get_sd_register(int dirfd, const char *reg, size_t n, unsigned char buf[n])
{
	int fd = openat(dirfd, reg, O_RDONLY);
	if (fd == -1) {
		return false;
	}

	for (size_t i = 0; i < n; i++) {
		unsigned char hex[2];
		read(fd, hex, sizeof(hex));
		buf[i] = twiddle(hex);
	}

	close(fd);

	return true;
}

unsigned char * genkey(int dirfd)
{
	static unsigned char key[KEY_SIZE];
	const unsigned char keycode[] = "eone";
	size_t codelen = sizeof(keycode) - 1;
	bool allread = true;

	unsigned char scr[SCR_SIZE] = "testtest";
	unsigned char csd[CSD_SIZE] = "testtesttesttest";
	unsigned char cid[CID_SIZE] = "testtesttesttest";

	if (!get_sd_register(dirfd, "cid", sizeof(cid), cid)) {
		fprintf(stderr, "warning: CID register not read\n");
		allread = false;
	}
	if (!get_sd_register(dirfd, "csd", sizeof(csd), csd)) {
		fprintf(stderr, "warning: CSD register not read\n");
		allread = false;
	}
	if (!get_sd_register(dirfd, "scr", sizeof(scr), scr)) {
		fprintf(stderr, "warning: SCR register not read\n");
		allread = false;
	}

	if (!allread) {
		fprintf(stderr, "warning: key may be incorrect\n");
	}

	memcpy(key, cid + 3, 10);
	memcpy(key + 10, csd + 7, 3);
	key[13] = scr[0];
	memcpy(key + 0xe, scr + 2, 2);

	for (size_t i = 0; i < KEY_SIZE; i++) {
		if (i < codelen) {
			key[i] += keycode[i];
		} else {
			key[i] += i - (codelen - 1);
		}
	}

	return key;
}

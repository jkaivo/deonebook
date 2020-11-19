#define _XOPEN_SOURCE 700
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "deonebook.h"

unsigned char *getkey(const char *keystring, const char *device)
{
	if (keystring) {
		static unsigned char key[KEY_SIZE] = "";
		if (strlen(keystring) != (KEY_SIZE * 2)) {
			fprintf(stderr, "key must be %d hex characters\n", KEY_SIZE * 2);
			return NULL;
		}

		for (size_t i = 0; i < sizeof(key); i++) {
			char hex[] = { keystring[i * 2], keystring[i * 2 + 1], '\0' };
			key[i] = (unsigned char)strtoul(hex, NULL, 16);
		}
		return key;
	}

	if (!device) {
		device = "mmcblk0";
	} else if (strstr(device, "mmcblk") != device) {
		fprintf(stderr, "device must be an mmcblk device\n");
		return NULL;
	}

	char path[256];
	snprintf(path, sizeof(path), "/sys/block/%s/device", device);
	DIR *d = opendir(path);
	if (d == NULL) {
		fprintf(stderr, "device '%s' not found\n", device);
		return NULL;
	}

	unsigned char *key = genkey(dirfd(d));
	if (key) {
		return key;
	}

	return readkey("./.shm");
}

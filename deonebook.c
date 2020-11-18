#define _XOPEN_SOURCE 700
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "deonebook.h"

int main(int argc, char *argv[])
{
	enum { DECRYPT, GENERATE, READ } mode = DECRYPT;

	int c;
	while ((c = getopt(argc, argv, "RGk:")) != -1) {
		switch (c) {
		case 'R':
			mode = READ;
			break;

		case 'G':
			mode = GENERATE;
			break;

		case 'k':
			/* keyfile = optarg; */
			break;

		default:
			return 1;
		}
	}

	if (mode != DECRYPT) {
		unsigned char *key = NULL;
		if (mode == READ) {
			key = readkey("./.shm");
		} else {
			char *device = "mmcblk0";
			if (optind < argc) {
				device = argv[optind];
			}

			if (strstr(device, "mmcblk") != device) {
				fprintf(stderr, "device must be an mmcblk device\n");
				return 1;
			}

			char path[256];
			snprintf(path, sizeof(path), "/sys/block/%s/device", device);
			DIR *d = opendir(path);
			if (d == NULL) {
				fprintf(stderr, "device '%s' not found\n", device);
				return 1;
			}

			key = genkey(dirfd(d));
		}

		if (key == NULL) {
			return 1;
		}

		for (int i = 0; i < KEY_SIZE; i++) {
			printf("%02hhx", key[i]);
		}
		printf("\n");

		return 0;
	}

	if (optind >= argc) {
		fprintf(stderr, "%s: missing operands\n", argv[0]);
		return 1;
	}

	do {
		printf("decrypting %s\n", argv[optind]);
	} while (++optind < argc);
}

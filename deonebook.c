#define _XOPEN_SOURCE 700
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
			key = genkey(device);
		}

		if (key == NULL) {
			return 1;
		}

		fwrite(key, 16, 1, stdout);
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

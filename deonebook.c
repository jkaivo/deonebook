#define _XOPEN_SOURCE 700
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "deonebook.h"

int main(int argc, char *argv[])
{
	char *keystring = NULL;
	char *device = NULL;

	int c;
	while ((c = getopt(argc, argv, "k:d:")) != -1) {
		switch (c) {
		case 'k':
			keystring = optarg;
			break;

		case 'd':
			device = optarg;
			break;

		default:
			return 1;
		}
	}

	unsigned char *key = getkey(keystring, device);

	if (key == NULL) {
		return 1;
	}

	if (optind >= argc) {
		for (int i = 0; i < KEY_SIZE; i++) {
			printf("%02hhx", key[i]);
		}
		printf("\n");
		return 0;
	}

	if (optind < argc - 2) {
		fprintf(stderr, "too many operands\n");
		return 1;
	}

	if (optind != argc - 2) {
		fprintf(stderr, "missing output file\n");
		return 1;
	}

	return decrypt(key, argv[optind], argv[optind + 1]);
}

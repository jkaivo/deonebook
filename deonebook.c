#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	char *serial = NULL;

	int c;
	while ((c = getopt(argc, argv, "s:")) != -1) {
		switch (c) {
		case 's':
			serial = optarg;
			break;

		default:
			return 1;
		}
	}

	if (serial == NULL) {
		fprintf(stderr, "%s: missing serial number (provide via -s)\n",
			argv[0]);
		return 1;
	}

	if (optind >= argc) {
		fprintf(stderr, "%s: missing operands\n", argv[0]);
		return 1;
	}

	do {
		printf("decrypting %s [%s]\n", argv[optind], serial);
	} while (++optind < argc);
}

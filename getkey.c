#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SCR_SIZE 8
#define CSD_SIZE 16
#define CID_SIZE 16
#define KEY_SIZE 16

unsigned char twiddle(unsigned char buf[2])
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

void get_sd_register(const char *dev, const char *reg, size_t n, unsigned char buf[n])
{
	char path[256];
	snprintf(path, sizeof(path), "/sys/block/%s/device/%s", dev, reg);

	FILE *f = fopen(path, "r");
	if (f == NULL) {
		return;
	}

	for (size_t i = 0; i < n; i++) {
		unsigned char hex[2];
		fread(hex, 2, 1, f);
		buf[i] = twiddle(hex);
	}

	fclose(f);
}

unsigned char * getkey(const char *dev)
{
	static unsigned char key[KEY_SIZE];
	const unsigned char keycode[] = "eone";
	size_t codelen = sizeof(keycode) - 1;

	unsigned char scr[SCR_SIZE] = "testtest";
	unsigned char csd[CSD_SIZE] = "testtesttesttest";
	unsigned char cid[CID_SIZE] = "testtesttesttest";

	get_sd_register(dev, "cid", sizeof(cid), cid);
	get_sd_register(dev, "csd", sizeof(csd), csd);
	get_sd_register(dev, "scr", sizeof(scr), scr);

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

int main(void)
{
	unsigned char *key = getkey("mmcblk0");
	printf("calculated:");
	for (size_t i = 0; i < KEY_SIZE; i++) {
		printf(" %02hhx", key[i]);
	}
	printf("\n");
}

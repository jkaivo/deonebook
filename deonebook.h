#ifndef DEONEBOOK_H
#define DEONEBOOK_H

#define KEY_SIZE 16
#define IV_SIZE 16

unsigned char *genkey(int dirfd);
unsigned char *readkey(const char *path);
unsigned char *getkey(const char *keystring, const char *device);
int decrypt(const unsigned char *key, const char *in, const char *out);

#endif

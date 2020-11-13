#ifndef DEONEBOOK_H
#define DEONEBOOK_H

#define KEY_SIZE 16

unsigned char *genkey(const char *dev);
unsigned char *readkey(const char *path);

#endif

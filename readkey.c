#define _XOPEN_SOURCE 700
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include "deonebook.h"

unsigned char *readkey(const char *path)
{
	key_t key = ftok(path, 0x2a);
	if (key == (key_t)(-1)) {
		perror("ftok");
		return NULL;
	}

	int shmid = shmget(key, 0, 0);
	if (shmid == -1) {
		perror("shmget");
		return NULL;
	}

	return shmat(shmid, NULL, 0);
}

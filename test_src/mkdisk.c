#include <stdio.h>
#include <stdlib.h>
#include "pdos.h"

//create a disk (MYFS) in dev/shm on 1024 blocks
int main() {
	long sz = BLOCK_SIZE * MAXBLOCKS;
	printf("Making disk of size %ld\n", sz);
	pdos_mkdisk(sz);
	struct stat buffer; //creating a stat buffer to be able
	// to access the file size
	int fd = open(MYFS, O_RDONLY);
	fstat(fd, &buffer);
	printf("Size of MYFS disk: %ld\n", buffer.st_size);
	return 0;
}
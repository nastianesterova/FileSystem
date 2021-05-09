#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include "pdos.h"

/**
 * Create shared memory of sz bytes
 * 
 * 
 **/

void pdos_mkdisk(int sz) {
	int fd = shm_open("MYFS", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO); // permit all
	if(fd == -1) {
		perror("Failed to open or create shared memory MYFS");
		exit(-1);	
	}	
	if (-1 == ftruncate(fd, sz)) {
		fprintf(stderr, "Failed to set shared memory to size %d: %s", sz, strerror(errno));
		exit(-1);
	}
}
 
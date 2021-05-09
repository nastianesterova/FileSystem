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

DISK_BLOCK* _pdos_open_disk() {
	int fd = open(MYFS, O_RDWR);
    void* disk = mmap(0, BLOCK_SIZE * MAXBLOCKS, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(disk == (void *) -1) {
        perror("Failed to open disk.");
        exit(-1);
    }
	return (DISK_BLOCK*) disk;
}

void _pdos_close_disk(DISK_BLOCK* disk_start) {
	    munmap(disk_start, BLOCK_SIZE * MAXBLOCKS);
}
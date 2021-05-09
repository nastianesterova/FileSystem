#include "pdos.h"

/**
 * formats in-memory FS
 * 1st block reserved, contains ID string
 * 2 FAT blocks
 * directory block
 * data blocks (initially -1)
**/

DISK_BLOCK* pdos_fs = 0;

void pdos_mkfs(char * ID) {
    DISK_BLOCK* blocks = _pdos_open_fs();
    // initialize reserved block with ID string, offset of 8 bytes
    strcpy(&blocks[0].data[8], ID); 
    // fill the FAT blocks
    //TODO: look over
    blocks[1].fat[0] = 0;
    blocks[1].fat[1] = 2;
    blocks[1].fat[2] = 0;
    blocks[1].fat[3] = 0;
    for(int i = 4; i < BLOCK_SIZE / 2; i++) {
        blocks[1].fat[i] = -1;
    }
    for(int i = 0; i < BLOCK_SIZE / 2; i++) {
        blocks[2].fat[i] = -1;
    }
    // initialize directory block
    blocks[3].dir.isdir = 1;
    blocks[3].dir.nextEntry = 0;

    _pdos_close_fs();
}

DISK_BLOCK* _pdos_open_fs() {
	if (pdos_fs) {
		return pdos_fs;
	}
	int fd = open(MYFS, O_RDWR);
    void* disk = mmap(0, BLOCK_SIZE * MAXBLOCKS, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(disk == (void *) -1) {
        perror("Failed to open disk.");
        exit(-1);
    }
	pdos_fs = (DISK_BLOCK*)disk;
	return pdos_fs;
}

void _pdos_close_fs() {
	if (pdos_fs) {
	    munmap(pdos_fs, BLOCK_SIZE * MAXBLOCKS);
	}
}
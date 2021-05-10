#include "pdos.h"

/**
 * formats in-memory FS
 * 1st block reserved, contains ID string
 * 2 FAT blocks
 * directory block
 * data blocks (initially -1)
**/

static DISK_BLOCK* pdos_fs = 0;

void pdos_mkfs(char * ID) {
    _pdos_open_fs();

    // initialize reserved block with ID string, offset of 8 bytes
    DISK_BLOCK block;
    memset(&block, 0, sizeof(DISK_BLOCK));
    strcpy(&block.data[8], ID);
    _pdos_write_block(&block, 0);

    // fill the FAT blocks
    //TODO: look over
    block.fat[0] = 0;
    block.fat[1] = 2;
    block.fat[2] = 0;
    block.fat[3] = 0;
    for(int i = 4; i < BLOCK_SIZE / 2; i++) {
        block.fat[i] = -1;
    }
    _pdos_write_block(&block, 1);
    for(int i = 0; i < BLOCK_SIZE / 2; i++) {
        block.fat[i] = -1;
    }
    _pdos_write_block(&block, 2);

    // initialize directory block
    memset(&block, 0, sizeof(DISK_BLOCK));
    block.dir.isdir = 1;
    block.dir.nextEntry = 0;
    _pdos_write_block(&block, 3);

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

void _pdos_write_block(DISK_BLOCK* block, int block_num) {
    memcpy(pdos_fs + block_num, block, sizeof(DISK_BLOCK));
}

// params:
// block: into which you will read a block
// block_num: number of block in disk that you read from
void _pdos_read_block(DISK_BLOCK* block, int block_num) {
    memcpy(block, pdos_fs + block_num, sizeof(DISK_BLOCK));
}

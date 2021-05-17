#include "pdos.h"

/**
 * formats in-memory FS
 * First four blocks responsible for management of files
 * -------- start of disk
 * 1st block reserved, contains ID string
 * --------
 * FAT block
 * FAT block
 * --------
 * root directory block
 * --------
 * data blocks
 * .....
 * -------- end of disk
 * 
 * FAT block data entries are short (2 bytes)
 * initialized to 0 2 0 0 -1 -1 ...
 * Directory block has first entry - root directory,
 * with is_dir byte set to 1, rest are 0's.
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
	pdos_fs = (DISK_BLOCK*)disk; //cast to disk block
	return pdos_fs;
}

void _pdos_close_fs() {
	if (pdos_fs) {
	    munmap(pdos_fs, BLOCK_SIZE * MAXBLOCKS);
	}
}

// simulates writing block of data to the disk
// block: block which you will write to disk
// block_num: block # on disk into which you will write to
void _pdos_write_block(DISK_BLOCK* block, int block_num) {
    memcpy(pdos_fs + block_num, block, sizeof(DISK_BLOCK));
}

// params:
// block: into which you will read a block
// block_num: number of block in disk that you read from
void _pdos_read_block(DISK_BLOCK* block, int block_num) {
    memcpy(block, pdos_fs + block_num, sizeof(DISK_BLOCK));
}

// This function will return the state of the block
// from fat table (2 FAT blocks)
// param fat_blocks is array of 2 FAT blocks.
short _pdos_get_block_state(DISK_BLOCK* fat_blocks, int block_num) {
    int block_index = block_num / (BLOCK_SIZE / 2);
    int entry_index = block_num % (BLOCK_SIZE / 2);
    return fat_blocks[block_index].fat[entry_index];
}

// opposite of get.
void _pdos_set_block_state(DISK_BLOCK* fat_blocks, int block_num, short state) {
    int block_index = block_num / (BLOCK_SIZE / 2);
    int entry_index = block_num % (BLOCK_SIZE / 2);
    fat_blocks[block_index].fat[entry_index] = state;
}
#include "pdos.h"

/**
 * formats in-memory FS
 * 1st block reserved, contains ID string
 * 2 FAT blocks
 * directory block
 * data blocks (initially -1)
**/

void pdos_mkfs(char * ID) {
    DISK_BLOCK* blocks = _pdos_open_disk();
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

    _pdos_close_disk(blocks);
}
#include "pdos.h"

/**
 * formats in-memory FS
 * 1st block reserved, contains ID string
 * 2 FAT blocks
 * directory block
 * data blocks (initially -1)
**/

void pdos_mkfs(char * ID) {
    int fd = open(MYFS, O_RDWR);
    void* disk = mmap(0, BLOCK_SIZE * MAXBLOCKS, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(disk == (void *) -1) {
        perror("Failed to open disk.");
        exit(-1);
    }
    munmap(disk, BLOCK_SIZE * MAXBLOCKS);
}
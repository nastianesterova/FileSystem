#include "pdos.h"

PDOS_FILE *pdos_open(const char *fname, const char *mode) {
    printf("max directory entries: %ld\n", MAX_NUM_DIRECTORIES_ENTRIES);
    _pdos_open_fs();
    // open fs, look for fname in directory block pdos_fs + 3
    // need to initialize block to something
    DISK_BLOCK* block;
    _pdos_read_block(block, 3);

    return 0;
}
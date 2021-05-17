#include "pdos.h"

int pdos_fgetc(PDOS_FILE * file) {
    // need a way to keep track of WHICH byte in the file needs to be read
    // use pos for this
    
    // determine pos
    // using filename determine which block(s) need to be loaded (pdos_read_block)
    // load those files, use pos to read
    if(strcmp(file->mode, "w") == 0) {
        fprintf(stderr, "Cannot read file opened in 'w' mode.\n");
        exit(-1);
    }

    // file is in rw or r mode
    // we need to check if we are not reading past end of file.
    // for this we need to always load fat table to check if current
    // block is last or not. Also, if the block is last, we need to
    // load directory table to check the file size
    DISK_BLOCK fat_blocks[2];
    _pdos_read_block(&fat_blocks[0], 1);
    _pdos_read_block(&fat_blocks[1], 2);
    
    // if current block is last, we check is we are past the end of file
    int nextblock = _pdos_get_block_state(fat_blocks, file->blocknum);
    if (nextblock == 0) {
        // means file->blocknum is last
        DISK_BLOCK dir_block;
        _pdos_read_block(&dir_block, 3);
        // we count all blocks from the beginning except current which is the last
        int blocknum = dir_block.dir.dir_entry_list[file->entrylistIdx].filefirstblock;
        int fileoffset = 0;
        while (blocknum != file->blocknum) {
            fileoffset += BLOCK_SIZE;
            blocknum = _pdos_get_block_state(fat_blocks, blocknum); // next block
        }
        fileoffset += file->pos; // for current block add pos.
        // if offset past end of file, we cannot read.
        if (fileoffset >= dir_block.dir.dir_entry_list[file->entrylistIdx].filelength) {
            return -1; // as in fgetc() documentation
        }
    }
    // At this point we know we can read.
    if (file->pos == BLOCK_SIZE) {
        // load new buffer
        file->pos = 0;
        file->blocknum = nextblock;
        _pdos_read_block(&file->buffer, file->blocknum);
    }

    int c = file->buffer.data[file->pos];
    ++file->pos;
    return c;
}
#include "pdos.h"

void pdos_fputc(int b, PDOS_FILE* file) {
    // need a way to keep track of WHICH byte in the file needs to be written to
    // use pos for this
    
    // determine pos
    // using filename determine which block(s) need to be loaded (pdos_read_block)
    // load those files, use pos to write b
    // write back to disk using pdos_write_disk

    // search fat tables for name of file?
    if(strcmp(file->mode, "r") == 0) {
        fprintf(stderr, "Cannot write to read-only file.\n");
        exit(-1);
    }

    // otherwise can write to file
    if(file->pos < BLOCK_SIZE) {
        // there is space in buffer
        file->buffer.data[file->pos] = b;
        ++file->pos;
        return; // we don't update file system because still writing to the same buffer.
    }

    // We cannot write to current buffer, need to write to the next block.
    // But before, we need to update directory entry
    DISK_BLOCK fat_blocks[2];
    _pdos_read_block(&fat_blocks[0], 1);
    _pdos_read_block(&fat_blocks[1], 2);
    int current_block_fat_index = file->blocknum / (BLOCK_SIZE / 2);
    int current_block_fat_entry_index = file->blocknum % (BLOCK_SIZE / 2);

    DISK_BLOCK dir_block;
    _pdos_read_block(&dir_block, 3);
    // if this one was the last block for the file, we need to update file size
    // in the directory entry
    if (fat_blocks[current_block_fat_index].fat[current_block_fat_entry_index] == 0) {
        // This was the last block
        // we count all blocks from the beginning
        int blocknum = dir_block.dir.dir_entry_list[file->entrylistIdx].filefirstblock;
        int filesize = 0;
        while (blocknum != 0) {
            filesize += BLOCK_SIZE;
            int fat_index = blocknum % (BLOCK_SIZE / 2);
            int fat_entry_index = blocknum ;
            blocknum = fat_blocks[fat_index].fat[fat_entry_index]; // next block
        }
        dir_block.dir.dir_entry_list[file->entrylistIdx].filelength = filesize;
    }
    dir_block.dir.dir_entry_list[file->entrylistIdx].filemodtime = time(NULL);
    _pdos_write_block(&dir_block, 3);

    // then need to save current buffer to the disk
    _pdos_write_block(&(file->buffer), file->blocknum);


    // we may need to find new buffer
    int nextblocknum = fat_blocks[current_block_fat_index].fat[current_block_fat_entry_index];
    if (nextblocknum == 0) {
        // need to start a new buffer for new block
        // if there is space, need to still check if there is space on the disk
        // need to check for free block on the disk through FAT table
        int nextblock_fat_index = 0;
        int nextblock_fat_entry_index = 0;
        // searching for free block in a circular manner starting from the current block
        for(int i = file->blocknum + 1; i < MAXBLOCKS + file->blocknum; ++i) {
            nextblock_fat_index = (i % MAXBLOCKS) / (BLOCK_SIZE / 2);
            nextblock_fat_entry_index = i % (BLOCK_SIZE / 2); 
            if(fat_blocks[nextblock_fat_index].fat[nextblock_fat_entry_index] == -1) {
                nextblocknum = i % MAXBLOCKS;
                break;
            }
        }
        if(!nextblocknum) {
            fprintf(stderr, "No additional space on disk.\n");
            exit(-1);
        }
        // there is space to create an additional buffer, update fat 
        fat_blocks[current_block_fat_index].fat[current_block_fat_entry_index] = nextblocknum;
        fat_blocks[nextblock_fat_index].fat[nextblock_fat_entry_index] = 0;

        // update fat table
        _pdos_write_block(&fat_blocks[0], 1);
        _pdos_write_block(&fat_blocks[1], 2);
        file->blocknum = nextblocknum;
    }
    else {
        // next block exists for this file (this can happen for "rw")
        // load the next block from disk into buffer
        file->blocknum = nextblocknum;
        _pdos_read_block(&(file->buffer), file->blocknum);
    }

    // write byte at the start of buffer and update pos.
    file->buffer.data[0] = b;
    file->pos = 1;
}
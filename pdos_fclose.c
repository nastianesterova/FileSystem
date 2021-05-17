#include "pdos.h"

void pdos_fclose(PDOS_FILE * file) {
    if(!file) return;
    // if file has write permission we need to update size and write buffer to disk.
    if(strcmp(file->mode, "w") == 0 || strcmp(file->mode, "rw") == 0) {
        if(file->pos != 0) {
            // if pos not 0, then we might have been writing to current buffer.
            DISK_BLOCK fat_blocks[2];
            _pdos_read_block(&fat_blocks[0], 1);
            _pdos_read_block(&fat_blocks[1], 2);
            int current_block_fat_index = file->blocknum / (BLOCK_SIZE / 2);
            int current_block_fat_entry_index = file->blocknum % (BLOCK_SIZE / 2);

            DISK_BLOCK dir_block;
            _pdos_read_block(&dir_block, 3);
            // if this one is the last block for the file, we need to update file size
            // in the directory entry
            if (fat_blocks[current_block_fat_index].fat[current_block_fat_entry_index] == 0) {
                // This was the last block
                // we count all blocks from the beginning except last
                int blocknum = dir_block.dir.dir_entry_list[file->entrylistIdx].filefirstblock;
                int filesize = 0;
                while (blocknum != file->blocknum) {
                    filesize += BLOCK_SIZE;
                    int fat_index = blocknum % (BLOCK_SIZE / 2);
                    int fat_entry_index = blocknum ;
                    blocknum = fat_blocks[fat_index].fat[fat_entry_index]; // next block
                }
                filesize += file->pos;
                // for "rw" mode the filelength can finish past the current position (for ex reading
                // not up until the end)
                // in this case original length in the directory entry
                // will be bigger than calculated filesize
                if (filesize > dir_block.dir.dir_entry_list[file->entrylistIdx].filelength) {
                    dir_block.dir.dir_entry_list[file->entrylistIdx].filelength = filesize;
                }
            }
            dir_block.dir.dir_entry_list[file->entrylistIdx].filemodtime = time(NULL);
            _pdos_write_block(&dir_block, 3);

            // then need to save current buffer to the disk
            _pdos_write_block(&(file->buffer), file->blocknum);
        }
    }
    free(file); // because it was allocated on heap.
}
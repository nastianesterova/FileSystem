#include "pdos.h"

void pdos_mkdir(char *dir) {
    // parse dir
    
    // verify name length
    if (strlen(dir) >= MAXNAME) {
        fprintf(stderr, "Invalid dir name.\n");
        exit(-1);
    }

    // open fs, look for dir in directory block. If found cannot create directory
    _pdos_open_fs();
    DISK_BLOCK dir_block;
    int dir_block_idx = 3;
    _pdos_read_block(&dir_block, dir_block_idx);
    for(int i = 0; i < dir_block.dir.nextEntry; ++i) {
        if(dir_block.dir.dir_entry_list[i].isdir) {
            if(strcmp(dir_block.dir.dir_entry_list[i].name, dir) == 0) {
                fprintf(stderr, "Cannot create duplicate directory.\n");
                exit(-1);
            }
        }
    }

    // directory does not already exist. Create the directory

    // check if there is an available entry in directory block to create sub dir
    if(dir_block.dir.nextEntry == MAX_NUM_DIRECTORIES_ENTRIES) {
        fprintf(stderr, "No entries left to create directory.\n");
        exit(-1);
    }

    // if there is space in dir block, need to still check if there is space on the disk
    // need to check for available entries through FAT table
    DISK_BLOCK fat_blocks[2];
    _pdos_read_block(&fat_blocks[0], 1);
    _pdos_read_block(&fat_blocks[1], 2);
    int free_block = 0;
    for(int i = 4; i < MAXBLOCKS; ++i) {
        if (_pdos_get_block_state(fat_blocks, i) == -1) {
            free_block = i;
            break;
        }
    }
    if(!free_block) {
        fprintf(stderr, "No space on the disk left to create directory.\n");
        exit(-1);
    }

    // set free block in FATs to 0
    _pdos_set_block_state(fat_blocks, free_block, 0);
    // write fat back to disk
    _pdos_write_block(&fat_blocks[0], 1);
    _pdos_write_block(&fat_blocks[1], 2);

    // there is space to add the sub directory
    DIR_ENTRY* dir_entry = &dir_block.dir.dir_entry_list[dir_block.dir.nextEntry];
    dir_block.dir.nextEntry++;
    dir_entry->isdir = 1;
    dir_entry->filemodtime = time(NULL);
    dir_entry->filelength = 0;
    dir_entry->filefirstblock = free_block;
    strcpy(dir_entry->name, dir);
    // write dir block back to disk
    _pdos_write_block(&dir_block, dir_block_idx);
}
#include "pdos.h"

PDOS_FILE *pdos_open(const char *fname, const char *mode) {
    //printf("max directory entries: %ld\n", MAX_NUM_DIRECTORIES_ENTRIES);
    if(strcmp(mode, "r") && strcmp(mode, "w") && strcmp(mode, "rw")) {
        fprintf(stderr, "Invalid file mode.\n");
        exit(-1);
    }
    _pdos_open_fs();
    // open fs, look for fname in directory block pdos_fs + 3
    DISK_BLOCK dir_block;
    _pdos_read_block(&dir_block, 3);

    // search dir_block for fname (search for existing file)
    for(int i = 0; i < dir_block.dir.nextEntry; ++i) {
        if(!dir_block.dir.dir_entry_list[i].isdir) {
            if(strcmp(dir_block.dir.dir_entry_list[i].name, fname) == 0) {
                // file has been found
                // open file for r, w or rw
                // if w, delete file and exit loop
                if(strcmp(mode, "w") == 0) {
                    // File needs to be truncated to 0 size
                    // set FAT table entries to -1 except the first, which will be set to 0
                    DISK_BLOCK fat_block[2];
                    int second_loaded = 0;
                    short fat_index = dir_block.dir.dir_entry_list[i].filefirstblock;
                    if(fat_index < BLOCK_SIZE / 2) {
                        // first FAT aka block index 1 on disk
                        _pdos_read_block(&fat_block[0], 1);
                        while(fat_index < BLOCK_SIZE / 2) {
                            short next_index = fat_block[0].fat[fat_index];
                            fat_block[0].fat[fat_index] = -1;
                            fat_index = next_index;
                            if(fat_index == 0) break;
                        }
                    }
                    if(fat_index >= BLOCK_SIZE / 2) {
                        second_loaded = 1;
                        _pdos_read_block(&fat_block[1], 2);
                        while(fat_index < BLOCK_SIZE) {
                            short next_index = fat_block[1].fat[fat_index];
                            fat_block[1].fat[fat_index] = -1;
                            fat_index = next_index;
                            if(fat_index == 0) break;
                        }
                    }
                    if(dir_block.dir.dir_entry_list[i].filefirstblock < BLOCK_SIZE / 2) {
                        fat_block[0].fat[dir_block.dir.dir_entry_list[i].filefirstblock] = 0;
                        _pdos_write_block(&fat_block[0], 1);
                        if(second_loaded) {
                            _pdos_write_block(&fat_block[1], 2);
                        }
                    }
                    else {
                        fat_block[1].fat[dir_block.dir.dir_entry_list[i].filefirstblock - BLOCK_SIZE / 2] = 0;
                        _pdos_write_block(&fat_block[1], 2);
                    }
                    dir_block.dir.dir_entry_list[i].filelength = 0;
                    dir_block.dir.dir_entry_list[i].filemodtime = time(NULL);
                    _pdos_write_block(&dir_block, 3);
                }
                PDOS_FILE* file = malloc(sizeof(PDOS_FILE)); // must free in pdos_fclose
                strcpy(file->mode, mode);
                file->pos = 0;
                file->blocknum = dir_block.dir.dir_entry_list[i].filefirstblock;
                file->entrylistIdx = i;
                // must copy data from disk into buffer
                _pdos_read_block(&file->buffer, file->blocknum);
                return file;
            }
        }
    }

    // if file is not found and must be opened for writing or rw, create it
    // TODO: create only for w or rw?
    if(strcmp(mode, "r") == 0) {
        return NULL;
    }
    // if there is no space in dir block, return null
    if(dir_block.dir.nextEntry == MAX_NUM_DIRECTORIES_ENTRIES) return NULL;
    // if there is space, need to still check if there is space on the disk
    // need to check for available entries through FAT table
    DISK_BLOCK fat_block[2];
    _pdos_read_block(&fat_block[0], 1);
    _pdos_read_block(&fat_block[1], 2);
    int first_free_block = 0;
    for(int i = 4; i < MAXBLOCKS; ++i) {
        int block_index = i / (BLOCK_SIZE / 2);
        int entry_index = i % (BLOCK_SIZE / 2);
        if(fat_block[block_index].fat[entry_index] == -1) {
            first_free_block = i;
            break;
        }
    }
    if(!first_free_block) return NULL; // no free blocks to add file to
    // set first free block in FATs to 0
    int free_fat_block_idx = first_free_block / (BLOCK_SIZE / 2);
    int free_fat_entry_idx = first_free_block % (BLOCK_SIZE / 2);
    fat_block[free_fat_block_idx].fat[free_fat_entry_idx] = 0;
    _pdos_write_block(&fat_block[free_fat_block_idx], free_fat_block_idx + 1);

    // there is space to add the file
    DIR_ENTRY* dir_entry = &dir_block.dir.dir_entry_list[dir_block.dir.nextEntry];
    dir_block.dir.nextEntry++;
    dir_entry->isdir = 0;
    dir_entry->filemodtime = time(NULL);
    dir_entry->filelength = 0;
    dir_entry->filefirstblock = first_free_block;
    strncpy(dir_entry->name, fname, MAXNAME);
    _pdos_write_block(&dir_block, 3);

    PDOS_FILE* file = malloc(sizeof(PDOS_FILE)); // must free in pdos_fclose
    strcpy(file->mode, mode);
    file->pos = 0;
    file->blocknum = first_free_block;
    file->entrylistIdx = dir_block.dir.nextEntry - 1;
    return file;
    

    return 0;
}
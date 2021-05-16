#include "pdos.h"

// + 1 because last element is NULL
static char* contents[MAX_NUM_DIRECTORIES_ENTRIES+1];
static DISK_BLOCK dir_block;

char ** pdos_dir() {
    // figure out what the current directory is
    // for now assume root directory
    // load directory block
    // for every entry in dir_entry_list of the block
    // if isdir, add filename or subdir name to the list
    // return list

    // open filesystem to get directory info
    _pdos_open_fs();

    // load directory block
    int dir_block_index = 3;
    _pdos_read_block(&dir_block, dir_block_index);
    for(int i = 0; i < dir_block.dir.nextEntry; ++i) {
        // printf("%s\n", dir_block.dir.dir_entry_list[i].name);
        contents[i] = dir_block.dir.dir_entry_list[i].name;
    }
    contents[dir_block.dir.nextEntry] = NULL;
    return contents;
}
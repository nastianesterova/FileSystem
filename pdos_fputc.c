#include "pdos.h"

void pdos_fputc(int b, PDOS_FILE* file) {
    // need a way to keep track of WHICH byte in the file needs to be written to
    // use pos for this
    
    // determine pos
    // using filename determine which block(s) need to be loaded (pdos_read_block)
    // load those files, use pos to write b
    // write back to disk using pdos_write_disk

    // search fat tables for name of file?

}
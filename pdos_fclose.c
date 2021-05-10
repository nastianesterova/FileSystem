#include "pdos.h"

void pdos_fclose(PDOS_FILE * file) {
    if(!file) return;
    if(strcmp(file->mode, "w") == 0 || strcmp(file->mode, "rw") == 0) {
        if(file->pos != 0) {
            _pdos_write_block(&(file->buffer), file->blocknum);
        }
    }
}
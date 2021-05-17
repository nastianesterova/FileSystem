
/* 
 * File:   pseudoWriteFS.c
 * Author: osboxes
 * 1. Create a new file on pseudoFS
 * 2. Write out a single block (1024) characters (A-Z)
 * 3. Close file
 * *
 * * Check file system using hexdump -C MYFS
 *
 * Created on April 21, 2018, 1:29 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "pdos.h"
/*
 * 
 */
int main(int argc, char** argv) {
  
    PDOS_FILE *fd = NULL;
    char * fname = "file1.txt";
    char * mode = "rw";
    if (argc > 1) {
        fname = argv[1];
    }
    if (argc > 2) {
        mode = argv[2];
    }

    // open file for read/writing
    fd = pdos_open(fname, mode);
    if (!fd) {
        fprintf(stderr, "Cannot open file %s mode '%s'\n", fname, mode);
        exit(-1);
    }
    
    if(argc > 3) {
        int i = 0;
        while(argv[3][i]) {
            pdos_fputc(argv[2][i], fd);
            ++i;
        }
    }
    else {
        int charValue = 'A';  // Letter A
        for(int i = 0; i < BLOCK_SIZE - 1; i++, charValue++){
            if (charValue > 'Z')
                charValue = 'A';
            pdos_fputc(charValue, fd);
        }
    }
    // close file
    pdos_fclose(fd);
 
 /**/
    return (EXIT_SUCCESS);
}

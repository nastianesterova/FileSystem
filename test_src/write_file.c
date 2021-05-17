
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
    if (argc > 1) {
        fname = argv[1];
    }

    // open file for read/writing
    fd = pdos_open(fname, "rw");
    if (!fd) {
        fprintf(stderr, "Cannot open file %s\n", fname);
        exit(-1);
    }

    int numchars = BLOCK_SIZE - 1;
    if (argc > 2) {
        // if extra argument is 0 - write 1023 bytes, if 1 - write 1024 bytes
        if (argv[2][0] == '0') {
            numchars = BLOCK_SIZE - 1;
        }
        else if (argv[2][0] == '1') {
            numchars = BLOCK_SIZE;
        }
    }
    int charValue = 'A';  // Letter A
    if (argc > 3) {
        if (strcmp(argv[3], "expand") == 0) {
            // expand file by reading to the end and then writing
            int charInFile = pdos_fgetc(fd);
            while (charInFile != -1) {
                charValue = charInFile + 1;
                charInFile = pdos_fgetc(fd);
            }
        }
    }
    for(int i = 0; i < numchars; i++, charValue++){
        if (charValue > 'Z')
            charValue = 'A';
        pdos_fputc(charValue, fd);
    }
    // close file
    pdos_fclose(fd);
 
 /**/
    return (EXIT_SUCCESS);
}

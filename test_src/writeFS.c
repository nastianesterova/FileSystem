
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
    

    // open file for read/writing
    fd = pdos_open("file1.txt", "rw");
    if (!fd) {
        fprintf(stderr, "Cannot open file\n");
    }
    
    
    int charValue = 'A';  // Letter A
    for(int i = 0; i < BLOCK_SIZE - 1; i++, charValue++){
		if (charValue > 'Z')
			charValue = 'A';
        pdos_fputc(charValue, fd);
    }
    
    // close file
    pdos_fclose(fd);
   
 
 /**/
    return (EXIT_SUCCESS);
}

#include <stdio.h>
#include <stdlib.h>
#include "pdos.h"


int main(int argc, char** argv) 
{
    char * fname = argv[1]; 
    
    PDOS_FILE * file_in_system = pdos_open(fname, "r");
    
    // then loop through the file and compare
    
    int charValue = 'A';  // Letter A
    int i = 0;
    for(; i < BLOCK_SIZE * MAXBLOCKS; i++, charValue++)
    {
		if (charValue > 'Z')
        {
			charValue = 'A';
        }
        int charInFile = pdos_fgetc(file_in_system);
        if (charInFile == -1) break; // End of file
        if (charInFile != charValue)
        {
            printf("expected char: %c, Char in File: %c position %d\n", charValue, charInFile, i);
            printf("file not entered into the FS correctly\n");
            return -1;
        }
        // somehow advance the file_in_system ptr - it advance automatically 
    }
    printf("compared %d bytes. success\n", i);
    return 0;
}


    
    
    

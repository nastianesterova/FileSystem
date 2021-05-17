#include <stdio.h>
#include <stdlib.h>
#include "pdos.h"


int main(int argc, char** argv) 
{
    char * fname = argv[1]; 
    
    PDOS_FILE * file_in_system = pdos_open(fname, "r");
    
    // then loop through the file and compare
    
    int charInFile = pdos_fgetc(file_in_system);
    while (charInFile != -1)
    {
        putc(charInFile, stdout);
        charInFile = pdos_fgetc(file_in_system);
    }
    // printf("compared %d bytes. success\n", i);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "pdos.h"

// not really sure how the file is passed in here. Or do we just run the WriteFS in this function?
// also this is pretty crappy but I think it may be the gist

int main(int argc, char** argv) 
{
    // not sure how we get the file, maybe passed in?
    // maybe this
    //char * fname = argv[1]; 
    
    PDOS_FILE * file_in_system = pdos_open(fname, "r");
    
    // then loop through the file presumably and compare? (not sure what to compare to, maybe just the raw letters?)
    // need to somehow also get file size
    
    int charValue = 'A';  // Letter A
    for(int i = 0; i < file_sz - 1; i++, charValue++)
    {
		    if (charValue > 'Z')
			      charValue = 'A';
        if (pdos_fgetc(file_in_system) != charValue)
        {
            printf("file not entered intot the FS correctly);
            return 0;
        }
        printf("expected char: %c, Char in File: %c\n", charValue, (pdos_fgetc(file_in_system));
        // somehow advance the file_in_system ptr 
    }


    
    
    

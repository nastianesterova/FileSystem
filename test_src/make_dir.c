#include <stdio.h>
#include <stdlib.h>
#include "pdos.h"
/*
 * 
 */
int main(int argc, char** argv) {
  
    char * dir;
    if (argc > 1) {
        dir = argv[1];
    }
    else {
        printf("No directory name provided.\n");
        return -1;
    }

    pdos_mkdir(dir);
}
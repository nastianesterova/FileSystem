#include <stdio.h>
#include <stdlib.h>
#include "pdos.h"

int main(int argc, char** argv) {
    char** contents = pdos_dir();
    int i = 0;
    while(contents[i] != NULL) {
        printf("%s\n", contents[i]);
        ++i;
    }
    return 0;
}
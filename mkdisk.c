#include <stdio.h>
#include <stdlib.h>
#include "pdosfilesys.h"

//create a disk (MYFS) in dev/shm
int main() {
    pdos_mkdisk(10);
    return 0;
}

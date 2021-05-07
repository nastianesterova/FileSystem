#include <stdio.h>
#include <stdlib.h>
#include "pdosfilesys.h"

//create a disk (MYFS) in dev/shm
int main() {
	printf("Making disk\n");
	//pdos_mkdisk(10);
	printf("Made disk\n");
	printf("Hello");
	return 0;
}

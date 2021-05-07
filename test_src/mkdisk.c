#include <stdio.h>
#include <stdlib.h>
#include "/home/cosc255/FINAL/pdos.h"

//create a disk (MYFS) in dev/shm
int main() {
	printf("Making disk\n");
	pdos_mkdisk(10);
	printf("Made disk\n");
	return 0;
}

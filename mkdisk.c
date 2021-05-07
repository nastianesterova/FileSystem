#include <stdio.h>
#include <stdlib.h>
#include "pdosfilesys.h"

//create a disk (MYFS) in dev/shm
int main() {
	printf("Making disk\n");
<<<<<<< HEAD
	//pdos_mkdisk(10);
	printf("Made disk\n");
	printf("Hello");
=======
	pdos_mkdisk(10);
	printf("Made disk\n");
>>>>>>> 6291e7e013670ea113ce46d0e65e5988045a63a3
	return 0;
}

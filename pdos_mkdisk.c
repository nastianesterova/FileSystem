#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include "pdos.h"

/*
Creates disk of size sz
Normally 1024 blocks
*/
void pdos_mkdisk(int sz) {
	printf("pdos_mkdisk running\n");
	int fd = shm_open("MYFS", O_RDWR | O_CREAT, 0777); //0777 is for permissions
	if(fd == -1) {
		printf("File failed to open.\n");
		exit(-1);	
	}	
	ftruncate(fd, sz);
}
#ifndef PDOS_H
#define PDOS_H

#include "pdosfilesys.h"

void pdos_mkdisk(int sz);
void pdos_mkfs(char * ID);
PDOS_FILE *pdos_open(const char *fname, const char *mode);
int pdos_fgetc(PDOS_FILE * file);
void pdos_fputc(int b, PDOS_FILE* file);
void pdos_fclose(PDOS_FILE * file);
char **pdos_dir();
void pdos_mkdir(char *dir);

//extra functions
DISK_BLOCK* _pdos_open_fs();
void _pdos_close_fs();

#endif
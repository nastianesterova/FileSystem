#pragma once
typedef void PDOS_FILE;

void pdos_mkdisk(int sz);
void pdos_mkfs(char * ID);
PDOS_FILE *pdos_open(const char *fname, const char *mode);
int pdos_fgetc(PDOS_FILE *);
void pdos_fputc(int b, PDOS_FILE*);
void pdos_fclose(PDOS_FILE *);
char **pdos_dir();
void pdos_mkdir(char *dir);
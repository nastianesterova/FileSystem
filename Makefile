all: pdos_mkdisk pdos_mkfs pdos_open pdos_fgetc pdos_fputc pdos_fclose pdos_dir pdos_mkdir link

pdos_mkdisk:
	gcc -c pdos_mkdisk.c -o pdos_mkdisk.o

pdos_mkfs:
	gcc -c pdos_mkfs.c -o pdos_mkfs.o

pdos_open:
	gcc -c pdos_open.c -o pdos_open.o

pdos_fgetc:
	gcc -c pdos_fgetc.c -o pdos_fgetc.o

pdos_fputc:
	gcc -c pdos_fputc.c -o pdos_fputc.o

pdos_fclose:
	gcc -c pdos_fclose.c -o pdos_fclose.o

pdos_dir:
	gcc -c pdos_dir.c -o pdos_dir.o

pdos_mkdir:
	gcc -c pdos_mkdir.c -o pdos_mkdir.o
link:
	ar rcu libPseudoFS.a pdos_fgetc.o pdos_fputc.o pdos_mkdisk.o pdos_mkfs.o pdos_open.o pdos_fclose.o pdos_dir.o pdos_mkdir.o


clean:
	rm libPseudoFS.a pdos_mkdisk.o pdos_mkfs.o pdos_open.o pdos_fgetc.o pdos_fputc.o pdos_fclose.o pdos_dir.o pdos_mkdir.o



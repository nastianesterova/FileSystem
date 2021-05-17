#compile test code by hand/manually
#gcc -g -std=c11 -Wall -o mkdisk test_src/mkdisk.c -L./lib -lPseudoFS -lrt
#need to link librt, realtime extensions library, to the executable

OBJ_DIR = ./obj
LIB_DIR = ./lib
C_FLAGS = -std=c11 -Wall -D_XOPEN_SOURCE=700

tempdirs = $(OBJ_DIR) $(LIB_DIR)

lib_c_files = pdos_fgetc.c pdos_fputc.c pdos_mkdisk.c pdos_mkfs.c pdos_open.c pdos_fclose.c pdos_dir.c pdos_mkdir.c
# substitutes .c files and path with .o for all files listed in lib_c_files
lib_obj_files = $(patsubst %.c,$(OBJ_DIR)/%.o,$(lib_c_files))

# $@ is the name of the target being generated
# $< is the first prerequesite
# below code can be translated to
# ./obj/pdos_fgetc.o: pdos_fgetc.c
#		gcc -c -std=c11 -Wall -D_XOPEN_SOURCE=700 -o ./obj/pdos_fgetc.o pdos_fgetc.c
$(OBJ_DIR)/%.o: %.c
	gcc -c $(C_FLAGS) -o $@ $<

# ./lib/libPseudoFS.a: pdos_fgetc.o
#	ar rc -s ./lib/libPseudoFS.a pdos_fgetc.o
$(LIB_DIR)/libPseudoFS.a: $(lib_obj_files)
	ar rc -s $(LIB_DIR)/libPseudoFS.a $(lib_obj_files)

# creates subdirectory ./obj if it doesn't exist
$(OBJ_DIR): 
	@mkdir -p $(OBJ_DIR)

# creates subdirectory ./lib if it doesn't exist
$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

test_programs = mkdisk mkfs write_file read_from_file list_dir make_dir print_file

all_tests: $(test_programs)

mkdisk: test_src/mkdisk.c $(LIB_DIR)/libPseudoFS.a
	gcc $(C_FLAGS) -I. -o $@ $< $(LIB_DIR)/libPseudoFS.a -lrt

mkfs: test_src/mkfs.c $(LIB_DIR)/libPseudoFS.a
	gcc $(C_FLAGS) -I. -o $@ $< $(LIB_DIR)/libPseudoFS.a -lrt

write_file: test_src/write_file.c $(LIB_DIR)/libPseudoFS.a
	gcc $(C_FLAGS) -I. -o $@ $< $(LIB_DIR)/libPseudoFS.a -lrt

read_from_file: test_src/read_from_file.c $(LIB_DIR)/libPseudoFS.a
	gcc $(C_FLAGS) -I. -o $@ $< $(LIB_DIR)/libPseudoFS.a -lrt

print_file: test_src/print_file.c $(LIB_DIR)/libPseudoFS.a
	gcc $(C_FLAGS) -I. -o $@ $< $(LIB_DIR)/libPseudoFS.a -lrt

list_dir: test_src/list_dir.c $(LIB_DIR)/libPseudoFS.a
	gcc $(C_FLAGS) -I. -o $@ $< $(LIB_DIR)/libPseudoFS.a -lrt

make_dir: test_src/make_dir.c $(LIB_DIR)/libPseudoFS.a
	gcc $(C_FLAGS) -I. -o $@ $< $(LIB_DIR)/libPseudoFS.a -lrt

clean:
	rm $(OBJ_DIR)/*.o $(LIB_DIR)/*.a $(test_programs)
OBJ_DIR = ./obj
LIB_DIR = ./lib
C_FLAGS = -std=c11 -Wall -DXOPEN_SOURCE=700

tempdirs = $(OBJ_DIR) $(LIB_DIR)

lib_c_files = pdos_fgetc.c pdos_fputc.c pdos_mkdisk.c pdos_mkfs.c pdos_open.c pdos_fclose.c pdos_dir.c pdos_mkdir.c
lib_obj_files = $(patsubst %.c,$(OBJ_DIR)/%.o,$(lib_c_files))

$(OBJ_DIR)/%.o: %.c
	gcc -c $(C_FLAGS) -o $@ $<

$(LIB_DIR)/libPseudoFS.a: $(lib_obj_files)
	ar rc -s $(LIB_DIR)/libPseudoFS.a $(lib_obj_files)

$(OBJ_DIR): 
	@mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

test_programs = mkdisk

all_tests: $(test_programs)

mkdisk: test_src/mkdisk.c $(LIB_DIR)/libPseudoFS.a
	gcc $(C_FLAGS) -I. -o mkdisk test_src/mkdisk.c $(LIB_DIR)/libPseudoFS.a

clean:
	rm $(OBJ_DIR)/*.o $(LIB_DIR)/*.a $(test_programs)
OBJ_DIR=./obj
LIB_DIR=./lib

tempdirs = $(OBJ_DIR) $(LIB_DIR)

lib_c_files = pdos_fgetc.c pdos_fputc.c pdos_mkdisk.c pdos_mkfs.c pdos_open.c pdos_fclose.c pdos_dir.c pdos_mkdir.c
lib_obj_files = $(patsubst %.c,$(OBJ_DIR)/%.o,$(lib_c_files))

$(OBJ_DIR)/%.o: %.c
	gcc -c -std=c11 -Wall -DXOPEN_SOURCE=700 -o $@ $<

$(LIB_DIR)/libPseudoFS.a: $(lib_obj_files)
	ar rc -s $(LIB_DIR)/libPseudoFS.a $(lib_obj_files)

$(OBJ_DIR): 
	@mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

clean:
	rm $(OBJ_DIR)/*.o $(LIB_DIR)/*.a
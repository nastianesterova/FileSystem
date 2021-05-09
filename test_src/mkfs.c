#include "pdos.h"

int main(int argc, char** argv) {
    char* ID = "Anastasia John FS";
    if(argc > 1) {
        ID = argv[1];
    }

    pdos_mkfs(ID);
    return 0;
}
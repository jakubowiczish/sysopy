#include <dlfcn.h>
#include "lib.c"
#include "lib.h"

void *handle;

void init_dynamic(void *handle) {
    handle = dlopen("./lib.so", RTLD_LAZY);
}

void create_table_dynamic(__uint32_t size_of_array_) {
    init_dynamic(handle);
    void (*create_table__)(__uint32_t);
    create_table__ = (void (*)()) dlsym(handle, "create_table");
    (*create_table__)(size_of_array_);
}

void remove_block_dynamic(int block_index) {
    init_dynamic(handle);
    void (*remove_block__)(int);
    remove_block__ = (void (*)()) dlsym(handle, "remove_block");
    (*remove_block__)(block_index);
}

int search_directory_dynamic(char *dir, char *file, char *name_file_temp) {
    init_dynamic(handle);
    int (*search_directory__)(char *, char *, char *);
    search_directory__ = (int (*)(char *, char *, char *)) dlsym(handle, "search_directory");
    return search_directory__(dir, file, name_file_temp);
}

void dlclose_dynamic(void *handle) {
    dlclose(handle);
}




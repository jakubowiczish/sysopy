#ifndef SYSOPY_LIB_DYNAMIC_H
#define SYSOPY_LIB_DYNAMIC_H

#include <lzma.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

static void* handle = NULL;

void (*_create_table)(__uint32_t size_of_array);
void (*_remove_block)(int);
int (*_search_directory)(char *, char *, char *);

void init_dynamic_handler() {
    handle = dlopen("libfind.so", RTLD_LAZY);
    if(handle == NULL) {
        printf("ERROR WHILE LOADING DYNAMIC LIBRARY\n");
        return;
    }

    printf("SUCCESSFUL LOADING OF DYNAMIC LIBRARY\n");

    _create_table = dlsym(handle, "create_table");
    if (dlerror() != NULL) {
        printf("COULD NOT LOAD CREATE TABLE");
    }

    char* error;

    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }

    _remove_block = dlsym(handle, "remove_block");

    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }
    _search_directory = dlsym(handle, "search_directory");


    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(1);
    }
}

void create_table(__uint32_t size_of_array) {
    (*_create_table)(size_of_array);
}

void remove_block(int block_index) {
    (*_remove_block)(block_index);
}

int search_directory(char *dir, char *file, char *name_file_temp) {
    return (*_search_directory)(dir, file, name_file_temp);
}


void free_handler_memory() {
    dlclose(handle);
}
#endif //SYSOPY_LIB_DYNAMIC_H

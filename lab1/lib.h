#pragma once

#include <stdint.h>

const int SIZE_OF_BUFFER = 512;

struct Searcher {
    char *dir;
    char *file;
    char *name_file_temp;
};

long get_file_size(int fd);
void initialize_searching_array(__uint32_t size_of_array);
void initialize_result_array(__uint32_t size_of_array);
void delete_block_at_index(int block_index);
int search_directory(char *dir, char *file, char *name_file_temp);

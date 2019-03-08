#ifndef SYSOPY_LIBRARY_H
#define SYSOPY_LIBRARY_H

#include <stdint.h>

long get_file_size(int fd);
void initialize_result_array(__uint32_t size_of_array);
void delete_block_at_index(int block_index);
int search_directory(char *dir, char *file, char *name_file_temp);

#endif
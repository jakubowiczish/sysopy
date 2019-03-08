#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"

const int SIZE_OF_BUFFER = 512;

int ARRAY_SIZE;

struct Searcher {
    char *dir;
    char *file;
    char *name_file_temp;
};

struct Searcher *searcher;
char **result_blocks;

void initialize_result_array(__uint32_t size_of_array) {
    searcher = calloc(size_of_array, sizeof(struct Searcher));
    result_blocks = calloc(size_of_array, sizeof(char *));
    ARRAY_SIZE = size_of_array;

    printf("Allocated array of size: %d \n", size_of_array);
}

void delete_block_at_index(int block_index) {
    free(result_blocks[block_index]);

    printf("Deleted block at index: %d \n", block_index);
}

int search_directory(char *dir, char *file, char *name_file_temp) {
    searcher->dir = dir;
    searcher->file = file;
    searcher->name_file_temp = name_file_temp;

    char buffer[SIZE_OF_BUFFER];

    if (snprintf(buffer, sizeof(buffer),
                 "find %s -name %s > %s", searcher->dir, searcher->file, searcher->name_file_temp) >= sizeof(buffer)) {
        printf("ERROR");
    } else {
        system(buffer);
    }

    int fd = open(searcher->name_file_temp, O_RDONLY);
    long file_size = get_file_size(fd);
    char *result_block = calloc((size_t) file_size, sizeof(char));
    read(fd, result_block, (size_t) file_size);
    close(fd);

    int block_index = -1;

    for(int i = 0; i < ARRAY_SIZE; ++i) {
        if (result_blocks[i] == NULL) {
            result_blocks[i] = result_block;
            block_index = i;
            break;
        }
    }

    return block_index;
}

long get_file_size(int fd) {
    off_t currentPos = lseek(fd, (size_t)0, SEEK_CUR);
    long file_size = lseek(fd, 0, SEEK_END);
    lseek(fd, currentPos, SEEK_SET);
    return file_size;
}

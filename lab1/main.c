#include <stdio.h>
#include <stdlib.h>

const int SIZE_OF_BUFFER = 512;

struct Searcher {
    char *dir;
    char *file;
    char *name_file_temp;
};

struct Result {
    char *result_block;
};

struct Searcher *searcher;

void initialize_searching_array(size_t size_of_array) {
    searcher = calloc(size_of_array, sizeof(struct Searcher));
}

void search_directory(char *dir, char *file, char *name_file_temp) {
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
}


int main() {
    initialize_searching_array(100);
    search_directory("~/CLionProjects/sysopy/lab1", "main.c", "tmp.txt");
    return 0;
}
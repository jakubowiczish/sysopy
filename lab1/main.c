#include <stdio.h>
#include <stdlib.h>

const int SIZE_OF_BUFFER = 512;

struct Searcher {
    char *folder_name;
    char *filename;
};

struct Searcher *searcher;

void initialize_searcher() {
    searcher = malloc(sizeof(struct Searcher));
}

void set_searching(char *folder_name, char *filename) {
    searcher->folder_name = folder_name;
    searcher->filename = filename;
}

void call_searching() {
    char buffer[SIZE_OF_BUFFER];

    if (snprintf(buffer, sizeof(buffer),
                 "find %s -name %s > tmp.txt", searcher->folder_name, searcher->filename) >= sizeof(buffer)) {
        printf("ERROR");
    } else {
        system(buffer);
    }
}

int main() {
    initialize_searcher();
    set_searching("~/CLionProjects/sysopy/lab1", "main.c");
    call_searching();

    printf("elo");
    return 0;
}
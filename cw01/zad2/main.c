#include <stdio.h>
#include <memory.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include "lib.h"

bool check_if_argument_is_number(char *arg);

int main(int argc, char **argv) {
//    printf("%d", search_directory("/home/jakub/Downloads", "płotnikowski.jpg", "tmp.txt"));

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "create_table") == 0) {
            if (check_if_argument_is_number(argv[i + 1])) {
                char *end = NULL;
                long size = strtol(argv[i + 1], &end, 10);
                initialize_result_array((__uint32_t) size);
            }
        }

        if (strcmp(argv[i], "search_directory") == 0) {
            search_directory(argv[i + 1], argv[i + 2], argv[i + 3]);
        }

        if (strcmp(argv[i], "remove_block") == 0) {
            if (check_if_argument_is_number(argv[i + 1])) {
                char *end = NULL;
                int block_index = (int) strtol(argv[i + 1], &end, 10);
                delete_block_at_index(block_index);
            }
        }
    }

    return 0;
}

bool check_if_argument_is_number(char *arg) {
    size_t arg_length = strlen(arg);
    for (int i = 0; i < arg_length; ++i) {
        if (!isdigit(arg[i])) {
            printf("Entered input is not a number! Try again\n");
            return false;
        }
    }
    return true;
}
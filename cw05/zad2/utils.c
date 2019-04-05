#include "utils.h"

void print_error_message(char *error_message) {
    printf("%s\n", error_message);
}

void print_error_message_and_exit(char *error_message) {
    printf("%s\n", error_message);
    exit(EXIT_FAILURE);
}
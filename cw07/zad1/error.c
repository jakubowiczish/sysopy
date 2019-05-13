#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"


void print_coloured_message(char *message, char *colour) {
    printf("%s%s" RESET "\n", colour, message);
}


void print_error(char *message) {
    print_coloured_message(message, RED);
    fprintf(stderr, ": %s\n", strerror(errno));
    exit(1);
}

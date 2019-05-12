#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "error.h"


void print_coloured_message(char *message, char *colour) {
    if (strcmp(colour, RED) == 0) {
        printf("\n" RED "%s" RESET "\n", message);

    } else if (strcmp(colour, GREEN) == 0) {
        printf("\n" GREEN "%s" RESET "\n", message);

    } else if (strcmp(colour, GREEN) == 0) {
        printf("\n" YELLOW "%s" RESET "\n", message);

    } else if (strcmp(colour, BLUE) == 0) {
        printf("\n" BLUE "%s" RESET "\n", message);

    } else if (strcmp(colour, MAGENTA) == 0) {
        printf("\n" MAGENTA "%s" RESET "\n", message);

    } else if (strcmp(colour, CYAN) == 0) {
        printf("\n" CYAN "%s" RESET "\n", message);

    } else if (strcmp(colour, WHITE) == 0) {
        printf("\n" WHITE "%s" RESET "\n", message);
    }
}


void print_error(char *message) {
    print_coloured_message(message, RED);
    fprintf(stderr, ": %s\n", strerror(errno));
    exit(1);
}

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"


void print_coloured_message(char *message, char *colour) {
    if (strcmp(colour, "red") == 0) {
        printf(RED "%s" RESET "\n", message);
    } else if (strcmp(colour, "green") == 0) {
        printf(GRN "%s" RESET "\n", message);
    } else if (strcmp(colour, "yellow") == 0) {
        printf(YEL "%s" RESET "\n", message);
    } else if (strcmp(colour, "blue") == 0) {
        printf(BLU "%s" RESET "\n", message);
    } else if (strcmp(colour, "magenta") == 0) {
        printf(MAG "%s" RESET "\n", message);
    } else if (strcmp(colour, "cyan") == 0) {
        printf(CYN "%s" RESET "\n", message);
    } else if (strcmp(colour, "white") == 0) {
        printf(WHT "%s" RESET "\n", message);
    }
}


void print_error(char *message) {
    print_coloured_message(message, RED);
    fprintf(stderr, ": %s\n", strerror(errno));
    exit(1);
}

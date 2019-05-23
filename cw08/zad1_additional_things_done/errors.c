#include <stdio.h>
#include <stdlib.h>

void pdie(char *message, int exit_code) {
    perror(message);
    exit(exit_code);
}


void die(char *message, int exit_code) {
    fprintf(stderr, "%s\n", message);
    exit(exit_code);
}


int perr(char *message, int return_code) {
    perror(message);
    return return_code;
}


int err(char *message, int return_code) {
    fprintf(stderr, "%s\n", message);
    return return_code;
}


void outperr(char *message) {
    perror(message);
}


void outerr(char *message) {
    fprintf(stderr, "%s\n", message);
}
#ifndef SYSOPY_ERROR_H
#define SYSOPY_ERROR_H

#define RED   "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW   "\x1B[33m"
#define BLUE   "\x1B[34m"
#define MAGENTA   "\x1B[35m"
#define CYAN   "\x1B[36m"
#define WHITE   "\x1B[37m"
#define RESET "\x1B[0m"


void print_coloured_message(char *message, char *colour);

void print_error(char *message);

#endif //SYSOPY_ERROR_H

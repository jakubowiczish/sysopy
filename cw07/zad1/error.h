#ifndef SYSOPY_ERROR_H
#define SYSOPY_ERROR_H

#define RED   "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW   "\x1B[33m"
#define BLUE   "\x1B[34m"
#define MAGENTA   "\x1B[35m"
#define CYAN   "\x1B[36m"
#define WHITE   "\x1B[37m"
#define BRIGHT_RED "\x1B[91m"
#define BRIGHT_GREEN "\x1B[92m"
#define BRIGHT_YELLOW "\x1B[93m"
#define BRIGHT_BLUE "\x1B[94m"
#define BRIGHT_MAGENTA "\x1B[95m"
#define BRIGHT_CYAN "\x1B[96m"
#define BRIGHT_WHITE "\x1B[97m"

#define RESET "\x1B[0m"


void print_coloured_message(char *message, char *colour);

void print_error(char *message);

#endif //SYSOPY_ERROR_H

#ifndef SYSOPY_HELPER_H
#define SYSOPY_HELPER_H

#include <stdio.h>
#include <stdlib.h>

#define PROJ_ID 'P'

/* MESSAGE TYPES */

#define INIT 5


struct msg_text {
    int id;
    int additional_arg;
    char buf[512];
};


struct msg {
    long msg_type;
    struct msg_text msg_text;
};


void print_error_and_exit(char *error_message) {
    printf("\033[1;31m%s\n", error_message);
    exit(-1);
}


void print_some_info(char *info) {
    printf("\033[1;32m%s\n", info);
}


#endif //SYSOPY_HELPER_H

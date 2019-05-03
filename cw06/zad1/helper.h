#ifndef SYSOPY_HELPER_H
#define SYSOPY_HELPER_H

#include <stdio.h>
#include <stdlib.h>

#define PROJ_ID 'P'

#define SERVER_ID -100

#define SHUTDOWN -101

#define QUEUE_PERMISSIONS 0660

#define MAX_CLIENTS_NUMBER 16

#define MAX_GROUP_SIZE 8

#define SHIFTID 100

/* MESSAGE TYPES */

#define STOP 10

#define LIST 11

#define FRIENDS 12

#define INIT 15

#define ECHO 16

#define _2ALL 17

#define _2FRIENDS 18

#define _2ONE 19

#define ADD 23

#define DEL 24


#define ERROR 404

#define EVER ;;

struct msg_text {
    int id;
    int additional_arg;
    char buf[512];
};


struct msg {
    long msg_type;
    struct msg_text msg_text;
};


struct string_array {
    unsigned int size;
    char **data;
};


void print_sth_and_exit(char *error_message, int error_status) {
    printf("\033[1;31m%s\n", error_message);
    exit(error_status);
}

void print_error(char *error_message) {
    printf("\033[1;31m%s\n", error_message);
}


void print_some_info(char *info) {
    printf("\033[1;32m%s\n", info);
}


char *get_type_as_string(int type) {
    switch (type) {
        case STOP: {
            return "STOP";
        }

        case LIST: {
            return "LIST";
        }

        case FRIENDS: {
            return "FRIENDS";
        }

        case INIT: {
            return "INIT";
        }

        case ECHO: {
            return "ECHO";
        }

        case _2ALL: {
            return "2ALL";
        }

        case _2FRIENDS: {
            return "2FRIENDS";
        }

        case _2ONE: {
            return "2ONE";
        }

        case ADD: {
            return "ADD";
        }

        case DEL: {
            return "DEL";
        }
    }
}


#endif //SYSOPY_HELPER_H

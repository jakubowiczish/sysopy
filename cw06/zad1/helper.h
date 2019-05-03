#ifndef SYSOPY_HELPER_H
#define SYSOPY_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    printf("\033[1;31m%s \033[0m \n", error_message);
    exit(error_status);
}

void print_error(char *error_message) {
    printf("\033[1;31m%s \033[0m \n", error_message);
}


void print_some_info(char *info) {
    printf("\033[1;32m%s \033[0m \n", info);
}


char *type_to_string(int type) {
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

        default: {
            return "";
        }
    }
}


struct string_array process_file(char *string, long length, char delimiter) {
    struct string_array items_array;
    char **items = NULL;
    int items_counter = 0;

    items_array.size = 0;
    items_array.data = NULL;

    if (string == NULL || length == 0) {
        return items_array;
    }

    ++items_counter;

    for (int i = 0; i < length; ++i) {
        if (string[i] == delimiter)
            ++items_counter;
    }

    items = calloc(items_counter, sizeof(char *));

    int global_index = 0;
    int start_index = 0;

    for (int i = 0; i < items_counter; ++i) {
        start_index = global_index;

        while (global_index < length && string[global_index] != delimiter)
            ++global_index;

        if (start_index == global_index) {
            --items_counter;
            --i;
            continue;
        }

        items[i] = calloc(global_index - start_index + 1, sizeof(char));

        memcpy(items[i], string + start_index, (global_index - start_index) * sizeof(char));

        ++global_index;
    }

    items_array.size = items_counter;
    items_array.data = items;

    return items_array;
}


#endif //SYSOPY_HELPER_H

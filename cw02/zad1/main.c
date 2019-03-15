#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int generate(char *filename, int amount_of_records, int record_length) {
    FILE *file = fopen(filename, "w+");

    if (file == NULL) {
        printf("ERROR HAS OCCURRED WHILE OPENING FILE: %s\n", filename);
        return -1;
    }

    FILE *random_file = fopen("/dev/urandom", "r");
    if (random_file == NULL) {
        printf("ERROR HAS OCCURRED WHILE OPENING FILE: /dev/urandom\n");
        return -1;
    }

    char *record = malloc(record_length * sizeof(char) + 1);
    for (int i = 0; i < amount_of_records; ++i) {
        for (int j = 0; j < record_length; ++j) {
            record[j] = (char) (abs(record[j]) % 25 + 'a');
        }

        record[record_length] = 10;

        if (fwrite(record, sizeof(char), (size_t) record_length + 1, file) != record_length + 1) {
            return 1;
        }
    }

    fclose(file);
    fclose(random_file);
    free(record);
    return 0;
}


bool check_if_argument_is_number(char *arg) {
    size_t arg_length = strlen(arg);
    for (int i = 0; i < arg_length; ++i) {
        if (!isdigit(arg[i])) {
            printf("Entered input is not a number! Try again\n");
            return false;
        }
    }
    return true;
}


int main(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "generate") == 0) {
            if (!check_if_argument_is_number(argv[i + 2])) {
                printf("AMOUNT OF BLOCKS SHOULD BE AN INTEGER\n");
                return -1;
            }

            if (!check_if_argument_is_number(argv[i + 3])) {
                printf("RECORD LENGTH SHOULD BE AN INTEGER\n");
                return -1;
            }

            char *end = NULL;
            generate(argv[i + 1], (int) strtol(argv[i + 2], &end, 10), (int) strtol(argv[i + 3], &end, 10));
        }
    }
}
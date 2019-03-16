#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <fcntl.h>
#include <zconf.h>

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
        if (fread(record, sizeof(char), (size_t) record_length + 1, random_file) != record_length + 1) {
            printf("ERROR HAS OCCURRED WITH GENERATE METHOD - READING /dev/urandom!\n");
            return 1;

        }

        for (int j = 0; j < record_length; ++j) {
            record[j] = (char) (abs(record[j]) % 25 + 'a');
        }

        record[record_length] = 10;

        if (fwrite(record, sizeof(char), (size_t) record_length + 1, file) != record_length + 1) {
            printf("ERROR HAS OCCURRED WITH GENERATE METHOD - WRITING TO FILE\n");
            return 1;
        }
    }

    fclose(file);
    fclose(random_file);
    free(record);
    return 0;
}


void sort_lib(char *filename, int amount_of_records, int record_length) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        printf("ERROR HAS OCCURRED WHILE OPENING FILE IN SORT_LIB METHOD: %s\n", filename);
        return;
    }

    char *fragment_1 = malloc((record_length + 1) * sizeof(char));
    char *fragment_2 = malloc((record_length + 1) * sizeof(char));

    long int offset = (record_length + 1) * sizeof(char);

    for (int i = 0; i < amount_of_records; ++i) {
        fseek(file, i * offset, 0);

        if (fread(fragment_1, sizeof(char), (size_t) record_length + 1, file) != record_length + 1) {
            printf("ERROR HAS OCCURRED WHILE READING IN SORT_LIB METHOD\n");
            return;
        }

        for (int j = 0; j < i; ++j) {
            fseek(file, j * offset, 0);
            if (fread(fragment_2, sizeof(char), (size_t) record_length + 1, file) != record_length + 1) {
                printf("ERROR HAS OCCURRED WHILE READING IN SORT_LIB METHOD\n");
                return;
            }

            if (fragment_2[0] > fragment_1[0]) {
                fseek(file, j * offset, 0);
                if (fwrite(fragment_1, sizeof(char), (size_t) record_length + 1, file) != record_length + 1) {
                    printf("ERROR HAS OCCURRED WHILE WRITING IN SORT_LIB METHOD\n");
                    return;
                }

                fseek(file, i * offset, 0);
                if (fwrite(fragment_2, sizeof(char), (size_t) record_length + 1, file) != record_length + 1) {
                    printf("ERROR HAS OCCURRED WHILE WRITING IN SORT_LIB METHOD\n");
                    return;
                }

                char *tmp = fragment_1;
                fragment_1 = fragment_2;
                fragment_2 = tmp;
            }
        }
    }

    fclose(file);
    free(fragment_1);
    free(fragment_2);
}

void sort_sys(char *filename, int amount_of_records, int record_length) {
    int file = open(filename, O_RDWR);
    if (file == -1) {
        printf("ERROR HAS OCCURRED WHILE OPENING FILE IN SORT_SYS METHOD: %s\n", filename);
        return;
    }

    char *fragment_1 = malloc((record_length + 1) * sizeof(char));
    char *fragment_2 = malloc((record_length + 1) * sizeof(char));

    long int offset = (record_length + 1) * sizeof(char);

    for (int i = 0; i < amount_of_records; ++i) {
        lseek(file, i * offset, SEEK_SET);

        if (read(file, fragment_1, (size_t) record_length + 1) != record_length + 1) {
            printf("ERROR HAS OCCURRED WHILE READING IN SORT_SYS METHOD\n");
            return;
        }

        for (int j = 0; j < i; ++j) {
            lseek(file, j * offset, SEEK_SET);
            if (read(file, fragment_2, (size_t) record_length + 1) != record_length + 1) {
                printf("ERROR HAS OCCURRED WHILE READING IN SORT_LIB METHOD\n");
                return;
            }

            if (fragment_2[0] > fragment_1[0]) {
                lseek(file, j * offset, SEEK_SET);
                if (write(file, fragment_1, (size_t) record_length + 1) != record_length + 1) {
                    printf("ERROR HAS OCCURRED WHILE WRITING IN SORT_LIB METHOD\n");
                    return;
                }

                lseek(file, i * offset, SEEK_SET);
                if (write(file, fragment_2, (size_t) record_length + 1) != record_length + 1) {
                    printf("ERROR HAS OCCURRED WHILE WRITING IN SORT_LIB METHOD\n");
                    return;
                }

                char *tmp = fragment_1;
                fragment_1 = fragment_2;
                fragment_2 = tmp;
            }
        }
    }

    close(file);
    free(fragment_1);
    free(fragment_2);
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

        if (strcmp(argv[i], "sort") == 0) {
            if (!check_if_argument_is_number(argv[i + 2])) {
                printf("AMOUNT OF BLOCKS SHOULD BE AN INTEGER\n");
                return -1;
            }

            if (!check_if_argument_is_number(argv[i + 3])) {
                printf("RECORD LENGTH SHOULD BE AN INTEGER\n");
                return -1;
            }

            char *end = NULL;

            if (strcmp(argv[i + 4], "lib") == 0) {
                sort_lib(argv[i + 1], (int) strtol(argv[i + 2], &end, 10), (int) strtol(argv[i + 3], &end, 10));
            } else if (strcmp(argv[i + 4], "sys") == 0) {
                sort_sys(argv[i + 1], (int) strtol(argv[i + 2], &end, 10), (int) strtol(argv[i + 3], &end, 10));
            } else {
                printf("SOMETHING WENT WRONG WITH THE SORT METHOD!");
                return -1;
            }
        }
    }
}
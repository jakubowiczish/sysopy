#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <zconf.h>

clock_t start_time;
clock_t end_time;
struct tms start_cpu;
struct tms end_cpu;

int generate(char* file_path, int amount_of_records, int record_length) {
    FILE* file = fopen(file_path, "w+");

    if (file == NULL) {
        printf("ERROR HAS OCCURRED WHILE OPENING FILE: %s\n", file_path);
        return -1;
    }

    FILE* random_file = fopen("/dev/urandom", "r");
    if (random_file == NULL) {
        printf("ERROR HAS OCCURRED WHILE OPENING FILE: /dev/urandom\n");
        return -1;
    }

    char* record = malloc(record_length * sizeof(char) + 1);

    for (int i = 0; i < amount_of_records; ++i) {
        if (fread(record, sizeof(char), (size_t)record_length + 1,
                  random_file) != record_length + 1) {
            printf(
                "ERROR HAS OCCURRED WITH GENERATE METHOD - READING "
                "/dev/urandom!\n");
            return 1;
        }

        for (int j = 0; j < record_length; ++j) {
            record[j] = (char)(abs(record[j]) % 25 + 'a');
        }

        record[record_length] = 10;

        if (fwrite(record, sizeof(char), (size_t)record_length + 1, file) !=
            record_length + 1) {
            printf(
                "ERROR HAS OCCURRED WITH GENERATE METHOD - WRITING TO FILE\n");
            return 1;
        }
    }

    fclose(file);
    fclose(random_file);
    free(record);
    return 0;
}

void sort_lib(char* file_path, int amount_of_records, int record_length) {
    FILE* file = fopen(file_path, "r+");
    if (file == NULL) {
        printf("ERROR HAS OCCURRED WHILE OPENING FILE IN SORT_LIB METHOD: %s\n",
               file_path);
        return;
    }

    char* fragment_1 = malloc((record_length + 1) * sizeof(char));
    char* fragment_2 = malloc((record_length + 1) * sizeof(char));

    long int offset = (record_length + 1) * sizeof(char);

    for (int i = 0; i < amount_of_records; ++i) {
        fseek(file, i * offset, 0);

        if (fread(fragment_1, sizeof(char), (size_t)record_length + 1, file) !=
            record_length + 1) {
            printf("ERROR HAS OCCURRED WHILE READING IN SORT_LIB METHOD\n");
            return;
        }

        for (int j = 0; j < i; ++j) {
            fseek(file, j * offset, 0);
            if (fread(fragment_2, sizeof(char), (size_t)record_length + 1,
                      file) != record_length + 1) {
                printf("ERROR HAS OCCURRED WHILE READING IN SORT_LIB METHOD\n");
                return;
            }

            if (fragment_2[0] > fragment_1[0]) {
                fseek(file, j * offset, 0);
                if (fwrite(fragment_1, sizeof(char), (size_t)record_length + 1,
                           file) != record_length + 1) {
                    printf(
                        "ERROR HAS OCCURRED WHILE WRITING IN SORT_LIB "
                        "METHOD\n");
                    return;
                }

                fseek(file, i * offset, 0);
                if (fwrite(fragment_2, sizeof(char), (size_t)record_length + 1,
                           file) != record_length + 1) {
                    printf(
                        "ERROR HAS OCCURRED WHILE WRITING IN SORT_LIB "
                        "METHOD\n");
                    return;
                }

                char* tmp = fragment_1;
                fragment_1 = fragment_2;
                fragment_2 = tmp;
            }
        }
    }

    fclose(file);
    free(fragment_1);
    free(fragment_2);
}

void sort_sys(char* file_path, int amount_of_records, int record_length) {
    int file = open(file_path, O_RDWR);
    if (file == -1) {
        printf("ERROR HAS OCCURRED WHILE OPENING FILE IN SORT_SYS METHOD: %s\n",
               file_path);
        return;
    }

    char* fragment_1 = malloc((record_length + 1) * sizeof(char));
    char* fragment_2 = malloc((record_length + 1) * sizeof(char));

    long int offset = (record_length + 1) * sizeof(char);

    for (int i = 0; i < amount_of_records; ++i) {
        lseek(file, i * offset, SEEK_SET);

        if (read(file, fragment_1,
                 (size_t)(record_length + 1) * sizeof(char)) !=
            record_length + 1) {
            printf("ERROR HAS OCCURRED WHILE READING IN SORT_SYS METHOD\n");
            return;
        }

        for (int j = 0; j < i; ++j) {
            lseek(file, j * offset, SEEK_SET);
            if (read(file, fragment_2,
                     (size_t)(record_length + 1) * sizeof(char)) !=
                record_length + 1) {
                printf("ERROR HAS OCCURRED WHILE READING IN SORT_LIB METHOD\n");
                return;
            }

            if (fragment_2[0] > fragment_1[0]) {
                lseek(file, j * offset, SEEK_SET);
                if (write(file, fragment_1,
                          (size_t)(record_length + 1) * sizeof(char)) !=
                    record_length + 1) {
                    printf(
                        "ERROR HAS OCCURRED WHILE WRITING IN SORT_LIB "
                        "METHOD\n");
                    return;
                }

                lseek(file, i * offset, SEEK_SET);
                if (write(file, fragment_2,
                          (size_t)(record_length + 1) * sizeof(char)) !=
                    record_length + 1) {
                    printf(
                        "ERROR HAS OCCURRED WHILE WRITING IN SORT_LIB "
                        "METHOD\n");
                    return;
                }

                char* tmp = fragment_1;
                fragment_1 = fragment_2;
                fragment_2 = tmp;
            }
        }
    }

    close(file);
    free(fragment_1);
    free(fragment_2);
}

void copy_lib(char* source_path,
              char* destination_path,
              int amount_of_records,
              int record_length) {
    FILE* source_file = fopen(source_path, "r");
    FILE* destination_file = fopen(destination_path, "w+");

    char* fragment = malloc((record_length + 1) * sizeof(char));

    for (int i = 0; i < amount_of_records; ++i) {
        if (fread(fragment, sizeof(char), (size_t)record_length + 1,
                  source_file) != record_length + 1) {
            printf("ERROR HAS OCCURRED WHILE READING IN COPY_LIB METHOD\n");
            return;
        }

        if (fwrite(fragment, sizeof(char), (size_t)record_length + 1,
                   destination_file) != record_length + 1) {
            printf("ERROR HAS OCCURRED WHILE WRITING IN COPY_LIB METHOD\n");
            return;
        }
    }

    fclose(source_file);
    fclose(destination_file);
    free(fragment);
}

void copy_sys(char* source_path,
              char* destination_path,
              int amount_of_records,
              int record_length) {
    int source_file = open(source_path, O_RDONLY);
    int destination_file = open(destination_path, O_WRONLY);

    char* fragment = malloc((record_length + 1) * sizeof(char));

    for (int i = 0; i < amount_of_records; ++i) {
        if (read(source_file, fragment, (size_t)record_length + 1) !=
            record_length + 1) {
            printf("ERROR HAS OCCURRED WHILE READING IN COPY_LIB METHOD\n");
            return;
        }

        if (write(destination_file, fragment, (size_t)record_length + 1) !=
            record_length + 1) {
            printf("ERROR HAS OCCURRED WHILE WRITING IN COPY_LIB METHOD\n");
            return;
        }
    }

    close(source_file);
    close(destination_file);
    free(fragment);
}

bool check_if_argument_is_number(char* arg) {
    size_t arg_length = strlen(arg);
    for (int i = 0; i < arg_length; ++i) {
        if (!isdigit(arg[i])) {
            printf("Entered input is not a number! Try again\n");
            return false;
        }
    }
    return true;
}

void start_timer() {
    start_time = times(&start_cpu);
}

void stop_timer(char* logger_message, char* result_file_name) {
    end_time = times(&end_cpu);
    int64_t clk_tck = sysconf(_SC_CLK_TCK);

    char* statement =
        calloc(strlen("User time: 000.0000\nSystem time: 000.0000\n") + 1,
               sizeof(char*));

    double user_time =
        (double)(end_cpu.tms_utime - start_cpu.tms_utime) / clk_tck +
        (double)(end_cpu.tms_cutime - start_cpu.tms_cutime) / clk_tck;
    double system_time =
        (double)(end_cpu.tms_stime - start_cpu.tms_stime) / clk_tck +
        (double)(end_cpu.tms_cstime - start_cpu.tms_cstime) / clk_tck;

    sprintf(statement, "User time: %.4fs\nSystem time: %.4fs\n", user_time,
            system_time);

    printf("%s", logger_message);
    printf("%s", statement);

    FILE* fp = fopen(result_file_name, "a");
    fputs(logger_message, fp);
    fputs(statement, fp);
    fclose(fp);
    free(statement);
}

int main(int argc, char** argv) {
    char* result_file_name = "wyniki.txt";  // name required by the task
    char* copy_file_name = "copy.txt";

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "generate") == 0) {
            if (argc < 5) {
                printf("NOT ENOUGH ARGUMENTS FOR GENERATE METHOD!\n");
                return -1;
            }

            if (!check_if_argument_is_number(argv[i + 2])) {
                printf("AMOUNT OF BLOCKS SHOULD BE AN INTEGER\n");
                return -1;
            }

            if (!check_if_argument_is_number(argv[i + 3])) {
                printf("RECORD LENGTH SHOULD BE AN INTEGER\n");
                return -1;
            }

            char* end = NULL;

            int amount_of_records = (int)strtol(argv[i + 2], &end, 10);
            int record_length = (int)strtol(argv[i + 3], &end, 10);

            generate(argv[i + 1], amount_of_records, record_length);
        }

        if (strcmp(argv[i], "sort") == 0) {
            if (argc < 6) {
                printf("NOT ENOUGH ARGUMENTS FOR SORT METHOD!\n");
                return -1;
            }

            if (!check_if_argument_is_number(argv[i + 2])) {
                printf("AMOUNT OF BLOCKS SHOULD BE AN INTEGER\n");
                return -1;
            }

            if (!check_if_argument_is_number(argv[i + 3])) {
                printf("RECORD LENGTH SHOULD BE AN INTEGER\n");
                return -1;
            }

            char* end = NULL;
            int amount_of_records = (int)strtol(argv[i + 2], &end, 10);
            int record_length = (int)strtol(argv[i + 3], &end, 10);

            copy_lib(argv[i + 1], copy_file_name, amount_of_records,
                     record_length);

            if (strcmp(argv[i + 4], "lib") == 0) {
                char logger_message[256];
                sprintf(
                    logger_message,
                    "Time of SORTING (LIB), measured for %d blocks and record "
                    "length %d \n",
                    amount_of_records, record_length);

                copy_lib(copy_file_name, argv[i + 1], amount_of_records,
                         record_length);

                start_timer();
                sort_lib(argv[i + 1], (int)strtol(argv[i + 2], &end, 10),
                         (int)strtol(argv[i + 3], &end, 10));
                stop_timer(logger_message, result_file_name);

                //                remove(copy_file_name);
            } else if (strcmp(argv[i + 4], "sys") == 0) {
                char logger_message[256];
                sprintf(
                    logger_message,
                    "Time of SORTING (SYS), measured for %d blocks and record "
                    "length %d \n",
                    amount_of_records, record_length);

                copy_lib(copy_file_name, argv[i + 1], amount_of_records,
                         record_length);

                start_timer();
                sort_sys(argv[i + 1], (int)strtol(argv[i + 2], &end, 10),
                         (int)strtol(argv[i + 3], &end, 10));
                stop_timer(logger_message, result_file_name);

                //                remove(copy_file_name);
            } else {
                printf("SOMETHING WENT WRONG WITH THE SORT METHOD!");
                return -1;
            }
        }

        if (strcmp(argv[i], "copy") == 0) {
            if (argc < 6) {
                printf("NOT ENOUGH ARGUMENTS FOR COPY METHOD!\n");
                return -1;
            }

            if (!check_if_argument_is_number(argv[i + 3])) {
                printf("AMOUNT OF BLOCKS SHOULD BE AN INTEGER\n");
                return -1;
            }

            if (!check_if_argument_is_number(argv[i + 4])) {
                printf("RECORD LENGTH SHOULD BE AN INTEGER\n");
                return -1;
            }

            char* end = NULL;
            int amount_of_records = (int)strtol(argv[i + 3], &end, 10);
            int record_length = (int)strtol(argv[i + 4], &end, 10);

            if (strcmp(argv[i + 5], "lib") == 0) {
                char logger_message[256];
                sprintf(
                    logger_message,
                    "Time of COPYING (LIB), measured for %d blocks and record "
                    "length %d \n",
                    amount_of_records, record_length);

                start_timer();
                copy_lib(argv[i + 1], argv[i + 2],
                         (int)strtol(argv[i + 3], &end, 10),
                         (int)strtol(argv[i + 4], &end, 10));
                stop_timer(logger_message, result_file_name);
            } else if (strcmp(argv[i + 5], "sys") == 0) {
                char logger_message[256];
                sprintf(
                    logger_message,
                    "Time of COPYING (SYS), measured for %d blocks and record "
                    "length %d \n",
                    amount_of_records, record_length);

                start_timer();
                copy_sys(argv[i + 1], argv[i + 2],
                         (int)strtol(argv[i + 3], &end, 10),
                         (int)strtol(argv[i + 4], &end, 10));
                stop_timer(logger_message, result_file_name);
            } else {
                printf("SOMETHING WENT WRONG WITH THE COPY METHOD!");
                return -1;
            }
        }
    }
}
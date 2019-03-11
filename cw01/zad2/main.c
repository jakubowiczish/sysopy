#include <stdio.h>
#include <memory.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>

#ifdef DYNAMIC
#include "lib_dynamic.h"
#else

#include "lib.h"

#endif

void start_timer();

void stop_timer(char *logger_message);

bool check_if_argument_is_number(char *arg);

clock_t start_time;
clock_t end_time;
struct tms start_cpu;
struct tms end_cpu;


int main(int argc, char **argv) {

#ifdef DYNAMIC
    init_dynamic_handler();
#endif
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "create_table") == 0) {
            if (check_if_argument_is_number(argv[i + 1])) {
                char *end = NULL;
                long size = strtol(argv[i + 1], &end, 10);

                char buffer[100];
                sprintf(buffer, "CREATING ARRAY OF SIZE\n: %ld", size);

                start_timer();
                create_table((__uint32_t) size);
                stop_timer(buffer);

            }
        }

        if (strcmp(argv[i], "search_directory") == 0) {
            char buffer[256];
            sprintf(buffer, "SEARCHING DIRECTORY %s for file: %s, file with result: %s\n", argv[i + 1], argv[i + 2],
                    argv[i + 3]);

            start_timer();
            search_directory(argv[i + 1], argv[i + 2], argv[i + 3]);
            stop_timer(buffer);
        }

        if (strcmp(argv[i], "remove_block") == 0) {
            if (check_if_argument_is_number(argv[i + 1])) {
                char *end = NULL;
                int block_index = (int) strtol(argv[i + 1], &end, 10);
                char buffer[100];
                sprintf(buffer, "REMOVING BLOCK: %s", argv[i + 1]);
                start_timer();
                remove_block(block_index);
                stop_timer(buffer);
            }
        }
    }
#ifdef DYNAMIC
    free_handler_memory();
#endif

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

void start_timer() {
    start_time = times(&start_cpu);
}

void stop_timer(char *logger_message) {
    end_time = times(&end_cpu);
    int64_t clk_tck = sysconf(_SC_CLK_TCK);

    char *statement = calloc(strlen("Real time: 000.0000, User time: 000.0000, System time: 000.0000\n") + 1,
                             sizeof(char *));

    double real_time = (double) (end_time - start_time) / clk_tck;
    double user_time = (double) (end_cpu.tms_utime - start_cpu.tms_utime) / clk_tck
                       + (double) (end_cpu.tms_cutime - start_cpu.tms_cutime) / clk_tck;
    double system_time = (double) (end_cpu.tms_stime - start_cpu.tms_stime) / clk_tck
                         + (double) (end_cpu.tms_cstime - start_cpu.tms_cstime) / clk_tck;

    sprintf(statement, "Real time: %.4fs, User time: %.4fs, System time: %.4fs\n", real_time, user_time, system_time);

    printf("%s", logger_message);
    printf("%s", statement);

    FILE *fp = fopen("raport2.txt", "a");
    fputs(logger_message, fp);
    fputs(statement, fp);
    fclose(fp);
}
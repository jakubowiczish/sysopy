#include <stdio.h>
#include <memory.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include <dlfcn.h>

void start_timer();

void stop_timer(char *logger_message);

bool check_if_argument_is_number(char *arg);

clock_t start_time;
clock_t end_time;
struct tms start_cpu;
struct tms end_cpu;


int main(int argc, char **argv) {

    void *handle = dlopen("out_dynamic", RTLD_LAZY);

    if (handle == NULL) {
        printf("The dynamic library could not be read!\n");
        return -1;
    }

    void (*_create_table)(__uint32_t size_of_array);
    _create_table = dlsym(handle, "create_table");

    if (dlerror() != NULL) {
        printf("Something went wrong with reading create_table method\n");
        return -1;
    }

    void (*_remove_block)(int);
    _remove_block = dlsym(handle, "remove_block");

    if (dlerror() != NULL) {
        printf("Something went wrong with reading remove_block method\n");
        return -1;
    }

    int (*_search_directory)(char *, char *, char *);
    _search_directory = dlsym(handle, "search_directory");

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "_create_table") == 0) {
            if (check_if_argument_is_number(argv[i + 1])) {
                char *end = NULL;
                long size = strtol(argv[i + 1], &end, 10);

                start_timer();
                (*_create_table)((__uint32_t) size);
                stop_timer("Time of creating the array: ");

            }
        }

        if (strcmp(argv[i], "search_directory") == 0) {
            start_timer();
            _search_directory(argv[i + 1], argv[i + 2], argv[i + 3]);
            stop_timer("Time of searching the directory: ");
        }

        if (strcmp(argv[i], "remove_block") == 0) {
            if (check_if_argument_is_number(argv[i + 1])) {
                char *end = NULL;
                int block_index = (int) strtol(argv[i + 1], &end, 10);
                start_timer();
                _remove_block(block_index);
                stop_timer("Time of removing the block: ");
            }
        }
    }

    dlclose(handle);

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
    unsigned int clk_tck = (unsigned int) sysconf(_SC_CLK_TCK);

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
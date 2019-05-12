#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#include "error.h"

int main(int argc, char **argv) {
    if (argc != 3 && argc != 4) {
        print_coloured_message("invalid arguments given - necessary arguments: workers, max_weight, (cycles)", RED);
    }

    int workers;
    int max_weight;


    if (sscanf(argv[1], "%d", &workers) != 1 || sscanf(argv[2], "%d", &max_weight) != 1) {
        print_coloured_message("passed arguments are invalid", RED);
    }


    int cycles = 0;

    if (argc == 4) {
        if (sscanf(argv[3], "%d", &cycles) != 1) {
            print_coloured_message("passed arguments are invalid", RED);
        }
    }


    srand(time(NULL));

    char cycles_arr[16];
    sprintf(cycles_arr, "%d", cycles);

    char weight_arr[16];

    for (int i = 0; i < workers; ++i) {
        sprintf(weight_arr, "%d", rand() % max_weight + 1);

        switch (fork()) {
            case -1:
                print_error("problem with fork");
                break;
            case 0:
                execl("./loader", "./loader", weight_arr, cycles_arr, (char *) NULL);
                print_error("error while executing loader program");
                break;
        }
    }

    int status;

    for (int i = 0; i < workers; ++i) {
        wait(&status);

        char loader_finished_buffer[1024];

        sprintf(
                loader_finished_buffer,
                "loader %d finished with exit status: %d",
                i,
                WEXITSTATUS(status)
        );

        print_coloured_message(loader_finished_buffer, BLUE);
    }


    return 0;
}
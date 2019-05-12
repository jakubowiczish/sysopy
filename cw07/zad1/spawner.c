#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

#include "error.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        print_coloured_message("invalid arguments given - necessary arguments: workers, max_weight", RED);
    }

    int workers;
    int max_weight;

    if (sscanf(argv[1], "%d", &workers) != 1 || sscanf(argv[2], "%d", &max_weight) != 1) {
        print_coloured_message("passed arguments are invalid", RED);
    }

    srand(time(NULL));

    char weight[12];

    for (int i = 0; i < workers; ++i) {
        sprintf(weight, "%d", rand() % max_weight + 1);


        switch (fork()) {
            case -1:
                print_error("problem with fork");
                break;
            case 0:
                execl("./loader", "./loader", weight, NULL);
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

void kill_catch(int sig, siginfo_t *siginfo, void *context);

int signal_counter = 0;

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("WRONG NUMBER OF ARGUMENTS! TRY AGAIN!\n");
        return -1;
    }

    char *mode = argv[1];

    if (strcmp(mode, "KILL") == 0) {
        initialize_signals(kill_catch);
    } else {
        printf("UNKNOWN MODE");
    }

    printf("PID: %d\n", (int) getpid());

    while (1) {
        pause();
    }

    return 0;
}

void kill_catch(int sig, siginfo_t *siginfo, void *context) {
    if (sig == SIGUSR1) {
        ++signal_counter;
    } else {
        for (int i = 0; i < signal_counter; ++i) {
            if (kill(siginfo->si_pid, SIGUSR1) < 0) {
                printf("PROBLEM WITH KILL IN CATCHER\n");
            }
        }

        if (kill(siginfo->si_pid, SIGUSR2) < 0) {
            printf("PROBLEM WITH KILL IN CATCHER\n");
        }

        printf("CAUGHT %d: SIGNALS\n", signal_counter);

        exit(0);
    }
}
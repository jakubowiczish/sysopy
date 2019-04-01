#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

int expected_signal_counter = 0;
int actual_signal_counter = 0;

static void kill_send(int sig, siginfo_t *siginfo, void *context);

static void queue_send(int sig, siginfo_t *siginfo, void *context);

static void rt_send(int sig, siginfo_t *siginfo, void *context);

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("WRONG NUMBER OF ARGUMENTS! TRY AGAIN!\n");
        return -1;
    }

    int pid = atoi(argv[1]);
    expected_signal_counter = atoi(argv[2]);
    char *mode = argv[3];

    if (strcmp(mode, "KILL") == 0) {
        initialize_signals(kill_send, 0);

        int i;
        for (i = 0; i < expected_signal_counter; ++i) {
            if (kill(pid, SIGUSR1) != 0) {
                printf("PROBLEM WITH SIGUSR1\n");
            }
        }
        if (kill(pid, SIGUSR2) != 0) {
            printf("PROBLEM WITH SIGUSR2\n");
        }

    } else if (strcmp(mode, "SIGQUEUE") == 0) {
        initialize_signals(queue_send, 0);

        int i;
        for (i = 0; i < expected_signal_counter; ++i) {
            if (kill(pid, SIGUSR1) != 0) {
                printf("PROBLEM WITH SIGUSR1\n");
            }
        }
        if (kill(pid, SIGUSR2) != 0) {
            printf("PROBLEM WITH SIGUSR2\n");
        }

    } else if (strcmp(mode, "SIGRT") == 0) {
        initialize_signals(rt_send, 1);

        int i;
        for (i = 0; i < expected_signal_counter; ++i) {
            if (kill(pid, SIGRTMIN) != 0) {
                printf("PROBLEM WITH SIGRTMIN\n");
            }
        }
        if (kill(pid, SIGRTMAX) != 0) {
            printf("PROBLEM WITH SIGRTMAX\n");
        }

    } else {
        printf("UNKNOWN MODE");

    }

    while (1) {
        pause();
    }


}


static void kill_send(int sig, siginfo_t *siginfo, void *context) {
    if (sig == SIGUSR1) {
        ++actual_signal_counter;
    } else {
        printf("Expected counter: %d, Actual counter: %d\n", expected_signal_counter, actual_signal_counter);
        exit(0);
    }
}

static void queue_send(int sig, siginfo_t *siginfo, void *context) {
    if (sig == SIGUSR1) {
        ++actual_signal_counter;
        printf("%dth SIGUSR1 signal caught\n", siginfo->si_value.sival_int);
    } else {
        printf("Expected counter: %d, Actual counter: %d\n", expected_signal_counter, actual_signal_counter);
        exit(0);
    }
}

static void rt_send(int sig, siginfo_t *siginfo, void *context) {
    if (sig == SIGRTMIN) {
        ++actual_signal_counter;
    } else {
        printf("Expected counter: %d, Actual counter: %d\n", expected_signal_counter, actual_signal_counter);
        exit(0);
    }
}
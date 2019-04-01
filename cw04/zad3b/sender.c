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
        initialize_signals(kill_send);

        if (kill(pid, SIGUSR1) != 0) {
            printf("PROBLEM WITH SIGUSR1\n");
        }


    } else if (strcmp(mode, "SIGQUEUE") == 0) {
        initialize_signals(queue_send);

        union sigval value = {0};
        if (sigqueue(pid, SIGUSR1, value) != 0) {
            printf("PROBLEM WITH SIGUSR1\n");
        }

    } else if (strcmp(mode, "SIGRT") == 0) {
        initialize_rt_signals(rt_send);

        if (kill(pid, SIGRTMIN) != 0) {
            printf("PROBLEM WITH SIGRTMIN\n");
        }


    } else {
        printf("UNKNOWN MODE\n");

    }

    while (1) {
        pause();
    }


}


static void kill_send(int sig, siginfo_t *siginfo, void *context) {
    if (sig == SIGUSR1) {
        ++actual_signal_counter;

        if (actual_signal_counter < expected_signal_counter) {
            if (kill(siginfo->si_pid, SIGUSR1) != 0) {
                printf("PROBLEM WITH KILL IN SENDER\n");
            }
        } else {
            if (kill(siginfo->si_pid, SIGUSR2) != 0) {
                printf("PROBLEM WITH KILL IN SENDER\n");
            }
        }

    } else {
        printf("Expected counter: %d, Actual counter: %d\n", expected_signal_counter, actual_signal_counter);
        exit(0);
    }
}

static void queue_send(int sig, siginfo_t *siginfo, void *context) {
    if (sig == SIGUSR1) {
        ++actual_signal_counter;
        printf("%dth SIGUSR1 signal caught\n", siginfo->si_value.sival_int);

        if (actual_signal_counter < expected_signal_counter) {
            union sigval value = {actual_signal_counter};

            if (sigqueue(siginfo->si_pid, SIGUSR1, value) != 0) {
                printf("PROBLEM WITH KILL IN SENDER\n");
            }

        } else {
            if (kill(siginfo->si_pid, SIGUSR2) != 0) {
                printf("PROBLEM WITH KILL IN SENDER\n");
            }
        }

    } else {
        printf("Expected counter: %d, Actual counter: %d\n", expected_signal_counter, actual_signal_counter);
        exit(0);
    }
}

static void rt_send(int sig, siginfo_t *siginfo, void *context) {
    if (sig == SIGRTMIN) {
        ++actual_signal_counter;

        if (actual_signal_counter < expected_signal_counter) {
            if (kill(siginfo->si_pid, SIGRTMIN) != 0) {
                printf("PROBLEM WITH KILL IN SENDER\n");
            }
        } else {
            if (kill(siginfo->si_pid, SIGRTMAX) != 0) {
                printf("PROBLEM WITH KILL IN SENDER\n");
            }
        }

    } else {
        printf("Expected counter: %d, Actual counter: %d\n", expected_signal_counter, actual_signal_counter);
        exit(0);
    }
}
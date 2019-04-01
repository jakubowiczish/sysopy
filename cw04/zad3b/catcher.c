#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

static void kill_catch(int sig, siginfo_t *siginfo, void *context);

static void queue_catch(int sig, siginfo_t *siginfo, void *context);

static void rt_catch(int sig, siginfo_t *siginfo, void *context);

int signal_counter = 0;

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("WRONG NUMBER OF ARGUMENTS! TRY AGAIN!\n");
        return -1;
    }

    char *mode = argv[1];

    if (strcmp(mode, "KILL") == 0) {
        initialize_signals(kill_catch);
    } else if (strcmp(mode, "SIGQUEUE") == 0) {
        initialize_signals(queue_catch);
    } else if (strcmp(mode, "SIGRT") == 0) {
        initialize_rt_signals(rt_catch);
    } else {
        printf("UNKNOWN MODE\n");
    }

    printf("\nPID: %d\n", (int) getpid());

    while (1) {
        pause();
    }

    return 0;
}

static void kill_catch(int sig, siginfo_t *siginfo, void *context) {
    if (sig == SIGUSR1) {
        ++signal_counter;

        if (kill(siginfo->si_pid, SIGUSR1) < 0) {
            printf("PROBLEM WITH KILL IN CATCHER\n");
        }

    } else {
        if (kill(siginfo->si_pid, SIGUSR2) < 0) {
            printf("PROBLEM WITH KILL IN CATCHER\n");
        }
        exit(0);
    }
}

static void queue_catch(int sig, siginfo_t *siginfo, void *context) {
    if (sig == SIGUSR1) {
        ++signal_counter;

        if (sigqueue(siginfo->si_pid, SIGUSR1, siginfo->si_value) != 0) {
            printf("PROBLEM WITH SIGQUEUE METHOD\n");
        }

    } else {

        if (kill(siginfo->si_pid, SIGUSR2) != 0) {
            printf("PROBLEM WITH KILL IN SIGQUEUE CATCH (SIGUSR2)\n");
        }
        exit(0);
    }
}

static void rt_catch(int sig, siginfo_t *siginfo, void *context) {
    if (sig == SIGRTMIN) {
        ++signal_counter;

        if (kill(siginfo->si_pid, SIGRTMIN) != 0) {
            printf("PROBLEM WITH KILL IN RT CATCH\n");
        }

    } else {
        if (kill(siginfo->si_pid, SIGRTMAX) != 0) {
            printf("PROBLEM WITH KILL IN RT CATCH (SIGRTMAX)\n");
        }
        exit(0);
    }
}
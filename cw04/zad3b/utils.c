#include "utils.h"

void initialize_signals(void(*fun)(int, siginfo_t *, void *)) {
    sigset_t signals;

    if (sigfillset(&signals) == -1) {
        printf("PROBLEM WITH SIGFILLSET METHOD\n");
        return;
    }

    sigdelset(&signals, SIGUSR1);
    sigdelset(&signals, SIGUSR2);

    if (sigprocmask(SIG_BLOCK, &signals, NULL) < 0) {
        printf("PROBLEM WITH SIGPROCMASK METHOD\n");
        return;
    }


    if (sigemptyset(&signals) < 0) {
        printf("PROBLEM WITH SIGEMPTYSET METHOD\n");
        return;
    }

    struct sigaction sigaction1;
    sigaction1.sa_sigaction = fun;
    sigaction1.sa_mask = signals;
    sigaction1.sa_flags = SA_SIGINFO;

    if (sigaction(SIGUSR1, &sigaction1, NULL) == -1
        || sigaction(SIGUSR2, &sigaction1, NULL) == -1) {

        printf("PROBLEM WITH SIGACTION METHOD\n");
        return;
    }
}


void initialize_rt_signals(void(*fun)(int, siginfo_t *, void *)) {
    sigset_t signals;

    if (sigfillset(&signals) == -1) {
        printf("PROBLEM WITH SIGFILLSET METHOD\n");
        return;
    }

    sigdelset(&signals, SIGRTMIN);
    sigdelset(&signals, SIGRTMAX);

    if (sigprocmask(SIG_BLOCK, &signals, NULL) < 0) {
        printf("PROBLEM WITH SIGPROCMASK METHOD\n");
        return;
    }

    if (sigemptyset(&signals) < 0) {
        printf("PROBLEM WITH SIGEMPTYSET METHOD\n");
        return;
    }

    sigaddset(&signals, SIGRTMIN);
    sigaddset(&signals, SIGRTMAX);

    struct sigaction sigaction1;
    sigaction1.sa_sigaction = fun;
    sigaction1.sa_mask = signals;
    sigaction1.sa_flags = SA_SIGINFO;

    if (sigaction(SIGRTMIN, &sigaction1, NULL) == -1
        || sigaction(SIGRTMAX, &sigaction1, NULL) == -1) {

        printf("PROBLEM WITH SIGACTION METHOD\n");
        return;
    }
}
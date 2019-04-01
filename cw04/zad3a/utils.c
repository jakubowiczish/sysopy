#include "utils.h"


void initialize_signals(void(*fun)(int, siginfo_t *, void *), int is_rt) {
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

    sigset_t signals2;
    if (sigemptyset(&signals2) < 0) {
        printf("PROBLEM WITH SIGEMPTYSET METHOD\n");
        return;
    }

    struct sigaction sigaction1;
    sigaction1.sa_sigaction = fun;
    sigaction1.sa_mask = signals2;
    sigaction1.sa_flags = SA_SIGINFO;

    if (is_rt == 1) {
        if (sigaction(SIGUSR1, &sigaction1, NULL) == -1
            || sigaction(SIGUSR2, &sigaction1, NULL) == -1) {

            printf("PROBLEM WITH SIGACTION METHOD\n");
            return;
        }
    } else {
        if (sigaction(SIGRTMIN, &sigaction1, NULL) == -1
            || sigaction(SIGRTMAX, &sigaction1, NULL) == -1) {

            printf("PROBLEM WITH SIGACTION METHOD\n");
            return;
        }
    }

}
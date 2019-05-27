#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sig_handler(int sig, siginfo_t* siginfo, void* context) {
    printf("Value from sigval %d\n", siginfo->si_value.sival_int);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    struct sigaction action;

    action.sa_sigaction = &sig_handler;
    action.sa_flags = SA_SIGINFO;

    sigset_t mask;

    sigemptyset(&action.sa_mask);

    sigfillset(&mask);

    int child = fork();
    if (child == 0) {
        // zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1
        // zdefiniuj obsluge SIGUSR1 w taki sposob zeby proces potomny
        // wydrukowal na konsole przekazana przez rodzica wraz z sygnalem
        // SIGUSR1 wartosc

        sigdelset(&mask, SIGUSR1);

        sigprocmask(SIG_BLOCK, &mask, NULL);

        sigaction(SIGUSR1, &action, NULL);

        pause();

    } else {
        // wyslij do procesu potomnego sygnal przekazany jako argv[2]
        // wraz z wartoscia przekazana jako argv[1]

        sleep(1);

        int signal = atoi(argv[2]);

        int value = atoi(argv[1]);

        union sigval sigval;
        sigval.sival_int = value;

        sigqueue(child, signal, sigval);
    }

    return 0;
}

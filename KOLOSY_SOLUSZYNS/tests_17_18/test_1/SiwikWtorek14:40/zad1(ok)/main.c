#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * 1. Ojciec wysyła dany sygnał wraz wartością z argv[2]
 *    Dziecko blokuje wszystkie sygnały oprócz SIGUSR1,
 *    oraz wypisuje wiadaomość którą dostał wraz z sygnałem.
 *    sigqueue, siginfo itp.
 */

void sig_handler(int sig, siginfo_t* siginfo, void* context) {
    printf("Child: %d\n", siginfo->si_value.sival_int);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    /* CHANGES FROM HERE */

    struct sigaction action;

    action.sa_sigaction = &sig_handler;  // takes three arguments, thus
                                         // sa_sigaction, not sa_handler
    action.sa_flags = SA_SIGINFO;  // The signal handler takes three arguments,
                                   // not one - because of SA_SIGINFO

    sigemptyset(&action.sa_mask);

    sigset_t mask;

    if (sigfillset(&mask) == -1) {
        perror("SIGFILLSET problem");
    }

    sigdelset(&mask, SIGUSR1);

    int child = fork();

    if (child == 0) {
        // zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1
        // zdefiniuj obsluge SIGUSR1 w taki sposob zeby proces potomny
        // wydrukowal na konsole przekazana przez rodzica wraz z sygnalem
        // SIGUSR1 wartosc

        sigprocmask(SIG_BLOCK, &mask, NULL);

        sigaction(SIGUSR1, &action, NULL);

        pause();

    } else {
        // wyslij do procesu potomnego sygnal przekazany jako argv[2]
        // wraz z wartoscia przekazana jako argv[1]

        sleep(1);

        union sigval sv;
        sv.sival_int = atoi(argv[1]);

        int signal = atoi(argv[2]);

        sigqueue(child, signal, sv);
    }

    /* TILL HERE */

    return 0;
}

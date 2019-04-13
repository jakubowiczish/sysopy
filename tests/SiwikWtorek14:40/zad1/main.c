#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    struct sigaction action;
//    action.sa_sigaction = &sighandler;

    //..........


    int child = fork();
    if (child == 0) {
        //zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1
        //zdefiniuj obsluge SIGUSR1 w taki sposob zeby proces potomny wydrukowal
        //na konsole przekazana przez rodzica wraz z sygnalem SIGUSR1 wartosc
    } else {
        //wyslij do procesu potomnego sygnal przekazany jako argv[2]
        //wraz z wartoscia przekazana jako argv[KolosEom]
    }

    return 0;
}

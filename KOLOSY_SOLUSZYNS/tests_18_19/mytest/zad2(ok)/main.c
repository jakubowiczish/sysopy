#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// one change comparing to previous year

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("WRONG NUMBER OF ARGUMENTS");
        return -1;
    }

    pid_t child;
    int status, retval;
    if ((child = fork()) < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (child == 0) {
        sleep(100);
        exit(EXIT_SUCCESS);
    } else {
        /* Proces macierzysty pobiera status zakończenie potomka child, nie
         * zawieszając swojej pracy. Proces spi tyle ile podane w argumencie
         * Jeśli proces się nie zakończył, wysyła do dziecka sygnał SIGKILL /
         * SIGTERM - ktorys z tych mozna bylo Jeśli wysłanie sygnału się nie
         * powiodło, ponownie oczekuje na zakończenie procesu child, tym razem
         * zawieszając pracę do czasu zakończenia sygnału jeśli się powiodło,
         * wypisuje komunikat sukcesu zakończenia procesu potomka z numerem jego
         * PID i statusem zakończenia. CZYLI jedyna zmiana byl sleep z argv[1]
         * */

        /* ADDED BY ME - FROM HERE */

        char* sec = argv[1];
        int seconds = atoi(sec);

        sleep(seconds);

        waitpid(child, &status, WNOHANG);

        kill(child, SIGKILL);

        if (waitpid(child, &status, 0) != -1) {
            retval = WEXITSTATUS(status);
            printf("PID: %d, status: %d, return val: %d\n", getpid(), status,
                   retval);
        } else {
            perror("WAITPID");
            strerror(errno);
        }

        /* TILL HERE */
    }
    exit(EXIT_SUCCESS);
}

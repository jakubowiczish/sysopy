#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

// 3. Obustronna komunikacja między dwoma procesami za pomocą 2 pipeów

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Not a suitable number of program parameters\n");
        exit(1);
    }

    int toChildFD[2];
    int toParentFD[2];

    pipe(toChildFD);
    pipe(toParentFD);

    int val1, val2, val3 = 0;

    pid_t pid;

    if ((pid = fork()) == 0) {
        /* CHANGES FROM HERE */

        // odczytaj z potoku nienazwanego wartosc przekazana przez proces
        // macierzysty i zapisz w zmiennej val2

        read(toChildFD[0], &val2, sizeof(int));  // ADDED BY ME

        val2 = val2 * val2;

        // wyslij potokiem nienazwanym val2 do procesu macierzysego

        write(toParentFD[1], &val2, sizeof(int));  // ADDED BY ME

    } else {
        val1 = atoi(argv[1]);

        // wyslij val1 potokiem nienazwanym do priocesu potomnego

        write(toChildFD[1], &val1, sizeof(int));  // ADDED BY ME

        sleep(1);

        // odczytaj z potoku nienazwanego wartosc przekazana przez proces
        // potomny i zapisz w zmiennej val3

        read(toParentFD[0], &val3, sizeof(int));  // ADDED BY ME

        /* TILL HERE */

        printf("%d square is: %d\n", val1, val3);
    }

    return 0;
}
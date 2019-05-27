#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int i, pid;

    if (argc != 2) {
        printf("Not a suitable number of program arguments");
        exit(2);
    } else {
        for (i = 0; i < atoi(argv[1]); i++) {
            //*********************************************************
            // Uzupelnij petle w taki sposob aby stworzyc dokladnie argv[1]
            // procesow potomnych, bedacych dziecmi tego samego procesu
            // macierzystego. Kazdy proces potomny powinien:
            // - "powiedziec ktorym jest dzieckiem",
            // - jaki ma pid,
            // - kto jest jego rodzicem
            //******************************************************

            /* ADDED BY ME - FROM HERE */

            pid = fork();

            if (pid == 0) {
                printf(
                    "I am child number %d, my pid is: %d, my parent's pid is: "
                    "%d\n",
                    i, getpid(), getppid());
                break;
            }

            /* TILL HERE*/
        }
    }
    return 0;
}

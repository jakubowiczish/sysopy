#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void* hello(void* arg) {
    sleep(1);
    while (1) {
        printf("Hello world from thread number %d\n", *(int*)arg);
        /****************************************************
                przerwij dzialanie watku jesli bylo takie zadanie
        *****************************************************/
        printf("Hello again world from thread number %d\n", *(int*)arg);
        sleep(2);
    }
    return NULL;
}

int main(int argc, char** args) {
    if (argc != 3) {
        printf("Not a suitable number of program parameters\n");
        return (1);
    }

    int n = atoi(args[1]);

    /**************************************************
        Utworz n watkow realizujacych funkcje hello
    **************************************************/

    i = 0;
    while (i++ < atoi(args[2])) {
        printf("Hello from main %d\n", i);
        sleep(2);
    }

    i = 0;

    /*******************************************
        "Skasuj" wszystke uruchomione watki i poczekaj na ich zakonczenie
    *******************************************/
    printf("DONE");

    return 0;
}

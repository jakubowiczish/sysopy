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

        /****************************************************/

        /** ??? PROBABLY NOTHING SHOULD BE PLACED IN HERE  **/

        /****************************************************/

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

    /** ADDED BY ME FROM HERE **/

    pthread_t* threads = malloc(sizeof(pthread_t) * n);

    int* ids = malloc(sizeof(int) * n);

    for (int i = 0; i < n; ++i) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, hello, &ids[i]);
    }

    /** TILL HERE **/

    int i = 0;
    while (i++ < atoi(args[2])) {
        printf("Hello from main %d\n", i);
        sleep(2);
    }

    i = 0;

    /*******************************************
        "Skasuj" wszystke uruchomione watki i poczekaj na ich zakonczenie
    *******************************************/

    /** ADDED BY ME FROM HERE **/

    for (int i = 0; i < n; ++i) {
        pthread_cancel(threads[i]);
    }

    for (int i = 0; i < n; ++i) {
        pthread_join(threads[i], (void**)NULL);
    }

    /** TILL HERE **/

    printf("DONE");

    return 0;
}

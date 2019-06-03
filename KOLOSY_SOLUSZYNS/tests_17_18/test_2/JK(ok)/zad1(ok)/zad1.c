
/*
Program tworzy liczbe watkow rowno liczbie przekazanych do programu argumentow.
Kazdy watek po utworzeniu czeka zadany przez argument czas, a nastepnie sie
konczy. Program przechowuje w tablicy informacje o wszyskich stworzonych watkach
- ich tid i ich stany. Gdy watek konczy swoje dzialanie - stan TS_TERMINATED
informuje o tym przez odpowiednia zmienna warunkowa, na ktorej czeka glowny
watek

Uzupelnij oznaczone framenty kody zwiazane z operacjami na muteksie i zmiennej
warunkowej.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;

static int totThreads = 0; /* Calkowita liczba stworzonych watkow */
static int numLive = 0; /* Calkowita liczba watkow wciaz zywych lub zakonczonych
                           ale jeszcze nie joined */
static int numUnjoined = 0; /*  LIczba zakonczonych watkow jeszcze nie joined */

enum tstate {                /* Stany watkow */
              TS_ALIVE,      /* Watek zywa */
              TS_TERMINATED, /* Watek zakonczony, jeszcze nie  joined */
              TS_JOINED      /* Watek zakonczony i joined */
};

static struct {        /* Informacja o kazdym watku */
    pthread_t tid;     /* ID watku */
    enum tstate state; /* Stan watku (stale TS_*) */
    int sleepTime;     /* Liczba sekund zycia watku  */
} * thread;

static void* /* Funkcja startowa watku */
threadFunc(void* arg) {
    int idx = (int)arg;
    int s;

    sleep(thread[idx].sleepTime); /* Simulacja pracy watku */
    printf("Thread %d terminating\n", idx);

    /*UZUPELNIC: zajmij mutex threadMutex */

    /** ADDED BY ME FROM HERE **/

    s = pthread_mutex_lock(&threadMutex);

    /** TILL HERE **/

    if (s != 0) {
        printf(s, "%d error 1");
        exit(-1);
    }
    numUnjoined++;
    thread[idx].state = TS_TERMINATED;

    /*UZUPELNIC: zwolnij mutex threadMutex */

    /** ADDED BY ME FROM HERE **/

    s = pthread_mutex_unlock(&threadMutex);

    /** TILL HERE **/

    if (s != 0) {
        printf("%d error 2", s);
        exit(-1);
    }

    /*UZUPELNIC: zasygnalizuj zmienna warunkowa threadDied */

    /** ADDED BY ME FROM HERE **/

    s = pthread_cond_broadcast(&threadDied);

    /** TILL HERE **/

    if (s != 0) {
        printf("%d error 3", s);
        exit(-1);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    int s, idx;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        printf("%s num-secs...\n", argv[0]);
        exit(-1);
    }

    thread = calloc(argc - 1, sizeof(*thread));
    if (thread == NULL) {
        printf("calloc");
        exit(-1);
    }

    for (idx = 0; idx < argc - 1; idx++) {
        thread[idx].sleepTime = atoi(argv[idx + 1]);
        thread[idx].state = TS_ALIVE;

        s = pthread_create(&thread[idx].tid, NULL, threadFunc, (void*)idx);

        if (s != 0) {
            printf(s, "%d pthread_create");
            exit(-1);
        }
    }

    totThreads = argc - 1;
    numLive = totThreads;

    while (numLive > 0) {
        /*UZUPELNIC: zajmij mutex threadMutex */

        /** ADDED BY ME FROM HERE **/

        s = pthread_mutex_lock(&threadMutex);

        /** TILL HERE **/

        if (s != 0) {
            printf("%d error 4", s);
            exit(-1);
        }

        while (numUnjoined == 0) {
            /*UZUPELNIC:  czekaj na zmiennej warunkowej threadDied zwiazanej
                   z mutekesem threadMutex */

            /** ADDED BY ME FROM HERE **/

            s = pthread_cond_wait(&threadDied, &threadMutex);

            /** TILL HERE **/

            if (s != 0) {
                printf("%d error 5", s);
                exit(-1);
            }
        }

        for (idx = 0; idx < totThreads; idx++) {
            if (thread[idx].state == TS_TERMINATED) {
                s = pthread_join(thread[idx].tid, NULL);
                if (s != 0) {
                    printf("%d pthread_join", s);
                    exit(-1);
                }
                thread[idx].state = TS_JOINED;
                numLive--;
                numUnjoined--;

                printf("Odebrany watek %d (numLive=%d)\n", idx, numLive);
            }
        }

        /*UZUPELNIC: zwolnij mutex threadMutex */

        /** ADDED BY ME FROM HERE **/

        s = pthread_mutex_unlock(&threadMutex);

        /** ADDED BY ME FROM HERE **/

        if (s != 0) {
            printf("%d error 6", s);
            exit(-1);
        }
    }

    exit(EXIT_SUCCESS);
}

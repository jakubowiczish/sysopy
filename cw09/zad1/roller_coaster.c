#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "error.h"

#define CART_EMPTY 0

#define CART_BOARDED 1

static int people;
static int carts;
static int rides;
static int capacity;

static int current_cart = 0;
static int all_boarded = 0;

static int current_cart_people_count;
static int current_person = -1;
static int current_leaver = -1;

static int button_person = -1;
static int wait_for_button = 0;

static int active_carts;
static int* carts_status;
static int* queue;

static pthread_mutex_t mutex_cart_dock = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t mutex_person_current = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t mutex_wait_release = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t mutex_end = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t mutex_button = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t cond_cart_next = PTHREAD_COND_INITIALIZER;

static pthread_cond_t cond_person_next = PTHREAD_COND_INITIALIZER;

static pthread_cond_t cond_leaver = PTHREAD_COND_INITIALIZER;

static pthread_cond_t cond_end = PTHREAD_COND_INITIALIZER;

static pthread_cond_t cond_button = PTHREAD_COND_INITIALIZER;

void info(const char* format, ...) {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    int h = tv.tv_sec / 3600;
    int m = (tv.tv_sec - h * 3600) / 60;
    int s = tv.tv_sec - h * 3600 - m * 60;
    int ms = tv.tv_usec / 1000;

    printf("%02d:%02d:%02d:%03d: ", h % 24, m, s, ms);

    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);

    printf("\n");
}

void queue_add(int id) {
    for (int i = 0; i < people; ++i) {
        if (queue[i] == -1) {
            queue[i] = id;
            break;
        }
    }
}

int queue_pop() {
    if (queue[0] == -1) {
        return -1;
    }

    int id = queue[0];
    for (int i = 1; i < people; ++i) {
        queue[i - 1] = queue[i];
    }

    queue[people - 1] = -1;

    return id;
}

void* cart_worker(void* arg) {
    int id = *((int*)arg);

    int* passengers = malloc(capacity * sizeof(int));

    for (int ride = 0; ride < rides; ++ride) {
        pthread_mutex_lock(&mutex_cart_dock);

        while (current_cart != id && all_boarded != 0) {
            pthread_cond_wait(&cond_cart_next, &mutex_cart_dock);
        }

        button_person = -1;

        info("cart #%d awaiting passengers", id);

        pthread_mutex_lock(&mutex_person_current);

        for (int i = 0; i < capacity; ++i) {
            current_person = queue_pop();
            current_cart_people_count = i;

            passengers[i] = current_person;

            pthread_cond_broadcast(&cond_person_next);

            while (current_person != -1) {
                pthread_cond_wait(&cond_person_next, &mutex_person_current);
            }
        }

        pthread_mutex_unlock(&mutex_person_current);

        info("cart #%d closed door", id);

        pthread_mutex_lock(&mutex_button);

        button_person = passengers[rand() % capacity];

        pthread_cond_broadcast(&cond_button);

        wait_for_button = 1;
        while (wait_for_button == 1) {
            pthread_cond_wait(&cond_button, &mutex_button);
        }

        pthread_mutex_unlock(&mutex_button);

        carts_status[id] = CART_BOARDED;

        current_cart = (current_cart + 1) % carts;

        int boarded_carts = 0;
        for (int i = 0; i < carts; ++i) {
            if (carts_status[i] == CART_BOARDED) {
                boarded_carts++;
            }
        }

        if (boarded_carts == carts) {
            all_boarded = 1;
        }

        pthread_cond_broadcast(&cond_cart_next);

        pthread_mutex_unlock(&mutex_cart_dock);

        info("cart #%d left", id);

        sleep(1);

        info("cart #%d finished ride and awaits dock space", id);

        pthread_mutex_lock(&mutex_cart_dock);

        while (current_cart != id && all_boarded == 0) {
            pthread_cond_wait(&cond_cart_next, &mutex_cart_dock);
        }

        info("cart #%d opened door", id);

        for (int i = 0; i < capacity; ++i) {
            pthread_mutex_lock(&mutex_wait_release);

            current_leaver = passengers[i];

            queue_add(passengers[i]);

            passengers[i] = -1;

            current_cart_people_count = capacity - i - 1;

            pthread_cond_broadcast(&cond_leaver);

            while (current_leaver != -1) {
                pthread_cond_wait(&cond_leaver, &mutex_wait_release);
            }

            pthread_mutex_unlock(&mutex_wait_release);
        }

        carts_status[id] = CART_EMPTY;

        current_cart = (current_cart + 1) % carts;

        pthread_cond_broadcast(&cond_cart_next);

        pthread_mutex_unlock(&mutex_cart_dock);
    }

    pthread_mutex_lock(&mutex_person_current);

    active_carts -= 1;

    pthread_cond_broadcast(&cond_person_next);

    pthread_mutex_unlock(&mutex_person_current);

    free(passengers);

    return 0;
}

void* person_worker(void* arg) {
    int id = *((int*)arg);

    while (1) {
        pthread_mutex_lock(&mutex_person_current);

        while (current_person != id) {
            if (active_carts == 0) {
                pthread_mutex_unlock(&mutex_person_current);
                return 0;
            }

            pthread_cond_wait(&cond_person_next, &mutex_person_current);
        }

        info("person #%d entered the cart, %d people inside", id,
             current_cart_people_count);

        current_person = -1;
        pthread_cond_broadcast(&cond_person_next);
        pthread_mutex_unlock(&mutex_person_current);

        pthread_mutex_lock(&mutex_button);

        while (button_person == -1) {
            pthread_cond_wait(&cond_button, &mutex_button);
        }

        if (button_person == id) {
            info("person #%d pressed the start button", id);

            wait_for_button = 0;
            pthread_cond_broadcast(&cond_button);
        }

        pthread_mutex_unlock(&mutex_button);

        pthread_mutex_lock(&mutex_wait_release);

        while (current_leaver != id) {
            pthread_cond_wait(&cond_leaver, &mutex_wait_release);
        }

        info("person #%d left the cart, %d people still inside", id,
             current_cart_people_count);

        current_leaver = -1;
        pthread_cond_broadcast(&cond_leaver);
        pthread_mutex_unlock(&mutex_wait_release);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        err("usage: %s people carts capacity rides", argv[0]);
    }

    int scanf_result = 0;

    scanf_result += sscanf(argv[1], "%d", &people);

    scanf_result += sscanf(argv[2], "%d", &carts);

    scanf_result += sscanf(argv[3], "%d", &capacity);

    scanf_result += sscanf(argv[4], "%d", &rides);

    if (scanf_result != 4) {
        err("invalid arguments passed");
    }

    if (people < capacity) {
        err("not enough people");
    }

    active_carts = carts;

    current_cart = 0;

    carts_status = malloc(carts * sizeof(int));

    for (int i = 0; i < carts; ++i) {
        carts_status[i] = CART_EMPTY;
    }

    queue = malloc(people * sizeof(int));
    for (int i = 0; i < people; ++i) {
        queue[i] = -1;
    }

    int* people_indexes = malloc(people * sizeof(int));

    int* carts_indexes = malloc(carts * sizeof(int));

    pthread_t* people_threads = malloc(people * sizeof(pthread_t));

    pthread_t* carts_threads = malloc(carts * sizeof(pthread_t));

    for (int i = 0; i < people; ++i) {
        people_indexes[i] = i;

        queue_add(i);

        if (pthread_create(&people_threads[i], NULL, person_worker,
                           &people_indexes[i]) != 0) {
            err("unable to create person thread #%d", i);
        }
    }

    for (int i = 0; i < carts; ++i) {
        carts_indexes[i] = i;

        if (pthread_create(&carts_threads[i], NULL, cart_worker,
                           &carts_indexes[i]) != 0) {
            err("unable to create cart thread #%d", i);
        }
    }

    for (int i = 0; i < people; ++i) {
        if (pthread_join(people_threads[i], NULL) != 0) {
            err("unable to join person thread #%d", i);
        }

        info("person thread ended: #%d", i);
    }

    for (int i = 0; i < carts; ++i) {
        if (pthread_join(carts_threads[i], NULL) != 0) {
            err("unable to join cart thread #%d", i);
        }

        info("cart thread ended: #%d", i);
    }

    free(queue);
    free(people_indexes);
    free(carts_indexes);
    free(people_threads);
    free(carts_threads);

    pthread_mutex_destroy(&mutex_cart_dock);
    pthread_mutex_destroy(&mutex_person_current);
    pthread_mutex_destroy(&mutex_wait_release);
    pthread_mutex_destroy(&mutex_end);
    pthread_mutex_destroy(&mutex_button);

    pthread_cond_destroy(&cond_cart_next);
    pthread_cond_destroy(&cond_person_next);
    pthread_cond_destroy(&cond_leaver);
    pthread_cond_destroy(&cond_end);
    pthread_cond_destroy(&cond_button);

    return 0;
}
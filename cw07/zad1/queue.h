#ifndef SYSOPY_QUEUE_H
#define SYSOPY_QUEUE_H

#include <stddef.h>
#include "shared.h"

#define MAX_CAPACITY 16

typedef struct queue_t {
    int size;
    int capacity;

    int head;
    int tail;

    int sem_key;

    int array[MAX_CAPACITY];
} queue_t;


queue_t new_queue(int capacity, int sem_key);

int is_full(queue_t *q, sem_id_t sem);

int is_empty(queue_t *q, sem_id_t sem);

int enqueue(queue_t *q, sem_id_t sem, int item);

int *dequeue(queue_t *q, sem_id_t sem, int *item);

int *peek(queue_t *q, sem_id_t sem, int index, int *item);

#endif //SYSOPY_QUEUE_H

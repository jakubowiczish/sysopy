#ifndef SYSOPY_QUEUE_H
#define SYSOPY_QUEUE_H

#include <stddef.h>
#include "pack.h"
#include "shared.h"

#define MAX_CAPACITY 512


typedef struct queue_t {
    int size;
    int capacity;

    int weight;
    int max_weight;

    int head;
    int tail;

    int sem_key;

    pack_t array[MAX_CAPACITY];
} queue_t;


queue_t new_queue(int capacity, int max_weight, int sem_key);

int enqueue(queue_t *q, sem_id_t sem, pack_t item);

pack_t *dequeue(queue_t *q, sem_id_t sem, pack_t *item);


#endif //SYSOPY_QUEUE_H

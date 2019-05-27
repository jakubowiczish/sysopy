#ifndef SYSOPY_CONVEYOR_BELT_H
#define SYSOPY_CONVEYOR_BELT_H

#include <stddef.h>
#include "pack.h"
#include "shared.h"

#define MAX_CAPACITY 512

#define CB_SIZE sizeof(conveyor_belt_t)

typedef struct conveyor_belt_t {
    int size;
    int capacity;

    int weight;
    int max_weight;

    int head;
    int tail;

    int sem_key;

    pack_t array[MAX_CAPACITY];
} conveyor_belt_t;

conveyor_belt_t new_queue(int capacity, int max_weight, int sem_key);

int enqueue(conveyor_belt_t* cb, sem_id_t sem, pack_t item);

pack_t* dequeue(conveyor_belt_t* cb, sem_id_t sem, pack_t* item);

#endif  // SYSOPY_CONVEYOR_BELT_H

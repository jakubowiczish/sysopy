#include "conveyor_belt.h"
#include "shared.h"

conveyor_belt_t new_queue(int capacity, int max_weight, int sem_key) {
    conveyor_belt_t cb;
    cb.size = 0;
    cb.capacity = capacity;
    cb.weight = 0;
    cb.max_weight = max_weight;
    cb.head = 0;
    cb.tail = capacity - 1;
    cb.sem_key = sem_key;

    return cb;
}


int enqueue(conveyor_belt_t *cb, sem_id_t sem, pack_t pack) {
    if (cb->weight + pack.weight > cb->max_weight) {
        return -2;
    }

    int result;

    lock_semaphore(sem);

    if (cb->size == cb->capacity) {
        result = -1;
    } else {
        cb->tail = (cb->tail + 1) % cb->capacity;
        cb->array[cb->tail] = pack;
        cb->size += 1;
        cb->weight += pack.weight;
        result = 0;
    }

    unlock_semaphore(sem);

    return result;
}


pack_t *dequeue(conveyor_belt_t *cb, sem_id_t sem, pack_t *pack) {
    lock_semaphore(sem);

    if (pack == NULL || cb->size == 0) {
        pack = NULL;
    } else {
        *pack = cb->array[cb->head];
        cb->head = (cb->head + 1) % cb->capacity;
        cb->size -= 1;
        cb->weight -= pack->weight;
    }

    unlock_semaphore(sem);

    return pack;
}
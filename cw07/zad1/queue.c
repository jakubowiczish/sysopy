#include "queue.h"
#include "shared.h"

queue_t new_queue(int capacity, int max_weight, int sem_key) {
    queue_t q;
    q.size = 0;
    q.capacity = capacity;
    q.weight = 0;
    q.max_weight = max_weight;
    q.head = 0;
    q.tail = capacity - 1;
    q.sem_key = sem_key;

    return q;
}


int enqueue(queue_t *q, sem_id_t sem, pack_t pack) {
    if (q->weight + pack.weight > q->max_weight) {
        return -2;
    }

    int result;

    lock_semaphore(sem);

    if (q->size == q->capacity) {
        result = -1;
    } else {
        q->tail = (q->tail + 1) % q->capacity;
        q->array[q->tail] = pack;
        q->size += 1;
        q->weight += pack.weight;
        result = 0;
    }

    unlock_semaphore(sem);

    return result;
}


pack_t *dequeue(queue_t *q, sem_id_t sem, pack_t *pack) {
    lock_semaphore(sem);

    if (pack == NULL || q->size == 0) {
        pack = NULL;
    } else {
        *pack = q->array[q->head];
        q->head = (q->head + 1) % q->capacity;
        q->size -= 1;
        q->weight -= pack->weight;
    }

    unlock_semaphore(sem);

    return pack;
}
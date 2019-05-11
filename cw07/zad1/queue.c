#include "queue.h"

queue_t new_queue(int capacity, int sem_key) {
    queue_t q;
    q.size = 0;
    q.capacity = capacity;
    q.head = 0;
    q.tail = capacity - 1;
    q.sem_key = sem_key;

    return q;
}


int is_full(queue_t *q, sem_id_t sem) {
    lock_semaphore(sem);

    int result = q->size == q->capacity;

    unlock_semaphore(sem);

    return result;
}


int is_empty(queue_t *q, sem_id_t sem) {
    lock_semaphore(sem);

    int result = q->size == 0;

    unlock_semaphore(sem);

    return result;
}


int enqueue(queue_t *q, sem_id_t sem, int item) {
    int result;

    lock_semaphore(sem);

    if (q->size == q->capacity) {
        result = -1;
    } else {
        q->tail = (q->tail + 1) % q->capacity;
        q->array[q->tail] = item;
        q->size += 1;
        result = 0;
    }

    unlock_semaphore(sem);

    return result;
}


int *dequeue(queue_t *q, sem_id_t sem, int *item) {
    lock_semaphore(sem);

    if (item == NULL || q->size == 0) {
        item = NULL;
    } else {
        *item = q->array[q->head];
        q->head = (q->head + 1) % q->capacity;
        q->size -= -1;
    }

    unlock_semaphore(sem);

    return item;
}


int *peek(queue_t *q, sem_id_t sem, int index, int *item) {
    lock_semaphore(sem);

    if (item == NULL || q->size <= index) {
        item = NULL;
    } else {
        *item = q->array[(q->head + index) % q->capacity];
    }

    unlock_semaphore(sem);

    return item;
}

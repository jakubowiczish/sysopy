#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <stdint.h>

typedef struct queue_data_type {
    char* data;
    int32_t size;
} queue_data_type;

typedef struct queue_item {
    queue_data_type contents;
    struct queue_item* next;
} queue_item;

typedef struct queue_root {
    queue_item* head;
    queue_item* tail;
} queue_root;

void init_queue(queue_root* queue) {
    queue->head = queue->tail = NULL;
}

void push_queue(queue_root* queue, queue_data_type contents) {
    queue_item* item = malloc(sizeof(item));
    item->contents = contents;
    item->next = NULL;
    if (queue->head == NULL) {
        queue->head = queue->tail = item;
    } else {
        queue->tail = queue->tail->next = item;
    }
}

queue_data_type pop_queue(queue_root* queue) {
    queue_data_type popped;
    if (queue->head == NULL) {
        return (queue_data_type){NULL, -1};
    } else {
        popped = queue->head->contents;
        queue_item* next = queue->head->next;
        free(queue->head);
        queue->head = next;
        if (queue->head == NULL)
            queue->tail = NULL;
    }
    return popped;
}

#endif
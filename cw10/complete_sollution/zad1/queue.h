#pragma once

#include <stddef.h>

typedef struct node_t node_t;
typedef struct node_t {
    node_t* next;
    void* value;
} node_t;

typedef struct queue_t {
    node_t* tail;
    node_t* head;
} queue_t;

int quenew(queue_t* queue);
int queadd(queue_t* queue, void* item);
void* queget(queue_t* queue);
int queclear(queue_t* queue);
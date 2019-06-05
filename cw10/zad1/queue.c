#include "queue.h"
#include <stdlib.h>
int quenew(queue_t* queue) {
    queue->head = NULL;
    queue->tail = NULL;
    return 1;
}

int queclear(queue_t* queue) {
    node_t* node = queue->head;
    while (node != NULL) {
        node_t* tmp = node;
        node = node->next;
        free(tmp);
    }
    return 0;
}

int queadd(queue_t* queue, void* item) {
    node_t* node = malloc(sizeof(node_t));
    node->value = item;
    node->next = NULL;

    if (queue->tail == NULL) {
        queue->tail = queue->head = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }

    return -1;
}

void* queget(queue_t* queue) {
    if (queue->head == NULL)
        return NULL;
    else {
        node_t* tmp = queue->head;
        queue->head = queue->head->next;
        if (queue->head == NULL) {
            queue->tail = NULL;
        }
        void* value = tmp->value;
        free(tmp);
        return value;
    }
}

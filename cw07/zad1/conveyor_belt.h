#ifndef SYSOPY_CONVEYOR_BELT_H
#define SYSOPY_CONVEYOR_BELT_H

#include "queue.h"

#define CB_SIZE sizeof(conveyor_belt_t)


typedef struct conveyor_belt_t {
    int max_weight;
    queue_t q;
} conveyor_belt_t;


#endif //SYSOPY_CONVEYOR_BELT_H

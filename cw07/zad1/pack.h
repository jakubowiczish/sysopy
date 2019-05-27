#ifndef SYSOPY_PACK_H
#define SYSOPY_PACK_H

#include <sys/time.h>

typedef struct pack_t {
    int weight;
    int pid;
    struct timeval timestamp;
} pack_t;

pack_t new_pack(int weight);

#endif  // SYSOPY_PACK_H

#include <stddef.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "shared.h"

#include "error.h"

int create_shared_mem(int key, size_t size) {
    int segment_id = shmget(key, size, IPC_CREAT | IPC_EXCL | 0644);

    if (segment_id == -1) {
        print_error("error while creating shared memory");
    }

    return segment_id;
}



int open_shared_mem(int key, size_t size) {
    int id = shmget(key, size, 0);

    if (id == -1) {
        print_error("error while opening shared memory");
    }

    return id;
}



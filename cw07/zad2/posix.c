#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "error.h"
#include "shared.h"

/* ##################################################################################################################
 */

int create_shared_mem(int key, size_t size) {
    char path[64];
    sprintf(path, "/%d", key);

    int segment_id = shm_open(path, O_RDWR | O_CREAT | O_EXCL, 0644);

    if (segment_id == -1) {
        print_error("error while creating shared memory!");
    }

    int result = ftruncate(segment_id, size);

    if (result == -1) {
        print_error("error while resizing shared memory");
    }

    return segment_id;
}

int open_shared_mem(int key, size_t size) {
    char path[64];
    sprintf(path, "/%d", key);

    int id = shm_open(path, O_RDWR, 0);

    if (id == -1) {
        print_error("error while opening shared memory!");
    }

    return id;
}

void* map_shared_mem(int id, size_t size) {
    void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, id, 0);

    if (ptr == (void*)-1) {
        print_error("error while mapping shared memory!");
    }

    return ptr;
}

void unmap_shared_mem(void* ptr, size_t size) {
    if (munmap(ptr, size) == -1) {
        print_error("error while \"unmapping\" shared memory");
    }
}

void remove_shared_mem(int key, int id) {
    char path[64];
    sprintf(path, "/%d", key);

    if (shm_unlink(path) == -1) {
        print_error("error while removing shared memory");
    }
}

/* ##################################################################################################################
 */

sem_id_t create_semaphore(int key) {
    char path[64];
    sprintf(path, "/%d", key);

    sem_id_t id = sem_open(path, O_RDWR | O_CREAT | O_EXCL, 0644);

    if (id == SEM_FAILED) {
        print_error("error while creating semaphore");
    }

    unlock_semaphore(id);

    return id;
}

sem_id_t open_semaphore(int key) {
    char path[64];
    sprintf(path, "/%d", key);

    sem_id_t id = sem_open(path, O_RDWR, 0644);

    if (id == SEM_FAILED) {
        print_error("error while creating semaphore");
    }

    return id;
}

void lock_semaphore(sem_id_t id) {
    if (sem_wait(id) == -1) {
        print_error("error while locking semaphore");
    }
}

void unlock_semaphore(sem_id_t id) {
    if (sem_post(id) == -1) {
        print_error("error while unlocking semaphore");
    }
}

void close_semaphore(sem_id_t id) {
    if (sem_close(id) == -1) {
        print_error("error while closing semaphore");
    }
}

void remove_semaphore(int key, sem_id_t id) {
    close_semaphore(id);

    char path[64];
    sprintf(path, "/%d", key);

    if (sem_unlink(path) == -1) {
        print_error("error while removing semaphore");
    }
}

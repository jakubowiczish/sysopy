#include <stddef.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "shared.h"

#include "error.h"

/* ##################################################################################################################
 */

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

void* map_shared_mem(int id, size_t size) {
    void* ptr = shmat(id, NULL, 0);

    if (ptr == (void*)-1) {
        print_error("error while mapping shared memory");
    }

    return ptr;
}

void unmap_shared_mem(void* ptr, size_t size) {
    if (shmdt(ptr) == -1) {
        print_error("error while \"unmapping\" shared memory");
    }
}

void remove_shared_mem(int key, int id) {
    if (shmctl(id, IPC_RMID, NULL) == -1) {
        print_error("error while removing shared memory");
    }
}

/* ##################################################################################################################
 */

sem_id_t create_semaphore(int key) {
    sem_id_t semaphore_set_id = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644);

    if (semaphore_set_id == -1) {
        print_error("error while creating semaphore");
    }

    if (semctl(semaphore_set_id, 0, SETVAL, 1) == -1) {
        print_error("error while initializing semaphore");
    }

    return semaphore_set_id;
}

sem_id_t open_semaphore(int key) {
    sem_id_t semaphore_set_id = semget(key, 1, 0);

    if (semaphore_set_id == -1) {
        print_error("error while opening a semaphore");
    }

    return semaphore_set_id;
}

void lock_semaphore(sem_id_t id) {
    struct sembuf sem_buf;

    sem_buf.sem_num = 0;
    sem_buf.sem_op = -1;
    sem_buf.sem_flg = 0;

    if (semop(id, &sem_buf, 1) == -1) {
        print_error("error while locking semaphore");
    }
}

void unlock_semaphore(sem_id_t id) {
    struct sembuf sem_buf;

    sem_buf.sem_num = 0;
    sem_buf.sem_op = 1;
    sem_buf.sem_flg = 0;

    if (semop(id, &sem_buf, 1) == -1) {
        print_error("error while unlocking semaphore");
    }
}

void close_semaphore(sem_id_t sem) {
    return;
}

void remove_semaphore(int key, sem_id_t id) {
    if (semctl(id, 0, IPC_RMID) == -1) {
        print_error("error while removing semaphore");
    }
}

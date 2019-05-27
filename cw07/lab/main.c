#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SIZE 32

#define MEMORY_NAME "/memory"

int main(int argc, char** argv) {
    int fd = shm_open(MEMORY_NAME, O_RDWR | O_CREAT | O_EXCL, 0666);

    if (fd == -1) {
        printf("ERROR while creating shared memory!\n");
    }

    if (ftruncate(fd, SIZE) == -1) {
        printf("ERROR while ftruncate\n");
    }

    return 0;
}
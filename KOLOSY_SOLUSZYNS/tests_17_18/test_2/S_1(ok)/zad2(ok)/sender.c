#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SHM_NAME "/kol_shm"
#define MAX_SIZE 1024

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Not a suitable number of program parameters\n");
        return (1);
    }

    /***************************************
    Utworz/otworz posixowy obszar pamieci wspolnej "reprezentowany" przez
    SHM_NAME zapisz tam wartosc przekazana jako parametr wywolania programu
    posprzataj
    *****************************************/

    /** ADDED BY ME FROM HERE **/

    int fd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0666);

    ftruncate(fd, MAX_SIZE);

    char* addr =
        (char*)mmap(NULL, MAX_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    addr[0] = '\0';

    strncpy(addr, argv[1], MAX_SIZE);

    munmap(addr, MAX_SIZE);

    /** not told to clean up ?? */

    /** TILL HERE **/

    return 0;
}

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
    sleep(1);
    int val = 0;
    /*******************************************
    Utworz/otworz posixowy obszar pamieci wspolnej "reprezentowany" przez
    SHM_NAME odczytaj zapisana tam wartosc i przypisz ja do zmiennej val
    posprzataj
    *********************************************/

    /** ADDED BY ME FROM HERE **/

    int fd = shm_open(SHM_NAME, O_RDWR, 0666);

    char* addr =
        (char*)mmap(NULL, MAX_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    val = atoi(addr);

    munmap(addr, MAX_SIZE);

    /** not told to clean up ?? */

    /** TILL HERE **/

    printf("%d square is: %d \n", val, val * val);
    return 0;
}

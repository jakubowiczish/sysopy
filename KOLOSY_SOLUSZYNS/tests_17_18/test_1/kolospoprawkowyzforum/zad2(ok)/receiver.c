#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PIPE "./squareFIFO"

int main() {
    int val = 0;
    /***********************************
     * odczytaj z potoku nazwanego PIPE zapisana tam wartosc i przypisz ja do
     *zmiennej val posprzataj
     ************************************/

    /* FROM HERE */

    int fd = open(PIPE, O_RDONLY);

    char buf[512];

    read(fd, buf, sizeof(buf));

    val = atoi(buf);

    close(fd);

    /* TILL HERE */

    printf("%d square is: %d\n", val, val * val);
    return 0;
}

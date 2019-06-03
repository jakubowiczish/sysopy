#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define UNIX_PATH_MAX 108
#define SOCK_PATH "sock_path"

int main() {
    int fd = -1;

    /*********************************************
    Utworz socket domeny unixowej typu datagramowego
    Utworz strukture adresowa ustawiajac adres/sciezke komunikacji na SOCK_PATH
    Zbinduj socket z adresem/sciezka SOCK_PATH
    **********************************************/

    /** ADDED BY ME FROM HERE **/

    fd = socket(AF_UNIX, SOCK_DGRAM, 0);

    struct sockaddr_un addr;

    addr.sun_family = AF_UNIX;
    addr.sun_path[0] = '\0';

    strncpy(addr.sun_path, SOCK_PATH, UNIX_PATH_MAX);

    bind(fd, (const struct sockaddr*)&addr, sizeof(struct sockaddr_un));

    /** TILL HERE **/

    char buf[20];
    if (read(fd, buf, 20) == -1)
        perror("Error receiving message");
    int val = atoi(buf);
    printf("%d square is: %d\n", val, val * val);

    /***************************
    Posprzataj po sockecie
    ****************************/

    /** ADDED BY ME FROM HERE **/

    shutdown(fd, SHUT_RDWR);
    close(fd);
    unlink(SOCK_PATH);

    /** TILL HERE **/

    return 0;
}

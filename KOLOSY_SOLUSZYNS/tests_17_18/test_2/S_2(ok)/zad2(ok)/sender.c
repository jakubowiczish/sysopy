#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define UNIX_PATH_MAX 108
#define SOCK_PATH "sock_path"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Not a suitable number of program parameters\n");
        return (1);
    }
    sleep(1);

    /*********************************************
    Utworz socket domeny unixowej typu datagramowego
    Utworz strukture adresowa ustawiajac adres/sciezke komunikacji na SOCK_PATH
    Polacz sie korzystajac ze zdefiniowanych socketu i struktury adresowej
    ***********************************************/
    int fd = -1;

    /** ADDED BY ME FROM HERE **/

    fd = socket(AF_UNIX, SOCK_DGRAM, 0);

    struct sockaddr_un addr;

    addr.sun_family = AF_UNIX;
    addr.sun_path[0] = '\0';

    strncpy(addr.sun_path, SOCK_PATH, UNIX_PATH_MAX);

    connect(fd, (const struct sockaddr*)&addr, sizeof(struct sockaddr_un));

    /** TILL HERE **/

    char buff[20];
    int to_send = sprintf(buff, argv[1]);

    if (write(fd, buff, to_send + 1) == -1) {
        perror("Error sending msg to server");
    }

    /*****************************
    posprzataj po sockecie
    ********************************/

    close(fd);

    return 0;
}

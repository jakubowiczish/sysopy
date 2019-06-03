#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY "./queuekey"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Not a suitable number of program parameters\n");
        return (1);
    }

    /******************************************************
    Utworz kolejke komunikatow systemu V "reprezentowana" przez KEY
    Wyslij do niej wartosc przekazana jako parametr wywolania programu
    Obowiazuja funkcje systemu V
    ******************************************************/

    key_t key = ftok(KEY, 1);

    int queue = msgget(key, 0644);

    int value = atoi(argv[1]);

    struct msgbuf* buffer = calloc(1, sizeof(long) + sizeof(value));

    buffer->mtype = 1;

    memcpy(&buffer->mtext, &value, sizeof(value));

    if (msgsnd(queue, &buffer, sizeof(value), 0) < 0) {
        perror("Problem with msgsnd");
    }

    return 0;
}

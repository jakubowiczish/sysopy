#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/** path must be an existing one **/

#define KEY "/tmp"

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

    /** ADDED BY ME FROM HERE **/

    key_t key = ftok(KEY, 1);

    int value = atoi(argv[1]);

    int queue = msgget(key, IPC_CREAT | 0666);

    if (msgsnd(queue, &value, sizeof(int), 0) < 0) {
        perror("Problem with msgsnd");
        exit(1);
    }

    /** TILL HERE **/

    return 0;
}

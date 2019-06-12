#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define FILE_NAME "common.txt"
#define SEM_NAME "/kol_sem"

int main(int argc, char** args) {
    if (argc != 4) {
        printf("Not a suitable number of program parameters\n");
        return (1);
    }

    /**************************************************
    Stworz semafor systemu V
    Ustaw jego wartosc na 1
    ***************************************************/

    /** ADDED BY ME FROM HERE **/

    key_t key = ftok(SEM_NAME, 1);

    int sem = semget(key, 1, IPC_CREAT | 0666);

    semctl(sem, 0, SETVAL, 1);

    /** TILL HERE **/

    int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    int parent_loop_counter = atoi(args[1]);
    int child_loop_counter = atoi(args[2]);

    char buf[40];
    pid_t child_pid;
    int max_sleep_time = atoi(args[3]);

    child_pid = fork();

    if (child_pid) {
        int status = 0;
        srand((unsigned)time(0));

        while (parent_loop_counter--) {
            int s = rand() % max_sleep_time + 1;
            sleep(s);

            /*****************************************
            sekcja krytyczna zabezpiecz dostep semaforem
            **********************************************/

            /** ADDED BY ME FROM HERE **/

            struct sembuf sops;

            sops.sem_num = 0;
            sops.sem_op = -1;
            sops.sem_flg = 0;

            semop(sem, &sops, 1);

            /** TILL HERE **/

            sprintf(buf, "Wpis rodzica. Petla %d. Spalem %d\n",
                    parent_loop_counter, s);
            write(fd, buf, strlen(buf));
            write(1, buf, strlen(buf));

            /*********************************
            Koniec sekcji krytycznej
            **********************************/

            /** ADDED BY ME FROM HERE **/

            sops.sem_num = 0;
            sops.sem_op = 1;
            sops.sem_flg = 0;

            semop(sem, &sops, 1);

            /** TILL HERE **/
        }

        waitpid(child_pid, &status, 0);

    } else {
        srand((unsigned)time(0));
        while (child_loop_counter--) {
            int s = rand() % max_sleep_time + 1;
            sleep(s);

            /*****************************************
            sekcja krytyczna zabezpiecz dostep semaforem
            **********************************************/
            /** ADDED BY ME FROM HERE **/

            struct sembuf sops;

            sops.sem_num = 0;
            sops.sem_op = -1;
            sops.sem_flg = 0;

            semop(sem, &sops, 1);

            /** TILL HERE **/

            sprintf(buf, "Wpis dziecka. Petla %d. Spalem %d\n",
                    child_loop_counter, s);
            write(fd, buf, strlen(buf));
            write(1, buf, strlen(buf));

            /*********************************
            Koniec sekcji krytycznej
            **********************************/

            /** ADDED BY ME FROM HERE **/

            sops.sem_num = 0;
            sops.sem_op = 1;
            sops.sem_flg = 0;

            semop(sem, &sops, 1);

            /** TILL HERE **/
        }

        _exit(0);
    }

    /*****************************
    posprzataj semafor
    ******************************/

    /** ADDED BY ME FROM HERE **/

    semctl(sem, 0, IPC_RMID);

    /** TILL HERE **/

    close(fd);
    return 0;
}

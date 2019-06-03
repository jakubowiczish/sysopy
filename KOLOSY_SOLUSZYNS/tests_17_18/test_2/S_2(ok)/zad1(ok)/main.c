#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define FILE_NAME "common.txt"
#define SEM_NAME "my_kol_sem"

void print_sem_value(sem_t* sem) {
    int sem_val;
    sem_getvalue(sem, &sem_val);
    printf("Current sem value: %d\n", sem_val);
}

int main(int argc, char** args) {
    if (argc != 4) {
        printf("Not a suitable number of program parameters\n");
        return (1);
    }

    /************************************
    Utworz semafor posixowy. Ustaw jego wartosc na 1
    *************************************/

    /** ADDED BY ME FROM HERE **/

    sem_t* sem_id = sem_open(SEM_NAME, O_CREAT, 0644, 1);

    /** TILL HERE **/

    print_sem_value(sem_id);

    int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    int parent_loop_counter = atoi(args[1]);
    int child_loop_counter = atoi(args[2]);

    char buf[20];
    pid_t child_pid;
    int max_sleep_time = atoi(args[3]);

    child_pid = fork();

    if (child_pid) {
        int status = 0;
        srand((unsigned)time(0));

        while (parent_loop_counter--) {
            int s = rand() % max_sleep_time + 1;
            sleep(s);

            print_sem_value(sem_id);

            /******************************************
            Sekcja krytyczna. Zabezpiecz dostep semaforem
            ******************************************/

            /** ADDED BY ME FROM HERE **/

            sem_wait(sem_id);

            /** TILL HERE **/

            print_sem_value(sem_id);

            sprintf(buf, "Wpis rodzica. Petla %d. Spalem %d\n",
                    parent_loop_counter, s);
            write(fd, buf, strlen(buf));
            write(1, buf, strlen(buf));

            print_sem_value(sem_id);

            /****************************************************
            Koniec sekcji krytycznej
            ****************************************************/

            /** ADDED BY ME FROM HERE **/

            sem_post(sem_id);

            /** TILL HERE **/

            print_sem_value(sem_id);
        }
        waitpid(child_pid, &status, 0);
    } else {
        srand((unsigned)time(0));
        while (child_loop_counter--) {
            int s = rand() % max_sleep_time + 1;
            sleep(s);

            print_sem_value(sem_id);

            /******************************************
            Sekcja krytyczna. Zabezpiecz dostep semaforem
            ******************************************/

            /** ADDED BY ME FROM HERE **/

            sem_wait(sem_id);

            /** TILL HERE **/

            print_sem_value(sem_id);

            sprintf(buf, "Wpis dziecka. Petla %d. Spalem %d\n",
                    child_loop_counter, s);
            write(fd, buf, strlen(buf));
            write(1, buf, strlen(buf));

            print_sem_value(sem_id);

            /****************************************************
            Koniec sekcji krytycznej
            ****************************************************/

            /** ADDED BY ME FROM HERE **/

            sem_post(sem_id);

            /** TILL HERE **/

            print_sem_value(sem_id);
        }
        _exit(0);
    }

    /***************************************
    Posprzataj semafor
    ***************************************/

    /** ADDED BY ME FROM HERE **/

    sem_close(sem_id);
    sem_unlink(SEM_NAME);

    /** TILL HERE **/

    close(fd);
    return 0;
}

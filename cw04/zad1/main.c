#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int is_running = 1;

void ctrl_c_handle(int sg) {
    printf("\nCtrl C pressed - end of program\n");
    exit(0);
}

void ctrl_z_handle(int sg) {
    if (!is_running) {
        printf("\nCtrl Z pressed - continue with Ctrl Z or terminate with Ctrl C\n");
    } else {
        printf("\n");
    }

    is_running = 1 - is_running;
}

int main(int argc, char **argv) {
    signal(SIGINT, ctrl_c_handle);

    struct sigaction action;
    action.sa_handler = ctrl_z_handle;
    sigemptyset(&action.sa_mask);
    sigaction(SIGTSTP, &action, NULL);

    time_t actual_time;
    struct tm *time_info;

    printf("\nPRESS CTRL Z TO PAUSE (CONTINUE) OR CTRL C TO TERMINATE\n");
    while (1) {
        if (is_running) {
            time(&actual_time);
            time_info = localtime(&actual_time);

            printf("Actual time: %s", asctime(time_info));
        }

        sleep(1);
    }
}
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static int is_running = 1;

void ctrl_c_handle_SIGINT(int sg) {
    printf("\nCtrl C pressed - end of program\n");
    exit(sg);
}

void ctrl_z_handle_SIGTSTP(int sg) {
    printf(
        "\nCtrl Z pressed - continue with Ctrl Z or terminate with Ctrl C\n");

    is_running = 1 - is_running;
}

int main(int argc, char** argv) {
    signal(SIGINT, ctrl_c_handle_SIGINT);

    struct sigaction sigaction1;
    //    memset(&sigaction1, 0, sizeof(struct sigaction));
    sigaction1.sa_handler = ctrl_z_handle_SIGTSTP;
    sigaction1.sa_flags = 0;
    sigaction(SIGTSTP, &sigaction1, NULL);

    time_t actual_time;
    struct tm* time_info;

    printf("\nPRESS CTRL Z TO PAUSE (CONTINUE) OR CTRL C TO TERMINATE\n");

    while (1) {
        if (is_running) {
            time(&actual_time);
            time_info = localtime(&actual_time);

            printf("Actual time: %s", asctime(time_info));
            sleep(1);
        } else {
            pause();
        }
    }
}
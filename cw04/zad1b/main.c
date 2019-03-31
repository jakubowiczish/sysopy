#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

static int is_running = 1;

static pid_t date_pid;

void fork_script_print() {
    date_pid = fork();
    if (date_pid == -1) {
        printf("Error with fork!\n");
    } else if (date_pid > 0) {
        printf("\nfork PID: %d\n", date_pid);
    } else {
        execl("/bin/bash", "bash", "print_date.sh", NULL);
        exit(EXIT_FAILURE);
    }
}


void ctrl_c_handle_SIGINT(int sg) {
    printf("\nCtrl C pressed - end of program\n");
    exit(sg);
}


void ctrl_z_handle_SIGTSTP(int sg) {
    if (is_running == 1) {
        printf("\nCtrl Z pressed - continue with Ctrl Z or terminate with Ctrl C\n");
        kill(date_pid, SIGKILL);
    } else {
        fork_script_print();
    }

    is_running = 1 - is_running;
}


int main(int argc, char **argv) {
    signal(SIGINT, ctrl_c_handle_SIGINT);

    struct sigaction sigaction1;
    sigaction1.sa_handler = ctrl_z_handle_SIGTSTP;
    sigaction1.sa_flags = 0;
    sigaction(SIGTSTP, &sigaction1, NULL);

    fork_script_print();

    while (1) {
        sleep(1);
    }

    return 0;
}
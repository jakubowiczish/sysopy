#include <bits/types/FILE.h>
#include <sys/types.h>
#include <stdio.h>
#include <zconf.h>
#include <wait.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <printf.h>
#include <stdlib.h>

void monitor(char *list_path, int timeout) {
    FILE *file = fopen(list_path, "r");
    if (file == NULL) {
        printf("FILE DOES NOT EXIST\n");
        return;
    }

    char path_on_list[256];
    int interval;

    rewind(file);

    int PIDS_SIZE = 3;
    int pid_ids[PIDS_SIZE];
    int i_p = 0;

    while (fscanf(file, "%s %d", path_on_list, &interval) == 2) {
        pid_t child_pid = fork();
        int copy_counter = 0;

        struct stat status;

        if (lstat(path_on_list, &status) < 0) {
            printf("PROBLEM WITH LSTAT FOR FILE: %s\n", path_on_list);
            return;
        }

        time_t last_modification_time = status.st_mtime;

        if (child_pid == -1) {
            printf("FORK PROBLEM!\n");
        } else if (child_pid > 0) {
            pid_ids[i_p++] = child_pid;
        } else {
            while (timeout > 0) {
                sleep((unsigned int) interval);

                if (lstat(path_on_list, &status) < 0) {
                    printf("PROBLEM WITH LSTAT FOR FILE: %s\n", path_on_list);
                    return;
                }

                time_t actual_modification_time = status.st_mtime;

                if (actual_modification_time != last_modification_time) {
                    time_t time = status.st_mtime;
                    struct tm *tm;
                    tm = localtime(&time);

                    char time_buffer[256];
                    strftime(time_buffer, sizeof(time_buffer), "_%y_%m_%d_%H_%M_%S", tm);

                    char backup_path[256];
                    strcpy(backup_path, path_on_list);
                    strcat(backup_path, time_buffer);

                    printf("%s PATH\n", path_on_list);
                    printf("%s BACKUP PATH\n", backup_path);

                    copy_counter++;

                    pid_t pid = fork();
                    if (pid == 0) {
                        execl("/bin/cp", "cp", path_on_list, backup_path, (char *) 0);
                        exit(copy_counter);
                    }

                }

                timeout -= interval;
            }

            exit(copy_counter);
        }
    }

    for (int i = 0; i < PIDS_SIZE; ++i) {
        int exit_status;
        waitpid(pid_ids[i], &exit_status, 0);
        int number_of_copies = WEXITSTATUS(exit_status);
        printf("PID: %d, number of copies: %d\n", pid_ids[i], number_of_copies);
    }
}

int main(int argc, char **argv) {
//    printf("%s\n", argv[1]);
    monitor(argv[1], atoi(argv[2]));

}


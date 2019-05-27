#include <bits/types/FILE.h>
#include <printf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <wait.h>
#include <zconf.h>

void monitor(char* list_path, int timeout, char* mode) {
    FILE* file = fopen(list_path, "r");
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
                sleep((unsigned int)interval);

                if (lstat(path_on_list, &status) < 0) {
                    printf("PROBLEM WITH LSTAT FOR FILE: %s\n", path_on_list);
                    return;
                }

                time_t actual_modification_time = status.st_mtime;

                if (actual_modification_time != last_modification_time) {
                    time_t time = status.st_mtime;
                    struct tm* tm;
                    tm = localtime(&time);

                    char time_buffer[256];
                    strftime(time_buffer, sizeof(time_buffer),
                             "_%y_%m_%d_%H_%M_%S", tm);

                    char backup_path[256];
                    strcpy(backup_path, path_on_list);
                    strcat(backup_path, time_buffer);

                    printf("%s PATH\n", path_on_list);
                    printf("%s BACKUP PATH\n", backup_path);

                    copy_counter++;

                    if (strcmp(mode, "exec") == 0) {
                        pid_t pid = fork();
                        if (pid == 0) {
                            execl("/bin/cp", "cp", path_on_list, backup_path,
                                  (char*)0);
                            exit(copy_counter);
                        }
                    } else if (strcmp(mode, "regular") == 0) {
                        pid_t pid = fork();
                        if (pid == 0) {
                            FILE* source_file = fopen(path_on_list, "r");
                            if (source_file == NULL) {
                                printf("PROBLEM WITH OPENING FILE!");
                                return;
                            }
                            FILE* dest_file = fopen(backup_path, "w+");
                            char buffer[512];
                            size_t n = 1;
                            while (n == 1) {
                                n = fread(buffer, sizeof(char), sizeof(buffer),
                                          file);
                                if (n == 0)
                                    break;
                                fwrite(buffer, sizeof(char), sizeof(buffer),
                                       dest_file);
                            }
                            fclose(source_file);
                            fclose(dest_file);
                            exit(copy_counter);
                        }
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

int main(int argc, char** argv) {
    //    printf("%s\n", argv[mytest]);
    monitor(argv[1], atoi(argv[2]), argv[3]);
}

#define _XOPEN_SOURCE 500
#define __USE_XOPEN 1
#define _GNU_SOURCE 1

#include <bits/types/time_t.h>
#include <lzma.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <ftw.h>
#include <time.h>
#include <stdlib.h>
#include <zconf.h>
#include <wait.h>

void search_directory() {
    DIR *directory = opendir(".");
    if (directory == NULL) {
        printf("PROBLEM WITH OPENING DIRECTORY!\n");
        return;
    }

    struct dirent *reader;

    while ((reader = readdir(directory)) != NULL) {
        struct stat status;

        lstat(reader->d_name, &status);

        if (S_ISDIR(status.st_mode)) {
            if (strcmp(reader->d_name, ".") == 0 || strcmp(reader->d_name, "..") == 0) {
                continue;
            }

            if (chdir(reader->d_name) != 0) {
                printf("ERROR WHILE OPENING %s\n", reader->d_name);
            }

            search_directory();

            pid_t child_pid;
            child_pid = fork();

            if (child_pid == -1) {
                printf("FORK PROBLEM!\n");
            } else if (child_pid > 0) {
                int st;
                waitpid(child_pid, &st, 0);
            } else {
                char cwd_buff[2048];
                if (getcwd(cwd_buff, 2048) == NULL) {
                    printf("PROBLEM WITH CHANGING PATH TO CURRENT DIRECTORY!\n");
                }

                printf("%s\n", cwd_buff);
                execl("/bin/ls", "ls", "-l", NULL);
            }

            if (chdir("..") != 0) {
                printf("PROBLEM WITH CHANGING PATH TO %s /../\n", reader->d_name);
            }
        }

    }
    closedir(directory);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("WRONG NUMBER OF ARGUMENTS\n!");
        return -1;
    }

    char *path = argv[1];

    printf("RECURSIVE: \n\n");

    if (chdir(path) != 0) {
        printf("PROBLEM WITH CHANGING THE PATH TO: %s\n", path);
        return -1;
    }

    search_directory();

}

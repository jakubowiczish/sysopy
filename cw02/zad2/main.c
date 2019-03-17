#include <bits/types/time_t.h>
#include <lzma.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#define __USE_XOPEN 1
#define _GNU_SOURCE 1

#include <time.h>

#include <stdlib.h>

char *date_format = "%y.%m%.%d %H:%m";

void print_file_status(struct stat *status) {
    printf("File status: ");
    if (status->st_mode & S_IRUSR) {
        printf("r");
    } else {
        printf("-");
    }

    if (status->st_mode & S_IWUSR) {
        printf("w");
    } else {
        printf("-");
    }

    if (status->st_mode & S_IXUSR) {
        printf("x");
    } else {
        printf("-");
    }

    if (status->st_mode & S_IRGRP) {
        printf("r");
    } else {
        printf("-");
    }

    if (status->st_mode & S_IWGRP) {
        printf("w");
    } else {
        printf("-");
    }

    if (status->st_mode & S_IXGRP) {
        printf("x");
    } else {
        printf("-");
    }

    if (status->st_mode & S_IROTH) {
        printf("r");
    } else {
        printf("-");
    }

    if (status->st_mode & S_IWOTH) {
        printf("w");
    } else {
        printf("-");
    }

    if (status->st_mode & S_IXOTH) {
        printf("x\n");
    } else {
        printf("-\n");
    }
}


void print_file_information(char *file_path, struct stat *status) {
    printf("File absolute path: %s\n", realpath(file_path, NULL));
    print_file_status(status);
    printf("File size: %d\n", (int) status->st_size);

    printf("Time of last access: %s", ctime(&status->st_atime));
    printf("Time of last modification: %s\n", ctime(&status->st_mtime));
}

void search_directory(char *directory_path, char *eq_operator, time_t date_to_compare) {
    DIR *directory = opendir(directory_path);

    struct dirent *directory_reader = readdir(directory);
    struct stat status;

    char actual_path[PATH_MAX + 1];

    while (directory_reader != NULL) {
        strcpy(actual_path, directory_path);
        strcat(actual_path, "/");
        strcat(actual_path, directory_reader->d_name);


        lstat(actual_path, &status);

        if (strcmp(directory_reader->d_name, ".") == 0 || strcmp(directory_reader->d_name, "..") == 0) {
            directory_reader = readdir(directory);
            continue;
        } else {
            if (S_ISREG(status.st_mode)) {
                if (strcmp(eq_operator, "=") == 0 && difftime(date_to_compare, status.st_mtime) == 0) {
                    print_file_information(actual_path, &status);
                } else if (strcmp(eq_operator, "<") == 0 && difftime(date_to_compare, status.st_mtime) > 0) {
                    print_file_information(actual_path, &status);
                } else if (strcmp(eq_operator, ">") == 0 && difftime(date_to_compare, status.st_mtime) < 0) {
                    print_file_information(actual_path, &status);
                }
            }
            if (S_ISDIR(status.st_mode)) {
                search_directory(actual_path, eq_operator, date_to_compare);
            }

            directory_reader = readdir(directory);
        }
    }

    closedir(directory);
}


int main(int argc, char **argv) {
    if (argc < 4) {
        printf("NOT ENOUGH ARGUMENTS!");
        return -1;
    }

    if (argc > 4) {
        printf("TOO MANY ARGUMENTS!");
        return -1;
    }

    char *example_date = "2018.01.01 15:00";

    char *file_path = argv[1];
    char *eq_operator = argv[2];
    char *date_to_compare = argv[3];

    struct tm *tm = malloc(sizeof(struct tm));
    strptime(date_to_compare, date_format, tm);
    time_t date_argument = mktime(tm);


    search_directory(file_path, eq_operator, date_argument);
}

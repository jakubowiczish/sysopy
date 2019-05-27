#define _XOPEN_SOURCE 500
#define __USE_XOPEN 1
#define _GNU_SOURCE 1

#include <bits/types/time_t.h>
#include <dirent.h>
#include <ftw.h>
#include <lzma.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

char* date_format = "%y.%m.%d %H:%m";

void print_file_status(struct stat* status) {
    printf("File status: ");
    if (S_ISREG(status->st_mode)) {
        printf("File\n");
    }
    if (S_ISDIR(status->st_mode)) {
        printf("Dir\n");
    }
    if (S_ISCHR(status->st_mode)) {
        printf("Char dev\n");
    }
    if (S_ISBLK(status->st_mode)) {
        printf("Block dev\n");
    }
    if (S_ISFIFO(status->st_mode)) {
        printf("Fifo\n");
    }
    if (S_ISLNK(status->st_mode)) {
        printf("Slink\n");
    }
    if (S_ISSOCK(status->st_mode)) {
        printf("Sock\n");
    }
}

void print_file_information(char* file_path, struct stat* status) {
    printf("File absolute path: %s\n", realpath(file_path, NULL));
    print_file_status(status);
    printf("File size: %d\n", (int)status->st_size);

    printf("Time of last access: %s", ctime(&status->st_atime));
    printf("Time of last modification: %s\n", ctime(&status->st_mtime));
}

void search_directory(char* directory_path,
                      char* eq_operator,
                      time_t date_to_compare) {
    DIR* directory = opendir(directory_path);

    struct dirent* directory_reader = readdir(directory);
    struct stat status;

    char actual_path[PATH_MAX + 1];

    while (directory_reader != NULL) {
        strcpy(actual_path, directory_path);
        strcat(actual_path, "/");
        strcat(actual_path, directory_reader->d_name);

        lstat(actual_path, &status);

        if (strcmp(directory_reader->d_name, ".") == 0 ||
            strcmp(directory_reader->d_name, "..") == 0) {
            directory_reader = readdir(directory);
            continue;
        } else {
            if (S_ISREG(status.st_mode)) {
                if (strcmp(eq_operator, "=") == 0 &&
                    difftime(date_to_compare, status.st_mtime) == 0) {
                    print_file_information(actual_path, &status);
                } else if (strcmp(eq_operator, "<") == 0 &&
                           difftime(date_to_compare, status.st_mtime) > 0) {
                    print_file_information(actual_path, &status);
                } else if (strcmp(eq_operator, ">") == 0 &&
                           difftime(date_to_compare, status.st_mtime) < 0) {
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

time_t global_date;
char* global_eq_operator;

static int nftw_repeat(char* file_path, struct stat* status) {
    double date_difference = difftime(global_date, status->st_mtime);
    if (!((strcmp(global_eq_operator, "=") == 0 && date_difference == 0) ||
          (strcmp(global_eq_operator, "<") == 0 && date_difference > 0) ||
          (strcmp(global_eq_operator, ">") == 0 && date_difference < 0))) {
        return 0;
    }

    print_file_information(file_path, status);
    return 0;
}

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("NOT ENOUGH ARGUMENTS!");
        return -1;
    }

    if (argc > 4) {
        printf("TOO MANY ARGUMENTS!");
        return -1;
    }

    //    char *example_date = "2018.01.01 15:00";

    char* file_path = argv[1];
    char* eq_operator = argv[2];
    char* date_to_compare = argv[3];

    struct tm* tm = malloc(sizeof(struct tm));
    strptime(date_to_compare, date_format, tm);
    time_t date_argument = mktime(tm);

    printf("\n\n\n\n RECURSIVE \n\n\n\n");

    search_directory(file_path, eq_operator, date_argument);

    printf("\n\n\n\n NFTW \n\n\n\n");

    global_eq_operator = eq_operator;
    global_date = date_argument;

    nftw(file_path, (__nftw_func_t)nftw_repeat, 10, FTW_PHYS);
}

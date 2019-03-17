#include <bits/types/time_t.h>
#include <lzma.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void print_file_status(struct stat *status) {
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
        printf("x");
    } else {
        printf("-");
    }
}


void print_file_information(char *file_path, struct stat *status) {
//    char absolute_path[PATH_MAX + 1];
//    char *path = realpath(file_path, absolute_path);
    printf("%s", file_path);
    print_file_status(status);
    printf(file_path, status->st_size);
    printf("%s", ctime((const time_t *) status->st_atime));
    printf("%s", ctime((const time_t *) status->st_mtime));

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
                if (strcmp(eq_operator, "=") == 0 && difftime(date_to_compare, status.st_mtime)) {
                    print_file_information(actual_path, &status);
                }
            }

        }
    }
}


int main(int argc, char **argv) {
    if (argc < 4) {
        printf("NOT ENOUGH ARGUMENTS!");
        return -1;
    }
    char *date_format = "%y.%m%.%d %H:%m";
//    char *example_date = "2018.01.01 15:00";

    char *file_path = argv[1];
    char *eq_operator = argv[2];
    char *date_to_compare = argv[3];

    struct tm tm;
}

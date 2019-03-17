#include <bits/types/time_t.h>
#include <lzma.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

void search_directory(char *directory_path, char *eq_operator, time_t date) {
    DIR *directory = opendir(directory_path);
    struct dirent *directory_reader = readdir(directory);
    struct stat status;

    char actual_path[PATH_MAX];

    while (directory_reader != NULL) {
        strcpy(actual_path, directory_path);
        strcat(actual_path, "/");
        strcat(actual_path, directory_reader->d_name);

        lstat(actual_path, &status);

        if (strcmp(directory_reader->d_name, ".") == 0 || strcmp(directory_reader->d_name, "..") == 0) {
            directory_reader = readdir(directory);
            continue;
        } else {

        }
    }
}
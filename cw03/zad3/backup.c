#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "error.h"

static char* backup_mem_buffer;
static long backup_mem_buffer_size;

char* get_backup_name(const char* filename) {
    time_t rawtime;
    struct tm* timeinfo;
    char* buffer = malloc(4096);

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char date[80];
    strftime(date, sizeof(date), "%Y-%m-%d_%H-%M-%S", timeinfo);

    char* filename_copy = malloc(4096);
    strcpy(filename_copy, filename);
    sprintf(buffer, "archive/%s_%s", basename(filename_copy), date);
    return buffer;
}

void backup_exec(const char* filename) {
    char* backup_name = get_backup_name(filename);

    pid_t child_pid = fork();

    if (child_pid == -1) {
        perr("unable to fork");
    } else if (child_pid > 0) {
        int status;
        waitpid(child_pid, &status, 0);
    } else {
        execlp("cp", "cp", filename, backup_name, NULL);
        _exit(EXIT_FAILURE);
    }

    printf("PID: %d (EXEC)\t%s -> %s\n", getpid(), filename, backup_name);

    free(backup_name);
}

void backup_mem_read(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perr("unable to open %s", filename);
    }

    fseek(file, 0, SEEK_END);
    backup_mem_buffer_size = ftell(file);
    backup_mem_buffer = realloc(backup_mem_buffer, backup_mem_buffer_size);
    rewind(file);
    fread(backup_mem_buffer, backup_mem_buffer_size, 1, file);
    fclose(file);
}

void backup_mem(const char* filename) {
    char* backup_name = get_backup_name(filename);

    FILE* file = fopen(backup_name, "wb");
    if (file == NULL) {
        perr("unable to open %s", backup_name);
    }

    fwrite(backup_mem_buffer, backup_mem_buffer_size, 1, file);
    fclose(file);

    printf("PID: %d (MEM) \t%s -> %s\n", getpid(), filename, backup_name);

    free(backup_name);
    backup_mem_read(filename);
}

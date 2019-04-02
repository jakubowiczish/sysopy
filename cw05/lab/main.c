#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

const int SIZE = 16384;

int main(int argc, char **argv) {
    if (argc != 4) {
        printf("WRONG NUMBER OF ARGUMENTS!\n");
        return -1;
    }


    char *first_command = argv[1];
    char *second_command = argv[2];
    char *out_filename = argv[3];

    FILE *pipe_first = popen(first_command, "r");
    if (pipe_first == NULL) {
        printf("PROBLEM WITH POPEN IN MAIN!\n");
        return -1;
    }

    char buffer[SIZE];

    if (fread(buffer, sizeof(char), SIZE, pipe_first) <= 0) {
        printf("PROBLEM WITH FREAD\n");
        return -1;
    }

    pclose(pipe_first);

    int fd = open(out_filename, O_CREAT | O_WRONLY, 0644);

    if (fd == -1) {
        printf("PROBLEM WITH OPENING OUT FILE\n");
        return -1;
    }

    dup2(fd, 1);
    dup2(fd, 2);

    FILE *pipe_second = popen(second_command, "w");
    if (pipe_second == NULL) {
        printf("PROBLEM WITH POPEN IN MAIN\n");
        return -1;
    }

    if (fwrite(buffer, sizeof(char), SIZE, pipe_second) <= 0) {
        printf("PROBLEM WITH FWRITE\n");
        return -1;
    }

    pclose(pipe_second);
    close(fd);


    return 0;
}
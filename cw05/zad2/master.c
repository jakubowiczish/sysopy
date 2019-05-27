#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utils.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        print_error_message_and_exit("WRONG NUMBER OF ARGUMENTS! TRY AGAIN");
    }

    char* named_pipe_path = argv[1];

    if (mkfifo(named_pipe_path, 0777)) {
        print_error_message("PROBLEM WITH CREATING FIFO");
    }

    int named_pipe_file = open(named_pipe_path, O_RDONLY);

    if (named_pipe_file < 0) {
        print_error_message("PROBLEM WITH READING FILE");
    }

    char line[BUFFER_SIZE];
    while (1) {
        while (read(named_pipe_file, line, BUFFER_SIZE) > 0) {
            printf("%s\n", line);
        }
    }

    close(named_pipe_file);
}
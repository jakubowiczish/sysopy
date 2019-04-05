#include <sys/stat.h>
#include "utils.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        print_error_message_and_exit("WRONG NUMBER OF ARGUMENTS! TRY AGAIN");
    }

    char *named_pipe_path = argv[1];

    if (mkfifo(named_pipe_path, 0777)) {
        print_error_message("PROBLEM WITH CREATING FIFO");
    }

    FILE *named_pipe_file = fopen(named_pipe_path, "r");

    if (named_pipe_file == NULL) {
        print_error_message("PROBLEM WITH READING FILE");
    }

    char line[BUFFER_SIZE];
    while (1) {
        while (fgets(line, BUFFER_SIZE, named_pipe_file) != NULL) {
            printf("%s\n", line);
        }
    }

    fclose(named_pipe_file);
}
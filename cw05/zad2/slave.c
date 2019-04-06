#include "utils.h"
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        print_error_message_and_exit("WRONG NUMBER OF ARGUMENTS! TRY AGAIN");
    }

    srand((unsigned) time(NULL));

    char *end_ptr = NULL;
    char *named_pipe_path = argv[1];
    long number_of_savings = strtol(argv[2], &end_ptr, 10);

    int named_pipe_file = open(named_pipe_path, O_WRONLY);

    printf("THE PID OF CURRENT SLAVE: %d\n", getpid());

    char date_buffer[BUFFER_SIZE];

    int WHOLE_SIZE = 2 * BUFFER_SIZE;
    char whole_buffer[WHOLE_SIZE];

    for (int i = 0; i < number_of_savings; ++i) {

        FILE *date_file = popen("date", "r");
        if (fread(date_buffer, sizeof(char), BUFFER_SIZE, date_file) < 1) {
            print_error_message("SOME PROBLEMS WITH GETTING DATA WITH POPEN");
        }
        fclose(date_file);

        snprintf(whole_buffer, WHOLE_SIZE, "CURRENT PID OF SLAVE: %d, DATE: %s\n", getpid(), date_buffer);
        if (write(named_pipe_file, whole_buffer, strlen(whole_buffer)) < 1) {
            print_error_message("SOME PROBLEMS WITH WRITING TO PIPE FILE");
        }

        sleep(2 + rand() % 3);
    }

    close(named_pipe_file);
}
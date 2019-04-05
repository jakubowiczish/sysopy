#include "utils.h"
#include <unistd.h>
#include <time.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        print_error_message_and_exit("WRONG NUMBER OF ARGUMENTS! TRY AGAIN");
    }

    srand((unsigned)time(NULL));

    char *end_ptr = NULL;

    char *named_pipe_path = argv[1];
    long number_of_savings = strtol(argv[2], &end_ptr, 10);

    FILE *named_pipe_file = fopen(named_pipe_path, "r");

    printf("THE PID OF CURRENT SLAVE: %d\n", getpid());

    char date_buffer[BUFFER_SIZE];
    char whole_buffer[2 * BUFFER_SIZE];

    for (int i = 0; i < number_of_savings; ++i) {
        FILE *date_file = popen("date", "r");
        if (fread(date_buffer, sizeof(char), BUFFER_SIZE, date_file) != BUFFER_SIZE) {
            print_error_message("SOME PROBLEMS WITH GETTING DATA WITH POPEN");
        }
        fclose(date_file);

        snprintf(whole_buffer, 2 * BUFFER_SIZE, "CURRENT PID OF SLAVE: %d, DATE: %s\n", getpid(), date_buffer);
        if (fwrite(whole_buffer, sizeof(char), 2 * BUFFER_SIZE, named_pipe_file) != BUFFER_SIZE) {
            print_error_message("SOME PROBLEMS WITH WRITING TO PIPE FILE");
        }

        sleep(2 + rand() % 3);
    }
}
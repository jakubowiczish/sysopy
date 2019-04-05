#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_PROGRAMS_AMOUNT 16
#define MAX_ARGUMENTS_AMOUNT 8
#define MAX_LINE_LENGTH 128

void print_error_message(char *error_message) {
    printf("%s\n", error_message);
}

void print_error_message_and_exit(char *error_message) {
    printf("%s\n", error_message);
    exit(EXIT_FAILURE);
}

void execute_command(char *command_line) {
    int command_counter = 1;
    for (int i = 0; i < strlen(command_line); ++i) {
        if (command_line[i] == '|') {
            ++command_counter;
        }
    }

    if (command_counter > MAX_PROGRAMS_AMOUNT) {
        print_error_message_and_exit("MAXIMUM PROGRAMS AMOUNT EXCEEDED");
    }

    char *commands[MAX_PROGRAMS_AMOUNT][MAX_ARGUMENTS_AMOUNT];
    for (int i = 0; i < MAX_PROGRAMS_AMOUNT; ++i) {
        for (int j = 0; j < MAX_ARGUMENTS_AMOUNT; ++j) {
            commands[i][j] = NULL;
        }
    }

    char *command_end;
    char *command = strtok_r(command_line, "|", &command_end);
    int i = 0;
    while (command != NULL) {
        int j = 0;
        char *argument_end;
        char *argument = strtok_r(command, " \t", &argument_end);

        while (argument != NULL) {
            commands[i][j] = argument;
            argument = strtok_r(NULL, " \t", &argument_end);
            ++j;
        }
        command = strtok_r(NULL, "|", &command_end);
        ++i;
    }

    int pipe_counter = command_counter - 1;
    int pipes[MAX_PROGRAMS_AMOUNT][2];

    for (int k = 0; k < pipe_counter; ++k) {
        if (pipe(pipes[i]) < 0) {
            print_error_message("PROBLEM WITH CREATING PIPE");
        }
    }

    for (int i = 0; i < command_counter; ++i) {
        pid_t pid = fork();

        if (pid < 0) {
            print_error_message("PROBLEM WITH FORK");
        } else if (pid == 0) {

            if (i != command_counter - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            if (i != 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }

            for (int j = 0; j < pipe_counter; ++j) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            execvp(commands[i][0], commands[i]);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < pipe_counter; ++i) {
        close(pipes[i][1]);
    }

    for (int i = 0; i < command_counter; ++i) {
        wait(0);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        print_error_message_and_exit("WRONG NUMBER OF ARGUMENTS!");
    }

    char *file_with_commands = argv[1];
    FILE *file = fopen(file_with_commands, "r");

    if (file == NULL) {
        print_error_message_and_exit("PROBLEM WITH OPENING THE FILE WITH COMMANDS!");
    }

    char *line = malloc(sizeof(char) * MAX_LINE_LENGTH);
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        printf("CURRENTLY EXECUTED COMMAND: %s\n", line);
        execute_command(line);
    }

    free(line);
    fclose(file);
}
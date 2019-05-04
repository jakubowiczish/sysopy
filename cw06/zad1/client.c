#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <libgen.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#include "helper.h"


/* ##################################################################################################################
 * ################################################################################################################## */

void mem_cpy_command_args(struct string_array *commandArgs, int index);

void stop_command(struct string_array *command_args);

void list_command(struct string_array *command_args);

void friends_command(struct string_array *command_args);

void add_command(struct string_array *command_args);

void del_command(struct string_array *command_args);

void echo_command(struct string_array *command_args);

void _2all_command(struct string_array *command_args);

void _2friends_command(struct string_array *command_args);

void _2one_command(struct string_array *command_args);

/* ##################################################################################################################
 * ################################################################################################################## */

void sender();

void catcher();

void execute_file(struct string_array *command_args);

void end_client();

void handle_SIGINT(int signal_num);


/* ##################################################################################################################
 * ################################################################################################################## */


int is_client_running = 1;

int server_queue, client_queue;

int command_length = 512;

struct msg client_request, server_response;

int user_id = -1;

pid_t pid;

/* ##################################################################################################################
 * ################################################################################################################## */

int main(int argc, char *argv[]) {

    char *homedir = getenv("HOME");

    key_t server_queue_key;

    if ((client_queue = msgget(IPC_PRIVATE, 0660)) == -1) {
        print_sth_and_exit("ERROR while initializing client queue", 1);
    }

    if ((server_queue_key = ftok(homedir, PROJ_ID)) == -1) {
        print_sth_and_exit("ERROR while getting key using ftok", 2);
    }

    if ((server_queue = msgget(server_queue_key, 0)) == -1) {
        print_sth_and_exit("ERROR while initializing access to the server queue!", 3);
    }


    client_request.msg_type = INIT;
    client_request.msg_text.id = -1;

    sprintf(client_request.msg_text.buf, "%d", client_queue);


    // send message to server

    if (msgsnd(server_queue, &client_request, sizeof(struct msg_text), 0) == -1) {
        print_sth_and_exit("ERROR while sending INIT response to the server", 4);
    } else {
        print_some_info("INIT action sent to the server");
    }

    // read response from server

    if (msgrcv(client_queue, &server_response, sizeof(struct msg_text), 0, 0) == -1) {
        print_sth_and_exit("ERROR while reading INIT response from the server", 5);

    } else {
        char message_received_buffer[BUFFER_SIZE];

        sprintf(message_received_buffer,
                "Message RECEIVED\n\ttype: %ld, id: %d, message: %s \n",
                server_response.msg_type,
                server_response.msg_text.id,
                server_response.msg_text.buf
        );

        print_some_info(message_received_buffer);

        user_id = server_response.msg_type;
    }


    pid = fork();

    if (pid == 0) {
        struct sigaction action;

        action.sa_handler = NULL;
        action.sa_flags = 0;

        sigaction(SIGINT, &action, NULL);

        sender();

    } else if (pid > 0) {

        struct sigaction action;

        action.sa_handler = handle_SIGINT;

        sigemptyset(&action.sa_mask);
        sigaddset(&action.sa_mask, SIGINT);

        action.sa_flags = 0;

        sigaction(SIGINT, &action, NULL);

        catcher();

    } else {
        print_error("ERROR while creating fork");
    }


    end_client();

    return 0;
}


void send_message() {
    if (msgsnd(server_queue, &client_request, sizeof(struct msg_text), 0) == -1) {
        print_some_info("ERROR while sending request to the server");

    } else {
        char send_message_buffer[BUFFER_SIZE];

        sprintf(send_message_buffer,
                "Message SENT to the server %s\n",
                type_to_string(client_request.msg_type)
        );

        print_some_info(send_message_buffer);
    }
}


int execute_command(struct string_array *command_args) {

    client_request.msg_text.id = user_id;

    if (strcmp(command_args->data[0], "STOP") == 0) {

        client_request.msg_type = STOP;

        stop_command(command_args);

    } else if (strcmp(command_args->data[0], "LIST") == 0) {

        if (command_args->size != 1) return 0;

        client_request.msg_type = LIST;

        list_command(command_args);

    } else if (strcmp(command_args->data[0], "FRIENDS") == 0) {

        if (command_args->size != 1 && command_args->size != 2)
            return 0;

        client_request.msg_type = FRIENDS;

        friends_command(command_args);

    } else if (strcmp(command_args->data[0], "ADD") == 0) {

        if (command_args->size != 2)
            return 0;

        client_request.msg_type = ADD;

        add_command(command_args);

    } else if (strcmp(command_args->data[0], "DEL") == 0) {

        if (command_args->size != 2) return 0;
        client_request.msg_type = DEL;
        del_command(command_args);

    } else if (strcmp(command_args->data[0], "ECHO") == 0) {

        if (command_args->size != 2) return 0;

        client_request.msg_type = ECHO;

        echo_command(command_args);

    } else if (strcmp(command_args->data[0], "2ALL") == 0) {

        if (command_args->size != 2) return 0;

        client_request.msg_type = _2ALL;

        _2all_command(command_args);

    } else if (strcmp(command_args->data[0], "2FRIENDS") == 0) {

        if (command_args->size != 2)
            return 0;

        client_request.msg_type = _2FRIENDS;

        _2friends_command(command_args);

    } else if (strcmp(command_args->data[0], "2ONE") == 0) {

        if (command_args->size != 3)
            return 0;

        client_request.msg_type = _2ONE;

        _2one_command(command_args);

    } else {
        return 0;
    }

    send_message();

    return 1;
}


void mem_cpy_command_args(struct string_array *commandArgs, int index) {
    memcpy(client_request.msg_text.buf, commandArgs->data[index], strlen(commandArgs->data[index]));
    client_request.msg_text.buf[strlen(commandArgs->data[index])] = '\0';
}


void stop_command(struct string_array *command_args) {
    is_client_running = 0;

    mem_cpy_command_args(command_args, 0);
}


void list_command(struct string_array *command_args) {
    mem_cpy_command_args(command_args, 0);
}


void friends_command(struct string_array *command_args) {
    if (command_args->size == 2) {
        mem_cpy_command_args(command_args, 1);

    } else {
        client_request.msg_text.buf[0] = '\0';
    }
}


void add_command(struct string_array *command_args) {
    mem_cpy_command_args(command_args, 1);
}


void del_command(struct string_array *command_args) {
    mem_cpy_command_args(command_args, 1);
}


void echo_command(struct string_array *command_args) {
    mem_cpy_command_args(command_args, 1);
}


void _2all_command(struct string_array *command_args) {
    mem_cpy_command_args(command_args, 1);
}


void _2friends_command(struct string_array *command_args) {
    mem_cpy_command_args(command_args, 1);
}


void _2one_command(struct string_array *command_args) {
    mem_cpy_command_args(command_args, 2);

    client_request.msg_text.additional_arg = strtol(command_args->data[1], NULL, 0);
}


void sender() {
    //receive data from stdin

    while (is_client_running) {
        char *command = calloc(command_length, sizeof(char));

        printf(">> ");

        int simple_char = 0;

        char *ptr = command;
        while ((simple_char = fgetc(stdin)) != '\n') {

            if (command + command_length > ptr) {
                (*ptr++) = (char) simple_char;

            } else {
                command[command_length - 1] = '\0';
            }
        }

        struct string_array command_args = process_file(command, strlen(command), ' ');

        if (command_args.size < 1) {
            print_some_info("Client: Command not recognized");
        }

        if (strcmp(command_args.data[0], "READ") == 0) {
            execute_file(&command_args);
        } else {
            if (!execute_command(&command_args)) {
                print_some_info("Command not recognized!");
            }
        }

        free(command_args.data);
        free(command);

        sleep(1);
    }
}

void catcher() {
    for (EVER) {
        // read an incoming message, with priority order

        if (msgrcv(client_queue, &server_response, sizeof(struct msg_text), -200, 0) == -1) {
            print_error("ERROR while reading input data");

        } else {

            char message_received_buffer[BUFFER_SIZE];

            sprintf(message_received_buffer,
                    "Client, message received\n\t type: %ld, id: %d, message: %s \n",
                    server_response.msg_type,
                    server_response.msg_text.id,
                    server_response.msg_text.buf
            );

            print_some_info(message_received_buffer);
        }

        if (server_response.msg_type == SHUTDOWN) {
            break;
        }
    }
}


void execute_file(struct string_array *command_args) {
    if (command_args->size != 2) {
        print_error("Client: invalid arguments!");
    }


    FILE *file = fopen(command_args->data[1], "r");

    if (file == NULL) {
        char error_buffer[BUFFER_SIZE];

        sprintf(error_buffer,
                "Client: ERROR while opening file: %s \n",
                command_args->data[1]
        );

        print_error(error_buffer);
    }


    long file_size = 0;

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    char *fileContent = calloc(file_size, sizeof(char));

    fread(fileContent, file_size, sizeof(char), file);

    struct string_array command = process_file(fileContent, file_size, '\n');

    for (int i = 0; i < command.size; i++) {
        struct string_array command_args_inside = process_file(command.data[i], strlen(command.data[i]), ' ');

        execute_command(&command_args_inside);

        free(command_args_inside.data);
    }

    free(fileContent);
    free(command.data);
}


void end_client() {
    client_request.msg_text.id = user_id;
    client_request.msg_type = STOP;

    sprintf(client_request.msg_text.buf, "STOP from client %d", user_id);

    if (msgsnd(server_queue, &client_request, sizeof(struct msg_text), 0) == -1) {
        print_error("Client: ERROR while sending data about STOP");
    } else {
        print_error("Client: Sending information about STOP");
    }


    if (msgctl(client_queue, IPC_RMID, NULL) == -1) {
        print_sth_and_exit("ERROR while closing client queue", 1);
    }

    kill(pid, 9);

    print_some_info("Closing client!");

    exit(0);
}


void handle_SIGINT(int signal_num) {
    print_some_info("Received signal SIGINT");

    end_client();
}
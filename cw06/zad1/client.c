#include "helper.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void catcher();

void sender();

void handle_SIGINT();

void end_client();

int execute_command(struct string_array *command_args);

void send_message();

void stop_command(struct string_array *command_args);

void list_command(struct string_array *command_args);

void friends_command(struct string_array *pArray);

void add_command(struct string_array *command_args);

void del_command(struct string_array *command_args);

void echo_command(struct string_array *command_args);

void _2all_command(struct string_array *command_args);

void _2friends_command(struct string_array *command_args);

void _2one_command(struct string_array *command_args);


int server_queue, client_queue;

struct msg client_request, server_response;


int user_id = -1;

pid_t pid;

int mode = 0;

int is_client_running = 1;


int main(int argc, char **argv) {

    char *home_directory = getenv("HOME");

    key_t server_queue_key;

    if ((client_queue = msgget(IPC_PRIVATE, QUEUE_PERMISSIONS) == -1)) {
        print_sth_and_exit("ERROR occurred while initializing client queue", 1);
    }

    if ((server_queue_key = ftok(home_directory, PROJ_ID)) == -1) {
        print_sth_and_exit("ERROR occurred while getting key using ftok method", 2);
    }

    if ((server_queue = msgget(server_queue_key, 0)) == -1) {
        print_sth_and_exit("ERROR occurred while initializing server queue", 3);
    }


    client_request.msg_type = INIT;
    client_request.msg_text.id = -1;

    sprintf(client_request.msg_text.buf, "%d", client_queue);


    // sending message to the server

    if (msgsnd(server_queue, &client_request, sizeof(struct msg_text), 0) == -1) {
        print_sth_and_exit("ERROR occurred while sending INIT to the server", 4);
    } else {
        print_some_info("INIT sent to the server");
    }


    // reading response from the server

    if (msgrcv(client_queue, &server_response, sizeof(struct msg_text), 0, 0) == -1) {
        print_sth_and_exit("ERROR occurred while reading reading INIT response from the server", 5);
    } else {
        char message_received_buffer[512];

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

        mode = 1;

        sender();
    } else if (pid > 0) {
        struct sigaction action;
        action.sa_handler = handle_SIGINT;

        sigemptyset(&action.sa_mask);
        sigaddset(&action.sa_mask, SIGINT);

        action.sa_flags = 0;

        sigaction(SIGINT, &action, NULL);

        mode = 2;

        catcher();
    } else {
        print_error("ERROR while creating a fork");
    }


    end_client();

    return 0;
}


void send_message() {
    if (msgsnd(server_queue, &client_request, sizeof(struct msg_text), 0) == -1) {
        print_error("ERROR while sending a request to the server");
    } else {
        char send_message_buffer[512];

        sprintf(send_message_buffer,
                "Message SENT to the server %s\n",
                get_type_as_string(client_request.msg_type)
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

        if (command_args->size != 1) {
            return 0;
        }

        client_request.msg_type = LIST;
        list_command(command_args);

    } else if (strcmp(command_args->data[0], "FRIENDS") == 0) {
        if (command_args->size != 1 && command_args->size != 2) {
            return 0;
        }

        client_request.msg_type = FRIENDS;
        friends_command(command_args);

    } else if (strcmp(command_args->data[0], "ADD") == 0) {
        if (command_args->size != 2) {
            return 0;
        }

        client_request.msg_type = ADD;
        add_command(command_args);

    } else if (strcmp(command_args->data[0], "DEL") == 0) {
        if (command_args->size != 2) {
            return 0;
        }

        client_request.msg_type = DEL;
        del_command(command_args);

    } else if (strcmp(command_args->data[0], "ECHO") == 0) {
        if (command_args->size != 2) {
            return 0;
        }

        client_request.msg_type = ECHO;
        echo_command(command_args);

    } else if (strcmp(command_args->data[0], "2ALL") == 0) {
        if (command_args->size != 2) {
            return 0;
        }


        client_request.msg_type = _2ALL;
        _2all_command(command_args);

    } else if (strcmp(command_args->data[0], "2FRIENDS") == 0) {
        if (command_args->size != 2) {
            return 0;
        }

        client_request.msg_type = _2FRIENDS;
        _2friends_command(command_args);
    } else if (strcmp(command_args->data[0], "2ONE") == 0) {
        if (command_args->size != 2) {
            return 0;
        }

        client_request.msg_type = _2ONE;
        _2one_command(command_args);
    } else {
        return 0;
    }

    send_message();

    return 1;
}


void memcpy_command_args(struct string_array *command_args, int index) {
    memcpy(client_request.msg_text.buf, command_args->data[index], strlen(command_args->data[index]));
    client_request.msg_text.buf[strlen(command_args->data[index])] = '\0';
}


void stop_command(struct string_array *command_args) {
    is_client_running = 0;
    memcpy_command_args(command_args, 0);
}


void list_command(struct string_array *command_args) {
    memcpy_command_args(command_args, 0);
}


void friends_command(struct string_array *command_args) {
    if (command_args->size == 2) {
        memcpy_command_args(command_args, 1);
    } else {
        client_request.msg_text.buf[0] = '\0';
    }
}


void add_command(struct string_array *command_args) {
    memcpy_command_args(command_args, 1);
}


void del_command(struct string_array *command_args) {
    memcpy_command_args(command_args, 1);
}


void echo_command(struct string_array *command_args) {
    memcpy_command_args(command_args, 1);
}


void _2all_command(struct string_array *command_args) {
    memcpy_command_args(command_args, 1);
}


void _2friends_command(struct string_array *command_args) {
    memcpy_command_args(command_args, 1);
}


void _2one_command(struct string_array *command_args) {
    memcpy_command_args(command_args, 2);
    client_request.msg_text.additional_arg = strtol(command_args->data[1], NULL, 0);
}


void catcher() {
    for (EVER) {
        if ((msgrcv(client_queue, &server_response, sizeof(struct msg_text), -200, 0)) == -1) {
            print_error("ERROR while reading the data");
        } else {
            char message_received_buffer[512];

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


void sender() {

}


void execute_commands_from_file(struct string_array *command_args) {
    if (command_args->size != 2) {
        print_error("Invalid number of arguments (client)");
    }

    FILE *file = fopen(command_args->data[1], "r");

    if (file == NULL) {
        char error_buffer[512];

        sprintf(error_buffer,
                "ERROR while opening file: %s\n",
                command_args->data[1]
        );

        print_error(error_buffer);
    }

    long file_size = 0;

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);


    char *file_content = calloc(file_size, sizeof(char));
    fread(file_content, file_size, sizeof(char), file);

    struct string_array command = process_file(file_content, file_size, '\n');

    for (int i = 0; i < command.size; ++i) {
        struct string_array command_arguments = process_file(command.data[i], strlen(command.data[i]), ' ');

        execute_command(&command_arguments);

        free(command_arguments.data);
    }

    free(file_content);
    free(command.data);
}


void end_client() {
    client_request.msg_text.id = user_id;
    client_request.msg_type = STOP;

    sprintf(
            client_request.msg_text.buf,
            "STOP from client: %d",
            user_id
    );

    if ((msgsnd(server_queue, &client_request, sizeof(struct msg_text), 0) == -1)) {
        print_error("ERROR while sending data about STOP communicate");
    } else {
        print_some_info("Information about STOP has been sent");
    }


    if (msgctl(client_queue, IPC_RMID, NULL) == -1) {
        print_sth_and_exit("ERROR while closing client queue", -1);
    }

    kill(pid, 9);

    print_some_info("Closing client");

    exit(0);
}


void handle_SIGINT(int signal_num) {
    print_some_info("RECEIVED signal SIGINT");
    end_client();
}

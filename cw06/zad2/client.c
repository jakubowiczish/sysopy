#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <memory.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "helper.h"

/* ##################################################################################################################
 * ##################################################################################################################
 */

void sender();

void catcher();

void stop_command(struct string_array* command_args);

void list_command(struct string_array* command_args);

void friends_command(struct string_array* command_args);

void add_command(struct string_array* command_args);

void del_command(struct string_array* command_args);

void echo_command(struct string_array* command_args);

void _2all_command(struct string_array* command_args);

void _2friends_command(struct string_array* command_args);

void _2one_command(struct string_array* command_args);

void execute_file(struct string_array* command_args);

void end_client();

void handle_SIGINT(int signal_num);

void parse_server_response();

void parse_client_request();

/* ##################################################################################################################
 * ##################################################################################################################
 */

int is_client_running = 1;

int server_queue, client_queue;

int command_length = 256;

struct msg client_request, server_response;

int user_id = -1;

pid_t pid;

char client_queue_name[64];

mqd_t server_queue, client_queue;

char input[MAX_MESSAGE_SIZE];

char output[MAX_MESSAGE_SIZE];

/* ##################################################################################################################
 * ##################################################################################################################
 */

int main(int argc, char* argv[]) {
    sprintf(client_queue_name, "/client-%d", getpid());

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MESSAGE_SIZE;
    attr.mq_curmsgs = 0;

    if ((client_queue = mq_open(client_queue_name, O_RDONLY | O_CREAT,
                                QUEUE_PERMISSIONS, &attr)) == -1) {
        print_sth_and_exit("ERROR while initializing client queue", 1);
    }

    if ((server_queue = mq_open(SERVER_QUEUE_NAME, O_WRONLY)) == -1) {
        print_sth_and_exit("ERROR while getting access to the server queue", 2);
    }

    client_request.msg_type = INIT;
    client_request.msg_text.id = -1;

    sprintf(client_request.msg_text.buf, "%s", client_queue_name);

    parse_client_request();

    if (mq_send(server_queue, output, strlen(output) + 1, INIT) == -1) {
        print_sth_and_exit("ERROR while sending INIT response to the server",
                           4);
    }

    if (mq_receive(client_queue, input, MAX_MESSAGE_SIZE, NULL) == -1) {
        print_sth_and_exit("ERROR while reading INIT response from the server",
                           5);

    } else {
        parse_server_response();

        char message_received_buffer[BUFFER_SIZE];

        sprintf(message_received_buffer,
                "Message RECEIVED\n\ttype: %ld, id: %d, message: %s \n",
                server_response.msg_type, server_response.msg_text.id,
                server_response.msg_text.buf);

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
        print_error("Client: ERROR while creating fork");
    }

    end_client();

    return 0;
}

/* ##################################################################################################################
 * ##################################################################################################################
 */

void send_message() {
    parse_client_request();

    if (mq_send(server_queue, output, strlen(output) + 1,
                client_request.msg_type) == -1) {
        print_error("Client: ERROR while sending request to server");

    } else {
        char send_message_buffer[BUFFER_SIZE];

        sprintf(send_message_buffer, "Message SENT to the server %s\n",
                type_to_string(client_request.msg_type));

        print_some_info(send_message_buffer);
    }
}

int execute_command(struct string_array* command_args) {
    client_request.msg_text.id = user_id;

    if (strcmp(command_args->data[0], "STOP") == 0) {
        client_request.msg_type = STOP;

        stop_command(command_args);

    } else if (strcmp(command_args->data[0], "LIST") == 0) {
        if (command_args->size != 1)
            return 0;

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
        if (command_args->size != 2)
            return 0;
        client_request.msg_type = DEL;
        del_command(command_args);

    } else if (strcmp(command_args->data[0], "ECHO") == 0) {
        if (command_args->size != 2)
            return 0;

        client_request.msg_type = ECHO;

        echo_command(command_args);

    } else if (strcmp(command_args->data[0], "2ALL") == 0) {
        if (command_args->size != 2)
            return 0;

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

/* ##################################################################################################################
 * ##################################################################################################################
 */

void mem_cpy_command_args(struct string_array* commandArgs, int index) {
    memcpy(client_request.msg_text.buf, commandArgs->data[index],
           strlen(commandArgs->data[index]));
    client_request.msg_text.buf[strlen(commandArgs->data[index])] = '\0';
}

void stop_command(struct string_array* command_args) {
    is_client_running = 0;
    mem_cpy_command_args(command_args, 0);
}

void list_command(struct string_array* command_args) {
    mem_cpy_command_args(command_args, 0);
}

void friends_command(struct string_array* command_args) {
    if (command_args->size == 2) {
        mem_cpy_command_args(command_args, 1);

    } else {
        client_request.msg_text.buf[0] = '\0';
    }
}

void add_command(struct string_array* command_args) {
    mem_cpy_command_args(command_args, 1);
}

void del_command(struct string_array* command_args) {
    mem_cpy_command_args(command_args, 1);
}

void echo_command(struct string_array* command_args) {
    mem_cpy_command_args(command_args, 1);
}

void _2all_command(struct string_array* command_args) {
    mem_cpy_command_args(command_args, 1);
}

void _2friends_command(struct string_array* command_args) {
    mem_cpy_command_args(command_args, 1);
}

void _2one_command(struct string_array* command_args) {
    mem_cpy_command_args(command_args, 2);

    client_request.msg_text.additional_arg =
        strtol(command_args->data[1], NULL, 0);
}

/* ##################################################################################################################
 * ##################################################################################################################
 */

void sender() {
    while (is_client_running) {
        char* command = calloc(command_length, sizeof(char));
        printf(">> ");

        int single_char = 0;

        char* ptr = command;

        while ((single_char = fgetc(stdin)) != '\n') {
            if (command + command_length > ptr) {
                (*ptr++) = (char)single_char;
            } else {
                command[command_length - 1] = '\0';
            }
        }

        struct string_array command_args =
            process_file(command, strlen(command), ' ');

        if (command_args.size < 1) {
            print_some_info("Command not recognized!");
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
    while (1) {
        if (mq_receive(client_queue, input, MAX_MESSAGE_SIZE, NULL) == -1) {
            print_error("ERROR while reading init from server");

            continue;

        } else {
            parse_server_response();

            char message_received_buffer[BUFFER_SIZE];

            sprintf(message_received_buffer,
                    "Client, message received\n\t type: %ld, id: %d, message: "
                    "%s \n",
                    server_response.msg_type, server_response.msg_text.id,
                    server_response.msg_text.buf);

            print_some_info(message_received_buffer);
        }

        if (server_response.msg_type == SHUTDOWN) {
            break;
        }
    }
}

/* ##################################################################################################################
 * ##################################################################################################################
 */

void execute_file(struct string_array* command_args) {
    if (command_args->size != 2) {
        print_error("Client: invalid arguments!");
    }

    FILE* file = fopen(command_args->data[1], "r");

    if (file == NULL) {
        char error_buffer[BUFFER_SIZE];

        sprintf(error_buffer, "Client: ERROR while opening file: %s \n",
                command_args->data[1]);

        print_error(error_buffer);
    }

    long fileSize = 0;
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    char* fileContent = calloc(fileSize, sizeof(char));

    fread(fileContent, fileSize, sizeof(char), file);

    struct string_array command = process_file(fileContent, fileSize, '\n');

    for (int i = 0; i < command.size; i++) {
        struct string_array command_args_inside =
            process_file(command.data[i], strlen(command.data[i]), ' ');
        execute_command(&command_args_inside);
        free(command_args_inside.data);
    }

    free(fileContent);
    free(command.data);
}

/* ##################################################################################################################
 * ##################################################################################################################
 */

void end_client() {
    client_request.msg_text.id = user_id;
    client_request.msg_type = STOP;

    sprintf(client_request.msg_text.buf, "STOP from client %d", user_id);

    parse_client_request();

    if (mq_send(server_queue, output, strlen(output) + 1,
                client_request.msg_type) == -1) {
        print_error("Client: ERROR while sending data about STOP");

    } else {
        print_error("Client: Sending information about STOP");
    }

    if (mq_close(server_queue) == -1) {
        print_sth_and_exit("ERROR while closing server queue", 1);
    }

    if (mq_close(client_queue) == -1) {
        print_sth_and_exit("ERROR while closing client queue", 1);
    }

    if (mq_unlink(client_queue_name) == -1) {
        print_sth_and_exit("ERROR while removing client queue", 1);
    }

    kill(pid, 9);

    print_some_info("Closing client");

    exit(0);
}

void handle_SIGINT(int signal_num) {
    print_some_info("Received signal SIGINT");

    end_client();
}

void parse_server_response() {
    struct string_array msg_array = process_file(input, strlen(input), '|');

    if (msg_array.size > 0)
        server_response.msg_type = strtol(msg_array.data[0], NULL, 0);
    else
        server_response.msg_type = ERROR;

    if (msg_array.size > 1)
        server_response.msg_text.id = strtol(msg_array.data[1], NULL, 0);
    else
        server_response.msg_text.id = ERROR;

    if (msg_array.size > 2)
        server_response.msg_text.additional_arg =
            strtol(msg_array.data[2], NULL, 0);
    else
        server_response.msg_text.additional_arg = ERROR;

    if (msg_array.size > 3) {
        int len = strlen(msg_array.data[3]);

        memcpy(server_response.msg_text.buf, msg_array.data[3], len);

        server_response.msg_text.buf[len] = '\0';

    } else {
        server_response.msg_text.buf[0] = '\0';
    }

    free(msg_array.data);
}

void parse_client_request() {
    sprintf(output, "%ld|%d|%d|%s", client_request.msg_type,
            client_request.msg_text.id, client_request.msg_text.additional_arg,
            client_request.msg_text.buf);
}
#include "helper.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>

void catcher();

void sender();

void handle_SIGINT();

void end_client();

void send_message();


int server_queue, client_queue;

struct msg client_request, server_response;


int USR_ID = -1;

pid_t pid;

int mode = 0;

int main(int argc, char **argv) {

//    print_error_and_exit("oootakooo");

    char *home_directory = getenv("HOME");

    key_t server_queue_key;

    if ((client_queue = msgget(IPC_PRIVATE, 0660) == -1)) {
        print_error_and_exit("ERROR occurred while initializing client queue");
    }

    if ((server_queue_key = ftok(home_directory, PROJ_ID)) == -1) {
        print_error_and_exit("ERROR occurred while getting key using ftok method");
    }

    if ((server_queue = msgget(server_queue_key, 0)) == -1) {
        print_error_and_exit("ERROR occurred while initializing server queue");
    }


    client_request.msg_type = INIT;
    client_request.msg_text.id = -1;

    sprintf(client_request.msg_text.buf, "%d", client_queue);


    // sending message to the server

    if (msgsnd(server_queue, &client_request, sizeof(struct msg_text), 0) == -1) {
        print_error_and_exit("ERROR occurred while sending INIT to the server");
    } else {
        print_some_info("INIT sent to the server");
    }


    // reading response from the server

    if (msgrcv(client_queue, &server_response, sizeof(struct msg_text), 0, 0) == -1) {
        print_error_and_exit("ERROR occurred while reading reading INIT response from the server");
    } else {
        char message_received_buffer[512];

        sprintf(message_received_buffer,
                "Message RECEIVED\n\ttype: %ld, id: %d, message: %s \n",
                server_response.msg_type,
                server_response.msg_text.id,
                server_response.msg_text.buf
        );

        print_some_info(message_received_buffer);

        USR_ID = server_response.msg_type;
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


void catcher() {

}

void sender() {

}

void handle_SIGINT(int signal_num) {
    print_some_info("RECEIVED signal SIGINT");
    end_client();
}


void end_client() {

}










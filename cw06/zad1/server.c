#include <sys/ipc.h>
#include <sys/msg.h>
#include "helper.h"
#include <signal.h>
#include <asm/errno.h>
#include <errno.h>

void initialize_arrays();

void handle_SIGINT(int signal_num);

void send_shutdown_to_all_clients();

int user_exists(int usr_id);

void execute_command(struct msg *input, struct msg *output);


int clients_queue_id[MAX_CLIENTS_NUMBER];

int groups_size[MAX_CLIENTS_NUMBER];

int friends_groups[MAX_CLIENTS_NUMBER][MAX_GROUP_SIZE];

int active_users_counter = 0;
int actual_usr_id = 0;

int next_client_id = 0;

int is_server_running = 1;

int main(int argc, char **argv) {
    char *home_directory = getenv("HOME");

    initialize_arrays();

    key_t msg_queue_key;
    int qid;

    struct msg message, response;

    if ((msg_queue_key = ftok(home_directory, PROJ_ID)) == (key_t) -1) {
        print_sth_and_exit("ERROR while getting key using ftok (server)", 9);
    }

    if ((qid = msgget(msg_queue_key, IPC_CREAT | QUEUE_PERMISSIONS))) {
        print_sth_and_exit("ERROR while creating a queue (server)", 10);
    }

    struct sigaction action;

    action.sa_handler = handle_SIGINT;
    action.sa_flags = 0;

    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGINT);
    sigaction(SIGINT, &action, NULL);

    print_some_info("SERVER IS RUNNING!");

    while (is_server_running) {

        if (msgrcv(qid, &message, sizeof(struct msg_text), -100, 0)) {
            if (EINTR != errno) {
                print_error("ERROR while reading input!");
            }

            continue;
        } else {
            char message_received_buffer[512];

            sprintf(message_received_buffer,
                    "Message RECEIVED\n\ttype: %s, id: %d, message: %s \n",
                    get_type_as_string(message.msg_type),
                    message.msg_text.id,
                    message.msg_text.buf
            );

            print_some_info(message_received_buffer);

            execute_command(&message, &response);

        }

        if (message.msg_type == STOP) continue;

        send_message(actual_usr_id, &response, 0);
    }

    if (msgctl(qid, IPC_RMID, NULL) == -1) {
        print_error("ERROR while closing client queue");
    }

    print_some_info("CLOSING SERVER");

    return 0;
}


void execute_command(struct msg *input, struct msg *output) {
    actual_usr_id = input->msg_text.id - SHIFTID;


    if (!user_exists(actual_usr_id) && input->msg_type != INIT) {
        sprintf(output->msg_text.buf, "USER DOES NOT EXIST");

        output->msg_text.id = SERVER_ID;
        output->msg_type = ERROR;

        return;
    }

    switch (input->msg_type) {
        case STOP: {

        }
    }


}

void handle_SIGINT(int signal_num) {
    print_some_info("RECEIVED signal SIGINT");

    if (active_users_counter > 0) {
        send_shutdown_to_all_clients();
    } else {
        print_sth_and_exit("CLOSING SERVER", 0);
    }
}


void send_shutdown_to_all_clients() {
    struct msg message;
    message.msg_text.id = SERVER_ID;
    message.msg_type = SHUTDOWN;

    sprintf(message.msg_text.buf, "STOP");

    for (int i = 0; i < next_client_id; ++i) {
        if (!user_exists(i)) continue;

        if (msgsnd(clients_queue_id[i], &message, sizeof(struct msg_text), 0) == -1) {
            print_sth_and_exit("ERROR while sending shutdown", 7);
        } else {
            print_some_info("SHUTDOWN signal sent");
        }
    }
}


int user_exists(int usr_id) {
    if (usr_id > next_client_id) {
        return 0;
    }

    return clients_queue_id[usr_id] != -1;
}


void initialize_arrays() {
    for (int i = 0; i < MAX_CLIENTS_NUMBER; ++i) {
        clients_queue_id[i] = -1;
        groups_size[i] = -1;
    }
}

void _2one_command(struct msg *input, struct msg *output) {
    prepare_message(input, output);

    if (!user_exists(input->msg_text.additional_arg)) {
        sprintf(output->msg_text.buf, "2ONE - Destination user (%d) DOES NOT exist", input->msg_text.additional_arg);
    } else {
        send_message(input->msg_text.additional_arg, output, 1);
        sprintf(output->msg_text.buf, "2ONE - message SENT");
    }
}
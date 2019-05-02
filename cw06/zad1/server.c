#include <sys/ipc.h>
#include <sys/msg.h>
#include "helper.h"
#include <signal.h>

void initialize_arrays();

void handle_SIGINT(int signal_num);

void send_shutdown_to_all_clients();

int user_exists(int usr_id);

int clients_queue_id[MAX_CLIENTS_NUMBER];

int groups_size[MAX_CLIENTS_NUMBER];

int friends_groups[MAX_CLIENTS_NUMBER][MAX_GROUP_SIZE];

int active_users_counter = 0;
int active_user_id = 0;

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
}

void initialize_arrays() {
    for (int i = 0; i < MAX_CLIENTS_NUMBER; ++i) {
        clients_queue_id[i] = -1;
        groups_size[i] = -1;
    }
}
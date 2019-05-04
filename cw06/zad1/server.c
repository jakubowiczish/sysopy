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
#include <errno.h>

#include "helper.h"

int next_client_id = 0;

int clients_queue_id_arr[MAX_CLIENTS_NUMBER];

int groups_size_arr[MAX_CLIENTS_NUMBER];
int friends_groups_arr[MAX_CLIENTS_NUMBER][MAX_GROUP_SIZE];

int actual_usr_id = 0;

int active_users_counter = 0;

int is_server_running = 1;

void execute_command(struct msg *input, struct msg *output);

int user_exists(int user_id);

void send_shutdown_to_all_clients();

int send_message(int id, struct msg *output, int type);

void stop_command(struct msg *input, struct msg *output);

void list_command(struct msg *input, struct msg *output);

void friends_command(struct msg *input, struct msg *output);

void add_command(struct msg *input, struct msg *output);

void del_command(struct msg *input, struct msg *output);

void init_command(struct msg *input, struct msg *output);

void echo_command(struct msg *input, struct msg *output);

void _2all_command(struct msg *input, struct msg *output);

void _2friends_command(struct msg *input, struct msg *output);

void _2one_command(struct msg *input, struct msg *output);

void handle_SIGINT(int signal_num);


int main(int argc, char *argv[], char *env[]) {

    char *homedir = getenv("HOME");

    for (int i = 0; i < MAX_CLIENTS_NUMBER; i++) {
        clients_queue_id_arr[i] = -1;
        groups_size_arr[i] = -1;
    }


    key_t msg_queue_key;

    int qid;

    struct msg message, response;

    if ((msg_queue_key = ftok(homedir, PROJ_ID)) == (key_t) -1) {
        print_sth_and_exit("Server: ERROR while getting key using ftok", 15);
    }


    if ((qid = msgget(msg_queue_key, IPC_CREAT | QUEUE_PERMISSIONS)) == -1) {
        print_sth_and_exit("Server: ERROR while creating a queue!", 16);
    }

    struct sigaction action;

    action.sa_handler = handle_SIGINT;

    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGINT);

    action.sa_flags = 0;

    sigaction(SIGINT, &action, NULL);


    print_some_info("Server is running!");

    while (is_server_running) {

        if (msgrcv(qid, &message, sizeof(struct msg_text), -100, 0) == -1) {
            if (EINTR != errno) { //ignore interrupting by SIGINT
                print_error("Server: ERROR while reading input data!");
            }

            continue;

        } else {

            printf("\033[1;32mServer:\033[0m message received:\n\ttype: %s, id: %d, message: %s \n",
                   type_to_string(message.msg_type),
                   message.msg_text.id,
                   message.msg_text.buf
            );

            execute_command(&message, &response);
        }

        if (message.msg_type == STOP) continue;

        send_message(actual_usr_id, &response, 0);

    }

    //end working of server
    if (msgctl(qid, IPC_RMID, NULL) == -1) {
        printf("\033[1;32mServer:\033[0m Error while closing client queue.\n");
    }

    printf("\033[1;32mServer:\033[0m Server close.\n");

    return 0;
}

void execute_command(struct msg *input, struct msg *output) {

    actual_usr_id = input->msg_text.id - SHIFT_ID;

    if (!user_exists(actual_usr_id) && input->msg_type != INIT) {

        sprintf(output->msg_text.buf, "User DOES NOT exist.");

        output->msg_text.id = SERVER_ID;
        output->msg_type = ERROR;

        return;
    }

    switch (input->msg_type) {
        case STOP: {
            stop_command(input, output);
        }
            break;

        case LIST: {
            list_command(input, output);
        }
            break;

        case FRIENDS: {
            friends_command(input, output);
        }
            break;

        case ADD: {
            add_command(input, output);
        }
            break;

        case DEL: {
            del_command(input, output);
        }
            break;

        case INIT: {
            init_command(input, output);
        }
            break;

        case ECHO: {
            echo_command(input, output);
        }
            break;

        case _2ALL: {
            _2all_command(input, output);
        }
            break;

        case _2FRIENDS: {
            _2friends_command(input, output);
        }
            break;

        case _2ONE: {
            _2one_command(input, output);
        }
            break;

        default:
            break;
    }

    output->msg_text.id = SERVER_ID;

    output->msg_type = actual_usr_id + SHIFT_ID;
}


int user_exists(int user_id) {
    if (user_id > next_client_id) return 0;

    return clients_queue_id_arr[user_id] != -1;
}


int user_queue_exists(int userQueueId) {
    for (int i = 0; i < next_client_id; i++) {
        if (clients_queue_id_arr[i] == userQueueId)
            return 1;
    }

    return 0;
}


void prepare_message(struct msg *input, struct msg *output) {
    __time_t now;
    time(&now);

    char date[21];
    strftime(date, 21, "%d-%m-%Y_%H:%M:%S", localtime(&now));

    sprintf(
            output->msg_text.buf,
            "from %d, %s - %s",
            actual_usr_id,
            input->msg_text.buf,
            date
    );
}


int send_message(int id, struct msg *output, int type) {

    if (user_exists(id) && msgsnd(clients_queue_id_arr[id], output, sizeof(struct msg_text), 0) == -1) {
        print_error("Server: ERROR while sending data!");
        return -1;

    } else {

        if (type == 0) {

            printf("\033[1;32mServer:\033[0m send response to client - %d.\n\n", actual_usr_id);
        } else {
            printf("\033[1;32mServer:\033[0m send message to client %d from %d.\n", id, actual_usr_id);
        }
        return 1;
    }
}

int get_free_index() {
    if (next_client_id < MAX_CLIENTS_NUMBER) {

        ++next_client_id;
        return next_client_id - 1;

    } else {
        for (int i = 0; i < MAX_CLIENTS_NUMBER; i++) {
            if (clients_queue_id_arr[i] == -1)
                return i;
        }
    }


    return -1;
}

int existInGroup(int actual_user_id, int friends_id) {

    for (int i = 0; i < groups_size_arr[actual_user_id]; i++) {
        if (friends_groups_arr[actual_user_id][i] == friends_id)
            return 1;
    }

    return 0;
}


void send_shutdown_to_all_clients() {

    struct msg msg;

    msg.msg_text.id = SERVER_ID;
    msg.msg_type = SHUTDOWN;

    sprintf(msg.msg_text.buf, "STOP");

    for (int i = 0; i < next_client_id; i++) {

        if (!user_exists(i))
            continue;

        if (msgsnd(clients_queue_id_arr[i], &msg, sizeof(struct msg_text), 0) == -1) {
            print_error("Server: ERROR while sending data!");

        } else {
            print_some_info("Server: SHUTDOWN signal sent");
        }
    }
}


void handle_SIGINT(int signal_num) {

    print_some_info("Server: Signal SIGINT received");

    if (active_users_counter > 0) {
        send_shutdown_to_all_clients();
    } else {
        print_sth_and_exit("Closing server!", 0);
    }
}


/*
 * Zgłoszenie zakończenia pracy klienta.
 * Klient wysyła ten komunikat, kiedy kończy pracę, aby serwer mógł usunąć z listy jego kolejkę.
 * Następnie kończy pracę, usuwając swoją kolejkę.
 * Komunikat ten wysyłany jest również, gdy po stronie klienta zostanie wysłany sygnał SIGINT.
 */
void stop_command(struct msg *input, struct msg *output) {
    //remove queue

    clients_queue_id_arr[actual_usr_id] = -1;

    // remove from any group

    for (int i = 0; i < next_client_id; i++) {

        for (int j = 0; j < groups_size_arr[i]; j++) {

            if (friends_groups_arr[i][j] == actual_usr_id) {
                for (int k = j; k < groups_size_arr[i] - 1; k++) {
                    friends_groups_arr[i][k] = friends_groups_arr[i][k + 1];
                }

                --groups_size_arr[i];
                break;
            }
        }
    }

    sprintf(output->msg_text.buf, "STOP - user is removed.");

    --active_users_counter;

    if (active_users_counter == 0) {
        is_server_running = 0;
    }
}


// Zlecenie wypisania listy wszystkich aktywnych klientów

void list_command(struct msg *input, struct msg *output) {
    char item[10];

    int offset = 0;

    for (int i = 0; i < next_client_id; i++) {

        if (i != next_client_id - 1)
            sprintf(item, "%d, ", i);

        else
            sprintf(item, "%d", i);

        memcpy(output->msg_text.buf + offset, item, strlen(item) * sizeof(char));

        offset += strlen(item);
    }

    *(output->msg_text.buf + offset) = '\0';
}


/*
 * Klient wysyła do serwera listę klientów, z którymi chce się grupowo komunikować.
 * Serwer przechowuje tylko ostatnią listę.
 * Kolejne wysłanie komunikatu FRIENDS nadpisuje poprzednią listę.
 * Wysłanie samego FRIENDS czyści listę.
 */
void friends_command(struct msg *input, struct msg *output) {
    //explode ids list

    struct string_array id_list = process_file(input->msg_text.buf, strlen(input->msg_text.buf), ',');

    if (id_list.size > MAX_GROUP_SIZE) {
        sprintf(output->msg_text.buf, "FRIENDS - To many users.");
        return;
    }


    int groups_size = 0;

    for (int i = 0; i < id_list.size; i++) {

        friends_groups_arr[actual_usr_id][groups_size] = strtol(id_list.data[i], NULL, 0);

        if (
                friends_groups_arr[actual_usr_id][groups_size] >= 0
                && user_exists(friends_groups_arr[actual_usr_id][groups_size])
                && !existInGroup(actual_usr_id, friends_groups_arr[actual_usr_id][groups_size])
                && friends_groups_arr[actual_usr_id][groups_size] != actual_usr_id
                ) {

            ++groups_size;
        }
    }

    groups_size_arr[actual_usr_id] = groups_size;


    if (groups_size == 0) {
        sprintf(output->msg_text.buf, "FRIENDS - Empty group.");
    } else {
        sprintf(output->msg_text.buf, "FRIENDS - Create group with size %d.", groups_size_arr[actual_usr_id]);
    }

    free(id_list.data);
}


/*
 * Grupę można modyfikować, wysyłając do serwera komunikaty:
 * ADD lista_id_klientów oraz DEL lista_id_klientów.
 * Wysłanie ADD lista_id_klientów po uprzednim wyczyszczeniu listy jest analogiczne z wysłaniem FRIENDS lista_id_klientów.
 * Próba wysłania ADD i DEL bez argumentów powinna zostać obsłużona po stronie klienta.
 */
void add_command(struct msg *input, struct msg *output) {

    struct string_array id_list = process_file(input->msg_text.buf, strlen(input->msg_text.buf), ',');

    int groups_size = groups_size_arr[actual_usr_id];

    if (id_list.size + groups_size > MAX_GROUP_SIZE) {
        sprintf(output->msg_text.buf, "Too many users!");
        return;
    }

    for (int i = 0; i < id_list.size; i++) {

        friends_groups_arr[actual_usr_id][groups_size] = strtol(id_list.data[i], NULL, 0);

        if (
                friends_groups_arr[actual_usr_id][groups_size] >= 0
                && user_exists(friends_groups_arr[actual_usr_id][groups_size])
                && !existInGroup(actual_usr_id, friends_groups_arr[actual_usr_id][groups_size])
                && friends_groups_arr[actual_usr_id][groups_size] != actual_usr_id
                ) {

            ++groups_size;
        }
    }

    groups_size_arr[actual_usr_id] = groups_size;

    if (groups_size == 0) {
        sprintf(output->msg_text.buf, "ADD - Empty group.");
    } else {
        sprintf(output->msg_text.buf, "ADD - Create group with size %d.", groups_size_arr[actual_usr_id]);
    }

    free(id_list.data);
}



void del_command(struct msg *input, struct msg *output) {

    int user_id = actual_usr_id;

    struct string_array id_list = process_file(input->msg_text.buf, strlen(input->msg_text.buf), ',');


    int user_group_id = 0;

    int group_index = groups_size_arr[user_id];

    for (int i = 0; i < id_list.size; i++) {

        user_group_id = strtol(id_list.data[i], NULL, 0);

        for (int j = 0; j < group_index; j++) {

            if (friends_groups_arr[user_id][j] == user_group_id) {

                for (int k = j; k < group_index - 1; k++) {
                    friends_groups_arr[user_id][k] = friends_groups_arr[user_id][k + 1];
                }

                --group_index;
                break;
            }
        }
    }
    groups_size_arr[user_id] = group_index;

    if (group_index == 0) {
        sprintf(output->msg_text.buf, "DEL - Empty group.");
    } else {
        sprintf(
                output->msg_text.buf,
                "DEL - New group size is %d.",
                groups_size_arr[user_id]
        );
    }

    free(id_list.data);
}


void init_command(struct msg *input, struct msg *output) {
    int user_queue_id = strtol(input->msg_text.buf, NULL, 0);

    if (user_queue_exists(user_queue_id)) {
        sprintf(output->msg_text.buf, "INIT - User already exist.");
        output->msg_type = -1;

    } else {

        int index = get_free_index();

        if (index != -1) {

            sprintf(output->msg_text.buf, "%d", index);

            clients_queue_id_arr[index] = user_queue_id;

            actual_usr_id = index;

            output->msg_type = index;

            active_users_counter++;

        } else {
            sprintf(output->msg_text.buf, "INIT - Too many clients.");

            output->msg_type = -1;
        }
    }
}


/*
 *   Klient wysyła ciąg znaków.
 *   Serwer odsyła ten sam ciąg z powrotem, dodatkowo podając datę jego otrzymania.
 *   Klient po odebraniu wysyła go na standardowe wyjście.
 */
void echo_command(struct msg *input, struct msg *output) {
    __time_t now;
    time(&now);

    char date[21];

    strftime(date, 21, "%d-%m-%Y_%H:%M:%S", localtime(&now));

    sprintf(output->msg_text.buf, "%s - %s", input->msg_text.buf, date);
}


/*
 * Zlecenie wysłania komunikatu do wszystkich pozostałych klientów.
 * Klient wysyła ciąg znaków.
 * Serwer wysyła ten ciąg wraz z identyfikatorem klienta-nadawcy oraz aktualną datą do wszystkich pozostałych klientów.
 */
void _2all_command(struct msg *input, struct msg *output) {
    prepare_message(input, output);

    int sent_counter = 0;

    for (int i = 0; i < next_client_id; i++) {
        if (i != actual_usr_id && send_message(i, output, 1))
            ++sent_counter;
    }

    sprintf(
            output->msg_text.buf,
            "2ALL - Send %d/%d message",
            sent_counter,
            next_client_id - 1
    );
}


/*
 * Zlecenie wysłania komunikatu do zdefiniowanej wcześniej grupy klientów.
 * Klient wysyła ciąg znaków.
 * Serwer wysyła ten ciąg wraz z identyfikatorem klienta-nadawcy oraz aktualną datą do zdefiniowanej wcześniej grupy klientów.
 */
void _2friends_command(struct msg *input, struct msg *output) {
    prepare_message(input, output);

    int sent_counter = 0;

    for (int i = 0; i < groups_size_arr[actual_usr_id]; i++) {

        if (friends_groups_arr[actual_usr_id][i] != actual_usr_id
            && send_message(friends_groups_arr[actual_usr_id][i], output, 1))

            ++sent_counter;
    }

    sprintf(
            output->msg_text.buf,
            "2FRIENDS Send %d/%d message",
            sent_counter,
            groups_size_arr[actual_usr_id]
    );
}


/*
 * Zlecenie wysłania komunikatu do konkretnego klienta.
 * Klient wysyła ciąg znaków podając jako adresata konkretnego klienta o identyfikatorze z listy aktywnych klientów.
 * Serwer wysyła ten ciąg wraz z identyfikatorem klienta-nadawcy oraz aktualną datą do wskazanego klienta.
 */
void _2one_command(struct msg *input, struct msg *output) {
    prepare_message(input, output);

    if (!user_exists(input->msg_text.additional_arg)) {
        sprintf(
                output->msg_text.buf,
                "2ONE - Destination user (%d) DOES NOT exist",
                input->msg_text.additional_arg
        );

    } else {
        send_message(input->msg_text.additional_arg, output, 1);

        sprintf(output->msg_text.buf, "2ONE - OK, send message");
    }
}
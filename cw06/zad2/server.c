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

void execute_command();

int user_exists(int userId);

void send_shutdown_to_all_clients();

int send_message(int id, int type);

void stop_command();

void list_command();

void friends_command();

void add_command();

void del_command();

void init_command();

void echo_command();

void _2all_command();

void _2friends_command();

void _2one_command();

void handle_SIGINT(int signal_num);

void parse_input_msg();

/* ##################################################################################################################
 * ##################################################################################################################
 */

int next_client_id = 0;

mqd_t clients_queue_id_arr[MAX_CLIENTS_NUMBER];

int groups_size_arr[MAX_CLIENTS_NUMBER];

int friends_groups_size_arr[MAX_CLIENTS_NUMBER][MAX_GROUP_SIZE];

char input[MAX_MESSAGE_SIZE];

char output[MAX_MESSAGE_SIZE];

int actual_usr_id = 0;

int active_user_counter = 0;

int is_server_running = 1;

struct msg input_msg;

struct msg output_msg;

/* ##################################################################################################################
 * ##################################################################################################################
 */

int main(int argc, char* argv[]) {
    mqd_t server_queue;

    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MESSAGE_SIZE;
    attr.mq_curmsgs = 0;

    for (int i = 0; i < MAX_CLIENTS_NUMBER; i++) {
        clients_queue_id_arr[i] = -1;
        groups_size_arr[i] = -1;
    }

    if ((server_queue = mq_open(SERVER_QUEUE_NAME, O_RDONLY | O_CREAT,
                                QUEUE_PERMISSIONS, &attr)) == -1) {
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
        if (mq_receive(server_queue, input, MAX_MESSAGE_SIZE, NULL) == -1) {
            if (EINTR != errno) {
                print_error("ERROR while reading input data.");
            }

            continue;

        } else {
            parse_input_msg();

            char buffer[BUFFER_SIZE];

            sprintf(buffer, "Server: message received: %s \n \t %s",
                    type_to_string(input_msg.msg_type), input);

            print_some_info(buffer);

            execute_command();
        }

        if (input_msg.msg_type == STOP)
            continue;

        send_message(actual_usr_id, 0);
    }

    if (mq_close(server_queue) == -1) {
        print_error("ERROR while closing server queue");
    }

    if (mq_unlink(SERVER_QUEUE_NAME) == -1) {
        print_error("ERROR while removing server queue");
    }

    print_some_info("Closing server!");

    return 0;
}

void execute_command() {
    actual_usr_id = input_msg.msg_text.id;

    if (!user_exists(actual_usr_id) && input_msg.msg_type != INIT) {
        sprintf(output_msg.msg_text.buf, "User not exist.");

        output_msg.msg_text.id = SERVER_ID;
        output_msg.msg_type = ERROR;

        return;
    }

    switch (input_msg.msg_type) {
        case STOP: {
            stop_command();
        } break;

        case LIST: {
            list_command();
        } break;

        case FRIENDS: {
            friends_command();
        } break;

        case ADD: {
            add_command();
        } break;

        case DEL: {
            del_command();
        } break;

        case INIT: {
            init_command();
        } break;

        case ECHO: {
            echo_command();
        } break;
        case _2ALL: {
            _2all_command();
        } break;

        case _2FRIENDS: {
            _2friends_command();
        } break;

        case _2ONE: {
            _2one_command();
        } break;

        default:
            break;
    }

    output_msg.msg_text.id = SERVER_ID;

    output_msg.msg_type = actual_usr_id;
}

int user_exists(int userId) {
    if (userId > next_client_id)
        return 0;

    return clients_queue_id_arr[userId] != -1;
}

int user_queue_exists(int user_queue_id) {
    for (int i = 0; i < next_client_id; i++) {
        if (clients_queue_id_arr[i] == user_queue_id)
            return 1;
    }

    return 0;
}

void prepare_message() {
    char tmp[1024];
    __time_t now;
    time(&now);

    char date[21];

    strftime(date, 21, "%d-%m-%Y_%H:%M:%S", localtime(&now));

    sprintf(tmp, "from %d, %s - %s", actual_usr_id, input_msg.msg_text.buf,
            date);

    memcpy(output_msg.msg_text.buf, tmp, 256);

    int len = strlen(tmp);

    if (len > 255)
        len = 255;

    output_msg.msg_text.buf[len] = '\0';
}

int send_message(int id, int type) {
    sprintf(output, "%ld|%d|%d|%s", output_msg.msg_type, output_msg.msg_text.id,
            output_msg.msg_text.additional_arg, output_msg.msg_text.buf);

    if (user_exists(id) &&
        mq_send(clients_queue_id_arr[id], output, strlen(output) + 1,
                output_msg.msg_type) == -1) {
        print_error("ERROR while sending data");
        return -1;

    } else {
        if (type == 0) {
            char buffer[BUFFER_SIZE];

            sprintf(buffer, "Server: response sent to client - %d \n\n",
                    actual_usr_id);

            print_some_info(buffer);

        } else if (type == 1) {
            char buffer[BUFFER_SIZE];

            sprintf(buffer, "Server: message sent to client - %d \n\n",
                    actual_usr_id);

            print_some_info(buffer);

        } else if (type == 2) {
            print_some_info("Server: SHUTDOWN signal sent");
        }

        return 1;
    }
}

int get_free_index() {
    if (next_client_id < MAX_CLIENTS_NUMBER) {
        next_client_id++;

        return next_client_id - 1;

    } else {
        for (int i = 0; i < MAX_CLIENTS_NUMBER; i++) {
            if (clients_queue_id_arr[i] == -1)
                return i;
        }
    }

    return -1;
}

int exists_in_group(int actualUserId, int friendsId) {
    for (int i = 0; i < groups_size_arr[actualUserId]; i++) {
        if (friends_groups_size_arr[actualUserId][i] == friendsId)
            return 1;
    }

    return 0;
}

void send_shutdown_to_all_clients() {
    output_msg.msg_text.id = SERVER_ID;

    output_msg.msg_type = SHUTDOWN;

    sprintf(output_msg.msg_text.buf, "STOP");

    for (int i = 0; i < next_client_id; i++) {
        if (!user_exists(i))
            continue;

        send_message(i, 2);

        if (mq_close(clients_queue_id_arr[i]) == -1) {
            print_error("ERROR while closing server queue");
        }
    }
}

void handle_SIGINT(int signal_num) {
    print_some_info("RECEIVED signal SIGINT");

    if (active_user_counter > 0) {
        send_shutdown_to_all_clients();
    } else {
        print_sth_and_exit("Closing server!", 0);
    }
}

//------------------------------------------------------------------------------------------

void stop_command() {
    clients_queue_id_arr[actual_usr_id] = -1;

    for (int i = 0; i < next_client_id; i++) {
        for (int j = 0; j < groups_size_arr[i]; j++) {
            if (friends_groups_size_arr[i][j] == actual_usr_id) {
                for (int k = j; k < groups_size_arr[i] - 1; k++) {
                    friends_groups_size_arr[i][k] =
                        friends_groups_size_arr[i][k + 1];
                }

                groups_size_arr[i]--;
                break;
            }
        }
    }

    sprintf(output_msg.msg_text.buf, "STOP - User is removed.");

    active_user_counter--;

    if (active_user_counter == 0) {
        is_server_running = 0;
    }
}

void list_command() {
    char item[10];
    int offset = 0;

    for (int i = 0; i < next_client_id; i++) {
        if (i != next_client_id - 1)
            sprintf(item, "%d, ", i);

        else
            sprintf(item, "%d", i);

        memcpy(output_msg.msg_text.buf + offset, item,
               strlen(item) * sizeof(char));

        offset += strlen(item);
    }

    *(output_msg.msg_text.buf + offset) = '\0';
}

void friends_command() {
    struct string_array id_list = process_file(
        input_msg.msg_text.buf, strlen(input_msg.msg_text.buf), ',');

    if (id_list.size > MAX_GROUP_SIZE) {
        sprintf(output_msg.msg_text.buf, "FRIENDS - To many users.");
        return;
    }

    int groups_size = 0;

    for (int i = 0; i < id_list.size; i++) {
        friends_groups_size_arr[actual_usr_id][groups_size] =
            strtol(id_list.data[i], NULL, 0);

        if (friends_groups_size_arr[actual_usr_id][groups_size] >= 0 &&
            user_exists(friends_groups_size_arr[actual_usr_id][groups_size]) &&
            !exists_in_group(
                actual_usr_id,
                friends_groups_size_arr[actual_usr_id][groups_size]) &&
            friends_groups_size_arr[actual_usr_id][groups_size] !=
                actual_usr_id) {
            groups_size++;
        }
    }

    groups_size_arr[actual_usr_id] = groups_size;

    if (groups_size == 0) {
        sprintf(output_msg.msg_text.buf, "FRIENDS - Empty group.");

    } else {
        sprintf(output_msg.msg_text.buf, "FRIENDS - Create group with size %d.",
                groups_size_arr[actual_usr_id]);
    }

    free(id_list.data);
}

void add_command() {
    // explode ids list
    struct string_array id_list = process_file(
        input_msg.msg_text.buf, strlen(input_msg.msg_text.buf), ',');

    int group_size = groups_size_arr[actual_usr_id];

    if (id_list.size + group_size > MAX_GROUP_SIZE) {
        sprintf(output_msg.msg_text.buf, "To many users.");
        return;
    }

    for (int i = 0; i < id_list.size; i++) {
        friends_groups_size_arr[actual_usr_id][group_size] =
            strtol(id_list.data[i], NULL, 0);

        if (friends_groups_size_arr[actual_usr_id][group_size] >= 0 &&
            user_exists(friends_groups_size_arr[actual_usr_id][group_size]) &&
            !exists_in_group(
                actual_usr_id,
                friends_groups_size_arr[actual_usr_id][group_size]) &&
            friends_groups_size_arr[actual_usr_id][group_size] !=
                actual_usr_id) {
            group_size++;
        }
    }

    groups_size_arr[actual_usr_id] = group_size;

    if (group_size == 0) {
        sprintf(output_msg.msg_text.buf, "ADD - Empty group.");
    } else {
        sprintf(output_msg.msg_text.buf, "ADD - Create group with size %d.",
                groups_size_arr[actual_usr_id]);
    }

    free(id_list.data);
}

void del_command() {
    int usr_id = actual_usr_id;

    struct string_array id_list = process_file(
        input_msg.msg_text.buf, strlen(input_msg.msg_text.buf), ',');

    int user_group_id = 0;

    int group_index = groups_size_arr[usr_id];

    for (int i = 0; i < id_list.size; i++) {
        user_group_id = strtol(id_list.data[i], NULL, 0);

        for (int j = 0; j < group_index; j++) {
            if (friends_groups_size_arr[usr_id][j] == user_group_id) {
                for (int k = j; k < group_index - 1; k++) {
                    friends_groups_size_arr[usr_id][k] =
                        friends_groups_size_arr[usr_id][k + 1];
                }

                group_index--;
                break;
            }
        }
    }

    groups_size_arr[usr_id] = group_index;

    if (group_index == 0) {
        sprintf(output_msg.msg_text.buf, "DEL - Empty group.");
    } else {
        sprintf(output_msg.msg_text.buf, "DEL - New group size is %d.",
                groups_size_arr[usr_id]);
    }

    free(id_list.data);
}

void init_command() {
    mqd_t clientQueueId;

    if ((clientQueueId = mq_open(input_msg.msg_text.buf, O_WRONLY)) == 1) {
        sprintf(output_msg.msg_text.buf, "INIT - Unable to open client queue.");
        output_msg.msg_type = -1;

    } else {
        if (user_queue_exists(clientQueueId)) {
            sprintf(output_msg.msg_text.buf, "INIT - User already exist.");
            output_msg.msg_type = -1;

        } else {
            int index = get_free_index();

            if (index != -1) {
                sprintf(output_msg.msg_text.buf, "%d", index);

                clients_queue_id_arr[index] = clientQueueId;

                actual_usr_id = index;

                output_msg.msg_type = index;

                active_user_counter++;

            } else {
                sprintf(output_msg.msg_text.buf, "INIT - Too many clients.");

                output_msg.msg_type = -1;
            }
        }
    }
}

void echo_command() {
    char tmp[1024];
    __time_t now;
    time(&now);

    char date[21];

    strftime(date, 21, "%d-%m-%Y_%H:%M:%S", localtime(&now));
    sprintf(tmp, "%s - %s", input_msg.msg_text.buf, date);

    memcpy(output_msg.msg_text.buf, tmp, 256);

    output_msg.msg_text.buf[255] = '\0';
}

void _2all_command() {
    prepare_message();

    int sendCounter = 0;
    for (int i = 0; i < next_client_id; i++) {
        if (i != actual_usr_id && send_message(i, 1))
            sendCounter++;
    }

    sprintf(output_msg.msg_text.buf, "2ALL - Send %d/%d message", sendCounter,
            next_client_id - 1);
}

void _2friends_command() {
    prepare_message();

    int sent_counter = 0;

    for (int i = 0; i < groups_size_arr[actual_usr_id]; i++) {
        if (friends_groups_size_arr[actual_usr_id][i] != actual_usr_id &&
            send_message(friends_groups_size_arr[actual_usr_id][i], 1))

            sent_counter++;
    }

    sprintf(output_msg.msg_text.buf, "2FRIENDS Send %d/%d message",
            sent_counter, groups_size_arr[actual_usr_id]);
}

void _2one_command() {
    prepare_message();

    if (!user_exists(input_msg.msg_text.additional_arg)) {
        sprintf(output_msg.msg_text.buf,
                "2ONE - Destination user (%d) not exist",
                input_msg.msg_text.additional_arg);

    } else {
        send_message(input_msg.msg_text.additional_arg, 1);

        sprintf(output_msg.msg_text.buf, "2ONE - OK, send message");
    }
}

//----------------------------------------------------------------------

void parse_input_msg() {
    struct string_array msg_array = process_file(input, strlen(input), '|');

    if (msg_array.size > 0)
        input_msg.msg_type = strtol(msg_array.data[0], NULL, 0);
    else
        input_msg.msg_type = ERROR;

    if (msg_array.size > 1)
        input_msg.msg_text.id = strtol(msg_array.data[1], NULL, 0);
    else
        input_msg.msg_text.id = ERROR;

    if (msg_array.size > 2)
        input_msg.msg_text.additional_arg = strtol(msg_array.data[2], NULL, 0);
    else
        input_msg.msg_text.additional_arg = ERROR;

    if (msg_array.size > 3) {
        int len = strlen(msg_array.data[3]);

        memcpy(input_msg.msg_text.buf, msg_array.data[3], len);

        input_msg.msg_text.buf[len] = '\0';
    } else {
        input_msg.msg_text.buf[0] = '\0';
    }

    free(msg_array.data);
}

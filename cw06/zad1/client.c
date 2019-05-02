#include "helper.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


int server_queue, client_queue;

struct msg client_request, server_response;


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


    if (msgsnd(server_queue, &client_request, sizeof(struct msg_text), 0) == -1) {
        print_error_and_exit("ERROR occurred while sending INIT to the server");
    } else {
        print_some_info("INIT sent to the server");
    }





    return 0;
}










#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include "colors.h"
#include "errors.h"
#include "queue.h"
#include "utils.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define MODE_TCP 1
#define MODE_UDP 0

#define MSGTYPE_PING 1
#define MSGTYPE_RESULT 2
#define MSGTYPE_REQUEST 3
#define MSGTYPE_UDP_CONNECT 4

#ifdef UDP
int mode = MODE_UDP;
#else
int mode = MODE_TCP;
#endif

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_new_element_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t socket_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t socket_send_mutex = PTHREAD_MUTEX_INITIALIZER;

int sock1;

typedef struct request_t {
    int id;
    char* content;
    int size;
} request_t;

typedef struct word_node_t word_node_t;
typedef struct word_node_t {
    char* text;
    int text_size;
    int text_count;
    word_node_t* next;

} word_node_t;

typedef struct word_counter_t {
    word_node_t* first;
    int size;

} word_counter_t;

queue_t requests;

struct sockaddr* udp_sockaddr;
socklen_t udp_sockaddr_size;

void unix_tcp_start(char* path) {
    struct sockaddr_un name;

    sock1 = socket(PF_UNIX, SOCK_STREAM, 0);
    if (sock1 < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    name.sun_family = AF_UNIX;
    strcpy(name.sun_path, path);

    if (connect(sock1, (struct sockaddr*)&name, sizeof(name))) {
        perror("Connection failed");
    }
}

void unix_udp_start(char* path) {
    struct sockaddr_un* server = malloc(sizeof(struct sockaddr_un));
    server->sun_family = AF_UNIX;
    strcpy(server->sun_path, path);

    udp_sockaddr_size = sizeof(struct sockaddr_un);
    udp_sockaddr = (struct sockaddr*)server;

    struct sockaddr_un client;

    sock1 = socket(PF_UNIX, SOCK_DGRAM, 0);
    if (sock1 < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    char buffer[128];
    snprintf(buffer, 128, "/tmp/socket_%d", getpid());

    client.sun_family = AF_UNIX;
    strcpy(client.sun_path, buffer);

    unlink(buffer);
    if (bind(sock1, (struct sockaddr*)&client, sizeof(client)) < 0) {
        perror("bind2");
        exit(EXIT_FAILURE);
    }
}

void inet_tcp_start(char* address, int port) {
    struct sockaddr_in name;

    sock1 = socket(PF_INET, SOCK_STREAM, 0);
    if (sock1 < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    inet_pton(AF_INET, address, &(name.sin_addr));
    if (connect(sock1, (struct sockaddr*)&name, sizeof(name))) {
        perror("Connection failed");
    }
}

void inet_udp_start(char* address, int port) {
    struct sockaddr_in* name = malloc(sizeof(struct sockaddr_in));

    sock1 = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock1 < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    name->sin_family = AF_INET;
    name->sin_port = htons(port);
    inet_pton(AF_INET, address, &(name->sin_addr));

    udp_sockaddr_size = sizeof(struct sockaddr_in);
    udp_sockaddr = (struct sockaddr*)name;

    if (connect(sock1, (struct sockaddr*)name, sizeof(struct sockaddr_in))) {
        perror("Connection failed");
    }
}

int init_word_counter(word_counter_t* counter) {
    counter->first = malloc(sizeof(word_node_t));
    counter->first->next = NULL;
    counter->size = 0;
    return 0;
}

int add_to_word_counter(word_counter_t* counter, char* word, int size) {
    for (int i = 0; i < size; i++)
        word[i] = tolower(word[i]);

    word_node_t* node = counter->first->next;
    int found = 0;

    while (node != NULL) {
        if (strncmp(word, node->text, MIN(size, node->text_size)) == 0) {
            found = 1;
            node->text_count++;
            break;
        }
        node = node->next;
    }
    if (!found) {
        word_node_t* new_node = malloc(sizeof(word_node_t));
        new_node->text_count = 1;
        new_node->text_size = size;
        new_node->text = word;

        new_node->next = counter->first->next;
        counter->first->next = new_node;
        counter->size++;
    }
    return 0;
}

void free_request(request_t* request) {
    // todo
}
void* request_handler_fun(void* data) {
    while (1) {
        pthread_mutex_lock(&queue_mutex);
        request_t* request;
        while ((request = queget(&requests)) == NULL)
            pthread_cond_wait(&queue_new_element_cond, &queue_mutex);
        pthread_mutex_unlock(&queue_mutex);

        colorprintf(ANSI_COLOR_YELLOW, "Getting next element from queue");

        word_counter_t word_counter;
        init_word_counter(&word_counter);

        int words_count = 0;
        int state =
            1;  // state 1 => i'm after space : state 0 => i'm after some text
        for (int j = 0; j < request->size; j++) {
            char ch = request->content[j];
            if (ch == '\t' || ch == ' ' || ch == '\n' || ch == '\r') {
                state = 1;
            } else if (ch == '(') {
            } else {
                if (state == 1) {
                    words_count++;
                    int k =
                        j +
                        1;  // k => last index of word, j => first index of word
                    while (k < request->size) {
                        char ch = request->content[k];
                        if (ch == '\t' || ch == ' ' || ch == '\n' ||
                            ch == '\r' || ch == '.' || ch == ',' || ch == ':' ||
                            ch == ';' || ch == ')' || ch == '?' ||
                            ch == '!') {  // detect word end
                            break;
                        }
                        k++;
                    }
                    if (k - j > 0)
                        add_to_word_counter(&word_counter, request->content + j,
                                            k - j);
                }
                state = 0;
            }
        }
        if (mode == MODE_TCP) {
            colorprintf(ANSI_COLOR_YELLOW, "Sending result to server");

            pthread_mutex_lock(&socket_send_mutex);
            int type = MSGTYPE_RESULT;
            send(sock1, &type, 1, 0);
            send(sock1, &request->id, sizeof(int), 0);
            send(sock1, &words_count, sizeof(int), 0);

            int output_size = 0;
            word_node_t* node = word_counter.first->next;
            while (node != NULL) {
                output_size += node->text_size;
                output_size += 2;  //: space
                output_size += 1;  // \n
                int n_digits = floor(log10(abs(node->text_count))) + 1;
                output_size += n_digits;
                node = node->next;
            }
            output_size += 1;  // \0

            char buffer[128];

            char* output = malloc(sizeof(char) * output_size);
            output[0] = '\0';
            node = word_counter.first->next;

            while (node != NULL) {
                snprintf(buffer, 128, ": %d\n", node->text_count);
                strncat(output, node->text, node->text_size);
                strcat(output, buffer);
                node = node->next;
            }

            send(sock1, &output_size, sizeof(int), 0);
            int pointer = 0;
            while (pointer < output_size) {
                int size = 1024;
                if (pointer + size > output_size) {
                    size = output_size - pointer;
                }
                send(sock1, output + pointer, size, 0);
                pointer += size;
            }

            pthread_mutex_unlock(&socket_send_mutex);
            colorprintf(ANSI_COLOR_GREEN, "Sent result to server");

        } else if (mode == MODE_UDP) {
            colorprintf(ANSI_COLOR_YELLOW, "Sending result to server");

            pthread_mutex_lock(&socket_send_mutex);
            int type = MSGTYPE_RESULT;
            ;

            int output_size = 0;
            output_size += 1;  // request type
            word_node_t* node = word_counter.first->next;
            while (node != NULL) {
                output_size += node->text_size;
                output_size += 2;  //: space
                output_size += 1;  // \n
                int n_digits = floor(log10(abs(node->text_count))) + 1;
                output_size += n_digits;
                node = node->next;
            }
            output_size += 13;  // "word count: \n"
            output_size += floor(log10(abs(words_count))) + 1;
            output_size += 13;  // "response id: "

            if (request->id > 0)
                output_size += floor(log10(abs(request->id))) + 1;
            else
                output_size += 1;

            output_size += 1;  // \0

            char buffer[128];

            char* output = malloc(sizeof(char) * output_size);
            output[0] = type;
            output[1] = '\0';

            node = word_counter.first->next;

            while (node != NULL) {
                snprintf(buffer, 128, ": %d\n", node->text_count);
                strncat(output, node->text, node->text_size);
                strcat(output, buffer);
                node = node->next;
            }
            snprintf(buffer, 128, "words count: %d\n", words_count);
            strcat(output, buffer);

            snprintf(buffer, 128, "response id: %d", request->id);
            strcat(output, buffer);

            sendto(sock1, output, output_size, 0, udp_sockaddr,
                   udp_sockaddr_size);
            pthread_mutex_unlock(&socket_send_mutex);
            colorprintf(ANSI_COLOR_GREEN, "Sent result to server");
        }

        free_request(request);
    }
}

int inet = 0;
int port = 0;

int main(int argc, char* argv[]) {
    if (argc != 4 && argc != 5)
        return err(
            "Missing arguments <name> <mode:UNIX/INET> <addr:IP/path> <port>",
            -1);

    if (strncmp(argv[2], "INET", 4) == 0)
        inet = 1;
    else if (strncmp(argv[2], "UNIX", 4) == 0)
        inet = 0;
    else
        return err("Unknown mode", -1);

    if (inet) {
        if (parse_int(argv[4], &port) < 0)
            return err("Wrong port", -1);
    }

    quenew(&requests);

    pthread_t id;
    int data = 0;
    pthread_create(&id, NULL, request_handler_fun, &data);
    if (mode == MODE_TCP) {
        if (inet) {
            inet_tcp_start(argv[3], port);
        } else {
            unix_tcp_start(argv[3]);
        }
    } else {
        if (inet) {
            inet_udp_start(argv[3], port);
        } else {
            unix_udp_start(argv[3]);
        }
    }

    int n;
    if (mode == MODE_TCP) {
        send(sock1, argv[1], strlen(argv[1]), 0);

        while (1) {
            char msgtype;
            n = recv(sock1, &msgtype, 1, 0);
            if (n <= 0) {
                colorprintf(ANSI_COLOR_RED, "Lost connection");
                return -1;
            }
            if (msgtype == MSGTYPE_PING) {
                char sendmsg = MSGTYPE_PING;
                pthread_mutex_lock(&socket_send_mutex);
                send(sock1, &sendmsg, 1, 0);
                pthread_mutex_unlock(&socket_send_mutex);

            } else if (msgtype == MSGTYPE_REQUEST) {
                int request_id = 0;
                int block_size = 0;

                recv(sock1, &request_id, sizeof(int), MSG_WAITALL);
                recv(sock1, &block_size, sizeof(int), MSG_WAITALL);

                request_t* request = malloc(sizeof(request));
                request->id = request_id;
                request->size = block_size;
                request->content = malloc(sizeof(char) * request->size);

                recv(sock1, request->content, request->size, MSG_WAITALL);

                pthread_mutex_lock(&queue_mutex);
                queadd(&requests, request);
                pthread_cond_broadcast(&queue_new_element_cond);
                pthread_mutex_unlock(&queue_mutex);

                colorprintf(ANSI_COLOR_CYAN, "Got new request");
            }
        }
    } else if (mode == MODE_UDP) {
        int BUFFER_SIZE = 65536;
        char buffer[BUFFER_SIZE + 1];

        n = snprintf(buffer, 128, " %s", argv[1]);
        buffer[0] = MSGTYPE_UDP_CONNECT;
        sendto(sock1, buffer, n, 0, udp_sockaddr, udp_sockaddr_size);
        while (1) {
            n = recvfrom(sock1, &buffer, BUFFER_SIZE, 0, udp_sockaddr,
                         &udp_sockaddr_size);
            buffer[n] = '\0';
            char msgtype = buffer[0];
            if (n < 0) {
                printf("Connection lost\n");
                exit(1);
            }

            if (n <= 0) {
                continue;
            }

            if (msgtype == MSGTYPE_PING) {
                char sendmsg = MSGTYPE_PING;
                pthread_mutex_lock(&socket_send_mutex);
                sendto(sock1, &sendmsg, 1, 0, udp_sockaddr, udp_sockaddr_size);
                pthread_mutex_unlock(&socket_send_mutex);

            } else if (msgtype == MSGTYPE_REQUEST) {
                request_t* request = malloc(sizeof(request));

                request->id = buffer[1];
                request->size = n - 2;
                request->content = malloc(sizeof(char) * request->size);
                request->content[0] = '\0';

                strncpy(request->content, buffer + 2, request->size);

                pthread_mutex_lock(&queue_mutex);
                queadd(&requests, request);
                pthread_cond_broadcast(&queue_new_element_cond);
                pthread_mutex_unlock(&queue_mutex);

                colorprintf(ANSI_COLOR_CYAN, "Got new request");
            }
        }
    }

    close(sock1);

    return 0;
}

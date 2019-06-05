#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include "colors.h"
#include "errors.h"
#include "utils.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define MODE_TCP 1
#define MODE_UDP 0

#ifdef UDP
int mode = MODE_UDP;
#else
int mode = MODE_TCP;
#endif

#define CLIENT_ID_SIZE 10
#define CLIENTS_SIZE 1000

#define CLIENT_STATE_NULL 0
#define CLIENT_STATE_BUSY 1
#define CLIENT_STATE_IDLE 2

#define MSGTYPE_PING 1
#define MSGTYPE_RESULT 2
#define MSGTYPE_REQUEST 3
#define MSGTYPE_UDP_CONNECT 4

#define CLIENT_UDP_ID_SIZE 30

typedef struct tcp_client_t {
    char id[CLIENT_ID_SIZE];
    int state;
    struct timeval last_ping_time;
    int sockid;
} tcp_client_t;

typedef struct udp_client_t {
    char id[CLIENT_ID_SIZE];
    int state;
    struct timeval last_ping_time;
    struct sockaddr* udp_addr;
    int udp_addr_size;
    int sockid;
    int inet;
} udp_client_t;

tcp_client_t** clients;
udp_client_t** udp_clients;

pthread_mutex_t socket_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

int request_counter = 0;
int sock_inet;
int sock_unix;
fd_set active_fd_set, read_fd_set;

void timeval_diff(struct timeval* start,
                  struct timeval* stop,
                  struct timeval* diff) {
    if ((stop->tv_usec - start->tv_usec) < 0) {
        diff->tv_sec = stop->tv_sec - start->tv_sec - 1;
        diff->tv_usec = 1000000 + stop->tv_usec - start->tv_usec;
    } else {
        diff->tv_sec = stop->tv_sec - start->tv_sec;
        diff->tv_usec = stop->tv_usec - start->tv_usec;
    }
}

tcp_client_t* get_client_with_socket_id(int sockid) {
    tcp_client_t* client = NULL;
    for (int j = 0; j < CLIENTS_SIZE; j++)
        if (clients[j]->sockid == sockid) {
            client = clients[j];
            break;
        }
    return client;
}

int read_from_client_udp(int sockfd, int inet) {
    int BUFFER_SIZE = 65536;
    char buffer[BUFFER_SIZE + 1];
    int nbytes;

    struct sockaddr* name;
    socklen_t len;
    if (inet) {
        name = malloc(sizeof(struct sockaddr_in));
        len = sizeof(struct sockaddr_in);
    } else {
        name = malloc(sizeof(struct sockaddr_un));
        len = sizeof(struct sockaddr_un);
    }
    nbytes = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL,
                      (struct sockaddr*)name, &len);
    buffer[nbytes] = '\0';

    char msgtype = buffer[0];

    if (nbytes <= 0) {
        return -1;
    } else {
        if (msgtype == MSGTYPE_PING) {
            for (int j = 0; j < CLIENTS_SIZE; j++) {
                if (udp_clients[j]->state != CLIENT_STATE_NULL) {
                    if (udp_clients[j]->inet && inet) {
                        struct sockaddr_in* addr =
                            (struct sockaddr_in*)udp_clients[j]->udp_addr;

                        struct sockaddr_in* addr2 = (struct sockaddr_in*)name;

                        if (addr->sin_addr.s_addr == addr2->sin_addr.s_addr &&
                            addr->sin_port == addr2->sin_port)
                            gettimeofday(&udp_clients[j]->last_ping_time, NULL);

                    } else if (!udp_clients[j]->inet && !inet) {
                        struct sockaddr_un* addr =
                            (struct sockaddr_un*)udp_clients[j]->udp_addr;

                        struct sockaddr_un* addr2 = (struct sockaddr_un*)name;
                        if (strcmp(addr->sun_path, addr2->sun_path) == 0)
                            gettimeofday(&udp_clients[j]->last_ping_time, NULL);
                    }
                }
            }

        } else if (msgtype == MSGTYPE_RESULT) {
            udp_client_t* client = NULL;
            for (int j = 0; j < CLIENTS_SIZE; j++) {
                if (udp_clients[j]->state != CLIENT_STATE_NULL) {
                    if (udp_clients[j]->inet && inet) {
                        struct sockaddr_in* addr =
                            (struct sockaddr_in*)udp_clients[j]->udp_addr;

                        struct sockaddr_in* addr2 = (struct sockaddr_in*)name;

                        if (addr->sin_addr.s_addr == addr2->sin_addr.s_addr &&
                            addr->sin_port == addr2->sin_port) {
                            client = udp_clients[j];
                            break;
                        }
                    } else if (!udp_clients[j]->inet && !inet) {
                        struct sockaddr_un* addr =
                            (struct sockaddr_un*)udp_clients[j]->udp_addr;

                        struct sockaddr_un* addr2 = (struct sockaddr_un*)name;

                        if (strcmp(addr->sun_path, addr2->sun_path) == 0) {
                            client = udp_clients[j];
                            break;
                        }
                    }
                }
            }

            if (client != NULL) {
                printf("Response: %s\n", buffer + 1);
                colorprintf(ANSI_COLOR_GREEN,
                            "That was response from client[%s]", client->id);
            }

        } else if (msgtype == MSGTYPE_UDP_CONNECT) {
            int found_index = -1;
            for (int j = 0; j < CLIENTS_SIZE; j++)
                if (udp_clients[j]->state == CLIENT_STATE_NULL) {
                    gettimeofday(&udp_clients[j]->last_ping_time, NULL);
                    found_index = j;
                    break;
                }
            if (found_index == -1) {
                printf("Exceeded clients size\n");
                exit(2137);
            }

            int good_id = 1;
            for (int j = 0; j < CLIENTS_SIZE; j++)
                if (udp_clients[j]->state != CLIENT_STATE_NULL) {
                    if (strcmp(udp_clients[j]->id, buffer + 1) == 0) {
                        printf("Already client with this id [%s]\n",
                               udp_clients[j]->id);
                        good_id = 0;
                        break;
                    }
                }
            if (good_id == 0) {
                return -1;
            }

            udp_clients[found_index]->state = CLIENT_STATE_IDLE;
            strcpy(udp_clients[found_index]->id, buffer + 1);
            udp_clients[found_index]->udp_addr = (struct sockaddr*)name;
            udp_clients[found_index]->udp_addr_size = len;
            udp_clients[found_index]->sockid = sockfd;
            udp_clients[found_index]->inet = inet;
            char ch = MSGTYPE_PING;
            sendto(sockfd, &ch, 1, 0, (struct sockaddr*)name, len);
            colorprintf(ANSI_COLOR_GREEN, "Added new client[%s]",
                        udp_clients[found_index]->id);
        }
        return 0;
    }
}

int read_from_client_tcp(tcp_client_t* client) {
    char msgtype;
    int nbytes;

    nbytes = read(client->sockid, &msgtype, 1);
    if (nbytes <= 0) {
        return -1;
    } else {
        if (msgtype == MSGTYPE_PING) {
            gettimeofday(&client->last_ping_time, NULL);
        } else if (msgtype == MSGTYPE_RESULT) {
            printf("Result\n");

            int response_id = 0;
            recv(client->sockid, &response_id, sizeof(int), MSG_WAITALL);
            int words_len = 0;
            recv(client->sockid, &words_len, sizeof(int), MSG_WAITALL);
            int response_size = 0;
            recv(client->sockid, &response_size, sizeof(int), MSG_WAITALL);
            char* response = malloc(sizeof(char) * response_size);
            char* response_pointer = response;
            int remainging_bytes = response_size;

            while (remainging_bytes > 0) {
                int nbytes = recv(client->sockid, response_pointer,
                                  MIN(remainging_bytes, 1024), MSG_WAITALL);
                response_pointer += nbytes;
                remainging_bytes -= nbytes;
            }
            printf("Response words: %s", response);
            printf("Response [%d]: word count: %d\n", response_id, words_len);
            colorprintf(ANSI_COLOR_GREEN, "That was response from client[%s]",
                        client->id);
            free(response);

            pthread_mutex_lock(&clients_mutex);
            client->state = CLIENT_STATE_IDLE;
            pthread_mutex_unlock(&clients_mutex);
        }
        return 0;
    }
}

int make_inet_socket(uint16_t port) {
    int sock;
    struct sockaddr_in name;

    if (mode == MODE_TCP) {
        sock = socket(PF_INET, SOCK_STREAM, 0);
    } else {
        sock = socket(PF_INET, SOCK_DGRAM, 0);
    }
    int true = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(int));
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (struct sockaddr*)&name, sizeof(name)) < 0) {
        perror("bind1");
        exit(EXIT_FAILURE);
    }

    return sock;
}

int make_unix_socket(char* path) {
    int sock;
    struct sockaddr_un name;

    if (mode == MODE_TCP) {
        sock = socket(PF_UNIX, SOCK_STREAM, 0);
    } else {
        sock = socket(PF_UNIX, SOCK_DGRAM, 0);
    }
    int true = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &true, sizeof(int));
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    name.sun_family = AF_UNIX;
    strcpy(name.sun_path, path);
    unlink(path);
    if (bind(sock, (struct sockaddr*)&name, sizeof(name)) < 0) {
        perror("bind2");
        exit(EXIT_FAILURE);
    }

    return sock;
}

void* ping_thread_fun(void* data) {
    while (1) {
        if (mode == MODE_TCP) {
            sleep(2);
            struct timeval actual_time;
            gettimeofday(&actual_time, NULL);
            struct timeval diff;
            for (int j = 0; j < CLIENTS_SIZE; j++)
                if (clients[j]->state != CLIENT_STATE_NULL) {
                    timeval_diff(&clients[j]->last_ping_time, &actual_time,
                                 &diff);
                    if (diff.tv_sec > 5) {
                        close(clients[j]->sockid);
                        FD_CLR(clients[j]->sockid, &active_fd_set);
                        clients[j]->state = CLIENT_STATE_NULL;
                        colorprintf(ANSI_COLOR_RED,
                                    "Removing client[%s] due to inactivity",
                                    clients[j]->id);
                    }
                }

            pthread_mutex_lock(&socket_mutex);
            for (int i = 0; i < FD_SETSIZE; i++) {
                if (FD_ISSET(i, &active_fd_set)) {
                    if (i == sock_inet || i == sock_unix) {
                    } else {
                        char msgsend = MSGTYPE_PING;
                        send(i, &msgsend, 1, 0);
                    }
                }
            }
            pthread_mutex_unlock(&socket_mutex);

        } else if (mode == MODE_UDP) {
            sleep(2);
            struct timeval actual_time;
            gettimeofday(&actual_time, NULL);
            struct timeval diff;
            for (int j = 0; j < CLIENTS_SIZE; j++)
                if (udp_clients[j]->state != CLIENT_STATE_NULL) {
                    timeval_diff(&udp_clients[j]->last_ping_time, &actual_time,
                                 &diff);
                    if (diff.tv_sec > 5) {
                        free(udp_clients[j]->udp_addr);
                        udp_clients[j]->state = CLIENT_STATE_NULL;
                        colorprintf(ANSI_COLOR_RED,
                                    "Removing client[%s] due to inactivity",
                                    udp_clients[j]->id);
                    }
                }

            pthread_mutex_lock(&socket_mutex);

            for (int j = 0; j < CLIENTS_SIZE; j++)
                if (udp_clients[j]->state != CLIENT_STATE_NULL) {
                    char ch = MSGTYPE_PING;
                    sendto(udp_clients[j]->sockid, &ch, 1, 0,
                           udp_clients[j]->udp_addr,
                           udp_clients[j]->udp_addr_size);
                }
            pthread_mutex_unlock(&socket_mutex);
        }
    }
    return NULL;
}

int send_request(char* path) {
    FILE* f = fopen(path, "r");
    if (f == NULL) {
        colorprintf(ANSI_COLOR_RED, "Cannot open %s", path);
        return -1;
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* string = malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);
    string[fsize] = 0;

    if (mode == MODE_TCP) {
        pthread_mutex_lock(&clients_mutex);
        tcp_client_t* client = NULL;
        for (int j = 0; j < CLIENTS_SIZE; j++) {
            if (clients[j]->state == CLIENT_STATE_IDLE) {
                client = clients[j];
                break;
            }
        }

        if (client == NULL) {
            for (int j = 0; j < CLIENTS_SIZE; j++) {
                if (clients[j]->state != CLIENT_STATE_NULL) {
                    client = clients[j];
                    break;
                }
            }
        }
        if (client != NULL) {
            client->state = CLIENT_STATE_BUSY;
        }
        pthread_mutex_unlock(&clients_mutex);

        if (client == NULL) {
            colorprintf(ANSI_COLOR_RED, "There is no client to send request",
                        path);
            return -1;
        }
        colorprintf(ANSI_COLOR_YELLOW, "Sending request to client[%s]",
                    client->id);
        pthread_mutex_lock(&socket_mutex);

        char type = MSGTYPE_REQUEST;
        request_counter++;
        send(client->sockid, &type, 1, 0);
        send(client->sockid, &request_counter, sizeof(int), 0);
        send(client->sockid, &fsize, sizeof(int), 0);
        send(client->sockid, string, fsize, 0);

        pthread_mutex_unlock(&socket_mutex);
        colorprintf(ANSI_COLOR_YELLOW, "Sent request to client[%s]",
                    client->id);

    } else {
        pthread_mutex_lock(&clients_mutex);
        udp_client_t* client = NULL;
        for (int j = 0; j < CLIENTS_SIZE; j++) {
            if (udp_clients[j]->state == CLIENT_STATE_IDLE) {
                client = udp_clients[j];
                break;
            }
        }

        if (client == NULL) {
            for (int j = 0; j < CLIENTS_SIZE; j++) {
                if (udp_clients[j]->state != CLIENT_STATE_NULL) {
                    client = udp_clients[j];
                    break;
                }
            }
        }
        if (client != NULL) {
            client->state = CLIENT_STATE_BUSY;
        }
        pthread_mutex_unlock(&clients_mutex);

        if (client == NULL) {
            colorprintf(ANSI_COLOR_RED, "There is no client to send request",
                        path);
            return -1;
        }
        colorprintf(ANSI_COLOR_YELLOW, "Sending request to client[%s]",
                    client->id);

        pthread_mutex_lock(&socket_mutex);
        int BUFFER_SIZE = 65507;

        if (fsize > BUFFER_SIZE) {
            colorprintf(ANSI_COLOR_YELLOW,
                        "Sending only 65505 bytes from this file (UDP LIMIT)");
        }

        char buffer[BUFFER_SIZE];
        snprintf(buffer, BUFFER_SIZE, "  %s", string);

        buffer[0] = MSGTYPE_REQUEST;
        buffer[1] = request_counter;

        request_counter++;
        sendto(client->sockid, buffer, strlen(buffer + 2) + 2, 0,
               client->udp_addr, client->udp_addr_size);

        pthread_mutex_unlock(&socket_mutex);
        colorprintf(ANSI_COLOR_YELLOW, "Sent request to client[%s]",
                    client->id);
    }

    free(string);
    return 0;
}

int accept_client_from_tcp(int sockfd) {
    size_t new;
    unsigned int size = 0;
    if (sockfd == sock_inet) {
        struct sockaddr_in addr_in;
        size = sizeof(addr_in);
        new = accept(sockfd, (struct sockaddr*)&addr_in, &size);
    } else if (sockfd == sock_unix) {
        struct sockaddr_un addr_un;
        size = sizeof(addr_un);
        new = accept(sockfd, (struct sockaddr*)&addr_un, &size);
    }

    if (new < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(new, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    int found_index = -1;
    for (int j = 0; j < CLIENTS_SIZE; j++)
        if (clients[j]->state == CLIENT_STATE_NULL) {
            gettimeofday(&clients[j]->last_ping_time, NULL);
            clients[j]->sockid = new;
            found_index = j;
            break;
        }
    if (found_index == -1) {
        printf("Exceeded clients size\n");
        exit(2137);
    }

    int nbytes = recv(new, clients[found_index]->id, CLIENT_ID_SIZE, 0);
    clients[found_index]->id[nbytes] = '\0';

    int good_id = 1;
    for (int j = 0; j < CLIENTS_SIZE; j++)
        if (clients[j]->state != CLIENT_STATE_NULL && j != found_index) {
            if (strcmp(clients[j]->id, clients[found_index]->id) == 0) {
                printf("Already client with this id [%s]\n", clients[j]->id);
                clients[found_index]->state = CLIENT_STATE_NULL;
                close(new);
                good_id = 0;
                break;
            }
        }

    if (nbytes > 0 && good_id) {
        colorprintf(ANSI_COLOR_GREEN, "Added new client[%s]",
                    clients[found_index]->id);
        FD_SET(new, &active_fd_set);
        clients[found_index]->state = CLIENT_STATE_IDLE;
    } else {
        printf("Error adding new client\n");
    }
    return 0;
}

int inet;
int port;
int main(int argc, char* argv[]) {
    if (argc != 3)
        return err("Missing arguments <port> <unix_path>", -1);

    int port = 0;
    if (parse_int(argv[1], &port) < 0)
        return err("Wrong port", -1);

    if (mode == MODE_TCP) {
        colorprintf(ANSI_COLOR_CYAN, "Starting TCP server");
        clients = malloc(sizeof(tcp_client_t*) * CLIENTS_SIZE);
        for (int i = 0; i < CLIENTS_SIZE; i++) {
            clients[i] = malloc(sizeof(tcp_client_t) * CLIENTS_SIZE);
            clients[i]->state = CLIENT_STATE_NULL;
        }
    } else if (mode == MODE_UDP) {
        colorprintf(ANSI_COLOR_CYAN, "Starting UDP server");
        udp_clients = malloc(sizeof(udp_client_t*) * CLIENTS_SIZE);
        for (int i = 0; i < CLIENTS_SIZE; i++) {
            udp_clients[i] = malloc(sizeof(udp_client_t) * CLIENTS_SIZE);
            udp_clients[i]->state = CLIENT_STATE_NULL;
        }
    }

    sock_inet = make_inet_socket(port);
    sock_unix = make_unix_socket(argv[2]);

    if (mode == MODE_TCP)
        if (listen(sock_inet, 1) < 0 || listen(sock_unix, 1) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

    FD_ZERO(&active_fd_set);
    FD_SET(STDIN_FILENO, &active_fd_set);
    FD_SET(sock_inet, &active_fd_set);
    FD_SET(sock_unix, &active_fd_set);

    pthread_t id;
    int data = 0;
    pthread_create(&id, NULL, ping_thread_fun, &data);
    while (1) {
        read_fd_set = active_fd_set;
        if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; ++i)
            if (FD_ISSET(i, &read_fd_set)) {
                if (i == STDIN_FILENO) {
                    char path[128];
                    size_t sz = read(0, path, sizeof(path));
                    path[sz - 1] = '\0';
                    send_request(path);
                } else if (i == sock_inet || i == sock_unix) {
                    pthread_mutex_lock(&socket_mutex);

                    if (mode == MODE_TCP) {
                        accept_client_from_tcp(i);
                    } else if (mode == MODE_UDP) {
                        read_from_client_udp(i, (i == sock_inet) ? 1 : 0);
                    }
                    pthread_mutex_unlock(&socket_mutex);

                } else {
                    if (mode == MODE_TCP) {
                        tcp_client_t* client = get_client_with_socket_id(i);

                        if (read_from_client_tcp(client) < 0) {
                            close(i);
                            colorprintf(ANSI_COLOR_RED,
                                        "Client [%s] disconnected", client->id);
                            FD_CLR(i, &active_fd_set);
                            client->state = CLIENT_STATE_NULL;
                        }
                    }
                }
            }
    }
}

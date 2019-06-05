#ifndef PROTO_H
#define PROTO_H

#include <stdint.h>
#include <sys/socket.h>

void proto_send_udp(int fd,
                    struct sockaddr* addr,
                    const char* buffer,
                    int32_t size);

char* proto_recv_udp(int fd, struct sockaddr* addr);

void proto_send(int fd, const char* buffer, int32_t size);
char* proto_recv(int fd);

#endif

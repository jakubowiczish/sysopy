#include "utils.h"
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/time.h>

int get_trucker_key() {
    return ftok(getenv("HOME"), 2137);
}

int get_queue_key() {
    return ftok(getenv("HOME"), 1337);
}

long get_timestamp() {
    struct timeval timestamp;
    gettimeofday(&timestamp, NULL);

    return timestamp.tv_sec * MILLION + timestamp.tv_usec;
}
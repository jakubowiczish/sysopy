#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "error.h"
#include "pack.h"
#include "conveyor_belt.h"
#include "shared.h"
#include "utils.h"


static sem_id_t sem;

static conveyor_belt_t *cb;

static int weight;

static int last_result;

static pid_t pid;


void do_the_cleanup() {
    close_semaphore(sem);
    unmap_shared_mem(cb, CB_SIZE);

    char buffer[128];
    sprintf(buffer, "%d cleaned up\n\n", pid);
    print_coloured_message(buffer, GREEN);
}


void handle_SIGINT(int signal_num) {
    exit(0);
}


void place_pack() {
    pack_t pack = new_pack(weight);

    int result = enqueue(&cb->q, sem, pack);

    if (result == 0) {
        char loader_buffer[1024];

        sprintf(
                loader_buffer,
                "%7d %5ld %5ld  loaded %dkg\n",
                pid,
                pack.timestamp.tv_sec,
                pack.timestamp.tv_usec,
                weight
        );

        print_coloured_message(loader_buffer, BLUE);

    } else if (result == -1 && last_result != -1) {
        char full_conveyor_belt_buffer[1024];

        sprintf(
                full_conveyor_belt_buffer,
                "%7d conveyor belt is full!\n",
                pid
        );

        print_coloured_message(full_conveyor_belt_buffer, YELLOW);

    } else if (result == -2 && last_result != -2) {
        char maximum_weight_reached_buffer[1024];

        sprintf(
                maximum_weight_reached_buffer,
                "%7d maximum conveyor belt weight reached!\n",
                pid
        );

        print_coloured_message(maximum_weight_reached_buffer, YELLOW);
    }

    last_result = result;
}


int main(int argc, char **argv) {
    pid = getpid();

    int cycles = 0;

    if (argc == 2) {
        if (sscanf(argv[1], "%d", &weight) != 1) {
            print_coloured_message("passed argument is invalid!", RED);
        }
    } else if (argc == 3) {
        if (sscanf(argv[1], "%d", &weight) != 1 || sscanf(argv[2], "%d", &cycles)) {
            print_coloured_message("passed arguments are invalid!", RED);
        }
    } else {
        print_coloured_message("invalid number of arguments (loader)", RED);
    }


    int mem_id = open_shared_mem(get_trucker_key(), CB_SIZE);
    cb = map_shared_mem(mem_id, CB_SIZE);
    sem = open_semaphore(cb->q.sem_key);


    atexit(do_the_cleanup);

    signal(SIGINT, handle_SIGINT);


    if (cycles == 0) {
        while (1) {
            place_pack();
            usleep(1000);
        }
    } else {
        for (int i = 0; i < cycles; ++i) {
            place_pack();
            usleep(1000);
        }
    }


    return 0;
}




#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "error.h"
#include "pack.h"
#include "shared.h"
#include "utils.h"
#include "conveyor_belt.h"


static sem_id_t sem;

static conveyor_belt_t *cb;

static int weight;

static int last_result;

static pid_t pid;


void do_the_cleanup() {
    close_semaphore(sem);
    unmap_shared_mem(cb, CB_SIZE);

    char buffer[128];
    sprintf(buffer, "PID: %d - loader cleaned up, TIMESTAMP: %10ldus", pid, get_timestamp());
    print_coloured_message(buffer, GREEN);
}


void handle_SIGINT(int signal_num) {
    exit(0);
}


void place_pack() {
    pack_t pack = new_pack(weight);

    int result = enqueue(cb, sem, pack);

    if (result == 0) {
        char loader_buffer[1024];

        sprintf(
                loader_buffer,
                "PID: %7d, loaded %dkg. TIMESTAMP: %10ldus",
                pid,
                weight,
                get_timestamp()
        );

        print_coloured_message(loader_buffer, BRIGHT_BLUE);

    } else if (result == -1 && last_result != -1) {
        char full_conveyor_belt_buffer[1024];

        sprintf(
                full_conveyor_belt_buffer,
                "PID: %7d - conveyor belt is full! TIMESTAMP: %10ldus",
                pid,
                get_timestamp()
        );

        print_coloured_message(full_conveyor_belt_buffer, YELLOW);

    } else if (result == -2 && last_result != -2) {
        char maximum_weight_reached_buffer[1024];

        sprintf(
                maximum_weight_reached_buffer,
                "PID: %7d - maximum conveyor belt weight reached! TIMESTAMP: %10ldus",
                pid,
                get_timestamp()
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
            char buffer[1024];
            sprintf(buffer, "passed argument is invalid! TIMESTAMP: %10ldus", get_timestamp());

            print_coloured_message(buffer, RED);
        }
    } else if (argc == 3) {
        if (sscanf(argv[1], "%d", &weight) != 1
            || sscanf(argv[2], "%d", &cycles) != 1) {

            char buffer[1024];
            sprintf(buffer, "passed arguments are invalid! TIMESTAMP: %10ldus", get_timestamp());

            print_coloured_message(buffer, RED);
        }
    } else {

        char buffer[1024];
        sprintf(
                buffer,
                "\"invalid number of arguments (loader) - necessary argument: weight, optional argument: cycles\" TIMESTAMP: %10ldus",
                get_timestamp()
        );

        print_coloured_message(buffer, RED);
    }


    int mem_id = open_shared_mem(get_trucker_key(), CB_SIZE);
    cb = map_shared_mem(mem_id, CB_SIZE);
    sem = open_semaphore(cb->sem_key);


    atexit(do_the_cleanup);

    signal(SIGINT, handle_SIGINT);


    if (cycles == 0) {
        forever {
            place_pack();
            sleep(1);
        }
    } else {
        for (int i = 0; i < cycles; ++i) {
            place_pack();
            sleep(1);
        }
    }


    return 0;
}




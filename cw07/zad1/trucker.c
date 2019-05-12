#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "conveyor_belt.h"
#include "utils.h"
#include "error.h"


static int mem_id;

static conveyor_belt_t *cb;

static sem_id_t q_sem;


void do_the_cleanup() {
    fflush(stdout);
    remove_semaphore(get_queue_key(), q_sem);
    unmap_shared_mem(cb, CB_SIZE);
    remove_shared_mem(get_trucker_key(), mem_id);
    printf("cleaned up!\n");
}


void handle_SIGINT(int signal_num) {
    exit(0);
}


int main(int argc, char **argv) {
    if (argc != 4) {
        print_coloured_message(
                "invalid arguments given - necessary arguments: truck_capacity, belt_size, belt_capacity", RED);
    }

    atexit(do_the_cleanup);

    signal(SIGINT, handle_SIGINT);

    int truck_capacity;
    int belt_size;
    int belt_capacity;

    if (sscanf(argv[1], "%d", &truck_capacity) != 1
        || sscanf(argv[2], "%d", &belt_size) != 1
        || sscanf(argv[3], "%d", &belt_capacity) != 1) {
        print_coloured_message("given arguments are invalid!", RED);
    }


    mem_id = create_shared_mem(get_trucker_key(), CB_SIZE);
    cb = map_shared_mem(mem_id, CB_SIZE);

    cb->max_weight = belt_capacity;

    q_sem = create_semaphore(get_queue_key());
    cb->q = new_queue(belt_size, belt_capacity, get_queue_key());


    int current_weight = 0;

    pack_t pack;
    struct timeval timestamp;

    print_coloured_message("empty truck has arrived!", BLUE);

    int empty = 0;


    while (1) {
        fflush(stdout);
        sleep(1);

        if (dequeue(&cb->q, q_sem, &pack) == NULL) {
            if (empty == 0) {
                print_coloured_message("conveyor belt is empty!", MAGENTA);
            }

            empty = 1;
            continue;
        }

        gettimeofday(&timestamp, NULL);


        if (current_weight + pack.weight > truck_capacity) {
            lock_semaphore(q_sem);

            print_coloured_message("truck is full - unloading", GREEN);

            current_weight = 0;

            sleep(1);

            print_coloured_message("empty truck has just arrived!", CYAN);

            unlock_semaphore(q_sem);
        }


        if (pack.weight > truck_capacity) {
            print_coloured_message("capacity of the truck is too small for the pack", YELLOW);
        }

        current_weight += pack.weight;


        int elapsed_time = ((timestamp.tv_sec - pack.timestamp.tv_sec) * 1000000) +
                           (timestamp.tv_usec - pack.timestamp.tv_usec);

        char info_buffer[1024];
        sprintf(
                info_buffer,
                "PID %7d   TIME %10dus   WEIGHT %3dkg   OCCUPIED  %3dkg   LEFT %3dkg\n\n",
                pack.pid,
                elapsed_time,
                pack.weight,
                current_weight,
                truck_capacity - current_weight
        );


        print_coloured_message(info_buffer, BLUE);
    }

    return 0;
}
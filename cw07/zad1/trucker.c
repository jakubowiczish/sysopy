#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "conveyor_belt.h"
#include "error.h"
#include "shared.h"
#include "utils.h"

static int mem_id;

static conveyor_belt_t* cb;

static sem_id_t cb_sem;

void do_the_cleanup() {
    fflush(stdout);
    remove_semaphore(get_queue_key(), cb_sem);
    unmap_shared_mem(cb, CB_SIZE);
    remove_shared_mem(get_trucker_key(), mem_id);

    char cleaned_up_buffer[1024];
    sprintf(cleaned_up_buffer, "\n(TRUCKER) cleaned up!, TIMESTAMP: %10ldus\n",
            get_timestamp());

    print_coloured_message(cleaned_up_buffer, GREEN);
}

void handle_SIGINT(int signal_num) {
    exit(0);
}

int main(int argc, char** argv) {
    if (argc != 4) {
        char error_buffer[1024];
        sprintf(
            error_buffer,
            "invalid arguments given - necessary arguments: truck_capacity, "
            "belt_size, belt_capacity, TIMESTAMP %10ldus",
            get_timestamp());

        print_coloured_message(error_buffer, RED);
    }

    atexit(do_the_cleanup);

    signal(SIGINT, handle_SIGINT);

    int truck_capacity;
    int belt_size;
    int belt_capacity;

    if (sscanf(argv[1], "%d", &truck_capacity) != 1 ||
        sscanf(argv[2], "%d", &belt_size) != 1 ||
        sscanf(argv[3], "%d", &belt_capacity) != 1) {
        char error_buffer[1024];
        sprintf(error_buffer, "given arguments are invalid, TIMESTAMP %10ldus",
                get_timestamp());

        print_coloured_message(error_buffer, RED);
    }

    mem_id = create_shared_mem(get_trucker_key(), CB_SIZE);
    cb = map_shared_mem(mem_id, CB_SIZE);

    cb->max_weight = belt_capacity;

    cb_sem = create_semaphore(get_queue_key());
    *cb = new_queue(belt_size, belt_capacity, get_queue_key());

    int current_weight = 0;

    pack_t pack;
    struct timeval timestamp;

    char empty_truck_buffer[1024];
    sprintf(empty_truck_buffer, "empty truck has arrived %10ldus",
            get_timestamp());
    print_coloured_message(empty_truck_buffer, BRIGHT_MAGENTA);

    int empty = 0;

    forever {
        fflush(stdout);
        sleep(1);

        if (dequeue(cb, cb_sem, &pack) == NULL) {
            if (empty == 0) {
                char conveyor_belt_empty[1024];
                sprintf(conveyor_belt_empty,
                        "conveyor belt is empty! TIMESTAMP %10ldus",
                        get_timestamp());

                char waiting_buffer[1024];
                sprintf(waiting_buffer,
                        "waiting to be loaded.... TIMESTAMP %10ldus",
                        get_timestamp());

                print_coloured_message(conveyor_belt_empty, MAGENTA);
                print_coloured_message(waiting_buffer, WHITE);
            }

            empty = 1;
            continue;
        }

        gettimeofday(&timestamp, NULL);

        if (current_weight + pack.weight > truck_capacity) {
            lock_semaphore(cb_sem);

            char full_truck_buffer[1024];
            sprintf(full_truck_buffer,
                    "truck is full - unpacking. TIMESTAMP %10ldus",
                    get_timestamp());

            print_coloured_message(full_truck_buffer, GREEN);

            current_weight = 0;

            sleep(1);

            char empty_truck_buff[1024];
            sprintf(empty_truck_buff,
                    "empty truck has just arrived!. TIMESTAMP %10ldus",
                    get_timestamp());

            print_coloured_message(empty_truck_buff, CYAN);

            unlock_semaphore(cb_sem);
        }

        if (pack.weight > truck_capacity) {
            char capacity_buffer[1024];
            sprintf(capacity_buffer,
                    "capacity of the truck is too small for the pack. "
                    "TIMESTAMP %10ldus",
                    get_timestamp());

            print_coloured_message(capacity_buffer, YELLOW);
        }

        current_weight += pack.weight;

        // since he tried to put the pack till it's in the truck
        int elapsed_time =
            ((timestamp.tv_sec - pack.timestamp.tv_sec) * MILLION) +
            (timestamp.tv_usec - pack.timestamp.tv_usec);

        char info_buffer[1024];

        sprintf(
            info_buffer,
            "PID %7d, TIME %10dus, WEIGHT %3dkg, OCCUPIED %3dkg, LEFT %3dkg, "
            "TIMESTAMP %10ldus",
            pack.pid, elapsed_time, pack.weight, current_weight,
            truck_capacity - current_weight, get_timestamp());

        print_coloured_message(info_buffer, BRIGHT_GREEN);
    }

    return 0;
}
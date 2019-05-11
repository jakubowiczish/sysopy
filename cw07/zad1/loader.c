#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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





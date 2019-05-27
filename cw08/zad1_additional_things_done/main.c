#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "errors.h"
#include "filter.h"
#include "image.h"
#include "utils.h"

#define THREADS_TYPE_BLOCK 0

#define THREADS_TYPE_INTERLEAVED 1

static int threads_num;

static int threads_type;

static char* image_path;

static char* filter_path;

static char* out_image_path;

img_t image;

img_t out_image;

filter_t filter;

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

void* thread_fun(void* ptr) {
    struct timeval timestamp_start;

    gettimeofday(&timestamp_start, NULL);

    int* thread_num = (int*)ptr;

    if (threads_type == THREADS_TYPE_BLOCK) {
        int left_edge_x = *thread_num * (image.width / (float)threads_num);

        int right_edge_x =
            (*thread_num + 1) * (image.width / (float)threads_num);

        for (int x = left_edge_x; x < right_edge_x; x++)
            for (int y = 0; y < image.height; y++)
                filter_apply(&filter, &image, &out_image, x, y, EDGE_EXTEND);

    } else if (threads_type == THREADS_TYPE_INTERLEAVED) {
        int x = *thread_num;

        while (x < image.width) {
            for (int y = 0; y < image.height; y++)
                filter_apply(&filter, &image, &out_image, x, y, EDGE_EXTEND);

            x += threads_num;
        }
    }

    struct timeval timestamp_end;

    gettimeofday(&timestamp_end, NULL);

    struct timeval* timestamp_diff = malloc(sizeof(struct timeval));

    timeval_diff(&timestamp_start, &timestamp_end, timestamp_diff);

    pthread_exit(timestamp_diff);
}

int main(int argc, char* argv[]) {
    if (argc != 6)
        return err(
            "Missing arguments threads_num, thread_type, image_path, "
            "filter_path, "
            "out_image_path",
            -1);

    if (parse_int(argv[1], &threads_num) < 0)
        return -1;

    if (parse_int(argv[2], &threads_type) < 0)
        return -1;

    if (threads_type != THREADS_TYPE_BLOCK &&
        threads_type != THREADS_TYPE_INTERLEAVED)
        return err("threads_type must be 0 (block) or 1 (interleaved)", -1);

    image_path = argv[3];

    filter_path = argv[4];

    out_image_path = argv[5];

    printf("Printing args %d %d %s %s %s\n", threads_num, threads_type,
           image_path, filter_path, out_image_path);

    img_load(image_path, &image);

    img_new(&out_image, &image);

    filter_load(filter_path, &filter);

    struct timeval timestamp_start;

    gettimeofday(&timestamp_start, NULL);

    pthread_t* threads = malloc(sizeof(pthread_t) * threads_num);

    int* indexes = malloc(sizeof(int) * threads_num);

    for (int i = 0; i < threads_num; i++) {
        indexes[i] = i;
        pthread_create(&threads[i], NULL, thread_fun, &indexes[i]);
    }

    struct timeval* thread_return;

    for (int i = 0; i < threads_num; i++) {
        pthread_join(threads[i], (void**)&thread_return);

        printf("Thread[%d] with id: %ld finished task in %lds %ldus\n", i,
               threads[i], thread_return->tv_sec, thread_return->tv_usec);

        free(thread_return);
    }

    struct timeval timestamp_end;

    gettimeofday(&timestamp_end, NULL);

    struct timeval timestamp_diff;

    timeval_diff(&timestamp_start, &timestamp_end, &timestamp_diff);

    printf("Program finished task in: %2lds %6ldus\n", timestamp_diff.tv_sec,
           timestamp_diff.tv_usec);

    img_save(out_image_path, &out_image);

    free(threads);
    free(indexes);
    filter_deallocate(&filter);
    img_deallocate(&image);
    img_deallocate(&out_image);

    return 0;
}

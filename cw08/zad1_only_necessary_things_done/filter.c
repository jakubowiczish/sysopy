#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "error.h"

#define THREAD_BLOCK 0
#define THREAD_INTERLEAVED 1

typedef struct image_t {
    int width;
    int height;
    int* data;
} image_t;

typedef struct filter_t {
    int size;
    double* data;
} filter_t;

static int threads_num;
static int thread_type;

static image_t img_in;
static image_t img_out;

static filter_t filter;

filter_t filter_load(const char* filename) {
    filter_t filter;

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        perr("unable to open filter %s", filename);
    }

    if (fscanf(f, "%d", &filter.size) != 1) {
        err("unable to load filter size");
    }

    filter.data = malloc(sizeof(double) * filter.size * filter.size);

    for (int i = 0; i < filter.size * filter.size; ++i) {
        if (fscanf(f, "%lf", &filter.data[i]) != 1) {
            perr("unable to load filter element no. %d", i);
        }
    }

    return filter;
}

void clamp_coords(int* x, int* y, int w, int h) {
    if (*x < 0)
        *x = 0;
    else if (*x >= w)
        *x = w - 1;

    if (*y < 0)
        *y = 0;
    else if (*y >= h)
        *y = h - 1;
}

void filter_apply(image_t img_in,
                  image_t img_out,
                  filter_t filter,
                  int x,
                  int y) {
    int w = img_in.width;
    int h = img_in.height;

    int pixel = 0;
    int i = 0;
    for (int yf = y - filter.size / 2; yf <= y + filter.size / 2; ++yf) {
        for (int xf = x - filter.size / 2; xf <= x + filter.size / 2; ++xf) {
            int xt = xf;
            int yt = yf;
            clamp_coords(&xt, &yt, w, h);

            pixel += filter.data[i] * img_in.data[yt * w + xt];
            ++i;
        }
    }

    if (pixel > 255)
        pixel = 255;
    else if (pixel < 0)
        pixel = 0;

    img_out.data[y * w + x] = pixel;
}

image_t image_new(int width, int height) {
    image_t img;
    img.width = width;
    img.height = height;
    img.data = malloc(sizeof(int) * width * height);
    return img;
}

image_t image_load(const char* filename) {
    image_t img;

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        perr("unable to open image %s", filename);
    }

    if (fgetc(f) != 'P' || fgetc(f) != '2') {
        err("invalid image format");
    }

    if (fscanf(f, "%d %d", &img.width, &img.height) != 2) {
        err("unable to read image dimensions");
    }

    int colors;
    if (fscanf(f, "%d", &colors) != 1) {
        err("unable to read image colors");
    }

    if (colors != 255) {
        err("unable to read image with %d colors", colors);
    }

    img.data = malloc(sizeof(int) * img.width * img.height);

    for (int i = 0; i < img.width * img.height; ++i) {
        if (fscanf(f, "%d", &img.data[i]) != 1) {
            perr("unable to read pixel no. %d", i);
        }
    }

    fclose(f);
    return img;
}

void image_save(const char* filename, image_t img) {
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        perr("unable to open file to save %s", filename);
    }
    fprintf(f, "P2\n%d %d\n255\n", img.width, img.height);

    for (int i = 0; i < img.width * img.height; ++i) {
        if (i % img.width == 0) {
            fprintf(f, "\n");
        }

        fprintf(f, "%d ", img.data[i]);
    }

    fclose(f);
}

void* thread_worker(void* _thread_no) {
    int t = *(int*)_thread_no;

    struct timeval st;
    gettimeofday(&st, NULL);

    if (thread_type == THREAD_BLOCK) {
        int block_size = (float)img_in.width / (float)threads_num;
        int left = t * block_size;
        int right = (t + 1) * block_size;

        for (int x = left; x < right; ++x) {
            for (int y = 0; y < img_in.height; ++y) {
                filter_apply(img_in, img_out, filter, x, y);
            }
        }
    } else if (thread_type == THREAD_INTERLEAVED) {
        for (int x = t; x < img_in.width; x += threads_num) {
            for (int y = 0; y < img_in.height; ++y) {
                filter_apply(img_in, img_out, filter, x, y);
            }
        }
    }

    struct timeval et;
    gettimeofday(&et, NULL);

    struct timeval* diff = malloc(sizeof(struct timeval));

    if (et.tv_usec - st.tv_usec < 0) {
        diff->tv_sec = et.tv_sec - st.tv_sec - 1;
        diff->tv_usec = 1000000 + et.tv_usec - st.tv_usec;
    } else {
        diff->tv_sec = et.tv_sec - st.tv_sec;
        diff->tv_usec = et.tv_usec - st.tv_usec;
    }

    return diff;
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        err("usage: %s threads_num thread_type image filter out_image",
            argv[0]);
    }

    if (sscanf(argv[1], "%d", &threads_num) != 1) {
        err("invalid threads_num");
    }

    if (strcmp(argv[2], "block") == 0) {
        thread_type = THREAD_BLOCK;
    } else if (strcmp(argv[2], "interleaved") == 0) {
        thread_type = THREAD_INTERLEAVED;
    } else {
        err("invalid thread_type (block/interleaved)");
    }

    printf("applying %s to %s using %d threads in %s mode\n", argv[4], argv[3],
           threads_num, thread_type == THREAD_BLOCK ? "block" : "interleaved");

    img_in = image_load(argv[3]);
    img_out = image_new(img_in.width, img_in.height);
    filter = filter_load(argv[4]);

    pthread_t* threads = malloc(sizeof(pthread_t) * threads_num);
    int* indexes = malloc(sizeof(int) * threads_num);

    struct timeval st;
    gettimeofday(&st, NULL);

    for (int i = 0; i < threads_num; ++i) {
        indexes[i] = i;
        pthread_create(&threads[i], NULL, thread_worker, &indexes[i]);
    }

    struct timeval* elapsed;
    for (int i = 0; i < threads_num; ++i) {
        pthread_join(threads[i], (void**)&elapsed);
        printf("thread %ld finished in %2lds %06ldus\n", threads[i],
               elapsed->tv_sec, elapsed->tv_usec);
        free(elapsed);
    }

    struct timeval et;
    gettimeofday(&et, NULL);

    struct timeval diff;

    if (et.tv_usec - st.tv_usec < 0) {
        diff.tv_sec = et.tv_sec - st.tv_sec - 1;
        diff.tv_usec = 1000000 + et.tv_usec - st.tv_usec;
    } else {
        diff.tv_sec = et.tv_sec - st.tv_sec;
        diff.tv_usec = et.tv_usec - st.tv_usec;
    }

    printf("program finished in %2lds %06ldus\n\n", diff.tv_sec, diff.tv_usec);

    free(img_in.data);
    free(filter.data);
    free(threads);
    free(indexes);

    image_save(argv[5], img_out);
    free(img_out.data);
    return 0;
}

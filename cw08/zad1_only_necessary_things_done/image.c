#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

#define THREAD_BLOCK 0
#define THREAD_INTERLEAVED 1

static int thread_num;

static int thread_type;

static char* filename_image;

static char* filename_filter;

static char* filename_out_image;

typedef struct image_t {
    int width;
    int height;
    int* data;
} image_t;

typedef struct filter_t {
    int size;
    int* data;
} filter_t;

filter_t filter_load(const char* filename) {
    filter_t filter;

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        perr("unable to open filter %s", filename);
    }

    if (fscanf(f, "%d", &filter.size) != 1) {
        err("unable to load filter size");
    }

    filter.data = malloc(sizeof(int) * filter.size * filter.size);

    for (int i = 0; i < filter.size * filter.size; ++i) {
        if (fscanf(f, "%d", &filter.data[i]) != 1) {
            perr("unable to load filter element no. %d", i);
        }
    }

    return filter;
}

image_t image_load(const char* filename) {
    image_t img;

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        perr("unable to open image %s", filename);
    }

    if (fgetc(f) != 'P' || fgetc(f) != '2') {
        err("invalid image format.sh");
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

int main(int argc, char* argv[]) {
    if (argc != 6) {
        err("usage: %s thread_num thread_type image filter out_image", argv[0]);
    }

    if (sscanf(argv[1], "%d", &thread_num) != 1) {
        err("invalid thread_num");
    }

    if (strcmp(argv[2], "block") == 0) {
        thread_type = THREAD_BLOCK;
    } else if (strcmp(argv[2], "interleaved") == 0) {
        thread_type = THREAD_INTERLEAVED;
    } else {
        err("invalid thread_type (block/interleaved)");
    }

    filename_image = argv[3];
    filename_filter = argv[4];
    filename_out_image = argv[5];

    image_t img = image_load(filename_image);
    filter_t filter = filter_load(filename_filter);

    image_save(filename_out_image, img);
    free(filter.data);
    free(img.data);

    return 0;
}

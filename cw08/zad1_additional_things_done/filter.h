#ifndef SYSOPY_FILTER_H
#define SYSOPY_FILTER_H

#include "image.h"

#define EDGE_EXTEND (edge_mode_t)1

typedef int edge_mode_t;

typedef struct filter_t {
    double *array;
    int size;
    int normalized;
} filter_t;


int filter_allocate(filter_t *filter, int size);

int filter_deallocate(filter_t *filter);

int filter_load(const char *path, filter_t *out);

int filter_save(const char *path, filter_t *filter);

int filter_normalize(filter_t *filter);

int filter_apply(filter_t *filter, img_t *image_in, img_t *image_out, int x, int y, edge_mode_t edge_mode);


#endif //SYSOPY_FILTER_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "filter.h"
#include "gaussian_filter.h"
#include "image.h"

int main() {
    img_t image;
    img_load("images/lena.ascii.pgm", &image);

    img_t image_out;
    img_new(&image_out, &image);

    filter_t filter;

    filter_load("filters/emboss.txt", &filter);

    filter_normalize(&filter);

    for (int y = 0; y < image.height; y++)
        for (int x = 0; x < image.width; x++)
            filter_apply(&filter, &image, &image_out, x, y, EDGE_EXTEND);

    img_save("images_out/lena.ascii.pgm", &image_out);

    img_deallocate(&image);
    img_deallocate(&image_out);
    filter_deallocate(&filter);

    return 0;
}
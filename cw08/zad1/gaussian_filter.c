#define _USE_MATH_DEFINES

#include "gaussian_filter.h"
#include <math.h>

int gauss_filter_generate(double sigma, int size, filter_t *filter) {
    if (size % 2 == 0)
        return -1;

    if (filter->size != size)
        return -1;

    double r, s = 2.0 * sigma * sigma;

    int offset = size / 2;

    int index = 0;

    for (int y = -offset; y <= offset; y++)
        for (int x = -offset; x <= offset; x++) {

            r = sqrt(x * x + y * y);

            filter->array[index] = (exp(-(r * r) / s)) / (M_PI * s);

            index++;
        }

    return 0;
}
#ifndef SYSOPY_GAUSSIAN_FILTER_H
#define SYSOPY_GAUSSIAN_FILTER_H


#include "filter.h"

int gauss_filter_generate(double sigma, int size, filter_t *filter);

#endif //SYSOPY_GAUSSIAN_FILTER_H

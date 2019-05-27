#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "filter.h"
#include "gaussian_filter.h"

int main() {
    char buffer[255];

    for (int i = 3; i <= 65; i += 2) {
        filter_t filter_gauss;

        filter_allocate(&filter_gauss, i);

        double sigma = 1 + (i - 3) / (double)(65 - 3) * 9;

        gauss_filter_generate(sigma, i, &filter_gauss);

        filter_normalize(&filter_gauss);

        snprintf(buffer, 255, "generated_filters/gauss_%d.txt", i);

        filter_save(buffer, &filter_gauss);

        filter_deallocate(&filter_gauss);
    }

    return 0;
}
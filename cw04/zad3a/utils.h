#ifndef SYSOPY_UTILS_H
#define SYSOPY_UTILS_H
#include <signal.h>
#include <stdio.h>

void initialize_signals(void(*fun)(int, siginfo_t *, void *), int is_rt);

#endif //SYSOPY_UTILS_H

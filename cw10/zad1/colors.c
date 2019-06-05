#include "colors.h"
#include <stdarg.h>
#include <stdio.h>
void set_color(char* color) {
    printf("%s", color);
}
void reset_color() {
    printf("%s\n", ANSI_COLOR_RESET);
}
void colorprintf(char* color, char* format, ...) {
    set_color(color);
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
    reset_color();
}
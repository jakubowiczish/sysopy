#include <stdio.h>
#include "lib.c"

int main(int argc, char **argv) {
    initialize_result_array(100);
    initialize_searching_array(100);
    printf("%d", search_directory("/home/jakub/Downloads", "płotnikowski.jpg", "tmp.txt"));
}
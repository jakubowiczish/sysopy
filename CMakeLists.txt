cmake_minimum_required(VERSION 3.13)
project(sysopy C)

set(CMAKE_C_STANDARD 11)

add_executable(
        sysopy cw03/zad1/main.c cw03/zad2/main.c cw04/zad1a/main.c cw04/zad1b/main.c cw04/zad3a/sender.c cw04/zad3a/utils.h cw04/zad3a/utils.c cw04/zad3a/catcher.c
        cw04/zad3b/catcher.c cw04/zad3b/utils.c cw04/zad3b/utils.h cw04/zad3b/sender.c cw05/lab/main.c cw05/zad1/main.c)
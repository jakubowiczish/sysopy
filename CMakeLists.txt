cmake_minimum_required(VERSION 3.13)
project(sysopy C)

set(CMAKE_C_STANDARD 11)

add_executable(sysopy cw01/zad2/main.c
        cw01/zad2/lib.h
        cw01/zad2/lib.c
        cw01/zad2/lib_dynamic.h
        cw01/zad3/zad3a/lib.c
        cw01/zad3/zad3a/lib.h
        cw01/zad3/zad3a/lib_dynamic.h
        cw01/zad3/zad3a/main.c
        cw01/zad3/zad3b/lib.c
        cw01/zad3/zad3b/lib.h
        cw01/zad3/zad3b/lib_dynamic.h
        cw01/zad3/zad3b/main.c
        cw02/zad1/main.c cw02/zad2/main.c)
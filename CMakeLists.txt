cmake_minimum_required(VERSION 3.13)
project(sysopy C)

set(CMAKE_C_STANDARD 11)

add_executable(sysopy cw01/zad2/main.c cw01/zad2/lib.h cw01/zad2/lib.c cw01/zad2/lib_dynamic.c cw01/zad2/main_dynamic.c)
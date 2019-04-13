cmake_minimum_required(VERSION 3.13)
project(sysopy C)

set(CMAKE_C_STANDARD 11)

add_executable(
        sysopy cw03/zad1/main.c
        cw03/zad2/main.c
        cw04/zad1a/main.c
        cw04/zad1b/main.c
        cw04/zad3a/sender.c
        cw04/zad3a/utils.h
        cw04/zad3a/utils.c
        cw04/zad3a/catcher.c
        cw04/zad3b/catcher.c
        cw04/zad3b/utils.c
        cw04/zad3b/utils.h
        cw04/zad3b/sender.c
        cw05/lab/main.c
        cw05/zad1/main.c
        cw05/zad2/master.c
        cw05/zad2/utils.c
        cw05/zad2/utils.h
        cw05/zad2/slave.c
        "tests/KolosEom/zad1(ok)/main.c"
        tests/KozlakSroda9:35/zad1/main.c
        "tests/KozlakSroda9:35/zad2(ok)/main.c"
        "tests/KozlakSroda9:35/zad3(ok)/main.c"
        tests/SiwikWtorek14:40/zad1/main.c
        "tests/SiwikWtorek14:40/zad2(ok) - to samo co KozlakSroda9:35zad3/main.c"
        tests/SiwikWtorek14:40/zad3/main.c
        tests/KolosEom/zad2/main.c
        "tests/KolosEom/zad3(ok)/main.c"

)
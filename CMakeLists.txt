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
        "tests16_17/KolosEom/zad1(ok)/main.c"
        "tests16_17/KozlakSroda9:35/zad1(ok)/main.c"
        "tests16_17/KozlakSroda9:35/zad2(ok)/main.c"
        "tests16_17/KozlakSroda9:35/zad3(ok)/main.c"
        "tests16_17/SiwikWtorek14:40/zad1(ok)/main.c"
        "tests16_17/SiwikWtorek14:40/zad2(ok) - to samo co KozlakSroda9:35zad3/main.c"
        "tests16_17/SiwikWtorek14:40/zad3(ok)/main.c"
        "tests16_17/KolosEom/zad2(ok)/main.c"
        "tests16_17/KolosEom/zad3(ok)/main.c"
        "tests16_17/kolospoprawkowyzforum/zad2/sender.c"
        "tests16_17/kolospoprawkowyzforum/zad2/receiver.c"


)
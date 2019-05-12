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
        "tests_17_18/test_1/KolosEom/zad1(ok)/main.c"
        "tests_17_18/test_1/KozlakSroda9:35/zad1(ok)/main.c"
        "tests_17_18/test_1/KozlakSroda9:35/zad2(ok)/main.c"
        "tests_17_18/test_1/KozlakSroda9:35/zad3(ok)/main.c"
        "tests_17_18/test_1/SiwikWtorek14:40/zad1(ok)/main.c"
        "tests_17_18/test_1/SiwikWtorek14:40/zad2(ok) - to samo co KozlakSroda9:35zad3/main.c"
        "tests_17_18/test_1/SiwikWtorek14:40/zad3(ok)/main.c"
        "tests_17_18/test_1/KolosEom/zad2(ok)/main.c"
        "tests_17_18/test_1/KolosEom/zad3(ok)/main.c"
        "tests_17_18/test_1/kolospoprawkowyzforum/zad2(ok)/sender.c"
        "tests_17_18/test_1/kolospoprawkowyzforum/zad2(ok)/receiver.c"
        "tests_17_18/test_1/kolospoprawkowyzforum/zad3(ok)/main.c"
        "tests_18_19/mytest/zad1(ok)/main.c"
        "tests_18_19/mytest/zad2(ok)/main.c"
        "tests_18_19/mytest/zad3(ok)/main.c"

        cw06/zad1/client.c
        cw06/zad1/helper.h
        cw06/zad1/server.c
        cw06/zad2/client.c
        cw06/zad2/helper.h
        cw06/zad2/server.c
        cw07/lab/main.c
        cw07/zad1/error.c
        cw07/zad1/error.h
        cw07/zad1/shared.h
        cw07/zad1/systemv.c
        cw07/zad1/conveyor_belt.h
        cw07/zad1/conveyor_belt.c
        cw07/zad1/pack.h
        cw07/zad1/pack.c
        cw07/zad1/loader.c
        cw07/zad1/utils.h
        cw07/zad1/utils.c
        cw07/zad1/spawner.c
        cw07/zad1/trucker.c
        cw07/zad2/posix.c
)
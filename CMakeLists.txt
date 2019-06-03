cmake_minimum_required(VERSION 3.13)
project(sysopy C)

set(CMAKE_C_STANDARD 11)

add_executable(
        sysopy

        cw01/zad1/lib.c
        cw01/zad1/lib.h
        cw01/zad2/lib.c
        cw01/zad2/lib.h
        cw01/zad2/lib_dynamic.h
        cw01/zad2/main.c
        cw01/zad3/zad3a/lib.c
        cw01/zad3/zad3a/lib.h
        cw01/zad3/zad3a/lib_dynamic.h
        cw01/zad3/zad3a/main.c
        cw01/zad3/zad3b/lib.c
        cw01/zad3/zad3b/lib.h
        cw01/zad3/zad3b/lib_dynamic.h
        cw01/zad3/zad3b/main.c

        cw02/zad1/main.c
        cw02/zad2/main.c

        cw03/zad1/main.c
        cw03/zad2/main.c

        cw04/zad1a/main.c
        cw04/zad1b/main.c
        cw04/zad2/error.h
        cw04/zad2/error.c
        cw04/zad2/tester.c
        cw04/zad2/monitor.c
        cw04/zad2/backup.c
        cw04/zad2/backup.h
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

        "KOLOSY_SOLUSZYNS/tests_17_18/test_1/KozlakSroda9:35/zad1(ok)/main.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_1/KozlakSroda9:35/zad2(ok)/main.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_1/KozlakSroda9:35/zad3(ok)/main.c"

        "KOLOSY_SOLUSZYNS/tests_17_18/test_1/SiwikWtorek14:40/zad1(ok)/main.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_1/SiwikWtorek14:40/zad2(ok) - to samo co KozlakSroda9:35zad3/main.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_1/SiwikWtorek14:40/zad3(ok)/main.c"

        "KOLOSY_SOLUSZYNS/tests_17_18/test_1/KolosEom/zad1(ok)/main.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_1/KolosEom/zad2(ok)/main.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_1/KolosEom/zad3(ok)/main.c"

        "KOLOSY_SOLUSZYNS/tests_17_18/test_1/kolospoprawkowyzforum/zad2(ok)/sender.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_1/kolospoprawkowyzforum/zad2(ok)/receiver.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_1/kolospoprawkowyzforum/zad3(ok)/main.c"

        "KOLOSY_SOLUSZYNS/tests_18_19/mytest/zad1(ok)/main.c"
        "KOLOSY_SOLUSZYNS/tests_18_19/mytest/zad2(ok)/main.c"
        "KOLOSY_SOLUSZYNS/tests_18_19/mytest/zad3(ok)/main.c"

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

        cw08/zad1_additional_things_done/utils.c
        cw08/zad1_additional_things_done/utils.h
        cw08/zad1_additional_things_done/errors.c
        cw08/zad1_additional_things_done/errors.h
        cw08/zad1_additional_things_done/image.h
        cw08/zad1_additional_things_done/image.c
        cw08/zad1_additional_things_done/filter.h
        cw08/zad1_additional_things_done/filter.c
        cw08/zad1_additional_things_done/filters_generator.c
        cw08/zad1_additional_things_done/gaussian_filter.c
        cw08/zad1_additional_things_done/gaussian_filter.h
        cw08/zad1_additional_things_done/main.c
        cw08/zad1_additional_things_done/test.c

        cw08/zad1_only_necessary_things_done/error.h
        cw08/zad1_only_necessary_things_done/error.c
        cw08/zad1_only_necessary_things_done/filter.c

        cw09/zad1/error.h
        cw09/zad1/error.c
        cw09/zad1/roller_coaster.c

        "KOLOSY_SOLUSZYNS/tests_17_18/test_2/EOM(ok)/zad1(ok)/main.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_2/EOM(ok)/zad2(ok)/main.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_2/EOM(ok)/zad3(ok)/main.c"

        "KOLOSY_SOLUSZYNS/tests_17_18/test_2/JK(ok)/zad1(ok)/zad1.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_2/JK(ok)/zad2(ok)/zad2.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_2/JK(ok)/zad3(ok)/main.c"

        "KOLOSY_SOLUSZYNS/tests_17_18/test_2/S_1(ok)/zad1(ok)/main.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_2/S_1(ok)/zad2(ok)/sender.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_2/S_1(ok)/zad2(ok)/receiver.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_2/S_1(ok)/zad3(ok)/main.c"

        "KOLOSY_SOLUSZYNS/tests_17_18/test_2/S_2(ok)/zad1(ok)/main.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_2/S_2(ok)/zad2(ok)/sender.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_2/S_2(ok)/zad2(ok)/receiver.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_2/S_2(ok)/zad3(ok)/sender.c"
        "KOLOSY_SOLUSZYNS/tests_17_18/test_2/S_2(ok)/zad3(ok)/receiver.c"
)
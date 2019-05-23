#!/usr/bin/env bash
rm -f times.txt
touch times.txt

make filters_generator
./filters_generator

make main

for method in 0 1
do
    if (( $method == 0 )) ; then
        printf "%s\n" " === BLOCK METHOD === " >> times.txt
    else
        printf "%s\n" " === INTERLEAVED METHOD === " >> times.txt
    fi

    for gauss_size in $(seq 3 8 65)
    do
        printf " --- Kernel size: $gauss_size --- \n" $ >> times.txt
        if (( $gauss_size <= 20 )) ; then
            THREAD_MAX_POWER=12
        else
            THREAD_MAX_POWER=3
        fi
        for thread_num_power in $(seq 0 $THREAD_MAX_POWER)
        do
            thread_num=$((2**$thread_num_power))
            output=$(./main $thread_num $method images/lena.ascii.pgm \
                $(printf "generated_filters/gauss_%d.txt" "$gauss_size") \
                $(printf "images_out/output_%d.pgm" "$gauss_size") \
            | tail -1)
            value=${output#*:}
            printf "Threads num: %4d Time: %s \n" $thread_num "$value" >> times.txt
        done
    done
    printf "\n\n" >> times.txt
done
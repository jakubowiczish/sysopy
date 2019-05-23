CC = gcc
CFLAGS = -Wall
HEADERS = filter.h gaussian_filter.h image.h utils.h errors.h
SRC = filter.c image.c utils.c errors.c

.PHONY: clean

all : main filters_generator

main : $(SRC) $(HEADERS) main.c
	$(CC) $(CFLAGS) $(SRC) main.c -o main -lm -pthread

filters_generator : $(SRC) $(HEADERS) filters_generator.c gaussian_filter.h gaussian_filter.c
	$(CC) $(CFLAGS) $(SRC) filters_generator.c gaussian_filter.c -o filters_generator -lm -pthread

test : $(SRC) $(HEADERS) test.c
	$(CC) $(CFLAGS) $(SRC) test.c -o test -lm -pthread

clean :
	rm -f main
	rm -f filters_generator
	rm -rf generated_filters/*
	rm -rf images_out/*
	rm -f test

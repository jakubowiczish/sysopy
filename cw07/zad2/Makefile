CC = gcc

FLAGS = -Wall -O3 -std=c99 -I$(D) -DPOSIX -lrt -pthread

HEADERS = $(D)/shared.h $(D)/conveyor_belt.h $(D)/pack.h $(D)/error.h $(D)/utils.h

SRC_FILES = posix.c $(D)/conveyor_belt.c $(D)/pack.c $(D)/error.c $(D)/utils.c

D = ../zad1

.DEFAULT: all

all: trucker loader spawner

trucker: $(SRC_FILES) $(HEADERS) $(D)/trucker.c
	$(CC) $(FLAGS) $(SRC_FILES) $(D)/trucker.c -o trucker

loader: $(SRC_FILES) $(HEADERS) $(D)/loader.c
	$(CC) $(FLAGS) $(SRC_FILES) $(D)/loader.c -o loader

spawner: $(SRC_FILES) $(HEADERS) $(D)/spawner.c
	$(CC) $(FLAGS) $(SRC_FILES) spawner.c -o spawner


clean:
	rm -f trucker loader spawner
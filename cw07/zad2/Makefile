D = ../zad1

CC = gcc

FLAGS = -Wall -O3 -std=c99 -I$(D) -DPOSIX

OTHER_FLAGS = -lrt -pthread

HEADERS = $(D)/shared.h $(D)/conveyor_belt.h $(D)/pack.h $(D)/error.h $(D)/utils.h

SRC_FILES = posix.c $(D)/conveyor_belt.c $(D)/pack.c $(D)/error.c $(D)/utils.c

.DEFAULT: all

all: trucker loader spawner

trucker: $(SRC_FILES) $(HEADERS) $(D)/trucker.c
	$(CC) $(FLAGS) $(SRC_FILES) $(D)/trucker.c -o trucker $(OTHER_FLAGS)

loader: $(SRC_FILES) $(HEADERS) $(D)/loader.c
	$(CC) $(FLAGS) $(SRC_FILES) $(D)/loader.c -o loader $(OTHER_FLAGS)

spawner: $(SRC_FILES) $(HEADERS) $(D)/spawner.c
	$(CC) $(FLAGS) $(SRC_FILES) $(D)/spawner.c -o spawner $(OTHER_FLAGS)


run_trucker: trucker
	./trucker 20 5 10

run_spawner: spawner
	./spawner 5 5


clean:
	rm -f trucker loader spawner
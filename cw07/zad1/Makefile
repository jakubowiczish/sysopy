CC = gcc

FLAGS = -Wall -O3 -std=c99

HEADERS = shared.h queue.h pack.h error.h utils.h

SRC_FILES = systemv.c queue.c pack.c error.c utils.c

.DEFAULT: all

all: trucker loader spawner

trucker: $(SRC_FILES) $(HEADERS) trucker.c
	$(CC) $(FLAGS) $(SRC_FILES) trucker.c -o trucker

loader: $(SRC_FILES) $(HEADERS) loader.c
	$(CC) $(FLAGS) $(SRC_FILES) loader.c -o loader

spawner: $(SRC_FILES) $(HEADERS) spawner.c
	$(CC) $(FLAGS) $(SRC_FILES) loader.c -o spawner


clean:
	rm -f trucker loader spawner
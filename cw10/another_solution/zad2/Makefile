D       := ../zad1

CC      := gcc
CFLAGS  := -Wall -O3 -std=c99 -I$(D)
HEADERS := $(D)/error.h proto.h
SRC     := $(D)/error.c proto.c

.PHONY: $(OUT) all client server test clean
.DEFAULT: all

all: client server

client: $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) client.c -o client

server: $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) server.c -o server

test: client server
	./server

clean:
	$(RM) client server

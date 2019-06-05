CC = gcc
CFLAGS = -Wall
HEADERS = 
SRC = 

.PHONY: clean

all : main

client : $(SRC) $(HEADERS) ../zad1 ../../zad1 ../../zad1 ../../zad1 ../../zad1
	$(CC) $(CFLAGS) $(SRC) ../zad1/client.c ../zad1/queue.c ../zad1/errors.c ../zad1/utils.c ../zad1/colors.c -D UDP -o client -pthread -lm

server : $(SRC) $(HEADERS) ../zad1 ../../zad1 ../../zad1 ../../zad1
	$(CC) $(CFLAGS) $(SRC) ../zad1/server.c ../zad1/errors.c ../zad1/utils.c ../zad1/colors.c -D UDP -o server -pthread -lm

clean :
	rm -f client
	rm -f server


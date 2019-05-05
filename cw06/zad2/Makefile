CC = gcc -Wall -ggdb

all: server client

server: server.c
	$(CC) server.c -o server -lrt

client: client.c
	$(CC) client.c -o client -lrt

clean:
	rm -rf *.o server test_server client test_client
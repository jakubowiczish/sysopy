CC = gcc -Wall -ggdb

all: server client


client: client.c
	$(CC) client.c -o ./client

server: server.c
	$(CC) server.c -o ./server

test_server: server.c
	gcc ./server.c -ggdb -Wall -0O -o ./test_server

test_client: client.c
	gcc ./client.c -ggdb -Wall -O0 -o ./test_client

clean:
	rm -rf *.o server test_server client test_client

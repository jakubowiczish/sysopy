CC = gcc -Wall -ggdb

out_file: main.c
	$(CC) -o out_file main.c

test: out_sender
	./out_file

clean: out_sender
	rm out_file
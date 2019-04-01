CC = gcc -Wall -ggdb

out_sender: sender.c
	$(CC) -o out_sender sender.c utils.c


test: out_sender
	./out_sender

clean: out_sender
	rm out_sender
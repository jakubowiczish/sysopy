CC = gcc -Wall -ggdb

out_sender: sender.c
	$(CC) -o out_sender sender.c utils.c

out_catcher: catcher.c
	$(CC) -o out_catcher catcher.c utils.c

make catcher_and_sender:
	make out_sender
	make out_catcher

clean: out_sender out_catcher
	rm out_sender
	rm out_catcher
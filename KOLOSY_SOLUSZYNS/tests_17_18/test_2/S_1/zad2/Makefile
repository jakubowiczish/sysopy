CFLAGS = -Wall -lrt
CC = gcc

sender: sender.c
	$(CC) $^ -o $@ $(CFLAGS)

receiver: receiver.c
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm -f sender.o sender receiver.o receiver

test:  sender receiver 
	@echo -e "\nTESTING\n"
	./sender 22 &
	./receiver
	./sender 9 &
	./receiver
	./sender -13 &
	./receiver
	./sender cos &
	./receiver
all: sender receiver

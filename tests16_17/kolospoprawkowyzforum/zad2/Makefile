CFLAGS = -Wall
CC = gcc

sender: sender.o

receiver: receiver.o

clean:
	rm -f sender.o sender receiver.o receiver squareFIFO

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

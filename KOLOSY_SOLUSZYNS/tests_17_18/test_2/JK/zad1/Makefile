CFLAGS = -Wall
CC = gcc -pthread

main: zad1.o

clean:
	rm -f ./zad1.o ./zad1

test:  zad1
	@echo -e "\n\nTESTING\n\n"
	./zad1 4 3 2
	@echo -e "====================================================================\n\n"
	./zad1 5 5 
	@echo -e "====================================================================\n\n"
	./zad1 1 1 1 1 1
all: zad1 test

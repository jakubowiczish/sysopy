CFLAGS = -Wall
CC = gcc

main: main.o

clean:
	rm -f main.o main

test:  main
	@echo -e "\nTESTING\n"
	./main 5 
	./main 7
all: main test

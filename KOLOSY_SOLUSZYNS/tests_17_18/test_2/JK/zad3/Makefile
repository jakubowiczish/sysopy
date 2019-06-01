CFLAGS = -Wall
CC = gcc

main: main.o

clean:
	rm -f main.o main

test:  main
	@echo -e "\nTESTING\n"
	@rm -f ./us
	./main ./us
	@ls -l ./us
	@rm -f ./us

all: main test


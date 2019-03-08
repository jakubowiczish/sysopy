CC = gcc -Wall -std=c11 -ggdb

all:

static: lib.c
	$(CC) -c lib.c
	ar rcs lib.a lib.o

shared:
	$(CC) -c -fPIC lib.c
	$(CC) -o lib.so -fPIC -shared lib.o

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.gch
	rm -f *.so
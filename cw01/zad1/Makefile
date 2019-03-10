CC = gcc -Wall -std=c11 -ggdb
LIB_O = lib.o
LIB__STATIC = lib.a
LIB_SO = lib.so

create:
	$(CC) -c -fPIC -o $(LIB_O) lib.c

static: lib.c
	make create
	ar rcs $(LIB_SO) $(LIB_O)

shared:
	make create
	$(CC) -o $(LIB_SO) -fPIC -shared $(LIB_O)

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.gch
	rm -f *.so
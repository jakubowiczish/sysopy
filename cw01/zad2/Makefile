CC = gcc -Wall -std=c11 -ggdb

all:


static: lib.c
	$(CC) -c lib.c
	ar rcs lib.a lib.o


shared:
	$(CC) -c -fPIC lib.c
	$(CC) -o lib.so -fPIC -shared lib.o


run_static:
	make static
	$(CC) -o test.out main.c lib.o
	make tests_static


tests_static:
	# short
	./test.out create_table 10 search_directory /usr lib tmp_1.txt remove_block 0
	# medium
	./test.out create_table 100 search_directory /usr/lib xorg tmp_2.txt remove_block 0
	# long
	./test.out create_table 10000 search_directory /usr systemd-bootx64.efi tmp_3.txt remove_block 0


clean:
	rm -f *.o
	rm -f *.a
	rm -f *.gch
	rm -f *.so
	rm -f *.txt
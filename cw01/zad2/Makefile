CC = gcc -Wall -std=c11 -ggdb

SRC = main.c
SRC_DYNAMIC = main_dynamic.c
LIB = lib.c
LIB_A = lib.a
LIB_O = lib.o
LIB_DYNAMIC = lib_dynamic.c
OUT_STATIC = out_static
OUT_DYNAMIC = out_dynamic


static: lib.c
	$(CC) -c $(LIB)
	ar rcs $(LIB_A) $(LIB_O)


shared:
	$(CC) -c -fPIC lib_dynamic.c
	$(CC) -o lib.so -fPIC -shared lib_sh.o

run_static:
	make static
	$(CC) -o $(OUT_STATIC) $(SRC) $(LIB_O)
	make tests_static

run_dynamic:
	make dynamic
	./$(OUT_DYNAMIC)
	make tests_static

run_shared:
	make shared
	$(CC) -o test_sh.out main_dynamic.c lib_sh.o

tests_static:
	# short
	./out_static create_table 10 search_directory /usr lib tmp_1.txt remove_block 0
	# medium
	./out_static create_table 100 search_directory /usr/lib xorg tmp_2.txt remove_block 0
	# long
	./out_static create_table 10000 search_directory /usr systemd-bootx64.efi tmp_3.txt remove_block 0


clean:
	rm -f *.o
	rm -f *.a
	rm -f *.gch
	rm -f *.so
	rm -f *.txt
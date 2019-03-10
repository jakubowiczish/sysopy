CC = gcc -Wall -std=c11 -ggdb

SRC = main.c
SRC_DYNAMIC = main_dynamic.c
LIB_NAME = lib_name
LIB_C = lib.c
LIB_A = lib.a
LIB_O = lib.o
LIB_SO = lib.so
LIB_DYNAMIC_O = lib_dynamic.o
LIB_DYNAMIC = lib_dynamic.c
OUT_STATIC = out_static
OUT_SHARED = out_shared
OUT_DYNAMIC = out_dynamic


static: lib.c
	$(CC) -c $(LIB_C)
	ar rcs $(LIB_A) $(LIB_O)

shared:
	$(CC) -c -fPIC -o $(LIB_O) $(LIB_C)
	$(CC) -o $(LIB_SO) -fPIC -shared $(LIB_O)
#	$(CC) -shared –W1,-soname,lib.so -o lib.so.1.0.1 $(LIB_O) -lc
#	ln -s lib.so.1.0.1 lib.so.1
#	ln -s lib.so.1 lib.so

run_static:
	make static
	$(CC) -o $(OUT_STATIC) $(SRC) $(LIB_O)
	make tests_static

run_shared:
	make shared
#	sudo cp lib.so.1.0.1 /usr/lib
#	sudo rm -f /usr/lib/lib.so.1
#	sudo rm -f /usr/lib/lib.so
#	sudo ln -s /usr/lib.so.1.0.1 /url/lib.so.1
#	sudo ln -s /usr/lib.so.1 /url/lib.so
#	sudo ldconfig
	$(CC) main.c $(LIB_NAME) -L ./ -o $(OUT_SHARED)
	make tests_shared

tests_static:
	# short
	./out_static create_table 10 search_directory /usr lib tmp_1.txt remove_block 0
	# medium
	./out_static create_table 100 search_directory /usr/lib xorg tmp_2.txt remove_block 0
	# long
	./out_static create_table 10000 search_directory /usr systemd-bootx64.efi tmp_3.txt remove_block 0

tests_shared:
	# short
	./out_shared create_table 10 search_directory /usr lib tmp_1.txt remove_block 0
	# medium
	./out_shared create_table 100 search_directory /usr/lib xorg tmp_2.txt remove_block 0
	# long
	./out_shared create_table 10000 search_directory /usr systemd-bootx64.efi tmp_3.txt remove_block 0

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.gch
	rm -f *.so
	rm -f *.txt
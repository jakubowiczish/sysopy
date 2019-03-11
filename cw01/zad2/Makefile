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

shared: lib.c
	$(CC) -c -fPIC $(LIB_C)
	gcc -shared -fPIC lib.c -o libfind.so

run_static:
	make static
	$(CC) -o $(OUT_STATIC) $(SRC) $(LIB_O)
	make tests_static

run_shared:
	make shared
	gcc -Wall -std=c99 -g main.c -L./ -lfind -o out_shared
	make tests_shared

run_dynamic:
	$(CC) -o out_dynamic main.c -ldl -D DYNAMIC
	make tests_dynamic

run_all:
	make run_static
	make run_shared
	make run_dynamic

tests_static:
	./out_static create_table 10 search_directory /usr lib tmp_1.txt remove_block 0
	./out_static create_table 100 search_directory /usr/lib xorg tmp_2.txt remove_block 0
	./out_static create_table 10000 search_directory /usr systemd-bootx64.efi tmp_3.txt remove_block 0
	./out_static create_table 1 create_table 10 create_table 100 create_table 1000 create_table 10000

tests_shared:
	LD_LIBRARY_PATH=./ ./out_shared create_table 10 search_directory /usr lib tmp_1.txt remove_block 0
	LD_LIBRARY_PATH=./ ./out_shared create_table 100 search_directory /usr/lib xorg tmp_2.txt remove_block 0
	LD_LIBRARY_PATH=./ ./out_shared create_table 10000 search_directory /usr systemd-bootx64.efi tmp_3.txt remove_block 0
	LD_LIBRARY_PATH=./ ./out_shared create_table 1 create_table 10 create_table 100 create_table 1000 create_table 10000


tests_dynamic:
	LD_LIBRARY_PATH=./ ./out_dynamic create_table 10 search_directory /usr lib tmp_1.txt remove_block 0
	LD_LIBRARY_PATH=./ ./out_dynamic create_table 100 search_directory /usr/lib xorg tmp_2.txt remove_block 0
	LD_LIBRARY_PATH=./ ./out_dynamic create_table 10000 search_directory /usr systemd-bootx64.efi tmp_3.txt remove_block 0
	LD_LIBRARY_PATH=./ ./out_dynamic create_table 1 create_table 10 create_table 100 create_table 1000 create_table 10000


clean:
	rm -f *.o
	rm -f *.a
	rm -f *.gch
	rm -f *.so
	rm -f *.txt
	rm -f *.so.*

clean_all:
	make clean
	rm -f out_static
	rm -f out_shared
	rm -f out_dynamic


	#	sudo cp libcw01zad2.so.1.0.1 /usr/lib
    #	sudo rm -f /usr/lib/libcw01zad2.so.1
    #	sudo rm -f /usr/lib/libcw01zad2.so
    #	sudo ln -s /usr/libcw01zad2.so.1.0.1 /url/lib.so.1
    #	sudo ln -s /usr/lib.so.1 /url/lib.so
    #	sudo ldconfig
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
	# short search time
	LD_LIBRARY_PATH=./ ./out_static create_table 1 search_directory /usr/bin zip short.txt remove_block 0
	# medium search time
	LD_LIBRARY_PATH=./ ./out_static create_table 1 search_directory /usr lib medium.txt remove_block 0
	# long search time
	LD_LIBRARY_PATH=./ ./out_static create_table 1 search_directory / efi long.txt remove_block 0
	# alternately
	LD_LIBRARY_PATH=./ ./out_static create_table 2 search_directory /usr/bin xz tmp_4.txt remove_block 0 search_directory /usr/bin zip tmp_5.txt remove_block 1


tests_shared:
	# short search time
	LD_LIBRARY_PATH=./ ./out_shared create_table 1 search_directory /usr/bin zip short.txt remove_block 0
	# medium search time
	LD_LIBRARY_PATH=./ ./out_shared create_table 1 search_directory /usr lib medium.txt remove_block 0
	# long search time
	LD_LIBRARY_PATH=./ ./out_shared create_table 1 search_directory / efi long.txt remove_block 0
	# alternately
	LD_LIBRARY_PATH=./ ./out_shared create_table 2 search_directory /usr/bin xz tmp_4.txt remove_block 0 search_directory /usr/bin zip tmp_5.txt remove_block 1


tests_dynamic:
	# short search time
	LD_LIBRARY_PATH=./ ./out_dynamic create_table 1 search_directory /usr/bin zip short.txt remove_block 0
	# medium search time
	LD_LIBRARY_PATH=./ ./out_dynamic create_table 1 search_directory /usr lib medium.txt remove_block 0
	# long search time
	LD_LIBRARY_PATH=./ ./out_dynamic create_table 1 search_directory / efi long.txt remove_block 0
	# alternately
	LD_LIBRARY_PATH=./ ./out_dynamic create_table 2 search_directory /usr/bin xz tmp_4.txt remove_block 0 search_directory /usr/bin zip tmp_5.txt remove_block 1


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
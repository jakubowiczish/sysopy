CC = gcc -Wall -ggdb

create_outfile:
	$(CC) -o out_main main.c

generate_tests:
	./out_main generate test.txt 10000 1
	./out_main generate test2.txt 8000 4
	./out_main generate test3.txt 6000 512
	./out_main generate test4.txt 4000 4096
	./out_main generate test5.txt 2000 8192

sort_lib_test:
	./out_main sort test.txt 10000 1 lib
	./out_main sort test2.txt 8000 4 lib
	./out_main sort test3.txt 6000 512 lib
	./out_main sort test4.txt 4000 4096 lib
	./out_main sort test5.txt 2000 8192 lib

sort_sys_test:
	./out_main sort test.txt 100 512 sys

copy_lib_test:
	./out_main copy test.txt test2.txt 100 512 lib

copy_sys_test:
	./out_main copy test.txt test2.txt 100 512 sys

run_main:
	make create_outfile
	make generate_tests
	make copy_lib_test

clean:
	rm -f out_main
	rm -f *.txt

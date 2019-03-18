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
	./out_main sort test.txt 10000 1 sys
	./out_main sort test2.txt 8000 4 sys
	./out_main sort test3.txt 6000 512 sys
	./out_main sort test4.txt 4000 4096 sys
	./out_main sort test5.txt 2000 8192 sys

copy_lib_test:
	./out_main copy test.txt test_.txt 10000 1 lib
	./out_main copy test.txt test_2.txt 8000 4 lib
	./out_main copy test.txt test_3.txt 6000 512 lib
	./out_main copy test.txt test_4.txt 4000 4096 lib
	./out_main copy test.txt test_5.txt 2000 8192 lib

copy_sys_test:
	./out_main copy test.txt test_.txt 10000 1 sys
	./out_main copy test.txt test_2.txt 8000 4 sys
	./out_main copy test.txt test_3.txt 6000 512 sys
	./out_main copy test.txt test_4.txt 4000 4096 sys
	./out_main copy test.txt test_5.txt 2000 8192 sys

run_sort_lib:
	make create_outfile
	make generate_tests
	make sort_lib_test

run_sort_sys:
	make create_outfile
	make generate_tests
	make sort_sys_test

run_copy_lib:
	make create_outfile
	make generate_tests
	make copy_lib_test

run_copy_sys:
	make create_outfile
	make generate_tests
	make copy_sys_test


clean:
	rm -f out_main
	rm -f *.txt

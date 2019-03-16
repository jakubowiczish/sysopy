CC = gcc -Wall -ggdb

create_outfile:
	$(CC) -o out_main main.c

run_main:
	make create_outfile
	make generate_test
	make sort_lib_test


generate_test:
	./out_main generate test.txt 100 512

sort_lib_test:
	./out_main sort test.txt 100 512 lib

sort_sys_test:
	./out_main sort test.txt 100 512 sys

clean:
	rm -f out_main
	rm -f *.txt

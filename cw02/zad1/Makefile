CC = gcc -Wall -ggdb

create_outfile:
	$(CC) -o out_main main.c

run_main:
	make create_outfile
	./out_main generate test.txt 100 512

clean:
	rm -f out_main
	rm -f *.txt
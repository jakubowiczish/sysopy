CC = gcc -Wall -ggdb

create_outfile:
	$(CC) -o out_main main.c

generate_test:
	 ./out_main ~/CLionProjects/sysopy/

run:
	make create_outfile
	make generate_test

clean:
	rm -f out_main
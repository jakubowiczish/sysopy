CC = gcc -Wall -ggdb

create_outfile:
	$(CC) -o out_main main.c

generate_test:
	./out_main /home/jakub/CLionProjects/sysopy/cw03/zad2/list.txt 10 standard

run:
	make create_outfile
	make generate_test

make clean:
	rm -f out_main
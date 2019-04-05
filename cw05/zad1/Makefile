CC = gcc -Wall -ggdb

out_file: main.c
	$(CC) -o out_file main.c

run: main.c out_file
	./out_file commands.txt

make clean: out_file
	rm -f out_file
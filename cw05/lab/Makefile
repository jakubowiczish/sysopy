CC = gcc -Wall -ggdb

out_file: main.c
	$(CC) -o out_file main.c

test:
	./out_file "ls -l" "wc -l" result.txt

clean: result.txt
	rm -f result.txt
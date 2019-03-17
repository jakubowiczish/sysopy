CC = gcc -Wall -ggdb

create_outfile:
	$(CC) -o out_main main.c

generate_test:
	./out_main "/home/jakub/IdeaProjects" "<" "2019.03.13 15:18"

run:
	make create_outfile
	make generate_test

make clean:
	rm -f out_main
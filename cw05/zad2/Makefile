CC = gcc -Wall -ggdb

out_slave: slave.c
	$(CC) -o out_slave slave.c utils.c

out_master: master.c
	$(CC) -o out_master master.c utils.c

master_and_slave:
	make out_master
	make out_slave

run_slave: out_slave
	./out_slave pipe_file 3

run_master: out_master
	./out_master pipe_file

clean:
	rm -f out_master
	rm -f out_slave
	rm -f pipe_file
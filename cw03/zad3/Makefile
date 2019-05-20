CC      := gcc
CFLAGS  := -Wall -O3 -std=c99
HEADERS := error.h
SRC     := monitor.c backup.c error.c
NAME    := monitor

$(NAME): $(HEADERS) $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

tester: tester.c
	$(CC) $(CFLAGS) tester.c -o tester

clean:
	$(RM) -rf $(NAME) tester files archive

test: clean $(NAME) tester
	mkdir "files"
	echo "files/file_1 1" > files/list1
	echo "files/file_2 2" >> files/list1
	echo "files/file_3 3" > files/list2
	echo "files/file_4 2" >> files/list2

	./tester files/file_1 1 3 16 &
	./tester files/file_2 1 3 16 &
	./tester files/file_3 1 3 16 &
	./tester files/file_4 1 3 16 &

	./monitor files/list1 10 exec 1 7 &
	sleep 1
	./monitor files/list2 10 mem 1 3 &
	sleep 11
	pkill tester

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

test: $(NAME) tester
	mkdir -p "files"
	echo "files/file_1 1" > files/list
	echo "files/file_2 2" >> files/list
	echo "files/file_3 3" >> files/list
	echo "files/file_4 4" >> files/list

	./tester files/file_1 1 3 16 &
	./tester files/file_2 1 3 16 &
	./tester files/file_3 1 3 16 &
	./tester files/file_4 1 3 16 &

	./monitor files/list
	pkill tester

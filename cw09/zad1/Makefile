CC      := gcc
CFLAGS  := -Wall -O3 -std=c99 -pthread
HEADERS := error.h
SRC     := error.c roller_coaster.c
OUT     := roller_coaster

.PHONY: $(OUT) all test clean
.DEFAULT: all

all: $(OUT)

$(OUT): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

test: $(OUT)
	./$(OUT) 10 2 4 1

clean:
	$(RM) $(OUT)
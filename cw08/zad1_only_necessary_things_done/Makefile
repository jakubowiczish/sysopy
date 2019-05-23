CC      := gcc
CFLAGS  := -Wall -O3 -std=c99
HEADERS := error.h
SRC     := error.c image.c
OUT     := image

.PHONY: $(OUT) all test clean
.DEFAULT: all

all: $(OUT)

$(OUT): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

test: $(OUT)
	./$(OUT) 1 block mona_lisa.pgm filter.txt output.pgm

clean:
	$(RM) $(OUT)

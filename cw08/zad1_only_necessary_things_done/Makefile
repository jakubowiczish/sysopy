CC      := gcc
CFLAGS  := -Wall -O3 -std=c99 -pthread
HEADERS := error.h
SRC     := error.c filter.c
OUT     := filter
RESULT  := times.txt

.PHONY: $(OUT) all test clean
.DEFAULT: all

all: $(OUT)

$(OUT): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

test: $(OUT)
	./$(OUT) 1 block mona.pgm edge.fil mona_edge.pgm > $(RESULT)
	./$(OUT) 2 block mona.pgm edge.fil mona_edge.pgm >> $(RESULT)
	./$(OUT) 4 block mona.pgm edge.fil mona_edge.pgm >> $(RESULT)
	./$(OUT) 8 block mona.pgm edge.fil mona_edge.pgm >> $(RESULT)

	./$(OUT) 1 interleaved mona.pgm edge.fil mona_edge.pgm >> $(RESULT)
	./$(OUT) 2 interleaved mona.pgm edge.fil mona_edge.pgm >> $(RESULT)
	./$(OUT) 4 interleaved mona.pgm edge.fil mona_edge.pgm >> $(RESULT)
	./$(OUT) 8 interleaved mona.pgm edge.fil mona_edge.pgm >> $(RESULT)

	./$(OUT) 1 block mona.pgm gauss.fil mona_gauss.pgm >> $(RESULT)
	./$(OUT) 2 block mona.pgm gauss.fil mona_gauss.pgm >> $(RESULT)
	./$(OUT) 4 block mona.pgm gauss.fil mona_gauss.pgm >> $(RESULT)
	./$(OUT) 8 block mona.pgm gauss.fil mona_gauss.pgm >> $(RESULT)

	./$(OUT) 1 interleaved mona.pgm gauss.fil mona_gauss.pgm >> $(RESULT)
	./$(OUT) 2 interleaved mona.pgm gauss.fil mona_gauss.pgm >> $(RESULT)
	./$(OUT) 4 interleaved mona.pgm gauss.fil mona_gauss.pgm >> $(RESULT)
	./$(OUT) 8 interleaved mona.pgm gauss.fil mona_gauss.pgm >> $(RESULT)

	./$(OUT) 1 block mona.pgm sharpen.fil mona_sharpen.pgm >> $(RESULT)
	./$(OUT) 2 block mona.pgm sharpen.fil mona_sharpen.pgm >> $(RESULT)
	./$(OUT) 4 block mona.pgm sharpen.fil mona_sharpen.pgm >> $(RESULT)
	./$(OUT) 8 block mona.pgm sharpen.fil mona_sharpen.pgm >> $(RESULT)

	./$(OUT) 1 interleaved mona.pgm sharpen.fil mona_sharpen.pgm >> $(RESULT)
	./$(OUT) 2 interleaved mona.pgm sharpen.fil mona_sharpen.pgm >> $(RESULT)
	./$(OUT) 4 interleaved mona.pgm sharpen.fil mona_sharpen.pgm >> $(RESULT)
	./$(OUT) 8 interleaved mona.pgm sharpen.fil mona_sharpen.pgm >> $(RESULT)

clean:
	$(RM) $(OUT)

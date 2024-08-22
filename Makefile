CC=gcc
FLAGS=-O2 -Wall -Wextra -Werror -fsanitize=address
DFLAGS=-Wall -Wextra -Werror -g
TARGET=sudoku
SRC=src/sudoku.c
INCLUDE=include/sudoku.h

all: $(TARGET)

$(TARGET): $(SRC) $(INCLUDE)
	$(CC) $(FLAGS) $(SRC) -o $(TARGET)

debug: $(SRC) $(INCLUDE)
	$(CC) $(DFLAGS) $(SRC) -o debug

clean:
	rm $(TARGET) debug

.PHONY: clean
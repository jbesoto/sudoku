CC=gcc
FLAGS=-O2 -Wall -Wextra -Werror -fsanitize=address
TARGET=sudoku
SRC=src/sudoku.c
INCLUDE=include/sudoku.h

all: $(TARGET)

$(TARGET): $(SRC) $(INCLUDE)
	$(CC) $(FLAGS) $(SRC) -o $(TARGET)

clean:
	rm $(TARGET)

.PHONY: clean
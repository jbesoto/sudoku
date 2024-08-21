#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>  // strerror
#include <fcntl.h>   // O_RDONLY
#include <errno.h>

#include "../include/sudoku.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    PrintHelp();
    return EXIT_FAILURE;
  }
  char *filepath = argv[1];
  int (*board)[ROWS] = CreateBoard(filepath);
  PrintBoard(board);
  free(board);

  return 0;
}

int (*CreateBoard(char *filepath))[ROWS] {
  int (*board)[ROWS] = malloc(sizeof(int[ROWS][COLS]));
  if (!board) {
     fprintf(stderr, "Error: Unable to malloc board: %s\n", strerror(errno));
     return NULL;
  }

  memset(board, 0, sizeof(int[ROWS][COLS]));

  if (LoadBoard(filepath, board) < 0) {
    fprintf(stderr, "Error: Unable to read board into buffer: %s\n", strerror(errno));
    free(board);
    return NULL;
  }

  return board;
}

int LoadBoard(char *filepath, int board[ROWS][COLS]) {
  int fd = open(filepath, O_RDONLY);
  if (fd < 0) {
    return -1;
  }

  char buf[sizeof(char)];
  ssize_t bytes;

  size_t i = 0, j = 0;
  while ((bytes = read(fd, buf, sizeof(char))) > 0 && i < ROWS) {
    if (buf[0] == '\n') {
      i++;
      j = 0;
      continue;
    }
    board[i][j++] = (buf[0] > '0' && buf[0] <= '9') ? buf[0] - '0' : 0;
  }

  if (bytes < 0) {
    close(fd);
    return -1;
  }

  close(fd);
  return 0;
}

void PrintBoard(int board[ROWS][COLS]) {
  if (!board) {
    fprintf(stderr, "Error: board is NULL\n");
    return;
  }

  printf("#---#---#---#---#---#---#---#---#---#\n");
  for (size_t i = 0; i < ROWS; i++) {
    for (size_t j = 0; j < COLS; j++) {
      if (board[i][j] == 0) {
        printf("|   ");
      } else {
        printf("| %d ", board[i][j]);
      }

      if (j + 1 == COLS) {
        printf("|\n");
      }
    }
    if (i > 0 && (i + 1) % 3 == 0) {
      printf("#---#---#---#---#---#---#---#---#---#\n");
    } else {
      printf("#-----------#-----------#-----------#\n");
    }
  }
}

void PrintHelp(void) {
  printf("sudoku - A program that solves 9x9 sudoku puzzles.\n");
  printf("\n");
  printf("Usage:\n");
  printf("  sudoku [OPTIONS] <FILE>\n");
  printf("\n");
  printf("Options:\n");
  printf("  %-20s %s\n", "-h, --help", "Show this message and exit.");
}

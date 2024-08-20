#include <stdio.h>
#include <stdlib.h>

#include "../include/sudoku.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    PrintHelp();
    return EXIT_FAILURE;
  }

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

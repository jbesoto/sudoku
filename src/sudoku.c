#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>  // strerror
#include <fcntl.h>   // O_RDONLY
#include <errno.h>
#include <stdbool.h>

#include "../include/sudoku.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    PrintHelp();
    return EXIT_FAILURE;
  }

  char *filepath = argv[1];
  SudokuCell (*board)[ROWS] = CreateBoard(filepath);

  printf("Your Board:\n");
  PrintBoard(board);

  SolveSudoku(board);
  printf("\nSolved!\n");
  PrintBoard(board);

  free(board);
  return 0;
}

SudokuCell (*CreateBoard(char *filepath))[ROWS] {
  SudokuCell (*board)[ROWS] = malloc(sizeof(SudokuCell[ROWS][COLS]));
  if (!board) {
     fprintf(stderr, "Error: Unable to malloc board: %s\n", strerror(errno));
     return NULL;
  }

  memset(board, 0, sizeof(SudokuCell[ROWS][COLS]));

  if (LoadBoard(filepath, board) < 0) {
    fprintf(stderr, "Error: Unable to read board into buffer: %s\n", strerror(errno));
    free(board);
    return NULL;
  }

  return board;
}

bool IsValid(SudokuCell board[ROWS][COLS], int candidate, size_t row, size_t col) {
  size_t mid_i, mid_j;

  // Find middle of 3x3 grid
  mid_i = (row % 3 == 1) ? row : (row % 3 == 0) ? row + 1 : row - 1;
  mid_j = (col % 3 == 1) ? col : (col % 3 == 0) ? col + 1 : col - 1;

  // Check 3x3 grid
  for (size_t i = mid_i - 1; i <= mid_i + 1; i++) {
    for (size_t j = mid_j - 1; j <= mid_j + 1; j++) {
      if (&board[row][col] == &board[i][j]) {
        continue;
      }
      if (candidate == board[i][j].value) {
        return false;
      }
    }
  }

  // Check row
  for (size_t i = 0; i < COLS; i++) {
    if (&board[row][col] == &board[row][i]) {
      continue;
    }
    if (candidate == board[row][i].value) {
      return false;
    }
  }

  // Check column
  for (size_t i = 0; i < ROWS; i++) {
    if (&board[row][col] == &board[i][col]) {
      continue;
    }
    if (candidate == board[i][col].value) {
      return false;
    }
  }

  return true;
}

int LoadBoard(char *filepath, SudokuCell board[ROWS][COLS]) {
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
    if (buf[0] > '0' && buf[0] <= '9') {
      board[i][j].value = buf[0] - '0';
      board[i][j].locked = true;
    }
    j++;
  }

  if (bytes < 0) {
    close(fd);
    return -1;
  }

  close(fd);
  return 0;
}

void PrintBoard(SudokuCell board[ROWS][COLS]) {
  if (!board) {
    fprintf(stderr, "Error: board is NULL\n");
    return;
  }

  printf("#---#---#---#---#---#---#---#---#---#\n");
  for (size_t i = 0; i < ROWS; i++) {
    for (size_t j = 0; j < COLS; j++) {
      if (board[i][j].value == 0) {
        printf("|   ");
      } else {
        printf("| %d ", board[i][j].value);
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

void SolveSudoku(SudokuCell board[ROWS][COLS]) {
  for (size_t i = 0; i < ROWS; i++) {
    for (size_t j = 0; j < COLS; j++) {
      SolveCell(board, MIN_NUM, &i, &j);
    }
  }
}

void SolveCell(SudokuCell board[ROWS][COLS], int num, size_t *row, size_t *col) {
  if (board[*row][*col].locked) {
    return;
  }

  int candidate;

  for (candidate = num; candidate <= 9; candidate++) {
    if (IsValid(board, candidate, *row, *col)) {
      board[*row][*col].value = candidate;
      return;
    }
  }

  // Backtrack
  do {
    *row = (*col == 0) ? (*row - 1) % ROWS : *row;
    *col = (*col - 1) % COLS;
  } while (board[*row][*col].locked);

  candidate = board[*row][*col].value + 1;
  board[*row][*col].value = 0;
  SolveCell(board, candidate, row, col);
}
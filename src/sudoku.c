/**
 * @file   sudoku.c
 * @author Juan Diego Becerra (@jbesoto)
 * @date   Aug 22, 2024
 * @brief  Program to solve 9x9 Sudoku puzzles using a backtracking algorithm.
 *
 * This file contains the implementation of a Sudoku solver, which utilizes
 * a recursive backtracking algorithm to fill a 9x9 Sudoku grid with valid
 * numbers. The solver reads the initial board configuration from a file,
 * solves the puzzle, and prints both the initial and solved board states.
 *
 * @see sudoku.h
 */

#include "../include/sudoku.h"

#ifndef TEST
int main(int argc, char *argv[]) {
  if (argc < 2) {
    PrintHelp();
    return EXIT_FAILURE;
  }

  char *filepath = argv[1];
  SudokuCell (*board)[kRows] = CreateBoard(filepath);
  if (!board) {
    return 1;
  }

  printf("Your Board:\n");
  PrintBoard(board);

  SolveSudoku(board);
  printf("\nSolved!\n");
  PrintBoard(board);

  free(board);
  return 0;
}
#endif

/**
 * @brief Creates and initializes the Sudoku board from the given file.
 *
 * Allocates memory for the board, initializes it with zeros, and then loads
 * the board values from the specified file. If an error occurs during
 * allocation or loading, it returns NULL.
 *
 * @param filepath The path to the file containing the initial Sudoku board.
 *
 * @return SudokuCell (*)[ROWS] Pointer to the initialized Sudoku board, or NULL
 *         on failure.
 */
SudokuCell (*CreateBoard(char *filepath))[kRows] {
  SudokuCell (*board)[kRows] = malloc(sizeof(SudokuCell[kRows][kCols]));
  if (!board) {
     fprintf(stderr, "Error: Unable to malloc board: %s\n", strerror(errno));
     return NULL;
  }

  memset(board, 0, sizeof(SudokuCell[kRows][kCols]));

  if (LoadBoard(filepath, board) < 0) {
    fprintf(stderr, "Error: Unable to read board into buffer: %s\n", strerror(errno));
    free(board);
    return NULL;
  }

  return board;
}

/**
 * @brief Validates if a candidate number can be placed in the specified cell.
 *
 * Checks whether the candidate number can be placed in the specified row,
 * column, and 3x3 subgrid without violating Sudoku rules.
 *
 * @param board     The Sudoku board.
 * @param candidate The candidate number to validate (1-9).
 * @param row       The row index of the cell.
 * @param col       The column index of the cell.

 * @return bool true if the candidate can be placed, false otherwise.
 */
bool IsValid(SudokuCell board[kRows][kCols], int candidate, size_t row, size_t col) {
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
  for (size_t i = 0; i < kCols; i++) {
    if (&board[row][col] == &board[row][i]) {
      continue;
    }
    if (candidate == board[row][i].value) {
      return false;
    }
  }

  // Check column
  for (size_t i = 0; i < kRows; i++) {
    if (&board[row][col] == &board[i][col]) {
      continue;
    }
    if (candidate == board[i][col].value) {
      return false;
    }
  }

  return true;
}

/**
 * @brief Loads the Sudoku board from a file into memory.
 *
 * Reads the board values from the specified file and populates the board array.
 * Cells with numeric values (1-9) are marked as locked. Returns -1 on error.
 *
 * @param filepath The path to the file containing the Sudoku board.
 * @param board    The Sudoku board to be populated.
 *
 * @return int 0 on success, -1 on failure.
 */
int LoadBoard(char *filepath, SudokuCell board[kRows][kCols]) {
  int fd = open(filepath, O_RDONLY);
  if (fd < 0) {
    return -1;
  }

  char buf[sizeof(char)];
  ssize_t bytes;

  size_t i = 0, j = 0;
  while ((bytes = read(fd, buf, sizeof(char))) > 0 && i < kRows) {
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

/**
 * @brief Prints the current state of the Sudoku board.
 *
 * This function prints the Sudoku board to the standard output in a formatted
 * manner.
 *
 * @param board The Sudoku board to be printed.
 */
void PrintBoard(SudokuCell board[kRows][kCols]) {
  if (!board) {
    fprintf(stderr, "Error: board is NULL\n");
    return;
  }

  const char *kBackground = "\033[48;5;240m";
  const char *kReset = "\033[0m";

  printf("╔═══════════╦═══════════╦═══════════╗\n");
  for (size_t i = 0; i < kRows; i++) {
    for (size_t j = 0; j < kCols; j++) {
      // Print separator
      if (j % 3 == 0) {
        printf("║");
      } else {
        printf("│");
      }

      // Print cell value
      if (board[i][j].value == 0) {
        printf("   ");
      } else if (board[i][j].locked) {
        printf("%s %d %s", kBackground, board[i][j].value, kReset);
      } else {
        printf(" %d ", board[i][j].value);
      }

      // End of the row
      if (j == kCols - 1) {
        printf("║\n");
      }
    }

    // Print bottom and intermediate borders
    if (i == kRows - 1) {
      printf("╚═══════════╩═══════════╩═══════════╝\n");
    } else if ((i + 1) % 3 == 0) {
      printf("╠═══════════╬═══════════╬═══════════╣\n");
    } else {
      printf("╟───────────║───────────║───────────╢\n");
    }
  }
}

/**
 * @brief Prints the help message for the Sudoku solver program.
 *
 * Displays usage instructions and options for the program.
 */
void PrintHelp(void) {
  printf("sudoku - A program that solves 9x9 sudoku puzzles.\n");
  printf("\n");
  printf("Usage:\n");
  printf("  sudoku <FILE>\n");
}

/**
 * @brief Solves the Sudoku puzzle using a backtracking algorithm.
 *
 * Iterates through every cell in the board, attempting to solve each one
 * using the `SolveCell()` function. This function is the main driver of
 * the backtracking algorithm.
 *
 * @param board The Sudoku board to be solved.
 */
void SolveSudoku(SudokuCell board[kRows][kCols]) {
  for (size_t i = 0; i < kRows; i++) {
    for (size_t j = 0; j < kCols; j++) {
      SolveCell(board, kMinNum, &i, &j);
    }
  }
}

/**
 * @brief Attempts to solve the specified cell using a backtracking approach.
 *
 * Tries all candidate numbers (1-9) for the specified cell, checking if
 * they can be placed without violating Sudoku rules. If a valid candidate
 * is found, it is placed in the cell. If no valid candidate is found,
 * the function backtracks to the previous cell to try a different candidate.
 *
 * @param board The Sudoku board.
 * @param num   The starting candidate number.
 * @param row   The row index of the cell to be solved.
 * @param col   The column index of the cell to be solved.
 */
void SolveCell(SudokuCell board[kRows][kCols], int num, size_t *row, size_t *col) {
  if (board[*row][*col].locked) {
    return;
  }

  int candidate;

  for (candidate = num; candidate <= kMaxNum; candidate++) {
    if (IsValid(board, candidate, *row, *col)) {
      board[*row][*col].value = candidate;
      return;
    }
  }

  // Backtrack
  do {
    *row = (*col == 0) ? *row - 1 : *row;
    *col = (*col == 0) ? kCols - 1 : *col - 1;
  } while (board[*row][*col].locked);

  candidate = board[*row][*col].value + 1;
  board[*row][*col].value = 0;
  SolveCell(board, candidate, row, col);
}

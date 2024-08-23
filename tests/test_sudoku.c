/**
 * @file   test_sudoku.c
 * @author Juan Diego Becerra
 * @date   Aug 22, 2024
 * @brief  Test suite for the Sudoku solver.
 *
 * This file contains the main function that tests the Sudoku solver. It reads
 * a Sudoku board from a file, attempts to solve it, and verifies the solution.
 */

#include "../include/sudoku.h"

/**
 * @brief Entry point of the Sudoku solver program.
 *
 * This function reads a Sudoku puzzle from a file, attempts to solve it,
 * and validates the solution.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments.
 *
 * @return Returns 0 on successful solving and validation of the puzzle,
 *         otherwise returns 1 if there is an error.
 */
int main(int argc, char *argv[]) {
  if (argc < 1) {
    return 1;
  }

  char *filepath = argv[1];
  SudokuCell (*board)[ROWS] = CreateBoard(filepath);
  SolveSudoku(board);

  for (size_t i = 0; i < ROWS; i++) {
    for (size_t j = 0; j < COLS; j++) {
      int value = board[i][j].value;
      if (value == 0) {
        continue;
      }
      if (!IsValid(board, value, i, j)) {
        return 1;
      }
    }
  }

  free(board);
  return 0;
}
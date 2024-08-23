#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define kRows    9
#define kCols    9
#define kMinNum  1
#define kMaxNum  9

typedef struct {
  int value;
  bool locked;
} SudokuCell;

SudokuCell (*CreateBoard(char *filepath))[kRows];
bool IsValid(SudokuCell board[kRows][kCols], int candidate, size_t row, size_t col);
int LoadBoard(char *filepath, SudokuCell board[kRows][kCols]);
void PrintBoard(SudokuCell board[kRows][kCols]);
void PrintHelp(void);
void SolveCell(SudokuCell board[kRows][kCols], int num, size_t *row, size_t *col);
void SolveSudoku(SudokuCell board[kRows][kCols]);

#endif  // SUDOKU_H_
#ifndef SUDOKU_H_
#define SUDOKU_H_

#define ROWS     9
#define COLS     9
#define MIN_NUM  1
#define MAX_NUM  9

typedef struct {
  int value;
  bool locked;
} SudokuCell;

SudokuCell (*CreateBoard(char *filepath))[ROWS];
bool IsValid(SudokuCell board[ROWS][COLS], int candidate, size_t row, size_t col);
int LoadBoard(char *filepath, SudokuCell board[ROWS][COLS]);
void PrintBoard(SudokuCell board[ROWS][COLS]);
void PrintHelp(void);
void SolveCell(SudokuCell board[ROWS][COLS], int num, size_t *row, size_t *col);
void SolveSudoku(SudokuCell board[ROWS][COLS]);

#endif  // SUDOKU_H_
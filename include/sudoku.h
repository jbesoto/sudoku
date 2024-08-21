#ifndef SUDOKU_H_
#define SUDOKU_H_

#define ROWS 9
#define COLS 9

int (*CreateBoard(char *filepath))[ROWS];
int LoadBoard(char *filepath, int board[ROWS][COLS]);
void PrintBoard(int board[ROWS][COLS]);
void PrintHelp(void);

#endif  // SUDOKU_H_